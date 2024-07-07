#pragma once

#include <cmath>
#include <iostream>


struct Vector3
{
    float x,y,z = 0;

    float w = 1;

    Vector3() = default;

    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    //* the const modifier at the end of the operator functions basically
    //* state that the functions do not alter the objects within the function
    //* this is why the = operator isn't const because it assigns a new value to the object it is called on
    float magnitude() const
    {
        return std::sqrt(x*x + y*y + z*z);
    }

    static float dot(const Vector3& vecA, const Vector3& vecB)
    {
        return vecA.x*vecB.x + vecA.y*vecB.y + vecA.z*vecB.z;
    }

    static Vector3 cross(const Vector3& vecA, const Vector3& vecB)
    {
        Vector3 result;
        result.x = vecA.y*vecB.z - vecA.z*vecB.y;
        result.y = -(vecA.x*vecB.z-vecA.z*vecB.x);
        result.z = vecA.x*vecB.y - vecA.y*vecB.x;
        return result;
    }

    Vector3 normalized() const
    {
        float mag = magnitude();
        if(mag != 0)
        {
            return *this/(float)mag;
        }
        else
        {
            return Vector3();
        }
    }

    Vector3 operator*(float other) const
    {
        return Vector3{x*other,y*other,z*other};
    }

    Vector3 operator/(float other) const
    {
        return Vector3{x/other,y/other,z/other};
    }
    Vector3 operator%(int other) const
    {

        // Perform modulus operation on each component
        float resultX = fmodf(x, other);
        float resultY = fmodf(y, other);
        float resultZ = fmodf(z, other);

        return Vector3{resultX, resultY, resultZ};
    }

    Vector3 operator-(const Vector3& other) const
    {
        return Vector3{x-other.x, y-other.y, z-other.z};
    }

    Vector3 operator+(const Vector3& other) const
    {
        return Vector3{x+other.x, y+other.y, z+other.z};
    }

    Vector3& operator=(const Vector3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Vector3& operator*=(float other)
    {
        *this = *this*other;
        return *this;
    }

    Vector3& operator/=(float other)
    {
        *this = *this/other;
        return *this;
    }

    Vector3& operator%=(const float& other)
    {
        *this = *this%other;
        return *this;
    }

    Vector3& operator+=(const Vector3& other)
    {
        *this = *this+other;
        return *this;
    }

    Vector3& operator-=(const Vector3& other)
    {
        *this = *this-other;
        return *this;
    }


    bool operator==(const Vector3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vector3& other) const
    {
        return x != other.x || y != other.y || z != other.z;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& obj)
    {
        os << "(" << obj.x << ", " << obj.y << ", " << obj.z << ")";
        return os;
    }
};