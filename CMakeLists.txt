cmake_minimum_required(VERSION 3.20)
project(attic_physics VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/runtime/include)

# core library
add_library(attic_core STATIC
    lib/core/math/vec3.cpp
    lib/core/math/mat3.cpp
    lib/core/math/quat.cpp
)

# tests
enable_testing()
add_executable(test_core test/cpp/test_core.cpp)
target_link_libraries(test_core attic_core)
add_test(NAME core COMMAND test_core)

install(TARGETS attic_core ARCHIVE DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
