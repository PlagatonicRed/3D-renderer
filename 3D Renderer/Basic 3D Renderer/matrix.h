#pragma once
#include "vector3.h"

struct mat4x4
{
    float m[4][4] = {0};

    mat4x4() 
    {
        for (int i = 0; i < 4; ++i) {
            m[i][i] = 1.0f;
        }
    }

    static mat4x4 MakeRotation(const Vector3& angle)
    {
        return MakeRotation(angle.x, angle.y, angle.z);
    }

    static mat4x4 MakeTranslation(const Vector3& position)
    {
        return MakeTranslation(position.x, position.y, position.z);
    }

    static mat4x4 MakeRotation(const float& angleX, const float& angleY, const float& angleZ) 
    {
        mat4x4 result;
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
        result.m[0][0] = cosY * cosZ;
        result.m[0][1] = -cosX * sinZ + sinX * sinY * cosZ;
        result.m[0][2] = sinX * sinZ + cosX * sinY * cosZ;
        result.m[0][3] = 0.0f;

        result.m[1][0] = cosY * sinZ;
        result.m[1][1] = cosX * cosZ + sinX * sinY * sinZ;
        result.m[1][2] = -sinX * cosZ + cosX * sinY * sinZ;
        result.m[1][3] = 0.0f;

        result.m[2][0] = -sinY;
        result.m[2][1] = sinX * cosY;
        result.m[2][2] = cosX * cosY;
        result.m[2][3] = 0.0f;

        result.m[3][0] = 0.0f;
        result.m[3][1] = 0.0f;
        result.m[3][2] = 0.0f;
        result.m[3][3] = 1.0f;

        return result;
    }
    
    static mat4x4 MakeTranslation(const float& x, const float& y, const float& z)
    {
        mat4x4 result;
        result.m[3][0] = x;
        result.m[3][1] = y;
        result.m[3][2] = z;
        return result;
    }

    static mat4x4 MakeProjection(const float& fFovDegrees, const float& fAspectRatio, const float& fNear, const float& fFar)
    {
        mat4x4 projection;
        projection.m[0][0] = fAspectRatio * fFovDegrees;
        projection.m[1][1] = fFovDegrees;
        projection.m[2][2] = fFar / (fFar - fNear);
        projection.m[3][2] = (-fFar *fNear) / (fFar - fNear);
        projection.m[2][3] = 1.0f;
        projection.m[3][3] = 0.0f;
        return projection;

    }

    static mat4x4 MakePointAt(const Vector3& position, const Vector3& target, const Vector3& up)
    {
        Vector3 newForward = target - position;
        newForward = newForward.normalized();

        //TODO Ponder this a little bit more
        Vector3 a = newForward*Vector3::dot(up,newForward);
        Vector3 newUp = up - a;
        newUp = newUp.normalized();

        //finds the new right vector from the cross of the up and forward vectors
        Vector3 newRight = Vector3::cross(newUp, newForward);
        
        // constuct dimensioning and translation matrix
        //* Basically this is the transform to rotate a point in space in relation to the rotation of the observer
        //* In other words this matrix transforms a point in space to match the cameras view
        mat4x4 matrix;
        matrix.m[0][0] = newRight.x;    matrix.m[0][1] = newRight.y;    matrix.m[0][2] = newRight.z;    matrix.m[0][3] = 0; 
        matrix.m[1][0] = newUp.x;       matrix.m[1][1] = newUp.y;       matrix.m[1][2] = newUp.z;       matrix.m[1][3] = 0; 
        matrix.m[2][0] = newForward.x;  matrix.m[2][1] = newForward.y;  matrix.m[2][2] = newForward.z;  matrix.m[2][3] = 0; 
        matrix.m[3][0] = position.x;    matrix.m[3][1] = position.y;    matrix.m[3][2] = position.z;    matrix.m[3][3] = 1; 
        
        return matrix;
    }

	static mat4x4 Inverse(const mat4x4 &m) // Only for Rotation/Translation Matrices
	{
		mat4x4 matrix;
		matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}


    Vector3 operator*(const Vector3& other) const
    {
        Vector3 result;
        result.x = m[0][0]* other.x + m[1][0] * other.y + m[2][0] * other.z + m[3][0];
        result.y = m[0][1] * other.x + m[1][1] * other.y + m[2][1] * other.z + m[3][1];
        result.z = m[0][2] * other.x + m[1][2] * other.y + m[2][2] * other.z + m[3][2];
        result.w = m[0][3] * other.x + m[1][3] * other.y+ m[2][3] * other.z + m[3][3];

        if(result.w != 0)
        {
            result /= result.w;
        }
        return result;
    }

    mat4x4 operator*(const mat4x4& other) const
    {
        mat4x4 result;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                result.m[row][col] = 
                m[row][0] * other.m[0][col] +
                m[row][1] * other.m[1][col] +
                m[row][2] * other.m[2][col] +
                m[row][3] * other.m[3][col];
            }
        }

        return result;
    }

    bool operator==(const mat4x4& other) const
    {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (std::abs(m[i][j] - other.m[i][j]) > std::numeric_limits<float>::epsilon()) {
                    return false;
                }
            }
        }
        return true;
    }

    mat4x4& operator=(const mat4x4& other)
    {
        if (this == &other)
            return *this; // Handle self-assignment

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m[i][j] = other.m[i][j];
            }
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const mat4x4& mat)
    {
        os << "[";
        for (int i = 0; i < 4; ++i) {
            os << "[";
            for (int j = 0; j < 4; ++j) {
                os << mat.m[i][j];
                if (j < 3) os << ", ";
            }
            os << "]";
            if (i < 3) os << ",\n ";
        }
        os << "]";
        return os;
    }

};