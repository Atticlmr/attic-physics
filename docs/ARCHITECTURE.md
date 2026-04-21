# Attic Physics Architecture

## 统一架构设计

Attic Physics采用模块化设计，将原先的三个独立项目（attic-physics, taiee, taishy）整合为统一的代码库，但保持清晰的模块边界。

## 模块关系

```
┌─────────────────────────────────────────────────────────────┐
│                    Application Layer                         │
│              (Python API / C++ Application)                  │
└─────────────────────┬───────────────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────────────┐
│                  Physics Engine (P0)                         │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐          │
│  │    World    │  │    Body     │  │  Integrator │          │
│  └─────────────┘  └─────────────┘  └─────────────┘          │
│  ┌─────────────┐  ┌─────────────┐                           │
│  │  Collision  │  │   Solver    │                           │
│  └─────────────┘  └─────────────┘                           │
└─────────────────────┬───────────────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────────────┐
│                  GPU Runtime (P1)                            │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐          │
│  │   Device    │  │   Memory    │  │   Buffer    │          │
│  └─────────────┘  └─────────────┘  └─────────────┘          │
└─────────────────────┬───────────────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────────────┐
│                  DSL Compiler (P2)                           │
│  .physics file → Lexer → Parser → AST → LLVM → PTX           │
└─────────────────────────────────────────────────────────────┘
```

## 开发优先级

### P0 - 必须完成 (Phase 2)
- **Physics Engine**: World, Body, Integrator
- **Core**: Math library (Vec3, Mat3, Quat), Logging
- **Build**: CMake, Tests

**Why**: 这是项目的基础，没有物理引擎其他模块无用。

### P1 - 重要 (Phase 3-4)
- **Python Bindings**: pybind11
- **GPU Runtime**: SoA, Memory management, Device abstraction

**Why**: Python API提高可用性，GPU runtime为后续加速做准备。

### P2 - 可选 (Phase 5-6)
- **DSL Compiler**: .physics language → GPU code
- **Rendering**: Vulkan-based visualization

**Why**: 这些是高级功能，前期可用现有方案替代（Python matplotlib, external renderer）。

## 模块详解

### 1. Core (`include/attic/core/`)
- **Math**: Vec3, Mat3, Quat, basic operations
- **Log**: Logging macros (ATTIC_DEBUG, ATTIC_INFO, etc.)

**设计原则**:
- 无依赖（除标准库）
- GPU兼容（无STL容器）
- 高性能（SIMD-ready）

### 2. Physics (`include/attic/physics/`)
- **Body**: Rigid body state (position, velocity, mass, etc.)
- **World**: Physics world manager, simulation loop
- **Integrator**: Euler, Verlet, RK4, Implicit
- **Collision**: Shape types, collision detection

**设计原则**:
- AoS for CPU (user-friendly)
- Clear interfaces for GPU extension
- Deterministic simulation

### 3. GPU (`include/attic/gpu/`)
- **Device**: GPU device abstraction (CUDA/Vulkan)
- **Memory**: Memory pool, H2D/D2H transfers
- **Buffer**: SoA data structures

**设计原则**:
- Optional dependency (ATTIC_BUILD_GPU)
- SoA for GPU coalesced access
- RAII memory management

### 4. Compiler (`include/attic/compiler/`)
- **Lexer**: Tokenize .physics files
- **Parser**: Build AST
- **Codegen**: Generate LLVM IR
- **Runtime**: Execute compiled kernels

**设计原则**:
- Optional dependency (ATTIC_BUILD_COMPILER)
- AOT compilation for production
- JIT for development/debugging

### 5. Render (`include/attic/render/`)
- **Renderer**: Vulkan rendering pipeline
- **Scene**: Scene graph, camera, lights

**设计原则**:
- Lowest priority
- Optional dependency (ATTIC_BUILD_RENDER)
- Can be replaced by external tools

## 数据流

### CPU Simulation
```
User Code
    ↓
World::step(dt)
    ↓
Integrator::integrate(bodies, dt)
    ↓
Collision::detect(bodies)
    ↓
Solver::resolve(contacts)
```

### GPU Acceleration (Phase 4+)
```
World::step(dt)
    ↓
if (use_gpu) {
    upload_to_gpu(bodies)
    launch_kernel(integrate_kernel)
    download_from_gpu(bodies)
} else {
    cpu_integrate(bodies)
}
```

### DSL Compilation (Phase 5+)
```
.physics file
    ↓
attic-compile input.physics -o kernel.ptx
    ↓
PTX kernel embedded in binary
    ↓
Runtime loads and executes
```

## 构建系统

### CMake Options
- `ATTIC_BUILD_GPU`: Enable GPU runtime (requires CUDA)
- `ATTIC_BUILD_COMPILER`: Enable DSL compiler (requires LLVM)
- `ATTIC_BUILD_RENDER`: Enable rendering (requires Vulkan)
- `ATTIC_BUILD_PYTHON`: Enable Python bindings (requires pybind11)
- `ATTIC_BUILD_TESTS`: Build test suite
- `ATTIC_BUILD_EXAMPLES`: Build example programs

### Build Configurations
1. **Minimal**: Core + Physics only
   ```bash
   cmake ..
   ```

2. **Development**: +GPU +Python +Tests
   ```bash
   cmake -DATTIC_BUILD_GPU=ON \
         -DATTIC_BUILD_PYTHON=ON \
         -DATTIC_BUILD_TESTS=ON ..
   ```

3. **Full**: All features
   ```bash
   cmake -DATTIC_BUILD_GPU=ON \
         -DATTIC_BUILD_COMPILER=ON \
         -DATTIC_BUILD_RENDER=ON \
         -DATTIC_BUILD_PYTHON=ON ..
   ```

## 命名空间约定

```cpp
namespace attic {
    // Core
    namespace math { /* Vec3, Mat3, Quat */ }
    namespace log { /* logging */ }
    
    // Physics
    namespace physics { /* World, Body, Integrator */ }
    
    // GPU
    namespace gpu { /* Device, Memory, Buffer */ }
    
    // Compiler
    namespace compiler { /* Lexer, Parser, Codegen */ }
    
    // Render
    namespace render { /* Renderer, Scene */ }
}
```

## 文件命名

- **Headers**: `snake_case.h` (e.g., `vec3.h`, `world.h`)
- **Implementation**: `snake_case.cpp`
- **Kernel files**: `*.physics`
- **Tests**: `test_*.cpp`
- **Examples**: `example_*.cpp` or descriptive name

## 未来扩展

### Multi-Backend GPU
- CUDA (NVIDIA)
- Vulkan (Cross-platform)
- Metal (Apple)

### Advanced Physics
- FEM (Finite Element Method)
- SPH (Smoothed Particle Hydrodynamics)
- Robot dynamics (URDF/MJCF)

### Differentiable Physics
- Enzyme integration
- Gradient computation
- Optimization support

---

*Architecture Version: 1.0*  
*Last Updated: 2026-03-17*
