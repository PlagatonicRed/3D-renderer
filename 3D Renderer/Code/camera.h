#pragma once
#include "matrix.h"
#include "transform.h"

#include <cmath>

class Camera
{
public:

    Camera(const int& width, const int& height) : fAspectRatio(static_cast<float>(width) / height) 
    {
        matProj = mat4x4::MakeProjection(fFovRad, fAspectRatio, fNear, fFar);
    }

    Transform transform;


    mat4x4 matProj;
    mat4x4 camMat = mat4x4::Inverse(mat4x4::MakePointAt(transform.position, transform.forward, transform.up));
    void UpdateCamMat()
    {
        mat4x4 rotation = mat4x4::MakeRotation(transform.rotation);
        camMat = mat4x4::Inverse(mat4x4::MakePointAt(transform.position, (rotation*(transform.forward)).normalized() + transform.position, transform.up));
    }
    
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;

    float fFovRad = 1.0f/std::tan(fFov*0.5f/180.0f*3.14159f);

private:
    float fAspectRatio;

};