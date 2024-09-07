#pragma once
#include "vector3.h"
#include "transform.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "matrix.h"

class Object
{
public:
    struct Tri
    {
        Vector3 a,b,c = {0,0,0};
    };

    struct Mesh
    {
        std::vector<Tri> tris;
    };



    std::vector<Vector3> vertices;

    Transform transform;

    Mesh mesh;

    Vector3 velocity{0,0,0};

    Vector3 angularVelocity{0,0,0};

    Vector3 centroid{0,0,0};


    Vector3 minBounds {INFINITY,INFINITY,INFINITY};
    Vector3 maxBounds {-INFINITY,-INFINITY,-INFINITY};

    mat4x4 translationMatrix;
    mat4x4 rotationMatrix = mat4x4::MakeRotation({0,0,0});
    mat4x4 transformMatrix;

    bool intersected = false;

    Object(Vector3 _position, float sideLength)
    {
        transform.position = _position;
        translationMatrix = mat4x4::MakeTranslation(transform.position);
        transformMatrix = rotationMatrix*translationMatrix;
        GenerateSquare(transform.position, sideLength);
        transform.forward = centroid + Vector3{0,0,20};
        transform.right = centroid + Vector3{20,0,0};
        transform.up = centroid + Vector3{0,-20, 0};
        transform.position = centroid;
    }

    Object(std::vector<Vector3> _vertices, std::vector<Tri> _tris, Vector3 _position = {0,0,0})
    { 
  
        vertices = _vertices;
        mesh.tris = _tris;

    }   

    void updateTransform()
    {
        transformMatrix = rotationMatrix*translationMatrix;
    }

    void updateMesh()
    {
        sort(mesh.tris.begin(), mesh.tris.end(), [this](Tri& tri1, Tri& tri2)
        {
            Tri transTri1, transTri2;
            transTri1 = {transformMatrix * tri1.a, transformMatrix * tri1.b, transformMatrix * tri1.c};
            transTri2 = {transformMatrix * tri2.a, transformMatrix * tri2.b, transformMatrix * tri2.c};
            float zTri1 = (transTri1.a.z + transTri1.b.z + transTri1.c.z) / 3;
            float zTri2 = (transTri2.a.z + transTri2.b.z + transTri2.c.z) / 3;
            return zTri1 > zTri2;
        });
    }


    void GenerateSquare(Vector3 position, float sideLength)
    {
        for(int x = 0; x < 2; x++)
        {
            for(int y = 0; y < 2; y++)
            {
                for(int z = 0; z < 2; z++)
                {
                    Vector3 vertex(x,y,z);
                    vertices.push_back(position + Vector3{static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)}*sideLength - Vector3{sideLength, sideLength, sideLength}/2);
                }
            }
        }
        mesh.tris =
        {
            {vertices[0],vertices[2],vertices[6]},{vertices[0],vertices[6],vertices[4]},
            {vertices[4],vertices[6],vertices[7]},{vertices[4],vertices[7],vertices[5]},
            {vertices[5],vertices[7],vertices[3]},{vertices[5],vertices[3],vertices[1]},
            {vertices[1],vertices[3],vertices[2]},{vertices[1],vertices[2],vertices[0]},
            {vertices[1],vertices[0],vertices[4]},{vertices[1],vertices[4],vertices[5]},
            {vertices[2],vertices[3],vertices[7]},{vertices[2],vertices[7],vertices[6]}
        };
        CalcBoundingBox(vertices);

        updateMesh();



