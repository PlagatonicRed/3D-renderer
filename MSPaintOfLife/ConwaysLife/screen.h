#include <SDL2/SDL.h>
#include <unordered_map>
#include <vector>

struct Vector2Int
{
    int x = 0;
    int y = 0;

    Vector2Int() = default; // Default constructor
    Vector2Int(int _x, int _y) : x(_x), y(_y) {}
    bool operator==(const Vector2Int& other) const
    {
        return x == other.x && y == other.y;
    }
};


// Custom hash function for Vector2Int
struct Vector2IntHash
{
    std::size_t operator()(const Vector2Int& vec) const
    {
        // Combine the hash values of x and y using XOR (^)
        return std::hash<int>()(vec.x) ^ std::hash<int>()(vec.y);
    }
};

class Screen
{
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    SDL_Event e;
    std::unordered_map<Vector2Int, SDL_Point, Vector2IntHash> points;
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

    void pixel(int x, int y)
    {
        // Create a Vector2Int key from integer coordinates
        Vector2Int key;
        key.x = static_cast<int>(x);
        key.y = static_cast<int>(y);

        // Add the point to the unordered_map
        points.insert_or_assign(key, SDL_Point{ x, y });
    }

    void removePixel(int x, int y)
    {
        Vector2Int point = { x, y };
        points.erase(point);
    }


    void show()
    {
        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the points
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (const auto& pair : points)
        {
            const auto& point = pair.second;
            SDL_RenderDrawPointF(renderer, point.x, point.y);
        }

        // Render the frame
        SDL_RenderPresent(renderer);
    }

    void clear()
    {
        points.clear();
    }

    ~Screen()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};
