#pragma once
#include "attic/core/math/mat3.h"
#include "attic/core/math/quat.h"
#include "attic/core/math/vec3.h"
#include "string"

namespace attic {
namespace physics {

struct Body {
    /// identity
    int id = -1;
    std::string name;

    /// Linear State
    Vec3 position;     // pos in world space
    Vec3 velocity;     // m/s
    Vec3 acceleration; // m/s^2

    // mass propetry
    float mass;
    float mass_inv; // 1/mass precomputed for better performance

    // Rotation
    Quat rotation;         // w,x,y,z
    Vec3 angular_velocity; // rad/s
    Vec3 torque;           // N*m

    // Interia Tensor
    Mat3 Interia;
    Mat3 Interia_inv;

    // Damping
    float linear_damping = 0.0f;
    float angular_damping = 0.0f;

    // external force
    Vec3 force; // N

    // collision: default collision is sphere
    float radius = 0.5f;    // collision radius
    bool is_static = false; // static body
};

} // namespace physics
} // namespace attic
