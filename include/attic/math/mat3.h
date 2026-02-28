#pragma once
#include "vec3.h"

namespace atp {

// 3x3矩阵，用于旋转、惯性张量等
struct Mat3 {
    float m[3][3];

    // 构造函数
    Mat3() {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                m[i][j] = (i == j) ? 1.0f : 0.0f;
    }
    
    Mat3(float m00, float m01, float m02,
         float m10, float m11, float m12,
         float m20, float m21, float m22) {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
    }

    // 行访问
    Vec3 row(int i) const {
        return Vec3(m[i][0], m[i][1], m[i][2]);
    }
    
    // 列访问
    Vec3 col(int j) const {
        return Vec3(m[0][j], m[1][j], m[2][j]);
    }

    // 矩阵*向量
    Vec3 operator*(const Vec3& v) const {
        return Vec3(
            m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z,
            m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z,
            m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z
        );
    }

    // 矩阵*矩阵
    Mat3 operator*(const Mat3& other) const {
        Mat3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = m[i][0]*other.m[0][j] + 
                                m[i][1]*other.m[1][j] + 
                                m[i][2]*other.m[2][j];
            }
        }
        return result;
    }

    // 矩阵*标量
    Mat3 operator*(float s) const {
        Mat3 result;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                result.m[i][j] = m[i][j] * s;
        return result;
    }

    // 矩阵加法
    Mat3 operator+(const Mat3& other) const {
        Mat3 result;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                result.m[i][j] = m[i][j] + other.m[i][j];
        return result;
    }

    // 转置
    Mat3 transpose() const {
        return Mat3(
            m[0][0], m[1][0], m[2][0],
            m[0][1], m[1][1], m[2][1],
            m[0][2], m[1][2], m[2][2]
        );
    }

    // 行列式
    float determinant() const {
        return m[0][0] * (m[1][1]*m[2][2] - m[1][2]*m[2][1]) -
               m[0][1] * (m[1][0]*m[2][2] - m[1][2]*m[2][0]) +
               m[0][2] * (m[1][0]*m[2][1] - m[1][1]*m[2][0]);
    }

    // 逆矩阵
    Mat3 inverse() const {
        float det = determinant();
        if (std::abs(det) < 1e-6f) return Mat3(); // 返回单位矩阵
        
        float inv_det = 1.0f / det;
        Mat3 result;
        result.m[0][0] = (m[1][1]*m[2][2] - m[1][2]*m[2][1]) * inv_det;
        result.m[0][1] = (m[0][2]*m[2][1] - m[0][1]*m[2][2]) * inv_det;
        result.m[0][2] = (m[0][1]*m[1][2] - m[0][2]*m[1][1]) * inv_det;
        result.m[1][0] = (m[1][2]*m[2][0] - m[1][0]*m[2][2]) * inv_det;
        result.m[1][1] = (m[0][0]*m[2][2] - m[0][2]*m[2][0]) * inv_det;
        result.m[1][2] = (m[0][2]*m[1][0] - m[0][0]*m[1][2]) * inv_det;
        result.m[2][0] = (m[1][0]*m[2][1] - m[1][1]*m[2][0]) * inv_det;
        result.m[2][1] = (m[0][1]*m[2][0] - m[0][0]*m[2][1]) * inv_det;
        result.m[2][2] = (m[0][0]*m[1][1] - m[0][1]*m[1][0]) * inv_det;
        return result;
    }

    // 对角矩阵
    static Mat3 diagonal(float x, float y, float z) {
        Mat3 m;
        m.m[0][0] = x; m.m[1][1] = y; m.m[2][2] = z;
        return m;
    }
    
    static Mat3 diagonal(const Vec3& v) {
        return diagonal(v.x, v.y, v.z);
    }

    // 零矩阵
    static Mat3 zero() {
        Mat3 m;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                m.m[i][j] = 0.0f;
        return m;
    }

    // 外积
    static Mat3 outer_product(const Vec3& a, const Vec3& b) {
        Mat3 m;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                m.m[i][j] = a[i] * b[j];
        return m;
    }

    // 反对称矩阵 (用于角速度转叉积)
    static Mat3 skew_symmetric(const Vec3& v) {
        return Mat3(
            0.0f, -v.z,  v.y,
             v.z,  0.0f, -v.x,
            -v.y,  v.x,  0.0f
        );
    }
};

inline Mat3 operator*(float s, const Mat3& m) { return m * s; }

} // namespace atp
