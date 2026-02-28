#pragma once
#include "vec3.h"
#include "mat3.h"

namespace atp {

// 四元数，用于旋转表示
struct Quat {
    float x, y, z, w;  // 虚部(x,y,z)，实部(w)

    // 构造函数
    Quat() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}  // 单位四元数
    Quat(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    
    // 从角轴构造 (轴必须归一化)
    Quat(const Vec3& axis, float angle) {
        float half_angle = angle * 0.5f;
        float s = std::sin(half_angle);
        x = axis.x * s;
        y = axis.y * s;
        z = axis.z * s;
        w = std::cos(half_angle);
    }

    // 从欧拉角(弧度)构造 (ZYX顺序: Yaw-Pitch-Roll)
    static Quat from_euler(float yaw, float pitch, float roll) {
        float cy = std::cos(yaw * 0.5f), sy = std::sin(yaw * 0.5f);
        float cp = std::cos(pitch * 0.5f), sp = std::sin(pitch * 0.5f);
        float cr = std::cos(roll * 0.5f), sr = std::sin(roll * 0.5f);
        
        return Quat(
            cy * sp * cr + sy * cp * sr,  // x
            cy * cp * sr - sy * sp * cr,  // y
            sy * cp * cr - cy * sp * sr,  // z
            cy * cp * cr + sy * sp * sr   // w
        );
    }

    // 四元数乘法 (组合旋转: q2 * q1 表示先旋转q1再旋转q2)
    Quat operator*(const Quat& other) const {
        return Quat(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }

    // 向量旋转
    Vec3 rotate(const Vec3& v) const {
        // q * [0,v] * q_conjugate
        Quat p(v.x, v.y, v.z, 0.0f);
        Quat result = (*this) * p * conjugate();
        return Vec3(result.x, result.y, result.z);
    }

    // 共轭 (逆四元数对于单位四元数)
    Quat conjugate() const {
        return Quat(-x, -y, -z, w);
    }

    // 逆四元数
    Quat inverse() const {
        float len_sq = length_squared();
        if (len_sq > 0.0f) {
            float inv_len = 1.0f / len_sq;
            return Quat(-x * inv_len, -y * inv_len, -z * inv_len, w * inv_len);
        }
        return Quat();
    }

    // 归一化
    Quat normalized() const {
        float len = length();
        if (len > 0.0f) {
            float inv_len = 1.0f / len;
            return Quat(x * inv_len, y * inv_len, z * inv_len, w * inv_len);
        }
        return Quat();
    }

    Quat& normalize() {
        float len = length();
        if (len > 0.0f) {
            float inv_len = 1.0f / len;
            x *= inv_len; y *= inv_len; z *= inv_len; w *= inv_len;
        }
        return *this;
    }

    // 长度
    float length_squared() const { return x*x + y*y + z*z + w*w; }
    float length() const { return std::sqrt(length_squared()); }

    // 转3x3旋转矩阵
    Mat3 to_mat3() const {
        float xx = x * x, yy = y * y, zz = z * z;
        float xy = x * y, xz = x * z, yz = y * z;
        float wx = w * x, wy = w * y, wz = w * z;
        
        return Mat3(
            1.0f - 2.0f * (yy + zz), 2.0f * (xy - wz),     2.0f * (xz + wy),
            2.0f * (xy + wz),     1.0f - 2.0f * (xx + zz), 2.0f * (yz - wx),
            2.0f * (xz - wy),     2.0f * (yz + wx),     1.0f - 2.0f * (xx + yy)
        );
    }

    // 提取角轴
    void to_angle_axis(float& angle, Vec3& axis) const {
        float len = std::sqrt(x*x + y*y + z*z);
        if (len > 0.0f) {
            angle = 2.0f * std::atan2(len, w);
            float inv_len = 1.0f / len;
            axis = Vec3(x * inv_len, y * inv_len, z * inv_len);
        } else {
            angle = 0.0f;
            axis = Vec3::unit_x();
        }
    }

    // 球面插值
    Quat slerp(const Quat& other, float t) const {
        float dot_product = x * other.x + y * other.y + z * other.z + w * other.w;
        Quat target = other;
        
        // 如果点积为负，反转一个四元数以走最短路径
        if (dot_product < 0.0f) {
            dot_product = -dot_product;
            target = Quat(-other.x, -other.y, -other.z, -other.w);
        }
        
        // 如果非常接近，使用线性插值
        if (dot_product > 0.9995f) {
            Quat result = Quat(
                x + (target.x - x) * t,
                y + (target.y - y) * t,
                z + (target.z - z) * t,
                w + (target.w - w) * t
            );
            return result.normalized();
        }
        
        float theta_0 = std::acos(dot_product);
        float theta = theta_0 * t;
        float sin_theta = std::sin(theta);
        float sin_theta_0 = std::sin(theta_0);
        
        float s0 = std::cos(theta) - dot_product * sin_theta / sin_theta_0;
        float s1 = sin_theta / sin_theta_0;
        
        return Quat(
            x * s0 + target.x * s1,
            y * s0 + target.y * s1,
            z * s0 + target.z * s1,
            w * s0 + target.w * s1
        );
    }

    // 静态工厂
    static Quat identity() { return Quat(); }
    
    static Quat from_rotation_axis(const Vec3& axis, float angle) {
        return Quat(axis, angle);
    }

    // 从Mat3构造四元数
    static Quat from_mat3(const Mat3& mat) {
        float trace = mat.m[0][0] + mat.m[1][1] + mat.m[2][2];
        
        if (trace > 0.0f) {
            float s = std::sqrt(trace + 1.0f) * 2.0f;
            return Quat(
                (mat.m[2][1] - mat.m[1][2]) / s,
                (mat.m[0][2] - mat.m[2][0]) / s,
                (mat.m[1][0] - mat.m[0][1]) / s,
                0.25f * s
            );
        } else if (mat.m[0][0] > mat.m[1][1] && mat.m[0][0] > mat.m[2][2]) {
            float s = std::sqrt(1.0f + mat.m[0][0] - mat.m[1][1] - mat.m[2][2]) * 2.0f;
            return Quat(
                0.25f * s,
                (mat.m[0][1] + mat.m[1][0]) / s,
                (mat.m[0][2] + mat.m[2][0]) / s,
                (mat.m[2][1] - mat.m[1][2]) / s
            );
        } else if (mat.m[1][1] > mat.m[2][2]) {
            float s = std::sqrt(1.0f + mat.m[1][1] - mat.m[0][0] - mat.m[2][2]) * 2.0f;
            return Quat(
                (mat.m[0][1] + mat.m[1][0]) / s,
                0.25f * s,
                (mat.m[1][2] + mat.m[2][1]) / s,
                (mat.m[0][2] - mat.m[2][0]) / s
            );
        } else {
            float s = std::sqrt(1.0f + mat.m[2][2] - mat.m[0][0] - mat.m[1][1]) * 2.0f;
            return Quat(
                (mat.m[0][2] + mat.m[2][0]) / s,
                (mat.m[1][2] + mat.m[2][1]) / s,
                0.25f * s,
                (mat.m[1][0] - mat.m[0][1]) / s
            );
        }
    }
};

inline Quat operator*(float s, const Quat& q) {
    return Quat(q.x * s, q.y * s, q.z * s, q.w * s);
}

} // namespace atp
