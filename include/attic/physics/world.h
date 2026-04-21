#pragma once
#include "attic/core/log/log.h"
#include "attic/core/math/vec3.h"
#include "attic/physics/body.h"
#include "attic/physics/integrator.h"
#include "attic/physics/integrators/euler.h"

#include <memory>
#include <string>
#include <vector>

namespace attic {
namespace physics {

// Abstract base class for force fields
// Force fields apply forces to bodies based on their state
// Examples: gravity, drag, magnetic fields, etc.
struct ForceField {
    virtual ~ForceField() = default;

    // Apply force to a body
    // Called once per body per timestep
    virtual void apply(Body& body, float dt) = 0;

    // Get force field name for debugging
    virtual const char* name() const = 0;
};

// Constant uniform gravity field
// The most common force field - applies F = m * g to all dynamic bodies
struct GravityForceField : public ForceField {
    Vec3 gravity; // Gravitational acceleration vector (default: Earth gravity)

    explicit GravityForceField(const Vec3& g = Vec3(0.0f, -9.81f, 0.0f)) : gravity(g) {}

    void apply(Body& body, float dt) override {
        (void) dt; // Unused - gravity is independent of timestep for acceleration
        if (!body.is_static) {
            // F = m * g
            body.apply_force(gravity * body.mass);
        }
    }

    const char* name() const override { return "Gravity"; }
};

// Physics world - container and manager for all physics objects
//
// The World class manages:
// - Collection of rigid bodies
// - Force fields (gravity, etc.)
// - Integration of equations of motion
// - Collision detection and response
//
// Usage:
//   World world;
//   Body& ball = world.create_body(1.0f);
//   ball.position = Vec3(0, 10, 0);
//   for (int i = 0; i < 1000; i++) {
//       world.step(0.016f);  // 60 FPS
//   }
struct World {
    // Configuration
    Vec3 gravity = Vec3(0.0f, -9.81f, 0.0f); // Global gravity setting

    // State
    std::vector<Body> bodies;                              // All rigid bodies
    std::vector<std::unique_ptr<ForceField>> force_fields; // Active force fields
    std::unique_ptr<Integrator> integrator;                // Integration method

    // Simulation state
    float time = 0.0f;  // Current simulation time (seconds)
    int step_count = 0; // Number of steps taken

    // Collision settings
    bool collision_enabled = true; // Enable/disable collision handling
    float restitution = 0.8f;      // Bounciness coefficient (0-1)
    float friction = 0.5f;         // Friction coefficient

    // Constructor - initializes with default integrator and gravity
    World() {
        integrator = std::make_unique<SemiImplicitEulerIntegrator>();
        add_force_field(std::make_unique<GravityForceField>(gravity));
    }

    // Create a new rigid body with specified mass
    // Returns reference to the created body
    Body& create_body(float mass = 1.0f) {
        Body body(mass);
        body.id = (int) bodies.size();
        bodies.push_back(std::move(body));
        return bodies.back();
    }

    // Create a sphere body with proper inertia tensor
    // Automatically computes spherical inertia based on mass and radius
    Body& create_sphere_body(float radius, float mass) {
        Body& body = create_body(mass);
        body.radius = radius;
        float I = 0.4f * mass * radius * radius; // Solid sphere: I = 2/5 * m * r^2
        body.inertia = Mat3::diagonal(I, I, I);
        body.inv_inertia = Mat3::diagonal(1.0f / I, 1.0f / I, 1.0f / I);
        return body;
    }

    // Get body by ID
    // Returns nullptr if ID is invalid
    Body* get_body(int id) {
        if (id < 0 || id >= (int) bodies.size())
            return nullptr;
        return &bodies[id];
    }

    // Get number of bodies in the world
    int num_bodies() const { return (int) bodies.size(); }

    // Add a force field to the world
    // The world takes ownership of the force field
    void add_force_field(std::unique_ptr<ForceField> field) {
        force_fields.push_back(std::move(field));
    }

    // Set global gravity vector
    // Also updates existing gravity force fields
    void set_gravity(const Vec3& g) {
        gravity = g;
        for (auto& field : force_fields) {
            auto* grav = dynamic_cast<GravityForceField*>(field.get());
            if (grav)
                grav->gravity = g;
        }
    }

    // Set integration method
    // Examples: SemiImplicitEulerIntegrator, RK4Integrator, etc.
    void set_integrator(std::unique_ptr<Integrator> integ) { integrator = std::move(integ); }

    // Advance simulation by one timestep
    //
    // Simulation pipeline:
    // 1. Apply all force fields to all bodies
    // 2. Integrate equations of motion
    // 3. Handle collisions
    // 4. Update simulation time
    void step(float dt) {
        // Step 1: Apply force fields (e.g., gravity)
        for (auto& field : force_fields) {
            for (auto& body : bodies) {
                field->apply(body, dt);
            }
        }

        // Step 2: Integrate motion
        // Skip static bodies - they don't move
        for (auto& body : bodies) {
            if (!body.is_static) {
                integrator->integrate_full(body, dt);
            }
        }

        // Step 3: Collision detection and response
        if (collision_enabled) {
            handle_collisions(dt);
        }

        // Step 4: Update time
        time += dt;
        step_count++;
    }

    // Simple ground plane collision handler (y = 0)
    // For a full collision system, this would be replaced with broad/narrow phase detection
    void handle_collisions(float dt) {
        (void) dt; // Unused in simple ground collision

        for (auto& body : bodies) {
            if (body.is_static)
                continue; // Static bodies don't collide

            // Ground collision: check if body center is below radius
            if (body.position.y < body.radius && body.velocity.y < 0.0f) {
                // Position correction: push body up to surface
                body.position.y = body.radius;

                // Velocity response: reflect with restitution
                body.velocity.y = -body.velocity.y * restitution;

                // Apply friction to horizontal motion
                body.velocity.x *= (1.0f - friction);
                body.velocity.z *= (1.0f - friction);
            }
        }
    }

    // Run simulation for a duration
    // Divides duration into steps of size dt
    void simulate(float duration, float dt) {
        int steps = (int) (duration / dt);
        for (int i = 0; i < steps; i++) {
            step(dt);
        }
    }

    // Reset simulation state
    // Clears forces and resets time, but keeps body configurations
    void reset() {
        time = 0.0f;
        step_count = 0;
        for (auto& body : bodies) {
            body.clear_forces();
            body.acceleration = Vec3::zero();
        }
    }

    // Print world summary information
    void print_info() const {
        ATTIC_INFO("=== World ===");
        ATTIC_INFO("  Time: %.3f s", time);
        ATTIC_INFO("  Steps: %d", step_count);
        ATTIC_INFO("  Bodies: %d", num_bodies());
        ATTIC_INFO("  Gravity: (%.2f, %.2f, %.2f)", gravity.x, gravity.y, gravity.z);
        ATTIC_INFO("  Integrator: %s", integrator->name());
    }

    // Print state of all bodies
    void print_bodies() const {
        ATTIC_INFO("=== Bodies ===");
        for (const auto& body : bodies) {
            ATTIC_INFO("  [%d] %s: pos=(%.3f, %.3f, %.3f) vel=(%.3f, %.3f, %.3f)", body.id,
                       body.name.c_str(), body.position.x, body.position.y, body.position.z,
                       body.velocity.x, body.velocity.y, body.velocity.z);
        }
    }
};

} // namespace physics
} // namespace attic
