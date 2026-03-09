# GPU并行数值计算与物理引擎开源项目调研

> 基于 Context7 MCP 服务和 Web 搜索的综合调研
>
> 参考来源: MuJoCo, Taichi, Chrono, Bullet Physics, NVIDIA CUDA

---

## 1. GPU并行数值计算库

### 1.1 cuBLAS (NVIDIA)

**描述**: CUDA Basic Linear Algebra Subprograms，NVIDIA官方GPU加速BLAS库

**技术特点**:
- 专为NVIDIA GPU设计
- Level-1/2/3 BLAS操作完全GPU加速
- 支持批量矩阵运算 (Batched operations)
- 显存管理: 设备端指针直接传递

**核心API**:
```cuda
cublasStatus_t cublasSgemm(cublasHandle_t handle,
    cublasOperation_t transa, cublasOperation_t transb,
    int m, int n, int k,
    const float *alpha,
    const float *A, int lda,
    const float *B, int ldb,
    const float *beta,
    float *C, int ldc);
```

**License**: NVIDIA CUDA Toolkit License (免费，用于商业产品)

**性能**: GPU矩阵乘法比CPU快100-1000倍

---

### 1.2 cuSOLVER (NVIDIA)

**描述**: GPU加速的线性代数求解库，基于cuBLAS和cuSPARSE

**技术特点**:
- 密集/稀疏矩阵分解 (LU, QR, Cholesky)
- 稀疏线性最小二乘求解
- 特征值求解
- 支持单节点多GPU (cuSolverMG)

**核心功能**:
- Cholesky分解: `cusolverDnSpotrf()`
- LU分解: `cusolverDnSgetrf()`
- LDL^T分解: `cusolverDnSsytrf()`

**ImplicitFast积分器的关键**: LDL^T分解比LU分解快2-3倍！

---

### 1.3 cuSPARSE (NVIDIA)

**描述**: GPU加速的稀疏矩阵运算

**技术特点**:
- CSR/CSC格式支持
- 稀疏矩阵-向量乘法 (SpMV)
- 稀疏矩阵-矩阵乘法 (SpMM)
- 三角求解器

**物理引擎应用**:
- 碰撞检测的稀疏Jacobian计算
- 约束求解器的稀疏线性系统

---

### 1.4 Thrust (NVIDIA)

**描述**: C++并行算法库，类似STL

**技术特点**:
- 提供GPU/CPU统一抽象
- 自动选择最优实现
- 高级数据并行原语: scan, sort, reduce, transform

**代码示例**:
```cpp
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/reduce.h>

thrust::device_vector<float> d_x(N);
thrust::device_vector<float> d_y(N);

// 并行归约
float sum = thrust::reduce(d_x.begin(), d_x.end(), 0.0f, thrust::plus<float>());
```

---

### 1.5 Eigen

**描述**: C++模板库，线性代数矩阵运算

**技术特点**:
- 头文件-only，无依赖
- 模板元编程实现零开销抽象
- 支持SIMD加速 (SSE/AVX/NEON)
- 提供GPU后端接口

**使用场景**:
- CPU端物理模拟
- 小规模矩阵运算
- 与GPU库的桥接

---

### 1.6 MAGMA

**描述**: 矩阵代数线性加速库，UTK/UC Berkeley开发

**技术特点**:
- 混合CPU-GPU计算
- 多GPU支持
- 类似 LAPACK 的接口

**License**: BSD 3-Clause

---

### 1.7 ViennaCL

**描述**: OpenCL/Vulkan通用的线性代数库

**技术特点**:
- OpenCL后端 (AMD/Intel/NVIDIA通用)
- 稀疏矩阵求解器
- 迭代求解器 (CG, GMRES)

**License**: MIT

---

### 1.8 Taichi (线性代数模块)

**描述**: 高性能Python+GPU编程语言的内置线性代数

**技术特点**:
- 稀疏矩阵支持
- GPU/CPU统一API
- 自动微分支持

**代码示例**:
```python
import taichi as ti
ti.init(arch=ti.cuda)

K = ti.linalg.SparseMatrixBuilder(n, n, max_num_triplets=100)
solver = ti.linalg.SparseSolver(solver_type='LLT')
solver.analyze_pattern(A)
solver.factorize(A)
x = solver.solve(b)
```

---

## 2. 并行物理引擎

### 2.1 MuJoCo MJX

**描述**: Google DeepMind的MuJoCo GPU加速后端

