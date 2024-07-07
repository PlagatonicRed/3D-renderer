#include "screen.h"
#include <numeric>
#include <cmath>
#include <iostream>
#include <unordered_map>

bool gameStarted = false;
int delay = 100;

void input(Screen& screen)
{
    while(SDL_PollEvent(&screen.e))
    {
        if(screen.e.type == SDL_QUIT)
        {
            SDL_Quit();
            exit(0);
        }
        if(screen.e.type == SDL_KEYDOWN)
        {
            switch(screen.e.key.keysym.sym)
            {
                case SDLK_f:
                    gameStarted = true;
                    break;
                
                case SDLK_r:
                    gameStarted = false;
                    screen.clear();
                    break;

                case SDLK_1:
                    // 10 fps
                    delay = 100;
                    break;

                case SDLK_2:
                    // 20 fps
                    delay = 50;

                case SDLK_3:
                    //~ 30fps
                    delay = 33;
                    break;
                
                case SDLK_4:
                    // 40 fps
                    delay = 25;
                    break;

                case SDLK_5:
                    // 60 fps
                    delay = 16;
                    break;

                case SDLK_6:
                    // 100 fps
                    delay = 10;
                    break;
                
                case SDLK_7:
                    // 200 fps
                    delay = 5;
                    break;

                case SDLK_8:
                    // UNLIMITED POWER
                    delay = 0;
                    break;

            }
        }
        if (screen.e.type == SDL_MOUSEBUTTONDOWN) 
        {
            int mouseX, mouseY;
            Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

            screen.pixel(mouseX/screen.SCALE,mouseY/screen.SCALE);
        }
        else if (screen.e.type == SDL_MOUSEMOTION && screen.e.button.button == SDL_BUTTON_LEFT) 
        {
            int mouseX, mouseY;
            Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
            screen.pixel(mouseX/screen.SCALE, mouseY/screen.SCALE);
        }
    }
}

int main(int argc, char** argv)
{
    int width;
    int height;
    int scale;
    std::cin >> width >> height >> scale;
    Screen screen(width, height, scale);

    while (true)
    {
        input(screen); // Handle user input

        screen.show(); // Render the current state

        if (gameStarted)
        {
            // Collect coordinates of live cells
            std::vector<Vector2Int> liveCells;
            std::unordered_map<Vector2Int, int, Vector2IntHash> deadNeighborCount;

            for (const auto& pair : screen.points)
            {
                const auto& point = pair.first;
                liveCells.push_back(point);
            }

            std::vector<Vector2Int> pixelsToAdd;
            std::vector<Vector2Int> pixelsToRemove;

            for (const auto& liveCell : liveCells)
            {
                int liveNeighbors = 0;
                for (int i = liveCell.x - 1; i <= liveCell.x + 1; ++i)
                {
                    for (int j = liveCell.y - 1; j <= liveCell.y + 1; ++j)
                    {
                        if (i == liveCell.x && j == liveCell.y) continue; // Skip the live cell itself

                        Vector2Int neighbor(i, j);
                        if (screen.points.find(neighbor) != screen.points.end())
                        {
                            liveNeighbors++;
                        }
                        else
                        {
                            // Increment counter for dead neighbor
                            ++deadNeighborCount[neighbor];
                        }
                    }
                }

                // Apply rules of the Game of Life
                if (liveNeighbors < 2 || liveNeighbors > 3)
                {
                    pixelsToRemove.emplace_back(liveCell.x, liveCell.y);
                }
            }

            for(const auto& pair : deadNeighborCount)
            {
                const auto& cell = pair.first;
                if(pair.second == 3)
                {
                    // If a dead cell has exactly 3 live neighbors, mark it for addition
                    pixelsToAdd.emplace_back(cell.x, cell.y);
                }
            }

            // Add new pixels
            for (const auto& pixel : pixelsToAdd)
            {
                screen.pixel(pixel.x, pixel.y);
            }

            // Remove pixels
            for (const auto& pixel : pixelsToRemove)
            {
                screen.removePixel(pixel.x, pixel.y);
            }
        }

        // Add a delay to control the speed of the simulation
        SDL_Delay(delay);
    }

    return 0;
}
