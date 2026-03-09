# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**attic-physics** is a GPU-friendly C++ physics simulation library with JIT compilation support and Python bindings. Currently in **Phase 2** (Physics Core Development).

## Build Commands

```bash
# Full build
cd build && cmake .. && make -j$(nproc)

# Run all tests
cd build && ctest --output-on-failure

# Run single test
cd build && ./test_core

# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug .. && make

# Release build
cmake -DCMAKE_BUILD_TYPE=Release .. && make
```

## Code Style

- **Standard**: LLVM-based (see `.clang-format`)
- **Indentation**: 4 spaces, no tabs
- **Column limit**: 100
- **Naming**:
  - Classes/Structs: `PascalCase` (e.g., `World`)
  - Functions: `PascalCase` (e.g., `Step(dt)`)
  - Variables: `snake_case` (e.g., `velocity`)
  - Constants: `kCamelCase` (e.g., `kGravity`)
  - Files: `snake_case` (e.g., `vec3.h`)
- **Namespace**: `attic::`
- **Headers**: Use `#pragma once`, order: project → third-party → standard lib
- **Types**: `float` for physics, `int` for indices, `bool` for flags
- **Error handling**: Use logging macros (`ATTIC_DEBUG`, `ATTIC_INFO`, `ATTIC_WARN`, `ATTIC_ERROR`) - no exceptions

## GPU Compatibility Requirements

All code must be GPU-friendly:
- NO STL containers in headers
- NO virtual functions
- NO exceptions
- Use plain structs with fixed-size arrays

## Project Structure

```
attic-physics/
├── include/attic/          # Public headers
│   ├── core/math/          # Vec3, Mat3, Quat
│   ├── core/log/           # Logging
│   └── physics/            # Physics engine (World, Body, Integrator)
├── lib/                    # Implementation files
├── test/cpp/               # C++ unit tests
├── runtime/                # GPU runtime library
└── docs/                   # Documentation
```

## Development Phases

The project follows a phased development plan:
- **Phase 1**: Basic framework (math lib, logging) - complete
- **Phase 2**: Physics core (World, Body, Integrator, collision) - in progress
- **Phase 3**: Python bindings (pybind11)
- **Phase 4**: Runtime system (GPU memory, device abstraction)
- **Phase 5**: JIT compiler (MLIR-based)

## Key Documentation

- `docs/DEVELOPMENT.md` - Full development roadmap
- `docs/integrators.md` - Integrator documentation (MuJoCo recommends semi-implicit Euler)
- `docs/MATH_LIBRARY.md` - Math library reference
- `docs/GPU_PHYSICS_REFERENCES.md` - GPU physics resources
- `AGENTS.md` - OpenCode agent configuration (Sherlock, Sisyphus, Code Reviewer)

## Priorities

1. **Physics first** - Complete Phase 2 before JIT
2. **GPU compatibility** - All code must be GPU-friendly
3. **Python bindings** - Phase 3+
4. **C++ beginner friendly** - Add explanatory comments

## Adding New Code

### Add to core library:
1. Header → `include/attic/`
2. Implementation → `lib/`
3. Update `CMakeLists.txt`:
```cmake
add_library(attic_core STATIC
    lib/path/to/new_file.cpp
)
```

### Add tests:
```cmake
add_executable(test_foo test/cpp/test_foo.cpp)
target_link_libraries(test_foo attic_core)
add_test(NAME foo COMMAND test_foo)
```