**技术特点**:
- 完全基于JAX (Python)
- GPU/TPU加速的刚体动力学
- 自动微分原生支持
- 适合强化学习训练

**架构**:
```
Python/JAX → MJX (JAX transformations) → XLA → GPU/TPU
```

**性能**: 比CPU快100-1000倍

**License**: Apache 2.0

---

### 2.2 Taichi

**描述**: Python高性能计算语言，原生物理模拟

**技术特点**:
- 领域特定编译器
- 多后端: CPU/CUDA/Vulkan/Metal/OpenGL
- 自动微分
- 稀疏数据结构

**架构**:
```
Python → Taichi Compiler → LLVM/SPIR-V → GPU
```

**代码示例** (布料模拟):
```python
import taichi as ti
ti.init(arch=ti.cuda)

n = 128
x = ti.Vector.field(3, dtype=float, shape=(n, n))
v = ti.Vector.field(3, dtype=float, shape=(n, n))

@ti.kernel
def substep():
    for i in ti.grouped(x):
        v[i] += gravity * dt
        x[i] += v[i] * dt
```

**并行策略**:
- 细粒度并行: 每个粒子一个GPU线程
- 稀疏访问: 仅处理活跃区域
- 多物理场耦合: 通过ti.field交互

**License**: MIT

---

### 2.3 Chrono::GPU

**描述**: Project Chrono的GPU加速模块

**技术特点**:
- 专注颗粒材料 (DEM) 模拟
- CUDA加速
- 显式时间积分
- 摩擦模型

**CMake配置**:
```cmake
set(CHRONO_CUDA_ARCHITECTURES "native")
find_package(CUDA)
```

**源码组织**:
```
src/chrono_gpu/
├── cuda/ChGpu_SMC.cu       # 物质点方法
├── cuda/ChGpuCollision.cuh  # GPU碰撞检测
└── cuda/ChGpuBoundaryConditions.cuh
```

**License**: BSD 3-Clause

---

### 2.4 Bullet Physics

**描述**: 知名开源物理引擎，游戏/VR/机器人广泛使用

**并行策略**:
- CPU多线程: OpenMP / Intel TBB / Microsoft PPL
- 无原生GPU后端 (可通过OpenCL扩展)

**CMake选项**:
```cmake
BULLET2_MULTITHREADING ON
BULLET2_USE_OPEN_MP_MULTITHREADING ON
```

**代码示例**:
```cpp
#include <btBulletDynamicsCommon.h>

btDefaultCollisionConfiguration* config = new btDefaultCollisionConfiguration();
btCollisionDispatcher* dispatcher = new btCollisionDispatcher(config);
btDbvtBroadphase* broadphase = new btDbvtBroadphase();
btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
btDiscreteDynamicsWorld* world = new btDiscreteDynamicsWorld(
    dispatcher, broadphase, solver, config);
world->setGravity(btVector3(0, -9.8, 0));
```

**License**: ZLib (免费，商业友好)

---

### 2.5 NVIDIA PhysX

**描述**: NVIDIA物理引擎，游戏领域最广泛使用

**技术特点**:
- GPU加速 (CUDA)
- CPU多线程 (Job系统)
- 完整的游戏物理解决方案
- 布料/流体/粒子

**状态**: NVIDIA PhysX 5已开源 (BSD 3-Clause)

---

### 2.6 NVIDIA Flex

**描述**: 基于粒子的GPU物理API

**技术特点**:
- 完全GPU计算
- 粒子系统核心
- 统一材质模型
- 即时模式API

**状态**: 已停止维护，但技术仍有参考价值

---

### 2.7 AMD开放物理计划

**描述**: AMD的开放物理计算平台

**技术特点**:
- 基于OpenCL
- CPU+GPU联合计算
- 开源免费

**联盟成员**: Bullet Physics, AGEIA PhysX (后被NVIDIA收购)

---

## 3. 技术路线对比

### 3.1 架构风格对比

| 项目 | 架构 | 并行策略 | 自动微分 | 适用场景 |
|------|------|---------|---------|---------|
| **MuJoCo MJX** | JAX → XLA → GPU | 数据并行 | 原生 | 强化学习/机器人 |
| **Taichi** | 自研编译器 → LLVM/SPIR-V | 细粒度并行 | 支持 | 图形/模拟/可微编程 |
| **Chrono::GPU** | C++ → CUDA | 显式GPU | 需手动 | 颗粒材料/车辆 |
| **Bullet** | C++ → CPU多线程 | OpenMP/TBB | 无 | 游戏/VR |
| **PhysX** | CUDA/C++多线程 | Job系统 | 无 | AAA游戏 |

