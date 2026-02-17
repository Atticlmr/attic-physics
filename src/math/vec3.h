#pragma once
#include <cmath>

namespace atp {

struct Vec3 {
    // 成员变量
    float x;
    float y;
    float z;

    // 构造函数
    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    
    // 运算符重载（+ - * /）
    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    
    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }
    
    Vec3 operator-() const {
        return Vec3(-x, -y, -z);
    }
    
    Vec3 operator*(float s) const {
        return Vec3(x * s, y * s, z * s);
    }
    
    Vec3 operator/(float s) const {
        return Vec3(x / s, y / s, z / s);
    }
    
    // 复合赋值
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
    
    // ===== 比较运算符（必须在 struct 内部！）=====
    bool operator==(const Vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    
    bool operator!=(const Vec3& other) const {
        return !(*this == other);
    }
    
    // 成员函数
    float length_squared() const {
        return x*x + y*y + z*z;
    }
    
    float length() const {
        return std::sqrt(length_squared());
    }
    
    Vec3 normalized() const {
        float len = length();
        if (len > 0.0f) {
            return (*this) / len;
        }
        return Vec3(0, 0, 0);
    }
};  // <--- 注意这里 struct 结束了，分号不能少！

// 非成员函数（在 struct 外面，但在 namespace 里面）
inline Vec3 operator*(float s, const Vec3& v) {
    return v * s;
}

inline float dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

inline float distance(const Vec3& a, const Vec3& b) {
    return (a - b).length();
}

inline Vec3 lerp(const Vec3& a, const Vec3& b, float t) {
    return a * (1.0f - t) + b * t;
}

} // namespace atp