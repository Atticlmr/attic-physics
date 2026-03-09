# attic-physics 开发计划

## 项目概述

基于C++的通用GPU物理仿真库，支持JIT编译和GPU代码生成，Python友好调用。

### 核心特性
- **JIT实时编译** - 类似Taichi的DSL，运行时编译Kernel
- **多后端支持** - CPU/CUDA/Vulkan/Metal
- **Python绑定** - pybind11，易于使用
- **可微分物理** - 支持自动微分（Enzyme集成）

---

## 目录结构

```
attic-physics/
├── CMakeLists.txt                 # 根构建配置
├── cmake/                         # CMake模块
│   ├── AtticPhysicsUtils.cmake
│   └── FindEnzyme.cmake
├── include/attic/                 # 公共头文件
│   ├── core/
│   │   ├── math/                  # 数学库
│   │   │   ├── vec3.h
│   │   │   ├── mat3.h
│   │   │   └── quat.h
│   │   └── log/                   # 日志系统
│   │       └── log.h
│   ├── physics/                   # 物理仿真
│   │   ├── world.h               # 物理世界
│   │   ├── body.h                # 刚体
│   │   └── integrator.h          # 积分器
│   └── compiler/                  # JIT编译器（Phase 2）
│       └── ...
├── lib/                           # 实现文件
│   ├── core/
│   │   └── math/                  # 数学库实现
│   ├── physics/
│   │   ├── world.cpp
│   │   ├── body.cpp
│   │   └── integrator.cpp
│   └── Runtime/                   # 运行时（GPU管理）
│       ├── Device.h              # 设备抽象
│       ├── Memory.h              # GPU内存管理
│       └── Stream.h              # 执行流
├── runtime/                       # GPU运行时库
│   ├── include/CUDA/             # CUDA运行时头
│   └── lib/
├── tools/                         # 命令行工具
│   ├── attic-sim/                # 仿真器
│   └── attic-opt/                # 优化器
├── test/                          # 测试
│   ├── cpp/                      # C++单元测试
│   └── python/                   # Python测试
├── examples/                      # 示例
└── python/                        # Python包
    └── attic_physics/
```

---

## 开发阶段

### Phase 1: 基础框架 (v0.1) - Week 1-2 ✅ 完成

