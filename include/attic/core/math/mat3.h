#pragma once
#include "attic/core/math/vec3.h"
namespace attic {
namespace math {

struct Mat3 {
    float m[3][3];

    Mat3() {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                m[i][j] = (i == j) ? 1.0f : 0.0f;
    }

    Mat3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21,
         float m22) {
        m[0][0] = m00;
        m[0][1] = m01;
        m[0][2] = m02;
        m[1][0] = m10;
        m[1][1] = m11;
        m[1][2] = m12;
        m[2][0] = m20;
        m[2][1] = m21;
        m[2][2] = m22;
    }

    Vec3 operator*(const Vec3& v) const {
        return Vec3(m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
                    m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
                    m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
    }

    Mat3 operator*(const Mat3& other) const {
        Mat3 result;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                result.m[i][j] =
                    m[i][0] * other.m[0][j] + m[i][1] * other.m[1][j] + m[i][2] * other.m[2][j];
        return result;
    }

    Mat3 transpose() const {
        return Mat3(m[0][0], m[1][0], m[2][0], m[0][1], m[1][1], m[2][1], m[0][2], m[1][2],
                    m[2][2]);
    }

    static Mat3 diagonal(float x, float y, float z) {
        Mat3 m;
        m.m[0][0] = x;
        m.m[1][1] = y;
        m.m[2][2] = z;
        return m;
    }

    static Mat3 zero() {
        Mat3 m;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                m.m[i][j] = 0.0f;
        return m;
    }
};

} // namespace math
} // namespace attic
