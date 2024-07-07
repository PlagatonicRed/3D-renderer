#include <SDL2/SDL.h>
#include <vector>

class Screen
{
public : 

    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SDL_FPoint> points;
    SDL_Event e;
    int SCALE = 1;
    int HEIGHT = 1000;
    int WIDTH = 1000;
    bool generateFractal = false;
    bool fractalGenerated = true;
    bool iterateFractals = false;


    Screen()
    {
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_CreateWindowAndRenderer(WIDTH*SCALE,HEIGHT*SCALE,0,&window, &renderer);
        SDL_RenderSetScale(renderer, SCALE, SCALE);
    }

    Screen(const int width, const int height)
    {
        WIDTH = width;
        HEIGHT = height;
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_CreateWindowAndRenderer(WIDTH*SCALE,HEIGHT*SCALE,0,&window, &renderer);
        SDL_RenderSetScale(renderer, SCALE, SCALE);
    }

    Screen(const int width, const int height, const int scale)
    {
        SCALE = scale;
        WIDTH = width;
        HEIGHT = height;
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_CreateWindowAndRenderer(WIDTH*SCALE,HEIGHT*SCALE,0,&window, &renderer);
        SDL_RenderSetScale(renderer, SCALE, SCALE);
    }

    void pixel(float x, float y)
    {
        //creates a point struct and adds it to the points vector
        points.emplace_back(SDL_FPoint{ x, y });
    }

    void show()
    {
        SDL_RenderPresent(renderer);
    }

    void clear()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        show();
    }

    void CheckQuit()
    {
        if(e.type == SDL_QUIT)
        {
            SDL_Quit();
            exit(0);
        }
    }

    void CheckKeys()
    {
        if(e.type == SDL_KEYDOWN)
        {
            switch(e.key.keysym.sym)
            {
                case SDLK_r:
                    clear();
                    fractalGenerated = false;
                    break;
                case SDLK_f:
                    generateFractal = true;
                    break;
                case SDLK_g:
                    iterateFractals = !iterateFractals;
                    break;

            }
        }
    }

    void Input()
    {
        while(SDL_PollEvent(&e))
        {
            CheckQuit();
            CheckKeys();
        }
    }


};