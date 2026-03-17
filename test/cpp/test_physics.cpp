#include "attic/core/log/log.h"
#include "attic/core/math/vec3.h"

#include <iostream>

// Placeholder test - Phase 2 will implement real physics tests
int main() {
    ATTIC_INFO("Running physics tests...");

    // Test basic math
    attic::math::Vec3 v1(1.0f, 2.0f, 3.0f);
    attic::math::Vec3 v2(4.0f, 5.0f, 6.0f);
    attic::math::Vec3 v3 = v1 + v2;

    std::cout << "v1 + v2 = (" << v3.x << ", " << v3.y << ", " << v3.z << ")" << std::endl;

    ATTIC_INFO("Physics tests placeholder complete.");
    ATTIC_INFO("Real physics tests will be implemented in Phase 2.");

    return 0;
}
