#include "SDL2/sdl.h"
#include <vector>
#include "vector3.h"
#include <algorithm>

class Screen
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SDL_FPoint> points;
    std::vector<Vector3> pointColors;
public : 
    SDL_Event e;
    bool rotateFaster = false;
    int WIDTH = 500;
    int HEIGHT = 500;
    int SCALE = 1;


    Screen()
    {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(WIDTH * SCALE, HEIGHT * SCALE, 0, &window, &renderer);
        SDL_RenderSetScale(renderer, SCALE, SCALE);
    }

    Screen(const int& width, const int& height, const int& scale = 1)
    {
        WIDTH = width;
        HEIGHT = height;
        SCALE = scale;
        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(WIDTH * SCALE, HEIGHT * SCALE, 0, &window, &renderer);
        SDL_RenderSetScale(renderer, SCALE, SCALE);        
    }

    void pixel(float x, float y, Vector3 color = {255, 255, 255})
    {
        points.emplace_back(SDL_FPoint{ x, y });
        pointColors.emplace_back(color);
    }

    void show()
    {
        //Makes the color black
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);

        for(int i = 0; i < points.size(); i++)
        {
            const SDL_FPoint&point = points[i];
            const Vector3&color = pointColors[i];
            SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, 255);
            SDL_RenderDrawPoint(renderer, point.x, point.y);
        }

        SDL_RenderPresent(renderer);
    }

    void fillPolygon(int polyCorners, std::vector<float> polyX, std::vector<float> polyY, Vector3 color = {255, 255, 255})
    {
        int nodes, nodeX[10], pixelX, pixelY, i, j, swap;
        for (pixelY = 0; pixelY < HEIGHT; pixelY++) {
            nodes = 0;
            j = polyCorners - 1;
            for (i = 0; i < polyCorners; i++) {
                if ((polyY[i] < static_cast<float>(pixelY) && polyY[j] >= static_cast<float>(pixelY)) ||
                    (polyY[j] < static_cast<float>(pixelY) && polyY[i] >= static_cast<float>(pixelY))) {
                    nodeX[nodes++] = static_cast<int>(polyX[i] + (pixelY - polyY[i]) / (polyY[j] - polyY[i]) * (polyX[j] - polyX[i]));
                }
                j = i;
            }

            i = 0;
            while (i < nodes - 1) {
                if (nodeX[i] > nodeX[i + 1]) {
                    swap = nodeX[i];
                    nodeX[i] = nodeX[i + 1];
                    nodeX[i + 1] = swap;
                    if (i)
                        i--;
                }
                else {
                    i++;
                }
            }

            for (i = 0; i < nodes; i += 2) {
                if (nodeX[i] >= WIDTH)
                    break;
                if (nodeX[i + 1] > 0) {
                    if (nodeX[i] < 0)
                        nodeX[i] = 0;
                    if (nodeX[i + 1] > WIDTH)
                        nodeX[i + 1] = WIDTH;
                    for (pixelX = nodeX[i]; pixelX < nodeX[i + 1]; pixelX++)
                        pixel(pixelX, pixelY, color); // Use the Screen's pixel function to fill pixels
                }
            }
        }
    }

    Vector3 GetCurrentColor()
    {
        Vector3 curColor;
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
        curColor.x = std::floor(static_cast<float>(r));
        curColor.y = std::floor(static_cast<float>(g));
        curColor.z = std::floor(static_cast<float>(b));

        return curColor;
    }

    void clear()
    {
        points.clear();
        pointColors.clear();
    }


};