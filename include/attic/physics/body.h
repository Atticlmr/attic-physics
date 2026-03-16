#pragma once
#include "attic/core/math/mat3.h"
#include "attic/core/math/quat.h"
#include "attic/core/math/vec3.h"

#include <string>

namespace attic {
namespace physics {

// Rigid body structure for physics simulation
// GPU-compatible: no STL containers in header, no virtual functions
struct Body {
    // Identity
    int id = -1;
    std::string name;

    // Linear state
    Vec3 position;     // Position in world space
    Vec3 velocity;     // Linear velocity (m/s)
    Vec3 acceleration; // Linear acceleration (m/s^2)

    // Mass properties
    float mass = 1.0f;     // Mass (kg)
    float inv_mass = 1.0f; // Inverse mass (1/mass), precomputed for performance

    // Rotational state
    Quat rotation;         // Orientation as quaternion
    Vec3 angular_velocity; // Angular velocity (rad/s)
    Vec3 torque;           // Applied torque (N*m)

    // Inertia tensor
    Mat3 inertia;     // Inertia tensor in body frame
    Mat3 inv_inertia; // Inverse inertia tensor

    // Damping coefficients
    float linear_damping = 0.0f;  // Linear velocity damping
    float angular_damping = 0.0f; // Angular velocity damping

    // External forces
    Vec3 force; // Total external force (N)

    // Collision
    float radius = 0.5f;    // Collision radius (sphere)
    bool is_static = false; // Static flag (infinite mass if true)

    // Default constructor - initializes to identity/default values
    Body()
        : position(Vec3::zero()), velocity(Vec3::zero()), acceleration(Vec3::zero()),
          rotation(Quat::identity()), angular_velocity(Vec3::zero()), torque(Vec3::zero()),
          inertia(Mat3()) // Default Mat3 constructor creates identity
          ,
          inv_inertia(Mat3()), force(Vec3::zero()) {}

    // Constructor with specified mass
    explicit Body(float mass_) : Body() { set_mass(mass_); }

    // Set mass and update inverse mass
    // Zero mass makes the body static
    void set_mass(float m) {
        mass = m;
        if (mass > 0.0f) {
            inv_mass = 1.0f / mass;
            is_static = false;
        } else {
            inv_mass = 0.0f;
            is_static = true;
        }
    }

    // Apply force at center of mass
    void apply_force(const Vec3& f) {
        if (!is_static)
            force += f;
    }

    // Apply force at a specific point (world coordinates)
    // Also generates torque: tau = r x F
    void apply_force_at(const Vec3& f, const Vec3& point) {
        if (is_static)
            return;
        force += f;
        Vec3 r = point - position;
        torque += cross(r, f);
    }

    // Apply torque
    void apply_torque(const Vec3& t) {
        if (!is_static)
            torque += t;
    }

    // Clear all forces and torques - call after each integration step
    void clear_forces() {
        force = Vec3::zero();
        torque = Vec3::zero();
    }

    // Compute linear acceleration: a = F / m
    Vec3 compute_acceleration() const { return force * inv_mass; }

    // Compute angular acceleration: alpha = I^(-1) * tau
    Vec3 compute_angular_acceleration() const { return inv_inertia * torque; }

    // Check if body is dynamic (not static and has positive mass)
    bool is_dynamic() const { return !is_static && mass > 0.0f; }
};

// Create a sphere body with proper inertia tensor
// For a solid sphere: I = (2/5) * m * r^2
inline Body create_sphere(float radius, float mass) {
    Body body(mass);
    body.radius = radius;
    float I = 0.4f * mass * radius * radius;
    body.inertia = Mat3::diagonal(I, I, I);
    body.inv_inertia = Mat3::diagonal(1.0f / I, 1.0f / I, 1.0f / I);
    return body;
}

// Create a static body (zero mass, immovable)
inline Body create_static(const Vec3& pos = Vec3::zero()) {
    Body body(0.0f);
    body.position = pos;
    return body;
}

} // namespace physics
} // namespace attic
