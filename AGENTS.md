# AGENTS.md - Agent Guidelines for attic-physics

---

## 1. Build Commands

### Full Build
```bash
cd build && cmake .. && make -j$(nproc)
```

### Run All Tests
```bash
cd build && ctest --output-on-failure
```

### Run Single Test
```bash
cd build && ./test_core
# or
cd build && ctest -R test_core
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
namespace math {
struct Vec3 { /* ... */ };
} // namespace math
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
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
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
- NO STL containers in headers
- NO virtual functions
- NO exceptions
- Use plain structs with fixed-size arrays

---

## 3. Project Structure

```
attic-physics/
├── CMakeLists.txt          # Build config
├── include/attic/           # Public headers
│   ├── core/math/          # Vec3, Mat3, Quat
│   ├── core/log/           # Logging
│   └── physics/            # Physics engine
├── lib/                    # Implementation
├── test/cpp/               # C++ tests
├── examples/               # Example programs
└── docs/                   # Documentation
```

---

## 4. Adding New Code

### Add to Core Library
1. Header → `include/attic/`
2. Implementation → `lib/`
3. Update `CMakeLists.txt`:

```cmake
add_library(attic_core STATIC
    lib/core/math/vec3.cpp
    lib/path/to/new_file.cpp
)
```

### Add Tests
```cmake
add_executable(test_foo test/cpp/test_foo.cpp)
target_link_libraries(test_foo attic_core)
add_test(NAME foo COMMAND test_foo)
```

---

## 5. Key References

- Integrators: `docs/integrators.md` (MuJoCo recommends semi-implicit Euler)
- Math: `docs/MATH_LIBRARY.md`
- GPU: `docs/GPU_PHYSICS_REFERENCES.md`
- Context7: `docs/context7_guide.md`

---

## 6. Priorities

1. **Physics first** (Phase 2) - Before JIT
2. **GPU compatibility** - All code GPU-friendly
3. **Python bindings** - pybind11 (Phase 3+)
4. **C++ beginner friendly** - Add explanatory comments

---

## 7. Quick Commands

```bash
# Edit → Rebuild → Test
cd build && make -j4 && ./test_core

# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug .. && make

# Release build
cmake -DCMAKE_BUILD_TYPE=Release .. && make
```
