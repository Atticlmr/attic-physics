# Attic Physics - 统一开发计划

## 项目概述

Attic Physics 是一个统一的物理仿真框架，整合了三个原先独立的项目：
- **attic-physics**: 物理引擎核心
- **taiee**: GPU编译器与运行时（已整合）
- **taishy**: 渲染引擎（可选，后期开发）

### 命名理念
- **太易 (Taiyi)**: 未见气也 - DSL编译器
- **太始 (Taishi)**: 形之始也 - 渲染引擎
- **太素 (Taisu)**: 质之始也 - 物理引擎（新增）

## 项目结构

```
attic-physics/
├── include/attic/           # 公共头文件
│   ├── core/               # 核心库（数学、日志）
│   ├── physics/            # 物理引擎
│   │   ├── body.h
│   │   ├── world.h
│   │   ├── integrator.h
│   │   └── collision/
│   ├── compiler/           # DSL编译器（原taiee）
│   │   ├── lexer/
│   │   ├── parser/
│   │   ├── ast/
│   │   ├── codegen/
│   │   └── runtime/
│   ├── gpu/                # GPU运行时
│   │   ├── device.h
│   │   ├── memory.h
│   │   └── buffer.h
│   └── render/             # 渲染引擎（原taishy）
│       ├── renderer.h
│       └── scene.h
├── lib/                    # 实现文件
│   ├── core/
│   ├── physics/
│   ├── compiler/
│   ├── gpu/
│   └── render/
├── kernels/                # GPU kernel定义
│   └── *.physics
├── python/                 # Python绑定
├── test/                   # 测试
├── examples/               # 示例
├── tools/                  # 工具
│   └── attic-compile.cpp  # DSL编译器CLI
└── docs/                   # 文档
```

## 开发阶段（重新规划）

### Phase 1: 基础框架 ✅ (已完成)
- [x] 项目结构
- [x] 数学库 (Vec3, Mat3, Quat)
- [x] 日志系统
- [x] 构建系统

### Phase 2: 物理引擎核心（当前 - 第1-2个月）⭐ 最高优先级

**目标**: 完成CPU物理引擎，实现基本刚体仿真

| 周次 | 任务 | 文件 | 优先级 |
|------|------|------|--------|
| W1 | Body类实现 | `lib/physics/body.cpp` | P0 |
| W1 | World类实现 | `lib/physics/world.cpp` | P0 |
| W1 | Euler积分器 | `lib/physics/integrator.cpp` | P0 |
| W2 | 碰撞检测基础 | `lib/physics/collision/` | P0 |
| W2 | 测试程序 | `test/cpp/test_physics.cpp` | P0 |
| W2 | 示例：小球下落 | `examples/bouncing_ball.cpp` | P0 |

**里程碑**: 小球从高处下落并弹跳

### Phase 3: Python绑定（第2-3个月）

**目标**: Python API，便于使用和快速原型

| 周次 | 任务 | 文件 |
|------|------|------|
| W3 | pybind11集成 | `python/CMakeLists.txt` |
| W3 | 基础绑定 | `python/attic_physics.cpp` |
| W4 | Python API设计 | `python/attic_physics/__init__.py` |
| W4 | 示例notebook | `examples/python/` |

**里程碑**: `import attic_physics as ap`

### Phase 4: GPU运行时（第3-4个月）

**目标**: GPU内存管理和kernel执行

```bash
# 构建时启用
cmake -DATTIC_BUILD_GPU=ON ..
```

| 周次 | 任务 | 文件 |
|------|------|------|
| W5 | SoA数据结构 | `include/attic/gpu/buffer.h` |
| W5 | GPU内存管理 | `lib/gpu/memory.cpp` |
| W6 | Device抽象 | `lib/gpu/device.cpp` |
| W6 | CPU vs GPU对比测试 | `test/cpp/test_gpu.cpp` |

**里程碑**: 数据能在CPU和GPU间正确传输

### Phase 5: DSL编译器（第4-6个月）

**目标**: 将.physics文件编译为GPU kernel

```bash
# 构建时启用
cmake -DATTIC_BUILD_COMPILER=ON ..

# 使用
./tools/attic-compile kernels/euler.physics -o euler.ptx
```

| 阶段 | 任务 | 说明 |
|------|------|------|
| 5.1 | Lexer/Parser | 基础DSL语法 |
| 5.2 | AST | 抽象语法树 |
| 5.3 | LLVM IR生成 | 编译到LLVM |
| 5.4 | PTX生成 | GPU代码生成 |
| 5.5 | AOT预编译 | 生产环境无需LLVM |

**里程碑**: 编译euler.physics并执行

### Phase 6: 渲染引擎（第6-8个月，可选）

**目标**: 基础可视化

```bash
# 构建时启用
cmake -DATTIC_BUILD_RENDER=ON ..
```

**注意**: 此阶段优先级较低，可以先使用Python matplotlib或集成现有渲染库。

### Phase 7: 高级物理（第8-10个月）

- 完整刚体动力学（旋转、惯性张量）
- 约束求解器
- 复杂碰撞检测（GJK, SAT）
- 机器人动力学（URDF/MJCF支持）

### Phase 8: 优化与发布（第10-12个月）

- 性能优化
- 多后端支持（CUDA/Vulkan/Metal）
- 文档完善
- PyPI发布

## 开发优先级

### P0 - 必须完成（Phase 2）
- World/Body类
- 至少一个积分器
- 基础碰撞检测
- 测试覆盖

### P1 - 重要（Phase 3-4）
- Python绑定
- GPU内存管理
- SoA数据结构

### P2 - 可选（Phase 5-6）
- DSL编译器
- 渲染引擎

### P3 - 后期（Phase 7-8）
- 高级物理特性
- 多后端支持

## 构建配置

### 基础构建（仅CPU物理引擎）
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 开发构建（全功能）
```bash
cmake -DATTIC_BUILD_GPU=ON \
      -DATTIC_BUILD_COMPILER=ON \
      -DATTIC_BUILD_PYTHON=ON \
      -DATTIC_BUILD_TESTS=ON \
      -DATTIC_BUILD_EXAMPLES=ON ..
```

### 最小构建（仅运行时）
```bash
cmake -DATTIC_BUILD_GPU=ON \
      -DATTIC_BUILD_COMPILER=OFF \
      -DATTIC_BUILD_TESTS=OFF ..
```

## 依赖项

| 组件 | 版本 | Phase | 必需 |
|------|------|-------|------|
| CMake | ≥3.20 | 1 | 是 |
| C++ | ≥17 | 1 | 是 |
| CUDA | ≥11 | 4 | 否 |
| LLVM | ≥15 | 5 | 否 |
| pybind11 | ≥2.10 | 3 | 否 |
| Vulkan | ≥1.3 | 6 | 否 |

## 成功标准

### Phase 2完成标准
- [ ] 1000个小球稳定60FPS（CPU）
- [ ] 碰撞检测正确
- [ ] 能量守恒（误差<1%）
- [ ] Python API可用

### Phase 5完成标准
- [ ] GPU加速比CPU快10倍以上
- [ ] AOT预编译kernel可用
- [ ] 自定义kernel支持

## 注意事项

1. **渐进式开发**: 不要一次性实现所有功能
2. **测试先行**: 每个模块都要有对应的测试
3. **性能基准**: 定期对比CPU/GPU性能
4. **文档同步**: 代码和文档同时更新

---

*版本: 2.0*  
*更新: 2026-03-17*  
*作者: attic-physics team*
