cmake_minimum_required(VERSION 3.20)
project(attic_physics VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Options
option(ATTIC_BUILD_GPU "Build GPU support (requires CUDA)" OFF)
option(ATTIC_BUILD_COMPILER "Build DSL compiler (requires LLVM)" OFF)
option(ATTIC_BUILD_RENDER "Build rendering support (requires Vulkan)" OFF)
option(ATTIC_BUILD_PYTHON "Build Python bindings" OFF)
option(ATTIC_BUILD_TESTS "Build tests" ON)
option(ATTIC_BUILD_EXAMPLES "Build examples" ON)

# Core library - always built
set(ATTIC_CORE_SOURCES
    lib/core/math/vec3.cpp
    lib/core/math/mat3.cpp
    lib/core/math/quat.cpp
)

add_library(attic_core STATIC ${ATTIC_CORE_SOURCES})

# Physics library - Phase 2
set(ATTIC_PHYSICS_SOURCES
    lib/physics/body.cpp
    lib/physics/world.cpp
    lib/physics/integrator.cpp
)

add_library(attic_physics STATIC ${ATTIC_PHYSICS_SOURCES})
target_link_libraries(attic_physics attic_core)

# GPU Runtime - Phase 4 (optional)
if(ATTIC_BUILD_GPU)
    find_package(CUDA REQUIRED)
    set(ATTIC_GPU_SOURCES
        lib/gpu/memory.cpp
        lib/gpu/device.cpp
        lib/gpu/buffer.cpp
    )
    add_library(attic_gpu STATIC ${ATTIC_GPU_SOURCES})
    target_link_libraries(attic_gpu attic_core ${CUDA_LIBRARIES})
    target_include_directories(attic_gpu PRIVATE ${CUDA_INCLUDE_DIRS})
endif()

# DSL Compiler - Phase 5 (optional)
if(ATTIC_BUILD_COMPILER)
    find_package(LLVM REQUIRED CONFIG)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    
    set(ATTIC_COMPILER_SOURCES
        lib/compiler/lexer/lexer.cpp
        lib/compiler/parser/parser.cpp
        lib/compiler/ast/ast.cpp
        lib/compiler/codegen/codegen.cpp
        lib/compiler/runtime/runtime.cpp
    )
    
    add_library(attic_compiler STATIC ${ATTIC_COMPILER_SOURCES})
    target_link_libraries(attic_compiler attic_core LLVM)
    
    # Compiler tool
    add_executable(attic-compile tools/attic-compile.cpp)
    target_link_libraries(attic-compile attic_compiler)
endif()

# Rendering - Phase 6 (optional)
if(ATTIC_BUILD_RENDER)
    find_package(Vulkan REQUIRED)
    set(ATTIC_RENDER_SOURCES
        lib/render/renderer.cpp
        lib/render/scene.cpp
    )
    add_library(attic_render STATIC ${ATTIC_RENDER_SOURCES})
    target_link_libraries(attic_render attic_core Vulkan::Vulkan)
endif()

# Python bindings - Phase 3
if(ATTIC_BUILD_PYTHON)
    find_package(pybind11 REQUIRED)
    pybind11_add_module(attic_physics_py python/attic_physics.cpp)
    target_link_libraries(attic_physics_py PRIVATE attic_physics)
    if(ATTIC_BUILD_GPU)
        target_link_libraries(attic_physics_py PRIVATE attic_gpu)
    endif()
endif()

# Tests
if(ATTIC_BUILD_TESTS)
    enable_testing()
    
    add_executable(test_core test/cpp/test_core.cpp)
    target_link_libraries(test_core attic_core)
    add_test(NAME core COMMAND test_core)
    
    add_executable(test_physics test/cpp/test_physics.cpp)
    target_link_libraries(test_physics attic_physics)
    add_test(NAME physics COMMAND test_physics)
endif()

# Examples
if(ATTIC_BUILD_EXAMPLES)
    add_executable(example_bouncing_ball examples/bouncing_ball.cpp)
    target_link_libraries(example_bouncing_ball attic_physics)
endif()

# Install
install(TARGETS attic_core attic_physics ARCHIVE DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

# Summary
message(STATUS "")
message(STATUS "attic-physics Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  GPU Support: ${ATTIC_BUILD_GPU}")
message(STATUS "  Compiler: ${ATTIC_BUILD_COMPILER}")
message(STATUS "  Rendering: ${ATTIC_BUILD_RENDER}")
message(STATUS "  Python: ${ATTIC_BUILD_PYTHON}")
message(STATUS "  Tests: ${ATTIC_BUILD_TESTS}")
message(STATUS "  Examples: ${ATTIC_BUILD_EXAMPLES}")
message(STATUS "")
