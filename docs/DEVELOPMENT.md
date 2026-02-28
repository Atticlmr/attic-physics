# Attic Physics 开发计划

## 项目概述

基于GPU的通用机器人仿真库，使用LLVM/MLIR实现类似Taichi的高性能仿真框架。支持刚体动力学、有限元分析（布料、杨氏模量）等功能。

## 目录结构

```
attic-physics/
├── include/attic/              # 公共API头文件
│   ├── attic.h                # 主入口
│   ├── math/                  # 数学库
│   ├── rigid_body/            # 刚体动力学
│   ├── robot/                 # 机器人
│   ├── sim/                   # 仿真API
│   │   ├── field.h            # 场（类似taichi Field）
│   │   ├── kernel.h           # 计算核
│   │   └── executor.h         # 执行器
│   ├── fea/                   # 有限元分析
│   │   └── solid/             # 固体有限元
│   │       ├── cloth.h        # 布料仿真
│   │       └── elasticity.h   # 杨氏模量
│   ├── fem/                   # FEM基础
│   │   ├── mesh.h
│   │   └── material.h
│   └── common/                 # 通用工具
├── src/
│   ├── llvm/                   # LLVM/MLIR核心
│   │   ├── dialect/            # 定义仿真DSL的Dialect
│   │   ├── codegen/            # 代码生成
│   │   ├── jit/                # JIT编译驱动
│   │   └── mlir/               # MLIR工具
│   ├── runtime/                # 运行时
│   │   ├── MemoryManager.cpp   # GPU内存管理
│   │   ├── DeviceRuntime.cpp  # 设备运行时
│   │   └── TaskScheduler.cpp  # 任务调度
│   ├── physics/               # 物理仿真
│   │   ├── rigid_body/        # 刚体
│   │   ├── collision/         # 碰撞
│   │   ├── fem/               # 有限元
│   │   │   ├── solid/         # 固体
│   │   │   └── fluid/         # 流体
│   │   └── robot/             # 机器人动力学
│   ├── frontend/              # 前端（Python/C++ API）
│   ├── stdlib/                 # 标准库（内置Kernel）
│   └── tests/
├── third_party/
│   └── llvm/                  # LLVM/MLIR
└── scripts/
```

## 开发阶段

### Phase 1: 基础框架 (v0.1)
- [x] 项目基础结构搭建
- [x] CMake构建系统配置
- [ ] 基础数学库（vec3, mat3, quat）
- [ ] 基础调试日志系统

### Phase 2: LLVM/MLIR核心 (v0.2)
- [ ] MLIR Dialect定义（SimDialect）
- [ ] 基本Ops定义（field, kernel, for, reduce等）
- [ ] IR Builder实现
- [ ] CPU Codegen后端
- [ ] JIT Driver实现
- [ ] 简单Kernel示例

### Phase 3: 运行时系统 (v0.3)
- [ ] GPU内存管理
- [ ] Device抽象层
- [ ] 任务调度器
- [ ] Kernel缓存机制

### Phase 4: 刚体动力学 (v0.4)
- [ ] 刚体数据结构
- [ ] 碰撞检测（AABB, SAT）
- [ ] 接触求解器
- [ ] 积分器（Euler, Verlet, RK4）

### Phase 5: 机器人动力学 (v0.5)
- [ ] 关节模型（revolute, prismatic, fixed）
- [ ] 正/逆运动学
- [ ] 动力学（正向、逆向）
- [ ] URDF解析

### Phase 6: 有限元分析 (v0.6)
- [ ] 网格数据结构
- [ ] 布料仿真（质点-弹簧模型）
- [ ] 弹性体仿真（杨氏模量）
- [ ] 隐式/显式积分器

### Phase 7: 内存优化 (v0.7)
- [ ] SoA (Structure of Arrays) 布局
- [ ] 内存池
- [ ] 数据预取优化
- [ ] GPU显存优化

### Phase 8: 多后端支持 (v0.8)
- [ ] CUDA Codegen
- [ ] Vulkan/SPIR-V Codegen
- [ ] Metal Codegen（可选）

### Phase 9: 前端与工具 (v0.9)
- [ ] Python绑定 (pybind11)
- [ ] C++ API完善
- [ ] 可视化工具集成

### Phase 10: 测试与优化 (v1.0)
- [ ] 单元测试
- [ ] 性能基准测试
- [ ] Bug修复与优化
- [ ] 文档完善

## 核心技术要点

### 1. MLIR Dialect设计
```mlir
// 示例：仿真Kernel的IR
func.func @simulation_kernel(%pos: !sim.field<tensor<1000xf32>>, %vel: !sim.field<tensor<1000xf32>>) {
  sim.for %i = 0 to 1000 {
    %v = sim.load %vel[%i]
    %p = sim.load %pos[%i]
    %new_p = sim.add %p %v
    sim.store %pos[%i], %new_p
  }
}
```

### 2. 有限元数据结构
- **节点**: position, velocity, force
- **单元**: connectivity, material properties
- **材料**: Young's modulus, Poisson ratio, density

### 3. 内存布局策略
- 优先使用SoA布局（利于GPU并行）
- 避免 atomic write conflict
- 使用 shared memory 做数据复用

## 依赖项

- LLVM/MLIR >= 17
- CMake >= 3.20
- C++17
- (可选) CUDA Toolkit
- (可选) Vulkan SDK
- (可选) Python 3.8+

## 里程碑

| 版本 | 里程碑 | 预计时间 |
|------|--------|----------|
| v0.1 | 基础框架 | Week 1-2 |
| v0.2 | LLVM核心 | Week 3-5 |
| v0.3 | 运行时 | Week 6-7 |
| v0.4 | 刚体 | Week 8-10 |
| v0.5 | 机器人 | Week 11-13 |
| v0.6 | 有限元 | Week 14-17 |
| v0.7 | 内存优化 | Week 18-19 |
| v0.8 | 多后端 | Week 20-22 |
| v0.9 | 前端 | Week 23-25 |
| v1.0 | 稳定版 | Week 26+ |

## 参考项目

- [Taichi](https://github.com/taichi-dev/taichi) - GPU编程框架
- [Flex](https://github.com/YichengShen/Flex) - 基于Taichi的物理仿真
- [PhyX](https://github.com/zekun000/PhyX) - GPU物理仿真
- [MuJoCo](https://github.com/deepmind/mujoco) - 机器人仿真
- [Drake](https://github.com/RobotLocomotion/drake) - 机器人动力学