| 任务 | 文件 | 状态 |
|------|------|------|
| 项目结构完善 | CMakeLists.txt | ✅ |
| 基础数学库 | include/attic/core/math/{vec3,mat3,quat}.h, lib/core/math/*.cpp | ✅ |
| 日志系统 | include/attic/core/log/log.h | ✅ |
| 代码格式 | .clang-format | ✅ |
| CI/CD | .github/workflows/ci.yml | ✅ |

**里程碑**: 基础库可编译，测试通过

---

## 当前开发进度 (2026-03-05)

### ✅ 已完成

| 模块 | 文件 | 说明 |
|------|------|------|
| **数学库** | `lib/core/math/{vec3,mat3,quat}.cpp` | Vec3, Mat3, Quat 完整实现 |
| **数学头文件** | `include/attic/core/math/{vec3,mat3,quat,math}.h` | 完整数学类型定义 |
| **日志系统** | `include/attic/core/log/log.h` | DEBUG/INFO/WARN/ERROR 级别 |
| **构建系统** | `CMakeLists.txt` | 基础构建配置 |
| **代码规范** | `.clang-format` | LLVM 风格 |
| **CI/CD** | `.github/workflows/ci.yml` | 多平台构建 + 测试 |
| **文档** | `docs/{log,integrators,MATH_LIBRARY,PARSER_DESIGN}.md` | 使用指南和设计文档 |

### ⏳ 开发中 (Phase 2)

| 模块 | 文件 | 状态 | 说明 |
|------|------|------|------|
| **物理积分器** | `include/attic/physics/integrator.h` | 空文件 | 待实现 |
| **积分器实现** | `include/attic/physics/integrators/{euler,verlet,rk4,implicit,implicit_fast}.h` | 空文件 | 待实现 |
| **机器人模型** | `include/attic/physics/robot/robot_model.h` | 空文件 | 待实现 |
| **URDF解析器** | `include/attic/physics/parser/urdf_parser.h` | 空文件 | 待实现 |
| **MJCF解析器** | `include/attic/physics/parser/mjcf_parser.h` | 空文件 | 待实现 |
| **World/Body** | 缺失 | 未创建 | Phase 2 核心功能 |

### 📋 待开发

- 物理世界 (World) 和刚体 (Body) 类
- 碰撞检测系统
- 机器人动力学 (RNEA/CRBA/ABA)
- Python 绑定 (pybind11)
- GPU 运行时

---

### Phase 2: 物理仿真核心 (v0.2) - Week 3-5 ⭐ 优先实现

#### 2.1 刚体系统

```
include/attic/physics/
├── world.h              # 物理世界管理
├── body.h               # 刚体定义
├── integrator.h         # 积分器接口
└── force.h              # 力场定义

lib/physics/
├── world.cpp
├── body.cpp
└── integrator.cpp
```

**核心功能**:
- `World` 类 - 管理所有刚体和仿真
- `Body` 类 - 刚体状态（位置、速度、质量、旋转）
- `Euler` / `Verlet` 积分器 - 运动更新
- 重力场 - 恒定重力

**里程碑**: 小球从高处下落并落地

> ⚠️ **当前状态**: 积分器头文件已创建但为空，需要实现 `World`/`Body` 类

---

## 项目统计

| 类型 | 数量 |
|------|------|
| 头文件 (.h) | 18 |
| 实现文件 (.cpp) | 12 |
| 测试文件 | 3 |
| 示例文件 | 4 |
| 文档 (.md) | 8 |
| Agent 配置 | 4 |

---

## 分支策略

| 分支 | 用途 | 状态 |
|------|------|------|
| `main` | 稳定版 | - |
| `dev` | 开发主分支 | 当前 |
| `bugfix/initial` | Bug 修复分支 | 远程已创建 |

#### 2.2 碰撞检测（基础）

```
include/attic/physics/collision/
├── shapes.h             # 基础形状（球、盒子）
├── aabb.h               # AABB包围盒
└── detector.h           # 碰撞检测接口

lib/physics/collision/
└── detector.cpp
```

**核心功能**:
- 球-球碰撞检测
- 简单的地面碰撞
- 碰撞响应（反弹）

**里程碑**: 小球落地后弹跳

#### 2.3 测试程序

```
test/cpp/test_physics.cpp
```

```cpp
int main() {
    World world;
    Body* ball = world.create_body();
    ball->position = Vec3(0, 10, 0);
    ball->radius = 0.5f;
    
    for (int i = 0; i < 1000; ++i) {
        world.step(0.016f);  // 60fps
        if (i % 60 == 0) {
            printf("t=%.2f, pos=(%.2f, %.2f, %.2f)\n", 
                   i * 0.016f,
                   ball->position.x,
                   ball->position.y,
                   ball->position.z);
        }
    }
}
```

**里程碑**: 运行测试程序，观察小球弹跳

---

### Phase 3: Python绑定 (v0.3) - Week 6-8

#### 3.1 pybind11 集成

```
python/
├── CMakeLists.txt         # Python绑定构建
├── attic_physics.cc       # pybind11绑定
└── attic_physics/         # Python包
    ├── __init__.py
    ├── vec3.py
    └── world.py
```

**绑定内容**:
- `Vec3` → `ap.Vec3`
- `Body` → `ap.Body`
- `World` → `ap.World`

**里程碑**: 
```python
import attic_physics as ap

world = ap.World()
ball = world.create_body()
ball.position = (0, 10, 0)

for _ in range(100):
    world.step(0.016)
    print(ball.position)
```

#### 3.2 高级Python API

```
python/attic_physics/
├── scene.py              # 场景加载
├── shapes.py             # 形状创建
└── simulator.py          # 高级仿真器
```

---

### Phase 4: 运行时系统 (v0.4) - Week 9-11

#### 4.1 设备抽象

```
lib/Runtime/
├── Device.h              # 抽象设备接口
├── MemoryManager.h        # 统一内存管理
└── StreamManager.h        # 执行流管理
```

**功能**:
- 抽象设备层，支持CPU/GPU
- 统一内存管理（Host/Device）
- 异步执行与同步

#### 4.2 GPU内存管理

```
lib/Runtime/
├── Memory.h              # 统一内存
├── Buffer.h              # GPU缓冲区
└── CopyEngine.h           # 内存拷贝
```

**里程碑**: 可在GPU上执行简单物理计算

---

### Phase 5: JIT编译器核心 (v0.5) - Week 12-16

> 注：此时已了解物理仿真需求，JIT目标更明确

#### 5.1 MLIR Dialect定义

```
lib/Compiler/Dialect/Sim/
├── SimDialect.td         # Dialect定义
├── SimOps.td             # 操作定义
└── SimTypes.td           # 类型定义
```

**核心操作**:
- `sim.field` - 定义场数据
- `sim.kernel` - 定义计算核
- `sim.for` - 并行循环
- `sim.reduce` - 归约操作

#### 5.2 JIT引擎

```
lib/Compiler/JIT/
├── JITCompiler.h         # JIT入口
├── KernelCache.h         # Kernel缓存
└── CompilationPipeline.h # 编译管道
```

**功能**:
- 运行时编译SimDialect → LLVM IR → 机器码
- Kernel缓存与复用
- 热重载支持

**里程碑**: 可动态编译简单的SimDialect Kernel

---

### Phase 6: 高级物理 (v0.6) - Week 17-20

#### 6.1 刚体动力学（完整）

```
include/attic/physics/
├── rigid_body/
│   ├── Body.h            # 完整刚体（含旋转）
│   ├── Inertia.h         # 转动惯量
│   ├── ContactSolver.h   # 接触求解
│   └── ImpulseSolver.h   # 冲量求解
├── collision/
│   ├── BroadPhase.h      # 宽阶段
│   ├── NarrowPhase.h     # 窄阶段
│   └── SAT.h             # SAT碰撞检测
└── integrator/
    ├── Verlet.h          # Verlet积分
    ├── RK4.h             # 4阶龙格库塔
    └── Implicit.h         # 隐式积分
```

#### 6.2 有限元 (FEM)

```
include/attic/physics/fem/
├── Mesh.h                # 网格
├── Material.h            # 材料模型
├── Cloth.h               # 布料
└── Elasticity.h          # 弹性体
```

**功能**:
- 质点-弹簧模型（布料）
- 线性/非线性弹性
- 隐式/显式积分

**里程碑**: 刚体碰撞 + 布料仿真

---

### Phase 7: 机器人动力学 (v0.7) - Week 21-24

```
include/attic/physics/robot/
├── Joint.h               # 关节类型
├── Chain.h               # 运动链
├── ForwardKinematics.h  # 正向运动学
├── InverseKinematics.h  # 逆向运动学
├── Dynamics.h            # 动力学
└── URDFLoader.h         # URDF解析
```

**功能**:
- 关节建模（revolute, prismatic, fixed）
- 正/逆运动学
- 动力学（正向，逆向）
- URDF模型导入

**里程碑**: 可加载URDF并模拟机器人运动

---

### Phase 8: 内存优化 (v0.8) - Week 25-26

```
include/attic/core/
├── SoA.h                 # Structure of Arrays
├── AoS.h                 # Array of Structures
└── MemoryPool.h          # 内存池
```

**优化**:
- SoA布局（GPU友好）
- 内存池减少分配
- 数据预取优化
- GPU显存优化

---

### Phase 9: 多后端支持 (v0.9) - Week 27-29

| 后端 | 描述 |
|------|------|
| CUDA | NVIDIA GPU |
| Vulkan | 跨平台GPU |
| Metal | Apple GPU |

```
lib/Compiler/Codegen/
├── CPUCodegen.cpp
├── CUDACodegen.cpp
└── VulkanCodegen.cpp
```

**里程碑**: 一套代码，多后端运行

---

### Phase 10: 测试与优化 (v0.10) - Week 30-31

- 单元测试覆盖
- 性能基准测试
- Bug修复与优化

---

### Phase 11: 稳定版 (v1.0) - Week 32

- 文档完善
- API冻结
- Release发布

---

## 技术架构

### 1. 开发顺序（Phase 2 优先物理）

```
Phase 1: 基础框架
    │
    ▼
Phase 2: 物理仿真核心 ⭐ 当前
    ├── World/Body/Integrator
    ├── 简单碰撞检测
    └── 测试程序
    │
    ▼
Phase 3: Python绑定
    ├── pybind11集成
    └── Python API
    │
    ▼
Phase 4: 运行时系统
    ├── 设备抽象
    └── GPU内存管理
    │
    ▼
Phase 5: JIT编译器（此时已了解需求）
    ├── MLIR Dialect
    └── JIT引擎
    │
    ▼
Phase 6-11: 高级功能与优化
```

### 2. JIT编译流程（Phase 5）

```
Python/C++ API
       ↓
SimDialect IR (MLIR)
       ↓
   [优化Passes]
       ↓
   LLVM IR / SPIRV
       ↓
   JIT编译
       ↓
   GPU/CPU执行
```

### 3. 核心组件交互

```
┌─────────────────────────────────────────┐
│              Python API                  │
└─────────────────┬───────────────────────┘
                   ↓
┌─────────────────────────────────────────┐
│           Physics Engine                 │
│  ┌─────────┐  ┌─────────┐  ┌─────────┐  │
│  │  World  │→ │  Body   │→ │Integrator│ │
│  └─────────┘  └─────────┘  └─────────┘  │
│         ↓                                │
│  ┌─────────┐  ┌─────────┐               │
│  │ Collision│→ │ Solver  │               │
│  └─────────┘  └─────────┘               │
└─────────────────┬───────────────────────┘
                   ↓
┌─────────────────────────────────────────┐
│           Runtime Layer                 │
│  ┌─────────┐  ┌─────────┐  ┌─────────┐  │
│  │ Device  │  │ Memory  │  │ Stream  │  │
│  └─────────┘  └─────────┘  └─────────┘  │
└─────────────────┬───────────────────────┘
                   ↓
┌─────────────────────────────────────────┐
│            JIT Compiler                  │
│  ┌─────────┐  ┌─────────┐  ┌─────────┐  │
│  │ Frontend│→ │ Dialect │→ │ Codegen │  │
│  └─────────┘  └─────────┘  └─────────┘  │
└─────────────────────────────────────────┘
```

---

## 依赖项

| 组件 | 版本 | 用途 |
|------|------|------|
| CMake | ≥3.20 | 构建 |
| C++ | ≥17 | 语言 |
| pybind11 | ≥2.10 | Python绑定 |
| LLVM/MLIR | ≥17 | JIT编译（Phase 5） |
| CUDA | ≥11 | GPU后端（Phase 4+） |
| GoogleTest | ≥1.12 | C++测试 |

---

## 里程碑总结

| 版本 | 里程碑 | 时间 |
|------|--------|------|
| v0.1 | 基础框架 | Week 1-2 |
| v0.2 | **物理仿真核心** ⭐ | Week 3-5 |
| v0.3 | Python绑定 | Week 6-8 |
| v0.4 | 运行时系统 | Week 9-11 |
| v0.5 | JIT核心 | Week 12-16 |
| v0.6 | 高级物理 | Week 17-20 |
| v0.7 | 机器人动力学 | Week 21-24 |
| v0.8 | 内存优化 | Week 25-26 |
| v0.9 | 多后端 | Week 27-29 |
| v0.10 | 测试优化 | Week 30-31 |
| v1.0 | 稳定版 | Week 32 |

---

## Phase 2 详细任务分解

### Week 3: 基础物理系统

| 任务 | 文件 | 预计行数 |
|------|------|---------|
| Body类定义 | include/attic/physics/body.h | 50 |
| Body实现 | lib/physics/body.cpp | 30 |
| World类定义 | include/attic/physics/world.h | 80 |
| World实现 | lib/physics/world.cpp | 60 |
| Euler积分器 | include/attic/physics/integrator.h | 40 |
| 积分器实现 | lib/physics/integrator.cpp | 30 |
| 测试程序 | test/cpp/test_physics.cpp | 50 |

### Week 4: 碰撞检测

| 任务 | 文件 | 预计行数 |
|------|------|---------|
| 形状定义 | include/attic/physics/shapes.h | 60 |
| AABB包围盒 | include/attic/physics/collision/aabb.h | 50 |
| 球-球碰撞 | lib/physics/collision/sphere_sphere.cpp | 40 |
| 地面碰撞 | lib/physics/collision/ground.cpp | 30 |
| 碰撞响应 | include/attic/physics/collision/response.h | 40 |

### Week 5: 完善与测试

| 任务 | 文件 | 预计行数 |
|------|------|---------|
| Verlet积分器 | include/attic/physics/integrator_verlet.h | 50 |
| CMake更新 | CMakeLists.txt | 20 |
| 集成测试 | test/cpp/test_physics.cpp | 100 |
| 调试输出 | - | - |

---

## 参考项目

- [Taichi](https://github.com/taichi-dev/taichi) - GPU编程框架
- [Drake](https://github.com/RobotLocomotion/drake) - 机器人动力学
- [MuJoCo](https://github.com/deepmind/mujoco) - 物理仿真
- [Enzyme](https://github.com/EnzymeAD/Enzyme) - 自动微分
- [Box2D](https://github.com/erincatto/box2d) - 2D物理引擎（参考设计）
- [PhysX](https://github.com/NVIDIA-Omniverse/PhysX) - NVIDIA物理引擎
