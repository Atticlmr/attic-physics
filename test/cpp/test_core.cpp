#include "attic/core/log/log.h"
#include "attic/core/math/math.h"

#include <iostream>

int main() {
    attic::log::set_level(attic::log::DEBUG);
    attic::log::set_output(stdout);

    ATTIC_INFO("attic_physics v0.1.0 starting...");
    ATTIC_DEBUG("Math library test: Vec3");

    attic::math::Vec3 a(1, 2, 3);
    attic::math::Vec3 b(4, 5, 6);
    attic::math::Vec3 c = a + b;

    ATTIC_DEBUG("a + b = (%f, %f, %f)", c.x, c.y, c.z);
    ATTIC_INFO("Dot product: %f", attic::math::dot(a, b));

    return 0;
}
