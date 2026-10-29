#include <memory>
#include <string>
#include <cmath>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Vertex.h"
#include "SDL3/SDL.h"
#include <glm/gtc/matrix_transform.hpp>

constexpr int WIDTH = 960;
constexpr int HEIGHT = 540;

struct Triangle
{
    glm::vec2 a, b, c;
    Vertex v0_proj, v1_proj, v2_proj;
    float w0, w1, w2;
    float minX, maxX, minY, maxY;
};

inline int linearToSRGB(const float& norm)
{
    if (norm <= 0.0f) return 0;
    if (!(norm <= 1.0f)) return 255;
    float srgb = (norm <= 0.0031308f)
                     ? (norm * 12.92f)
                     : (1.055f * std::pow(norm, 1.0f / 2.4f) - 0.055f);
    return static_cast<int>(srgb * 255.0f + 0.5f);
}

int toSRGB(const glm::vec3& color)
{
    int r = linearToSRGB(color.r);
    int g = linearToSRGB(color.g);
    int b = linearToSRGB(color.b);
    int a = 0xFF;
    return (a << 24) | (r << 16) | (g << 8) | b;
}

int main(int argc, char** argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) return -1;

    SDL_Window* window = SDL_CreateWindow("Morty it turns - Cube (Press Space for Sphere)", WIDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    uint32_t* frameBuffer = new uint32_t[WIDTH * HEIGHT];

    auto cubeMesh = Mesh::CreateCube(
        glm::vec3(1.0f, 0.2f, 0.3f), glm::vec3(0.1f, 1.0f, 0.1f),
        glm::vec3(0.8f, 0.4f, 1.0f), glm::vec3(1.0f, 1.0f, 0.5f),
        glm::vec3(1.0f, 0.4f, 1.0f), glm::vec3(0.7f, 1.0f, 1.0f)
    );

    auto sphereMesh = Mesh::CreateSphere(16, glm::vec3(0.2f, 0.6f, 0.9f));
    std::shared_ptr<Mesh> currentMesh = cubeMesh;
    bool isSphere = false;

    // Camera & Light positions in World Space
    const glm::vec3 cameraPos(0.0f, 0.0f, 4.0f);
    const glm::vec3 lightPos(3.0f, 3.0f, 4.0f);

    // Projection: 45 degree FOV, aspect ratio, near=0.1, far=100.0
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(WIDTH) / static_cast<float>(HEIGHT),
        0.1f,
        100.0f
    );

    // View: Camera placed at cameraPos looking at origin
    glm::mat4 view = glm::lookAt(
        cameraPos,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    float step = 0.01f;
    float angleInRadians = 0.0f;
    bool running = true;
    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        std::memset(frameBuffer, 0, WIDTH * HEIGHT * sizeof(uint32_t));

        constexpr float halfW = WIDTH / 2.0f;
        constexpr float halfH = HEIGHT / 2.0f;

        // Model animated per frame with normalized rotation axis
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), angleInRadians, glm::normalize(glm::vec3(0.5f, 1.0f, 0.0f)));
        glm::mat4 mvp = projection * view * model;
        angleInRadians += step;
        if (angleInRadians > 360.0f) { angleInRadians = 0.0f; }

        std::vector<Triangle> triangles;

        for (const auto& tri : currentMesh->Tris())
        {
            const Vertex& v0 = *currentMesh->Vertices()[tri->a];
            const Vertex& v1 = *currentMesh->Vertices()[tri->b];
            const Vertex& v2 = *currentMesh->Vertices()[tri->c];

            Vertex vs0 = Vertex::VertexShader(v0, mvp, model);
            Vertex vs1 = Vertex::VertexShader(v1, mvp, model);
            Vertex vs2 = Vertex::VertexShader(v2, mvp, model);

            // Perspective divide
            Vertex v0_proj = vs0 * (1.0f / vs0.Position().w);
            Vertex v1_proj = vs1 * (1.0f / vs1.Position().w);
            Vertex v2_proj = vs2 * (1.0f / vs2.Position().w);

            // Screen mapping
            glm::vec2 p0(halfW + v0_proj.Position().x * halfW, halfH - v0_proj.Position().y * halfH);
            glm::vec2 p1(halfW + v1_proj.Position().x * halfW, halfH - v1_proj.Position().y * halfH);
            glm::vec2 p2(halfW + v2_proj.Position().x * halfW, halfH - v2_proj.Position().y * halfH);

            float visible = glm::cross(glm::vec3(p1 - p0, 0.0f), glm::vec3(p2 - p0, 0.0f)).z;

            if (visible > 0.00f)
            {
                float minX = std::max(0.0f, std::min({p0.x, p1.x, p2.x}));
                float maxX = std::min(static_cast<float>(WIDTH - 1), std::max({p0.x, p1.x, p2.x}));
                float minY = std::max(0.0f, std::min({p0.y, p1.y, p2.y}));
                float maxY = std::min(static_cast<float>(HEIGHT - 1), std::max({p0.y, p1.y, p2.y}));

                triangles.push_back({
                    p0, p1, p2,
                    v0_proj, v1_proj, v2_proj,
                    vs0.Position().w, vs1.Position().w, vs2.Position().w,
                    minX, maxX, minY, maxY
                });
            }
        }

#pragma omp parallel for schedule(dynamic, 16)
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                glm::vec2 p(j, i);
                for (const auto& tri : triangles)
                {
                    if (p.x < tri.minX || p.x > tri.maxX || p.y < tri.minY || p.y > tri.maxY)
                        continue;

                    auto d1 = (tri.b.x - tri.a.x) * (p.y - tri.a.y) - (tri.b.y - tri.a.y) * (p.x - tri.a.x);
                    auto d2 = (tri.c.x - tri.b.x) * (p.y - tri.b.y) - (tri.c.y - tri.b.y) * (p.x - tri.b.x);
                    auto d3 = (tri.a.x - tri.c.x) * (p.y - tri.c.y) - (tri.a.y - tri.c.y) * (p.x - tri.c.x);

                    if (d1 >= 0 && d2 >= 0 && d3 >= 0)
                    {
                        float total = d1 + d2 + d3;
                        float lambda1 = d2 / total; // weight for A
                        float lambda2 = d3 / total; // weight for B
                        float lambda3 = d1 / total; // weight for C

                        // Perspective-correct reconstructed vertex Q
                        Vertex numerator = (lambda1 * tri.v0_proj) +
                                           (lambda2 * tri.v1_proj) +
                                           (lambda3 * tri.v2_proj);
                        float denominator = (lambda1 / tri.w0) +
                                            (lambda2 / tri.w1) +
                                            (lambda3 / tri.w2);
                        Vertex Q = numerator * (1.0f / denominator);

                        // Fragment Shader: Diffuse & Specular shading
                        glm::vec3 shadedColor = Vertex::FragmentShader(Q, cameraPos, lightPos);
                        frameBuffer[i * WIDTH + j] = toSRGB(shadedColor);
                        break;
                    }
                }
            }
        }

        SDL_UpdateTexture(texture, NULL, frameBuffer, WIDTH * sizeof(uint32_t));
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT ||
                (e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE))
            {
                running = false;
            }
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_SPACE)
            {
                isSphere = !isSphere;
                currentMesh = isSphere ? sphereMesh : cubeMesh;
                SDL_SetWindowTitle(window, isSphere
                    ? "Morty it turns - Sphere (Press Space for Cube)"
                    : "Morty it turns - Cube (Press Space for Sphere)");
            }
        }
        SDL_Delay(16);
    }

    delete[] frameBuffer;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
