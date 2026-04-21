#include "attic/core/log/log.h"
#include "attic/core/math/vec3.h"

#include <iostream>

// Bouncing ball demo - Phase 2 implementation target
int main() {
    ATTIC_INFO("Bouncing Ball Demo");
    ATTIC_INFO("==================");

    // Simulation parameters
    const float gravity = -9.8f;
    const float dt = 0.016f; // 60 FPS
    const int max_steps = 1000;

    // Ball state
    attic::math::Vec3 position(0.0f, 10.0f, 0.0f);
    attic::math::Vec3 velocity(0.0f, 0.0f, 0.0f);
    const float radius = 0.5f;
    const float restitution = 0.8f; // Bounciness

    std::cout << "Initial position: (" << position.x << ", " << position.y << ", " << position.z
              << ")" << std::endl;
    std::cout << "Gravity: " << gravity << " m/s^2" << std::endl;
    std::cout << "Time step: " << dt << " s" << std::endl;
    std::cout << "Radius: " << radius << " m" << std::endl;
    std::cout << "Restitution: " << restitution << std::endl;
    std::cout << std::endl;

    // Simulation loop
    std::cout << "Simulation:" << std::endl;
    for (int i = 0; i < max_steps; ++i) {
        // Semi-implicit Euler integration
        velocity.y += gravity * dt;
        position.y += velocity.y * dt;

        // Ground collision
        if (position.y < radius) {
            position.y = radius;
            velocity.y = -velocity.y * restitution;

            // Stop if velocity is small
            if (std::abs(velocity.y) < 0.1f) {
                velocity.y = 0.0f;
            }
        }

        // Output every 60 frames (1 second)
        if (i % 60 == 0) {
            std::cout << "t=" << i * dt << "s: "
                      << "pos.y=" << position.y << ", vel.y=" << velocity.y << std::endl;
        }

        // Stop if ball is at rest on ground
        if (position.y <= radius && std::abs(velocity.y) < 0.01f) {
            std::cout << "\nBall came to rest at t=" << i * dt << "s" << std::endl;
            break;
        }
    }

    std::cout << "\nDemo complete!" << std::endl;
    std::cout << "In Phase 2, this will use the physics engine API:" << std::endl;
    std::cout << "  World world;" << std::endl;
    std::cout << "  Body* ball = world.create_body();" << std::endl;
    std::cout << "  world.step(dt);" << std::endl;

    return 0;
}
