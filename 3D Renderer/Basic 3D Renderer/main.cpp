#include "shape.h"
#include "screen.h"
#include "vector3.h"
#include "camera.h"
#include <numeric>
#include <iostream>
#include <random>
#include <ctime>
#include <fstream>
#include <sstream>
bool rotatei = false;

bool isMouseLeftPressed = false;
bool isMouseRightPressed = false;


void input(Screen& screen, std::vector<Object>& objects, Camera& cam)
{
    while(SDL_PollEvent(&screen.e))
    {
        if(screen.e.type == SDL_QUIT)
        {
            SDL_Quit();
            exit(0);
        }
        //TODO change temp implementation
        for(auto& object : objects)
        {
            if(screen.e.type == SDL_KEYDOWN)
            {

                switch(screen.e.key.keysym.sym)
                {
                    case SDLK_LEFT:
                        rotatei = true;
                        break;

                    case SDLK_RIGHT:
                        rotatei = false;
                        break;

                    case SDLK_w:
                        cam.transform.position -= {0,1,0};
                        break;
                    case SDLK_a:
                        cam.transform.position -= {1,0,0};
                        break;
                    case SDLK_s:
                        cam.transform.position += {0,1,0};
                        break;
                    case SDLK_d:
                        cam.transform.position += {1,0,0};

                        break;
                    case SDLK_SPACE:
                        object.translate({0,0,3});
                        break;
                    case SDLK_LSHIFT:
                        cam.transform.position -= {0,0,1};
                        break;

                    
                }
                cam.UpdateCamMat();

            }
            if (screen.e.type == SDL_MOUSEBUTTONDOWN) {
                if (screen.e.button.button == SDL_BUTTON_LEFT) {
                    isMouseLeftPressed = true;
                }
                else if(screen.e.button.button == SDL_BUTTON_RIGHT)
                {
                    isMouseRightPressed = true;
                }
            }
            if (screen.e.type == SDL_MOUSEBUTTONUP) {
                if (screen.e.button.button == SDL_BUTTON_LEFT) {
                    isMouseLeftPressed = false;
                }
                else if(screen.e.button.button == SDL_BUTTON_RIGHT)
                {
                    isMouseRightPressed = false;
                }
            }
            if (isMouseLeftPressed && screen.e.type == SDL_MOUSEMOTION) {
                object.angularVelocity.y = (float)-screen.e.motion.xrel/10;
                object.angularVelocity.x = (float)screen.e.motion.yrel/10;
            }
            else if (isMouseRightPressed && screen.e.type == SDL_MOUSEMOTION) 
            {
                cam.transform.rotation.y += (float)-screen.e.motion.xrel/10;
                cam.transform.rotation.x += (float)screen.e.motion.yrel/10;
                cam.UpdateCamMat();
            }
            else
            {
                object.angularVelocity.y = 0;
                object.angularVelocity.x = 0;
            }
        }

    }
}

void line(Screen& screen, Vector3 pos1, Vector3 pos2, Vector3 color = {255,255,255})
{
    float dx = pos2.x - pos1.x;
    float dy = pos2.y - pos1.y;

    float length = std::sqrt(dx * dx + dy * dy);
    float angle = std::atan2(dy, dx);

    for (float i = 0; i < length; i++)
    {
        screen.pixel(pos1.x + std::cos(angle) * i, pos1.y + std::sin(angle) * i, color);
    }
}


float getRandomNumber(float min, float max) {
    static std::mt19937 mt(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}

bool ObjectLoaded(std::string sFileName)
{
    std::ifstream file(sFileName);
    if (!file.is_open())
    {
        return false;
    }

    return true;
}

Object LoadObject(std::string sFileName, Vector3 position = {0,0,0})
{
    if(ObjectLoaded(sFileName))
    {
        std::vector<Vector3> vertices;
        std::vector<Object::Tri> tris;
        std::ifstream file(sFileName);
        while(!file.eof())
        {
            char line[128];
            file.getline(line,128);

            std::stringstream s;
            // can populate stringstream in a similar manner to how you can populate cout
            s << line;

            char junk;

            if (line[0] == 'v')
            {
                Vector3 v;
                s >> junk >> v.x >> v.y >> v.z;
                vertices.push_back(v);
            }

            if (line[0] == 'f')
            {
                // a holder for the indexes of triangle vertexs
                int f[3];
                s >> junk >> f[0] >> f[1] >> f[2];
                tris.push_back({vertices[f[0]-1], vertices[f[1]-1], vertices[f[2]-1]});
            }

        }
        return Object(vertices, tris, position);
        
    }
}

int main(int argv, char** args)
{
    Screen screen(500,500, 1);
    Camera cam(250,250);

    std::vector<Object> objects;
    for(int i = 0; i < 5; i++)
    {
        Object group(Vector3{getRandomNumber(0,screen.WIDTH), getRandomNumber(0, screen.HEIGHT), 900}, getRandomNumber(25,50));
        group.velocity = {getRandomNumber(-4,4), getRandomNumber(-4,4), 0};
        objects.push_back(group);
    }

    Object teapot = LoadObject("teapot.obj", {1000,1000,6});
    objects.push_back(teapot);


    while(true)
    {
        for(auto& object : objects)
        {
            if(rotatei)
            {
                object.rotate(0, 2, 0);
            }
            else
            {
                object.rotate();         
            }
            object.translate();

            for(int i = 0; i < object.mesh.tris.size(); i++)
            {
                Object::Tri tri = object.mesh.tris[i];
                Vector3 lightDirection = Vector3{1,0,0}.normalized();
                Object::Tri triProjected, triTransformed, triView;

                triTransformed = {object.transformMatrix*tri.a, object.transformMatrix*tri.b, object.transformMatrix*tri.c};

                Vector3 normal = Vector3::cross(triTransformed.b - triTransformed.a,  triTransformed.a-triTransformed.c).normalized();
                triView = {cam.camMat*triTransformed.a, cam.camMat*triTransformed.b, cam.camMat*triTransformed.c};

                triProjected = {cam.matProj*triView.a, cam.matProj*triView.b, cam.matProj*triView.c};
                triProjected.a.x *= screen.WIDTH;
                triProjected.a.y *= screen.HEIGHT;
                triProjected.b.x *= screen.WIDTH;
                triProjected.b.y *= screen.HEIGHT;
                triProjected.c.x *= screen.WIDTH;
                triProjected.c.y *= screen.HEIGHT;

                if(Vector3::dot(normal, triView.a) < 0)
                {
                    continue;
                }

                Vector3 color = Vector3{255,255,255} * (Vector3::dot(normal, lightDirection) + 1)/2;
                screen.fillPolygon(3,{triProjected.a.x, triProjected.b.x, triProjected.c.x}, {triProjected.a.y, triProjected.b.y, triProjected.c.y}, color);

            }

            for(auto& obj : objects)
            {
                //! Will always skip if only one object
                if(&object == &obj)
                {
                    //std::cout << "Skipped";
                    continue;
                }
                object.handleCollision(object.minBounds, object.maxBounds, obj.minBounds, obj.maxBounds);
            }

        }

        screen.show();
        screen.clear();
        input(screen, objects, cam);
        SDL_Delay(30);
    }

    return 0;
}