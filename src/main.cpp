#include <memory>
#include <string>
#include <cmath>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Vertex.h"
#include "SDL3/SDL.h"

constexpr int WIDTH = 960;
constexpr int HEIGHT = 540;
const std::string NAME = "Boxerino now without box (still has laser magic)";



int main(int argc, char** argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) return -1;

    SDL_Window* window = SDL_CreateWindow(NAME.c_str(), WIDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    int* frameBuffer = new int[WIDTH * HEIGHT];

    glm::vec3* buffer = new glm::vec3[WIDTH * HEIGHT];
    for (int i = 0; i < WIDTH * HEIGHT; ++i)
    {
        buffer[i] = glm::vec3(0.0f);
    }

    glm::mat4x4 rot =

    auto mesh = Mesh::CreateCube(
        glm::vec3(1, 0, 0), glm::vec3(0, 1, 0),
        glm::vec3(0, 0, 1), glm::vec3(1, 1, 0),
        glm::vec3(1, 0, 1), glm::vec3(0, 1, 1)
    );



    bool running = true;
    int frame = 0;
    while (running)
    {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        constexpr float C = WIDTH / 2.0f;
        constexpr float halfW = WIDTH / 2.0f;
        constexpr float halfH = HEIGHT / 2.0f;

        for (const auto& tri : mesh->Tris())
        {
            const Vertex& v0 = *mesh->Vertices()[tri->a];
            const Vertex& v1 = *mesh->Vertices()[tri->b];
            const Vertex& v2 = *mesh->Vertices()[tri->c];

            Vertex vs0 = VertexShader(v0);
            Vertex vs1 = VertexShader(v1);
            Vertex vs2 = VertexShader(v2);

            Vertex v0_proj = vs0 * (1.0f / vs0.Position().w);
            Vertex v1_proj = vs1 * (1.0f / vs1.Position().w);
            Vertex v2_proj = vs2 * (1.0f / vs2.Position().w);

            glm::vec2 p0(halfW + v0_proj.Position().x * C, halfH - v0_proj.Position().y * C);
            glm::vec2 p1(halfW + v1_proj.Position().x * C, halfH - v1_proj.Position().y * C);
            glm::vec2 p2(halfW + v2_proj.Position().x * C, halfH - v2_proj.Position().y * C);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderLine(renderer, p0.x, p0.y, p1.x, p1.y);
            SDL_RenderLine(renderer, p1.x, p1.y, p2.x, p2.y);
            SDL_RenderLine(renderer, p2.x, p2.y, p0.x, p0.y);
        }
        SDL_RenderPresent(renderer);

        frame++;
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

    delete[] buffer;
    delete[] frameBuffer;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
