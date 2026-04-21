#pragma once
#include "attic/physics/integrator.h"

namespace attic {
namespace physics {

// Semi-implicit Euler integrator (also called Symplectic Euler)
//
// This is the standard integrator used in game physics engines (Box2D, PhysX, MuJoCo)
// because it offers the best stability/cost trade-off.
//
// Algorithm:
//   v_{n+1} = v_n + a_n * dt
//   x_{n+1} = x_n + v_{n+1} * dt  <-- uses NEW velocity, not old
//
// Why it is stable:
// - Using the new velocity for position update makes the method symplectic
// - Energy error is O(dt^2) and bounded, unlike explicit Euler which gains energy
// - No additional memory required (unlike Verlet or RK4)
//
// Performance: O(1) per body, very cache-friendly
struct SemiImplicitEulerIntegrator : public Integrator {
    void integrate(Body& body, float dt) override {
        // Early return for static bodies - no computation needed
        // Branch predictor will learn this pattern after a few frames
        if (body.is_static)
            return;

        // Compute acceleration from accumulated forces: a = F / m
        Vec3 acceleration = body.compute_acceleration();

        // Update velocity: v_new = v_old + a * dt
        body.velocity += acceleration * dt;

        // Update position using the NEW velocity (this is the "semi-implicit" part)
        // Using v_new instead of v_old makes the method stable
        body.position += body.velocity * dt;

        // Apply linear damping to simulate air resistance/friction
        // Damping is modeled as exponential decay: v *= exp(-damping * dt)
        // Approximated as: v *= (1 - damping * dt) for small dt
        if (body.linear_damping > 0.0f) {
            float damping_factor = 1.0f - body.linear_damping * dt;
            // Clamp to prevent negative velocities from large damping
            damping_factor = std::max(0.0f, damping_factor);
            body.velocity *= damping_factor;
        }

        // Store acceleration for debugging/analysis
        body.acceleration = acceleration;

        // Clear forces for next timestep
        body.force = Vec3::zero();
    }

    void integrate_rotation(Body& body, float dt) override {
        if (body.is_static)
            return;

        // Compute angular acceleration from torque: alpha = I^(-1) * tau
        Vec3 angular_accel = body.compute_angular_acceleration();

        // Update angular velocity: omega_new = omega_old + alpha * dt
        body.angular_velocity += angular_accel * dt;

        // Update orientation (rotation quaternion)
        // For small rotations: q_new = q_old * dq, where dq is rotation by angle = |omega| * dt
        // Quaternion formula for rotation by angle theta around axis u:
        //   dq = [cos(theta/2), sin(theta/2) * u]
        if (body.angular_velocity.length_squared() > 1e-10f) {
            float angle = body.angular_velocity.length() * dt;
            Vec3 axis = body.angular_velocity.normalized();

            // Construct delta rotation quaternion
            Quat delta_rot;
            delta_rot.w = std::cos(angle * 0.5f);
            delta_rot.x = axis.x * std::sin(angle * 0.5f);
            delta_rot.y = axis.y * std::sin(angle * 0.5f);
            delta_rot.z = axis.z * std::sin(angle * 0.5f);

            // Apply rotation: q_new = delta_rot * q_old (quaternion multiplication)
            body.rotation = delta_rot * body.rotation;

            // Renormalize to prevent numerical drift
            // Quaternions should stay unit length, but rounding errors accumulate
            body.rotation.normalize();
        }

        // Apply angular damping
        if (body.angular_damping > 0.0f) {
            float damping_factor = 1.0f - body.angular_damping * dt;
            damping_factor = std::max(0.0f, damping_factor);
            body.angular_velocity *= damping_factor;
        }

        // Clear torque for next timestep
        body.torque = Vec3::zero();
    }

    // Convenience method that integrates both linear and rotational motion
    void integrate_full(Body& body, float dt) override {
        integrate(body, dt);
        integrate_rotation(body, dt);
    }

    const char* name() const override { return "SemiImplicitEuler"; }
};

// Backward compatibility alias
using EulerIntegrator = SemiImplicitEulerIntegrator;

} // namespace physics
} // namespace attic
