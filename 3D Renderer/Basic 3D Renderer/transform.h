#pragma once

#include "vector3.h"
  
struct Transform
{
    Vector3 position = {0,0,0};

    Vector3 rotation {0,0,0};    

    Vector3 forward = {0,0,1}, right = {1,0,0}, up = {0,1,0};
};