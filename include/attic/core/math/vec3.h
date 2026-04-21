#pragma once
#include <cmath>

namespace attic {
namespace math {

struct Vec3 {
    float x, y, z;

    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    Vec3 operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
    Vec3 operator-(const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
    Vec3 operator-() const { return Vec3(-x, -y, -z); }
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
    Vec3 operator/(float s) const { return Vec3(x / s, y / s, z / s); }

    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    Vec3& operator-=(const Vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    Vec3& operator*=(float s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    bool operator==(const Vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    bool operator!=(const Vec3& other) const { return !(*this == other); }

    float operator[](int i) const { return (&x)[i]; }
    float& operator[](int i) { return (&x)[i]; }

    float length_squared() const { return x * x + y * y + z * z; }
    float length() const { return std::sqrt(length_squared()); }
    Vec3 normalized() const {
        float len = length();
        return len > 0.0f ? (*this) / len : Vec3(0, 0, 0);
    }
    Vec3& normalize() {
        float len = length();
        if (len > 0.0f) {
            x /= len;
            y /= len;
            z /= len;
        }
        return *this;
    }

    float dot(const Vec3& other) const { return x * other.x + y * other.y + z * other.z; }
    Vec3 cross(const Vec3& other) const {
        return Vec3(y * other.z - z * other.y, z * other.x - x * other.z,
                    x * other.y - y * other.x);
    }

    Vec3 lerp(const Vec3& other, float t) const { return *this * (1.0f - t) + other * t; }

    static Vec3 zero() { return Vec3(0, 0, 0); }
    static Vec3 one() { return Vec3(1, 1, 1); }
    static Vec3 unit_x() { return Vec3(1, 0, 0); }
    static Vec3 unit_y() { return Vec3(0, 1, 0); }
    static Vec3 unit_z() { return Vec3(0, 0, 1); }
};

inline Vec3 operator*(float s, const Vec3& v) {
    return v * s;
}
inline float dot(const Vec3& a, const Vec3& b) {
    return a.dot(b);
}
inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return a.cross(b);
}
inline float distance(const Vec3& a, const Vec3& b) {
    return (a - b).length();
}
inline Vec3 normalize(const Vec3& v) {
    return v.normalized();
}
inline Vec3 lerp(const Vec3& a, const Vec3& b, float t) {
    return a.lerp(b, t);
}

} // namespace math
} // namespace attic
