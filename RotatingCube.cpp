#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "Cube.h"
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int CUBE_X = 400;
const int CUBE_Y = 400;

const int CUBE_LENGTH = 200;
const int VIEWER_DISTANCE = 4;
const int FOV = 800;

void handleInputs(SDL_Renderer *renderer, bool &running);
void changeCoordinatesAlongYaxis(Cube &cube, Cube &newCube, float &angle);
void changeCoordinatesAlongXaxis(Cube &cube, Cube &newCube, float &angle);
void changeCoordinatesAlongZaxis(Cube &cube, Cube &newCube, float &angle);
void transform(Cube &newCube, Cube &two_DCube);
void renderCube(SDL_Renderer *renderer, Cube &two_DCube);

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cerr << "Error initializing SDL: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Fluid Simulation",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (!window)
    {
        cerr << "Error creating SDL window: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        cerr << "Error creating SDL renderer: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Cube cube({0, 0, 0}, CUBE_LENGTH / 100);
    Cube newCube({0, 0, 0}, CUBE_LENGTH / 100);

    Cube two_DCube({0, 0, 0}, CUBE_LENGTH / 100);
    auto v1 = cube.vertices[0];
    auto v2 = cube.vertices[1];

    cout << v1.x << " " << v1.y << " " << v1.z << endl;
    cout << v2.x << " " << v2.y << " " << v2.z << endl;

    bool running = true;
    float angle;
    while (running)
    {

        angle += 0.05;
        handleInputs(renderer, running);
        changeCoordinatesAlongXaxis(cube, newCube, angle);
        transform(newCube, two_DCube);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        renderCube(renderer, two_DCube);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void handleInputs(SDL_Renderer *renderer, bool &running)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {

        if (event.type == SDL_QUIT)
        {
            running = false;
            break;
        }
    }
}

void changeCoordinatesAlongYaxis(Cube &cube, Cube &newCube, float &A)
{

    for (int i = 0; i < 8; i++)
    {
        auto &vi = cube.vertices[i];
        auto &vf = newCube.vertices[i];

        vf.x = vi.x * cos(A) + vi.z * sin(A);
        vf.z = -vi.x * sin(A) + vi.z * cos(A);
    }
}

void changeCoordinatesAlongXaxis(Cube &cube, Cube &newCube, float &A)
{
    for (int i = 0; i < 8; i++)
    {
        auto &vi = cube.vertices[i];
        auto &vf = newCube.vertices[i];

        vf.y = vi.y * cos(A) - vi.z * sin(A);
        vf.z = vi.y * sin(A) + vi.z * cos(A);
    }
}
void changeCoordinatesAlongZaxis(Cube &cube, Cube &newCube, float &A)
{
    for (int i = 0; i < 8; i++)
    {
        auto &vi = cube.vertices[i];
        auto &vf = newCube.vertices[i];

        vf.x = vi.x * cos(A) - vi.y * sin(A);
        vf.y = vi.x * sin(A) + vi.y * cos(A);
    }
}

void transform(Cube &newCube, Cube &two_DCube)
{

    for (int i = 0; i < 8; i++)
    {
        auto &vi = newCube.vertices[i];
        auto &vf = two_DCube.vertices[i];

        vf.x = CUBE_X + (vi.x * FOV) / (VIEWER_DISTANCE + vi.z);
        vf.y = CUBE_Y - (vi.y * FOV) / (VIEWER_DISTANCE + vi.z);
        vf.z = 0;
    }
}

void renderCube(SDL_Renderer *renderer, Cube &two_DCube)
{

    for (int i = 0; i < 12; i++)
    {
        Vec3 v1 = two_DCube.vertices[two_DCube.edges[i][0]];

        Vec3 v2 = two_DCube.vertices[two_DCube.edges[i][1]];

        SDL_RenderDrawLine(renderer, v1.x, v1.y, v2.x, v2.y);
    }
}