        centroid = CalcCentroid(vertices);
    }

    Vector3 CalcCentroid(std::vector<Vector3>& vertices)
    {
        Vector3 centeroid{0,0,0};

        for(auto& vertex : vertices)
        {
            centeroid += vertex;
        }

        centeroid/=vertices.size();
        return centeroid;
    }

    void CalcBoundingBox(std::vector<Vector3>& vertices)
    {
        minBounds = {INFINITY,INFINITY,INFINITY};
        maxBounds = {-INFINITY,-INFINITY,-INFINITY};
        for (const auto& vertex : vertices) 
        {
            minBounds.x = std::min(minBounds.x, vertex.x);
            minBounds.y = std::min(minBounds.y, vertex.y);
            minBounds.z = std::min(minBounds.z, vertex.z);

            maxBounds.x = std::max(maxBounds.x, vertex.x);
            maxBounds.y = std::max(maxBounds.y, vertex.y);
            maxBounds.z = std::max(maxBounds.z, vertex.z);
        }
    }

    bool isSeparatingAxis(Vector3 minA, Vector3 maxA, Vector3 minB, Vector3 maxB) 
    {
        Vector3 minACopy = minA;
        minA.x = std::min(minA.x, maxA.x);
        minA.y = std::min(minA.y, maxA.y);
        minA.z = std::min(minA.z, maxA.z);

        maxA.x = std::max(maxA.x, minACopy.x);
        maxA.y = std::max(maxA.y, minACopy.y);
        maxA.z = std::max(maxA.z, minACopy.z);

        Vector3 minBCopy = minB;
        minB.x = std::min(minB.x, maxB.x);
        minB.y = std::min(minB.y, maxB.y);
        minB.z = std::min(minB.z, maxB.z);

        maxB.x = std::max(maxB.x, minBCopy.x);
        maxB.y = std::max(maxB.y, minBCopy.y);
        maxB.z = std::max(maxB.z, minBCopy.z);
        return (minA.x > maxB.x || maxA.x < minB.x ||
                minA.y > maxB.y || maxA.y < minB.y ||
                minA.z > maxB.z || maxA.z < minB.z);
    }

    bool isIntersecting(const Vector3& minA, const Vector3& maxA, const Vector3& minB, const Vector3& maxB) 
    {
        return !isSeparatingAxis(minA, maxA, minB, maxB);
    }

    void handleCollision(const Vector3& minA, const Vector3& maxA, const Vector3& minB, const Vector3& maxB)
    {
        if(isIntersecting(minA, maxA, minB, maxB) || centroid.x < 0 || centroid.x > 2000 || centroid.y < 0 || centroid.y > 2000)
        {
            if(!intersected)
            {
                velocity *= -1;
                intersected = true;
            }
        }
        else
        {
            intersected = false;
        }
    }

    void rotate()
    {
        rotate(angularVelocity.x,angularVelocity.y, angularVelocity.z);
    }

    void translate()
    {
        translate(velocity);
    }

    void rotate(float angleX, float angleY, float angleZ)
    {
        //forward point rotation
        rotationMatrix = mat4x4::MakeRotation(transform.rotation += Vector3{angleX, angleY, angleZ});
        transform.rotation %= 360;
        rotationMatrix = mat4x4::MakeTranslation(-centroid.x, -centroid.y, -centroid.z) * rotationMatrix;     
 
        updateTransform();
        updateMesh();

    }

    void translate(const Vector3& distance)
    {
        //transform.position += distance;
        translationMatrix = mat4x4::MakeTranslation(transform.position += distance);
        centroid += distance;
        for(auto& tri : mesh.tris)
        {
            translate(tri.a, distance);
            translate(tri.b, distance);
            translate(tri.c, distance);
        }
        updateTransform();
        updateMesh();

    }

    void rotate(Vector3& point, float angleX, float angleY, float angleZ) 
    {
        point -= centroid;

        // Convert angles to radians
        float radX = angleX * M_PI / 180.0f;
        float radY = angleY * M_PI / 180.0f;
        float radZ = angleZ * M_PI / 180.0f;

        // Calculate sin and cos values
        float cosX = std::cos(radX);
        float sinX = std::sin(radX);
        float cosY = std::cos(radY);
        float sinY = std::sin(radY);
        float cosZ = std::cos(radZ);
        float sinZ = std::sin(radZ);

        // Calculate combined rotation matrix
        float combinedMatrix[3][3] = {
            {cosY * cosZ, -cosX * sinZ + sinX * sinY * cosZ, sinX * sinZ + cosX * sinY * cosZ},
            {cosY * sinZ, cosX * cosZ + sinX * sinY * sinZ, -sinX * cosZ + cosX * sinY * sinZ},
            {-sinY, sinX * cosY, cosX * cosY}
        };

        // Apply combined matrix to the point
        float tempX = point.x;
        float tempY = point.y;
        float tempZ = point.z;

        point.x = combinedMatrix[0][0] * tempX + combinedMatrix[0][1] * tempY + combinedMatrix[0][2] * tempZ;
        point.y = combinedMatrix[1][0] * tempX + combinedMatrix[1][1] * tempY + combinedMatrix[1][2] * tempZ;
        point.z = combinedMatrix[2][0] * tempX + combinedMatrix[2][1] * tempY + combinedMatrix[2][2] * tempZ;
        point += centroid;

    }

    void translate(Vector3& point, const Vector3& distance)
    {
        point += distance;
    }

};