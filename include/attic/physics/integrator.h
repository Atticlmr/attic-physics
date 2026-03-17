#pragma once
#include "attic/physics/body.h"

namespace attic {
namespace physics {

// Integrator interface for numerical integration of equations of motion
// Abstract base class - concrete implementations in integrators/ subdirectory
struct Integrator {
    virtual ~Integrator() = default;

    // Integrate linear motion (position and velocity)
    // Updates body.position and body.velocity based on body.force
    virtual void integrate(Body& body, float dt) = 0;

    // Integrate rotational motion (orientation and angular velocity)
    // Updates body.rotation and body.angular_velocity based on body.torque
    virtual void integrate_rotation(Body& body, float dt) = 0;

    // Full integration - both linear and rotational
    virtual void integrate_full(Body& body, float dt) {
        integrate(body, dt);
        integrate_rotation(body, dt);
    }

    // Get integrator name for debugging/logging
    virtual const char* name() const = 0;
};

} // namespace physics
} // namespace attic
