cmake_minimum_required(VERSION 3.20)
project(attic_physics VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含目录 - 头文件在include，对应include/attic
include_directories(${CMAKE_SOURCE_DIR}/include)
# 实现文件在src
include_directories(${CMAKE_SOURCE_DIR}/src)

# ===== 数学库（最基础）=====
add_library(attic_math STATIC
    src/math/vec3.cpp
    src/math/quat.cpp
    src/math/mat3.cpp
)

# ===== 刚体动力学核心 =====
add_library(attic_rigid_body STATIC
    src/rigid_body/body.cpp
    src/rigid_body/shape.cpp
)
target_link_libraries(attic_rigid_body attic_math)

# ===== 碰撞检测 =====
add_library(attic_collision STATIC
    src/collision/broad_phase.cpp
    src/collision/sphere_vs_sphere.cpp
)
target_link_libraries(attic_collision attic_math attic_rigid_body)

# ===== 物理世界/求解器 =====
add_library(attic_solver STATIC
    src/solver/world.cpp
    src/solver/contact_solver.cpp
)
target_link_libraries(attic_solver attic_math attic_rigid_body attic_collision)

# ===== 主库 =====
add_library(attic_physics STATIC
    src/physics/physics.cpp
)
target_link_libraries(attic_physics attic_solver)

# ===== 测试 =====
enable_testing()
add_executable(test_math src/tests/test_math.cpp)
target_link_libraries(test_math attic_math)
add_test(NAME math COMMAND test_math)

# ===== 示例程序 =====
add_executable(demo_sphere src/demo/01_single_sphere.cpp)
target_link_libraries(demo_sphere attic_physics)

add_executable(demo_stack src/demo/02_sphere_stack.cpp)
target_link_libraries(demo_stack attic_physics)

add_executable(demo_box src/demo/03_rotating_box.cpp)
target_link_libraries(demo_box attic_physics)
