#include <iostream>
#include "player26680488/Core/Math.h"
#include "player26680488/World/World.h"

using namespace atp;

int main() {
    std::cout << "Hello Sphere - player26680488 Physics Demo" << std::endl;
    
    // 创建物理世界
    World world;
    world.set_gravity(Vec3(0, -9.8f, 0));
    
    // 创建球体
    Body* sphere = world.create_body();
    sphere->set_position(Vec3(0, 10, 0));
    sphere->set_mass(1.0f);
    sphere->set_shape(new SphereShape(1.0f));
    
    // 模拟
    for (int i = 0; i < 100; ++i) {
        world.step(0.01f);
        std::cout << "Step " << i << ": pos = " 
                  << sphere->position().x << ", "
                  << sphere->position().y << ", "
                  << sphere->position().z << std::endl;
    }
    
    return 0;
}
