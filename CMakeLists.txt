cmake_minimum_required(VERSION 3.20)
project(flux_physics VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# ===== 数学库（最基础）=====
add_library(flux_math STATIC
    src/math/vec3.cpp
    src/math/quat.cpp
    src/math/mat3.cpp
)

# ===== 刚体动力学核心 =====
add_library(flux_rigid_body STATIC
    src/rigid_body/rigid_body.cpp
    src/rigid_body/shape.cpp
    src/rigid_body/inertia.cpp
)
target_link_libraries(flux_rigid_body flux_math)

# ===== 碰撞检测 =====
add_library(flux_collision STATIC
    src/collision/aabb.cpp
    src/collision/broad_phase.cpp
    src/collision/narrow_phase.cpp
    src/collision/contact.cpp
    src/collision/algorithms/sphere_sphere.cpp
    src/collision/algorithms/box_box.cpp
)
target_link_libraries(flux_collision flux_math flux_rigid_body)

# ===== 约束求解 =====
add_library(flux_constraint STATIC
    src/constraint/constraint.cpp
    src/constraint/contact_constraint.cpp
    src/constraint/joint.cpp
    src/constraint/solvers/sequential_impulse.cpp
)
target_link_libraries(flux_constraint flux_math flux_rigid_body)

# ===== 物理世界 =====
add_library(flux_physics STATIC
    src/physics/world.cpp
    src/physics/integrators/euler.cpp
)
target_link_libraries(flux_physics flux_rigid_body flux_collision flux_constraint)

# ===== 测试 =====
enable_testing()

# 数学测试
add_executable(test_vec3 tests/rigid_body/test_free_fall.cpp)
target_link_libraries(test_vec3 flux_math)
add_test(NAME vec3 COMMAND test_vec3)

# 碰撞测试
add_executable(test_collision tests/collision/test_sphere_collision.cpp)
target_link_libraries(test_collision flux_collision)
add_test(NAME collision COMMAND test_collision)

# ===== 示例程序 =====
add_executable(example_free_fall examples/rigid_body/01_free_fall.cpp)
target_link_libraries(example_free_fall flux_physics)

add_executable(example_stack examples/rigid_body/04_box_stack.cpp)
target_link_libraries(example_stack flux_physics)
