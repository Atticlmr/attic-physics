# Attic Physics

> **太素者，质之始也** ——《列子·天瑞》

一个统一的物理仿真框架，整合物理引擎、GPU编译器和渲染引擎。

## 项目整合

本项目整合了三个原先独立的项目：

| 原项目 | 整合位置 | 状态 |
|--------|----------|------|
| attic-physics | `include/attic/physics/` | 基础框架完成 |
| taiee (太易) | `include/attic/compiler/` | 待开发 |
| taishy (太始) | `include/attic/render/` | 待开发 |

## 特性

- **CPU物理引擎** - 刚体动力学、碰撞检测、约束求解
- **GPU加速** - CUDA/Vulkan多后端支持
- **DSL编译器** - 自定义.physics语言，AOT预编译
- **Python绑定** - 简单易用的Python API
- **可选渲染** - Vulkan-based可视化

## 快速开始

### 基础构建（仅CPU物理引擎）

```bash
git clone https://github.com/yourusername/attic-physics.git
cd attic-physics
mkdir build && cd build
cmake ..
make -j$(nproc)
./test_core
```

### 启用GPU支持

```bash
cmake -DATTIC_BUILD_GPU=ON ..
```

### 启用DSL编译器

```bash
cmake -DATTIC_BUILD_COMPILER=ON ..
```

## 项目结构

```
attic-physics/
├── include/attic/        # 头文件
│   ├── core/            # 数学库、日志
│   ├── physics/         # 物理引擎
│   ├── compiler/        # DSL编译器
│   ├── gpu/             # GPU运行时
│   └── render/          # 渲染引擎
├── lib/                 # 实现文件
├── kernels/             # GPU kernel定义
├── python/              # Python绑定
├── test/                # 测试
├── examples/            # 示例
└── tools/               # 工具
```

## 开发阶段

1. **Phase 1** ✅ 基础框架 - 数学库、日志、构建系统
2. **Phase 2** ⭐ 物理核心 - World、Body、积分器（当前）
3. **Phase 3** Python绑定 - pybind11集成
4. **Phase 4** GPU运行时 - SoA、内存管理
5. **Phase 5** DSL编译器 - .physics → GPU
6. **Phase 6** 渲染引擎 - Vulkan可视化
7. **Phase 7** 高级物理 - 刚体动力学、机器人
8. **Phase 8** 优化发布 - 性能优化、PyPI

详见 [docs/DEVELOPMENT.md](docs/DEVELOPMENT.md)

## 使用示例

### C++

```cpp
#include <attic/physics/world.h>

int main() {
    attic::physics::World world;
    
    auto& ball = world.create_body();
    ball.position = Vec3(0, 10, 0);
    ball.mass = 1.0f;
    
    for (int i = 0; i < 1000; ++i) {
        world.step(0.016f);
        printf("t=%.2f, y=%.2f\n", i * 0.016f, ball.position.y);
    }
    
    return 0;
}
```

### Python

```python
import attic_physics as ap

world = ap.World()
ball = world.create_body(mass=1.0)
ball.position = (0, 10, 0)

world.simulate(duration=10.0, dt=0.016)
```

### DSL (Phase 5+)

```physics
// kernels/euler.physics
kernel void integrate(
    float[] x, float[] y, float[] z,
    float[] vx, float[] vy, float[] vz,
    float[] mass, int n, float dt
) {
    int i = globalId();
    if (i >= n) return;
    
    // Apply gravity
    vy[i] = vy[i] + 9.8 * dt;
    
    // Update position
    x[i] = x[i] + vx[i] * dt;
    y[i] = y[i] + vy[i] * dt;
    z[i] = z[i] + vz[i] * dt;
}
```

## 依赖项

- CMake ≥3.20
- C++17兼容编译器
- CUDA ≥11 (可选)
- LLVM ≥15 (可选)
- pybind11 ≥2.10 (可选)
- Vulkan ≥1.3 (可选)

## 贡献

欢迎贡献！请阅读 [docs/DEVELOPMENT.md](docs/DEVELOPMENT.md) 了解开发计划。

## 许可

MIT License

## 致谢

- 命名灵感：《列子·天瑞》
- 架构参考：Taichi, MuJoCo, Drake