### 3.2 数值计算库对比

| 库 | GPU后端 | 稀疏矩阵 | 求解器类型 | License |
|----|---------|---------|-----------|---------|
| **cuBLAS** | NVIDIA专用 | 基础 | 直接法 | NVIDIA |
| **cuSOLVER** | NVIDIA专用 | 支持 | 完整 | NVIDIA |
| **cuSPARSE** | NVIDIA专用 | 完整 | 迭代 | NVIDIA |
| **Thrust** | NVIDIA/AMD/Intel | 基础 | 无 | Apache 2.0 |
| **Eigen** | CPU SIMD | 支持 | 基础 | MPL 2.0 |
| **ViennaCL** | OpenCL | 完整 | 迭代 | MIT |
| **MAGMA** | 多GPU | 支持 | 完整 | BSD |

### 3.3 推荐的物理引擎技术路线

#### 方案A: 参考Taichi (推荐)

```
优势:
- Python用户友好
- 自动多后端
- 自动微分
- 活跃社区

劣势:
- 依赖Python
- JIT编译启动慢

适合: 快速原型/研究
```

#### 方案B: 参考Chrono

```
优势:
- C++高性能
- 成熟可靠
- 模块化

劣势:
- GPU支持有限
- 仅DEM强项

适合: 工程应用/车辆仿真
```

#### 方案C: 参考MuJoCo MJX

```
优势:
- 极致性能
- 自动微分
- 强化学习集成

劣势:
- 仅刚体
- 依赖JAX

适合: 机器人/强化学习
```

#### 方案D: 自建 (适合本项目)

```
优势:
- 完全控制
- 定制优化
- 学习价值

劣势:
- 工作量大
- 需要深入GPU编程

适合: 长期自研/差异化
```

---

## 4. 本项目技术路线建议

### 4.1 Phase 2: 基础物理 (当前)

- 使用CPU + Semi-implicit Euler
- 参考MuJoCo的积分器设计
- Eigen做小规模矩阵运算

### 4.2 Phase 4: GPU加速

**推荐策略**:
1. **数值计算层**: 
   - cuBLAS/cuSOLVER做密集/稀疏矩阵
   - Thrust做并行算法原语

2. **物理引擎层**:
   - 参考Taichi的细粒度并行
   - 参考Chrono的CUDA模块组织

3. **可选**:
   - 集成Eigen (CPU fallback)
   - OpenCL后端 (跨平台)

### 4.3 关键技术决策点

| 决策 | 推荐 | 备选 |
|------|------|------|
| GPU API | CUDA (NVIDIA) | OpenCL (跨平台) |
| CPU矩阵库 | Eigen | - |
| 并行原语 | Thrust | 自建 |
| 稀疏求解 | cuSOLVER | 自建CG |
| 可微编程 | 暂不 | 后续加Taichi反向模式 |

---

## 5. 参考资源汇总

### 官方文档

- [MuJoCo Documentation](https://mujoco.readthedocs.io/)
- [Taichi Docs](https://docs.taichi-lang.org/)
- [NVIDIA CUDA Libraries](https://developer.nvidia.com/cuda-toolkit)
- [Chrono GPU Module](https://projectchrono.org/)
- [Bullet Physics](https://github.com/bulletphysics/bullet3)

### 论文/教程

- "MuJoCo: A Physics Engine for Model-Based Control" (DeepMind, 2022)
- "Taichi: A Language for Portable High-Performance Computing" (SC 2017)
- "GPU Computing for Physics-based Simulation" (NVIDIA GTC)

---

## 6. 总结

本项目的GPU物理引擎技术路线推荐:

```
第一阶段 (CPU):
  Eigen (矩阵) + 自建物理 (Semi-implicit Euler)
        ↓
第二阶段 (GPU加速):
  cuBLAS/cuSOLVER/Thrust + CUDA物理kernel
        ↓
第三阶段 (高级):
  多后端支持 + 自动微分 + JIT
```

这个路线既符合渐进式开发原则，又能充分利用现有开源资源的最佳实践。

---

**文档创建时间**: 2026-03-03
**调研工具**: Context7 MCP Service, MiniMax Web Search
**主要参考**: MuJoCo, Taichi, Chrono, Bullet Physics, NVIDIA CUDA
