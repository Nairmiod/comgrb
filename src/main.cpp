#include <memory>
#include <string>
#include <cmath>
#include <cstring>
#include <iostream>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Vertex.h"
#include "SDL3/SDL.h"
#include <glm/gtc/matrix_transform.hpp>

constexpr int WIDTH = 960;
constexpr int HEIGHT = 540;
const std::string NAME = "Morty it turns";

struct Triangle
{
    glm::vec2 a, b, c;
    Vertex v0_proj, v1_proj, v2_proj;
    float w0, w1, w2;
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

    SDL_Window* window = SDL_CreateWindow(NAME.c_str(), WIDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    uint32_t* frameBuffer = new uint32_t[WIDTH * HEIGHT];


    auto mesh = Mesh::CreateCube(
        glm::vec3(1, 0.2, 0.3), glm::vec3(0.1, 1, 0.1),
        glm::vec3(0.8, 0.4, 1), glm::vec3(1, 1, 0.5),
        glm::vec3(1, 0.4, 1), glm::vec3(0.7, 1, 1)
    );


    float step = 0.01f;
    float angleInRadians = 0.0f;
    float frame = 0.0f;
    bool running = true;
    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        std::memset(frameBuffer, 0, WIDTH * HEIGHT * sizeof(uint32_t));

        constexpr float C = WIDTH / 2.0f;
        constexpr float halfW = WIDTH / 2.0f;
        constexpr float halfH = HEIGHT / 2.0f;

        // Morty it turns
        glm::mat4x4 rot = glm::rotate(glm::mat4(1.0f), angleInRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        angleInRadians += step;
        frame++;
        if (angleInRadians > 360.0f) { angleInRadians = 0.0f; }

        std::vector<Triangle> triangles;


        for (const auto& tri : mesh->Tris())
        {
            const Vertex& v0 = *mesh->Vertices()[tri->a];
            const Vertex& v1 = *mesh->Vertices()[tri->b];
            const Vertex& v2 = *mesh->Vertices()[tri->c];


            Vertex vs0 = VertexShader(v0, rot);
            Vertex vs1 = VertexShader(v1, rot);
            Vertex vs2 = VertexShader(v2, rot);

            Vertex v0_proj = vs0 * (1.0f / vs0.Position().w);
            Vertex v1_proj = vs1 * (1.0f / vs1.Position().w);
            Vertex v2_proj = vs2 * (1.0f / vs2.Position().w);

            glm::vec2 p0(halfW + v0_proj.Position().x * C, halfH - v0_proj.Position().y * C);
            glm::vec2 p1(halfW + v1_proj.Position().x * C, halfH - v1_proj.Position().y * C);
            glm::vec2 p2(halfW + v2_proj.Position().x * C, halfH - v2_proj.Position().y * C);


            float visible = glm::cross(glm::vec3(p1 - p0, 0.0f), glm::vec3(p2 - p0, 0.0f)).z;


            if (visible < 0.00f)
            {
                triangles.push_back({
                    p0, p1, p2,
                    v0_proj, v1_proj, v2_proj,
                    vs0.Position().w, vs1.Position().w, vs2.Position().w
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
                    auto d1 = (tri.b.x - tri.a.x) * (p.y - tri.a.y) - (tri.b.y - tri.a.y) * (p.
                        x - tri.a.
                                x);
                    auto d2 = (tri.c.x - tri.b.x) * (p.y - tri.b.y) - (tri.c.y - tri.b.y) * (p.
                        x - tri.b.
                                x);
                    auto d3 = (tri.a.x - tri.c.x) * (p.y - tri.c.y) - (tri.a.y - tri.c.y) * (p.
                        x - tri.c.
                                x);

                    if ((d1 <= 0 && d2 <= 0 && d3 <= 0))
                    {
                        float total = d1 + d2 + d3;
                        float lambda1 = d2 / total; // weight for A
                        float lambda2 = d3 / total; // weight for B
                        float lambda3 = d1 / total; // weight for C

                        // Numerator: λ1 * Â + λ2 * B̂ + λ3 * Ĉ
                        Vertex numerator = (lambda1 * tri.v0_proj) +
                                           (lambda2 * tri.v1_proj) +
                                           (lambda3 * tri.v2_proj);
                        // Denominator: λ1 / Aw + λ2 / Bw + λ3 / Cw
                        float denominator = (lambda1 / tri.w0) +
                                            (lambda2 / tri.w1) +
                                            (lambda3 / tri.w2);
                        // Q is the fully reconstructed, perspective-correct Vertex:
                        Vertex Q = numerator * (1.0f / denominator);


                        frameBuffer[i * WIDTH + j] = toSRGB(Q.Color());
                        break;
                    }
                }
            }
        }
        SDL_SetWindowTitle(window, (std::string(NAME)).c_str());
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
        }
        SDL_Delay(16);


        SDL_SetWindowTitle(window, std::string(NAME).c_str());
    }

    delete[] frameBuffer;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
