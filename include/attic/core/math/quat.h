#pragma once
#include "attic/core/math/vec3.h"
#include "attic/core/math/mat3.h"
#include <cmath>

namespace attic {

struct Quat {
    float x, y, z, w;

    Quat() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Quat(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    
    Quat(const Vec3& axis, float angle) {
        float half_angle = angle * 0.5f;
        float s = std::sin(half_angle);
        x = axis.x * s;
        y = axis.y * s;
        z = axis.z * s;
        w = std::cos(half_angle);
    }

    Quat operator*(float s) const { return Quat(x * s, y * s, z * s, w * s); }
    
    Quat operator/(float s) const { return Quat(x / s, y / s, z / s, w / s); }
    
    Quat operator*(const Quat& other) const {
        return Quat(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }

    Vec3 rotate(const Vec3& v) const {
        Quat p(v.x, v.y, v.z, 0.0f);
        Quat result = (*this) * p * conjugate();
        return Vec3(result.x, result.y, result.z);
    }

    Quat conjugate() const { return Quat(-x, -y, -z, w); }
    Quat inverse() const { float len_sq = length_squared(); return conjugate() / len_sq; }

    Quat normalized() const { float len = length(); return len > 0.0f ? (*this) * (1.0f / len) : Quat(); }
    Quat& normalize() { float len = length(); if (len > 0.0f) { float inv = 1.0f/len; x*=inv; y*=inv; z*=inv; w*=inv; } return *this; }

    float length_squared() const { return x*x + y*y + z*z + w*w; }
    float length() const { return std::sqrt(length_squared()); }

    Mat3 to_mat3() const {
        float xx = x*x, yy = y*y, zz = z*z;
        float xy = x*y, xz = x*z, yz = y*z;
        float wx = w*x, wy = w*y, wz = w*z;
        return Mat3(
            1-2*(yy+zz), 2*(xy-wz), 2*(xz+wy),
            2*(xy+wz), 1-2*(xx+zz), 2*(yz-wx),
            2*(xz-wy), 2*(yz+wx), 1-2*(xx+yy)
        );
    }

    Quat slerp(const Quat& other, float t) const {
        float dot_product = x*other.x + y*other.y + z*other.z + w*other.w;
        Quat target = (dot_product < 0) ? Quat(-other.x,-other.y,-other.z,-other.w) : other;
        if (dot_product > 0.9995f) return Quat(x+(target.x-x)*t, y+(target.y-y)*t, z+(target.z-z)*t, w+(target.w-w)*t).normalized();
        float theta_0 = std::acos(dot_product);
        float theta = theta_0 * t;
        float s0 = std::cos(theta) - dot_product * std::sin(theta) / std::sin(theta_0);
        float s1 = std::sin(theta) / std::sin(theta_0);
        return Quat(x*s0+target.x*s1, y*s0+target.y*s1, z*s0+target.z*s1, w*s0+target.w*s1);
    }

    static Quat identity() { return Quat(); }
    static Quat from_axis_angle(const Vec3& axis, float angle) { return Quat(axis, angle); }
    static Quat from_mat3(const Mat3& mat);
};

inline Quat operator*(float s, const Quat& q) { return q * s; }
inline float dot(const Quat& a, const Quat& b) { return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w; }

} // namespace attic
