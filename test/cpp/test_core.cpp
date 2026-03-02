#include <iostream>
#include "attic/core/math/math.h"
#include "attic/core/log/log.h"

int main() {
    using namespace attic;
    
    log::set_level(log::DEBUG);
    log::set_output(stdout);
    
    ATTIC_INFO("attic_physics v0.1.0 starting...");
    ATTIC_DEBUG("Math library test: Vec3");
    
    Vec3 a(1, 2, 3);
    Vec3 b(4, 5, 6);
    Vec3 c = a + b;
    
    ATTIC_DEBUG("a + b = (%f, %f, %f)", c.x, c.y, c.z);
    ATTIC_INFO("Dot product: %f", dot(a, b));
    
    return 0;
}
