#include "attic/core/log/log.h"
#include "attic/core/math/math.h"

int main() {
    using namespace atp;
    
    log::set_level(log::DEBUG);
    log::set_output(stdout);
    
    ATP_INFO("attic_physics v0.1.0 starting...");
    ATP_DEBUG("Math library test: Vec3");
    
    Vec3 a(1, 2, 3);
    Vec3 b(4, 5, 6);
    Vec3 c = a + b;
    
    ATP_DEBUG("a = (%f, %f, %f)", a.x, a.y, a.z);
    ATP_DEBUG("b = (%f, %f, %f)", b.x, b.y, b.z);
    ATP_DEBUG("a + b = (%f, %f, %f)", c.x, c.y, c.z);
    
    ATP_INFO("Dot product: %f", dot(a, b));
    ATP_INFO("Cross product: (%f, %f, %f)", c.x, c.y, c.z);
    
    ATP_WARN("This is a warning message");
    ATP_ERROR("This is an error message");
    
    log::set_level(log::WARN);
    ATP_DEBUG("This should not be printed");
    ATP_WARN("Warning after setting level to WARN");
    
    return 0;
}
