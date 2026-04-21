# Context7 MCP 服务配置指南

> 用于查询 MuJoCo 物理引擎的专业文档和实现细节

---

## 1. 概述

本项目使用 **Context7 MCP (Model Context Protocol)** 服务来获取权威的物理仿真参考资料。Context7 提供了对多个流行编程库和框架的专业文档访问，包括 Google DeepMind 的 **MuJoCo** 物理引擎。

### 用途

- 查询积分器的实现原理
- 了解物理引擎的架构设计
- 参考最佳实践
- 获取 API 使用示例

---

## 2. 配置要求

### 2.1 MCP 服务配置

确保你的开发环境已配置 Context7 MCP 服务。以下是典型的配置文件示例：

```json
{
  "mcpServers": {
    "context7": {
      "command": "npx",
      "args": ["-y", "@context7/mcp-server"],
      "env": {
        "CONTEXT7_API_KEY": "your-api-key-here"
      }
    }
  }
}
```

### 2.2 API 密钥

从 Context7 获取 API 密钥：
1. 访问 [Context7 官网](https://context7.com)
2. 注册账号
3. 获取 API Key
4. 在 MCP 配置中设置 `CONTEXT7_API_KEY`

---

## 3. 可用文档库

### 3.1 MuJoCo 相关

| Library ID | 名称 | 描述 |
|------------|------|------|
| `/google-deepmind/mujoco` | MuJoCo | 物理引擎核心（推荐） |
| `/websites/mujoco_readthedocs_io_en_stable` | MuJoCo Docs | 官方文档网站 |
| `/google-deepmind/dm_control` | dm_control | DeepMind 控制框架 |

### 3.2 其他相关

| Library ID | 名称 | 描述 |
|------------|------|------|
| `/scipy/scipy` | SciPy | 科学计算（数值积分参考） |
| `/websites/en_cppreference_w` | cppreference | C++ 参考手册 |

---

## 4. 查询示例

### 4.1 积分器相关

#### 查询可用的积分器类型

```
查询: "integrator semi-implicit Euler Runge-Kutta implicit time integration"
```

返回：MuJoCo 支持的积分器类型、配置选项、实现原理

#### 查询 Semi-implicit Euler

```
查询: "Euler semi-implicit position velocity update formula"
```

返回：半隐式欧拉方法的数学公式和代码实现

#### 查询 ImplicitFast

```
查询: "implicitfast integrator coriolis centrifugal derivatives skipped"
```

返回：ImplicitFast 的优化原理和性能对比

---

### 4.2 动力学相关

#### 查询前向动力学

```
查询: "forward dynamics mj_fwdVelocity computeAcceleration"
```

返回：如何从位置和速度计算加速度

#### 查询逆动力学

```
查询: "inverse dynamics RNE recursive Newton-Euler"
```

返回：逆动力学算法的实现原理

#### 查询约束求解

```
查询: "constraint solver contact collision response"
```

返回：接触约束的处理方法

---

### 4.3 碰撞检测相关

```
查询: "collision detection broad phase narrow phase"
```

返回：碰撞检测的两阶段方法

---

## 5. 查询模板

### 5.1 基础查询

```
# 查询特定功能
integrator implementation details

# 查询API使用
mj_step function usage example

# 查询数学原理
numerical integration methods comparison
```

### 5.2 高级查询

```
# 查询特定积分器
implicit integrator velocity derivatives matrix

# 查询性能优化
sparse matrix computation performance

# 查询最佳实践
robotics simulation best practices
```

---

## 6. 在项目中的使用流程

### 6.1 查询步骤

```
1. 确定查询目标
   ↓
2. 选择合适的文档库 (/google-deepmind/mujoco)
   ↓
3. 构造查询语句
   ↓
4. 使用 MCP 工具查询
   ↓
5. 整理返回的文档内容
   ↓
6. 参考实现
```

### 6.2 代码示例

```cpp
// 使用 Context7 查询到的信息实现积分器

/**
 * Semi-implicit Euler Integrator
 * 
 * 参考来源: MuJoCo 官方文档
 * 查询语句: "Euler semi-implicit position velocity update formula"
 * 
 * 数学公式:
 *   v(t+Δt) = v(t) + a(t)Δt
 *   x(t+Δt) = x(t) + v(t+Δt)Δt
 */
class SemiImplicitEulerIntegrator {
public:
    void integrate(Body& body, float dt) {
        // 1. 更新速度
        body.velocity += body.acceleration * dt;
        
        // 2. 更新位置（使用新速度）
        body.position += body.velocity * dt;
        
        // 3. 清除加速度
        body.acceleration = Vec3::zero();
    }
};
```

---

## 7. 常用查询记录

### 7.1 积分器

| 主题 | 查询语句 | 文档库 |
|------|---------|--------|
| Semi-implicit Euler | "Euler semi-implicit position velocity update formula" | `/google-deepmind/mujoco` |
| RK4 | "Runge-Kutta 4th order implementation" | `/google-deepmind/mujoco` |
| Implicit | "implicit integrator velocity derivatives" | `/google-deepmind/mujoco` |
| ImplicitFast | "implicitfast coriolis centrifugal skipped" | `/google-deepmind/mujoco` |

### 7.2 动力学

| 主题 | 查询语句 | 文档库 |
|------|---------|--------|
| 前向动力学 | "forward dynamics mj_fwdVelocity" | `/google-deepmind/mujoco` |
| 逆动力学 | "inverse dynamics RNE algorithm" | `/google-deepmind/mujoco` |
| 质量矩阵 | "mass matrix computation" | `/google-deepmind/mujoco` |

### 7.3 仿真流程

| 主题 | 查询语句 | 文档库 |
|------|---------|--------|
| 仿真步骤 | "mj_step simulation step function" | `/google-deepmind/mujoco` |
| 两步仿真 | "mj_step1 mj_step2" | `/google-deepmind/mujoco` |
| 选项配置 | "option integrator configuration" | `/google-deepmind/mujoco` |

---

## 8. 注意事项

### 8.1 查询限制

- 每个查询有字符限制，请保持查询简洁
- 复杂查询建议拆分成多个简单查询
- 查询结果有数量限制，请关注最重要的信息

### 8.2 结果验证

- Context7 返回的是文档引用，请核实最新版本
- 某些实现可能与最新版本有差异
- 建议配合官方 GitHub 源码一起参考

### 8.3 速率限制

- 免费账户有每日查询次数限制
- 如需大量查询，考虑升级付费套餐

---

## 9. 相关文档

- [积分器详解文档](./integrators.md) - 基于 Context7 查询结果整理
- [MuJoCo 官方文档](https://mujoco.readthedocs.io/)
- [MuJoCo GitHub](https://github.com/google-deepmind/mujoco)

---

## 10. 快速开始

### 查询积分器列表

```bash
# 使用 context7_resolve-library-id 查询可用的文档库
context7_resolve-library-id(libraryName="mujoco")
```

### 查询具体实现

```bash
# 使用 context7_query-docs 查询积分器详情
context7_query-docs(
    libraryId="/google-deepmind/mujoco",
    query="integrator semi-implicit Euler implementation"
)
```

---

## 11. 实际查询示例（持续更新）

### 11.1 逆动力学 (Inverse Dynamics)

**查询语句**: "forward dynamics inverse dynamics RNE recursive Newton Euler algorithm implementation"

**关键发现**:
- `mj_inverse()` 计算实现指定加速度所需的力
- 输入: `qpos`, `qvel`, `qacc`
- 输出: `qfrc_inverse` (需要的作用力)
- 使用 **RNE (Recursive Newton-Euler)** 算法

**代码示例**:
```c
// 设置状态
d->qpos[0] = 1.0;
d->qvel[0] = 0.5;
d->qacc[0] = 0.0;  // 期望零加速度（保持位置）

// 计算逆动力学
mj_inverse(m, d);

// 获取所需力
printf("Required force: %.4f\n", d->qfrc_inverse[0]);
```

**应用场景**:
- 计算力矩控制（Computed Torque Control）
- 生物力学分析
- 状态估计与系统辨识

---

### 11.2 前向动力学 (Forward Dynamics)

**查询语句**: "forward dynamics mj_fwdPosition mj_fwdVelocity acceleration computation"

**关键发现**:
前向动力学计算分为三个阶段：

```
dx/dt = f(t, x, u)

其中:
- t: 时间
- x: 状态向量 (qpos, qvel)
- u: 控制向量
- f: 决定状态导数的函数
```

**计算流程** (来自 `mj_forwardSkip`):

```c
// 阶段1: 位置相关计算
if (skipstage < mjSTAGE_POS) {
    mj_fwdPosition(m, d);      // 位置相关
    mj_sensorPos(m, d);        // 传感器
    mj_energyPos(m, d);        // 能量
}

// 阶段2: 速度相关计算  
if (skipstage < mjSTAGE_VEL) {
    mj_fwdVelocity(m, d);      // 速度相关
    mj_sensorVel(m, d);
    mj_energyVel(m, d);
}

// 阶段3: 加速度相关计算
mj_fwdActuation(m, d);        // 执行器力
mj_fwdAcceleration(m, d);      // 加速度（不含约束）
mj_fwdConstraint(m, d);        // 约束力 → 最终 qacc
```

**输出**: `mjData.qacc` (关节加速度)

---

### 11.3 积分器调度

**查询语句**: "mj_step simulation step function integrator selection"

**关键发现**:
`mj_step` 函数的积分器选择逻辑:

```c
void mj_step(const mjModel* m, mjData* d) {
    // 1. 状态检查
    mj_checkPos(m, d);
    mj_checkVel(m, d);
    
    // 2. 前向动力学
    mj_forward(m, d);
    mj_checkAcc(m, d);
    
    // 3. 选择积分器
    switch (m->opt.integrator) {
        case mjINT_EULER:
            mj_Euler(m, d);
            break;
        case mjINT_RK4:
            mj_RungeKutta(m, d, 4);
            break;
        case mjINT_IMPLICIT:
        case mjINT_IMPLICITFAST:
            mj_implicit(m, d);
            break;
    }
}
```

---

## 12. 推荐查询主题

| 主题 | 查询语句 | 预期结果 |
|------|---------|---------|
| 碰撞检测 | "collision detection contact constraint" | 接触约束模型 |
| 关节动力学 | "joint actuator motor torque" | 执行器建模 |
| 传感器 | "sensor force torque position" | 传感器计算 |
| 能量计算 | "energy kinetic potential computation" | 能量监控 |
| 稀疏矩阵 | "sparse matrix Jacobian computation" | 稀疏 Jacobian |

---

*本文档用于指导如何使用 Context7 MCP 服务获取 MuJoCo 相关参考*

