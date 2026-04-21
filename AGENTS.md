# AGENTS.md - Agent Guidelines for attic-physics

---

## 1. Build Commands

### Basic Build (CPU only)
```bash
cd build && cmake .. && make -j$(nproc)
```

### Full Build (All features)
```bash
cd build && cmake -DATTIC_BUILD_GPU=ON \
                  -DATTIC_BUILD_COMPILER=ON \
                  -DATTIC_BUILD_PYTHON=ON \
                  -DATTIC_BUILD_TESTS=ON .. && make -j$(nproc)
```

### Run All Tests
```bash
cd build && ctest --output-on-failure
```

### Run Single Test
```bash
cd build && ./test_core
# or
cd build && ./test_physics
```

### Clean Build
```bash
rm -rf build && mkdir build && cd build && cmake .. && make -j$(nproc)
```

---

## 2. Code Style

### Formatting (from .clang-format)
- Standard: LLVM-based
- Indent: 4 spaces, no tabs
- Column limit: 100
- Braces: Attach (`if (x) {`)
- Line endings: LF (Unix)

### Naming Conventions
| Element | Convention | Example |
|---------|------------|---------|
| Classes/Structs | PascalCase | `World` |
| Functions | PascalCase | `Step(dt)` |
| Variables | snake_case | `velocity` |
| Constants | kCamelCase | `kGravity` |
| Enums | PascalCase | `Level { Debug, Info }` |
| Files | snake_case | `vec3.h` |

### Namespace
```cpp
namespace attic {
namespace physics {
class World { /* ... */ };
} // namespace physics
} // namespace attic
```

### Headers
- Use `#pragma once`
- Order: 1) Project, 2) Third-party, 3) Standard lib

```cpp
#pragma once
#include "attic/core/math/vec3.h"
#include "attic/core/log/log.h"
#include <cmath>
```

### Types
- Use `float` for physics, `int` for indices, `bool` for flags
- Avoid `auto` unless type is obvious

### Classes
- Public → protected → private
- Use default constructors; delete copy/move if not needed

```cpp
struct Vec3 {
    float x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x_, float y_, float z_) : x(x_), y(x_), z(z_) {}
};
```

### Error Handling
- Use logging macros from `log.h`: `ATTIC_DEBUG`, `ATTIC_INFO`, `ATTIC_WARN`, `ATTIC_ERROR`
- No exceptions (C++ beginner project)

```cpp
ATTIC_INFO("Creating body at (%f, %f, %f)", x, y, z);
ATTIC_WARN("Velocity exceeded max");
ATTIC_ERROR("Memory allocation failed");
```

### GPU Compatibility
- NO STL containers in GPU-facing headers
- NO virtual functions in GPU kernels
- NO exceptions in GPU code
- Use plain structs with fixed-size arrays

---

## 3. Project Structure

```
attic-physics/ (Unified Project)
├── CMakeLists.txt              # Build config with options
├── include/attic/               # Public headers
│   ├── core/                   # Math, logging
│   ├── physics/                # Physics engine (Phase 2)
│   ├── compiler/               # DSL compiler (Phase 5)
│   ├── gpu/                    # GPU runtime (Phase 4)
│   └── render/                 # Rendering (Phase 6)
├── lib/                        # Implementation files
│   ├── core/
│   ├── physics/
│   ├── compiler/
│   ├── gpu/
│   └── render/
├── kernels/                    # GPU kernel definitions (*.physics)
├── python/                     # Python bindings (Phase 3)
├── test/cpp/                   # C++ tests
├── examples/                   # Example programs
├── tools/                      # Command-line tools
│   └── attic-compile.cpp      # DSL compiler CLI
└── docs/                       # Documentation
```

---

## 4. Development Phases

### Phase 1: Foundation ✅ (Completed)
- Math library (Vec3, Mat3, Quat)
- Logging system
- Build system

### Phase 2: Physics Core ⭐ (Current - Priority)
- World, Body classes
- Integrators (Euler, Verlet)
- Basic collision detection
- **Target**: Bouncing ball demo

### Phase 3: Python Bindings
- pybind11 integration
- Python API design

### Phase 4: GPU Runtime
- SoA data structures
- GPU memory management
- Device abstraction

### Phase 5: DSL Compiler
- Lexer/Parser for .physics files
- LLVM IR generation
- PTX/Vulkan code generation
- AOT pre-compilation

### Phase 6: Rendering (Optional)
- Vulkan-based renderer
- Scene management

---

## 5. Adding New Code

### Add to Core Library
1. Header → `include/attic/core/`
2. Implementation → `lib/core/`
3. Update `CMakeLists.txt`:

```cmake
add_library(attic_core STATIC
    lib/core/math/vec3.cpp
    lib/core/path/to/new_file.cpp
)
```

### Add to Physics Library
1. Header → `include/attic/physics/`
2. Implementation → `lib/physics/`

### Add GPU Kernel
1. Create `kernels/your_kernel.physics`
2. Follow existing examples (euler.physics, gravity.physics)
3. Document inputs/outputs in comments

### Add Tests
```cmake
add_executable(test_feature test/cpp/test_feature.cpp)
target_link_libraries(test_feature attic_physics)
add_test(NAME feature COMMAND test_feature)
```

---

## 6. Key References

- **Development Plan**: `docs/DEVELOPMENT.md`
- **Integrators**: `docs/integrators.md`
- **Math Library**: `docs/MATH_LIBRARY.md`
- **GPU Physics**: `docs/GPU_PHYSICS_REFERENCES.md`
- **Parser Design**: `docs/PARSER_DESIGN.md`

---

## 7. Priorities

1. **Physics first** (Phase 2) - Complete World/Body before anything else
2. **Test coverage** - Every module needs tests
3. **GPU compatibility** - Keep GPU headers clean
4. **Documentation** - Update docs with code changes

---

## 8. Quick Commands

```bash
# Edit → Rebuild → Test
cd build && make -j4 && ./test_physics

# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug .. && make

# Release build
cmake -DCMAKE_BUILD_TYPE=Release .. && make

# Build with GPU support
cmake -DATTIC_BUILD_GPU=ON .. && make

# Build with compiler
cmake -DATTIC_BUILD_COMPILER=ON .. && make
```

---

*Updated: 2026-03-17*  
*Project unified from: attic-physics, taiee, taishy*
