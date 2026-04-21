# 数学库文档 (Attic Math Library)

## 概述

Attic Physics 的数学库提供了物理仿真所需的基础数学类型和运算，采用**头文件内联实现**以获得最佳性能，同时支持GPU编译。

## 核心组件

### 1. Vec3 (三维向量)

**文件**: `include/attic/math/vec3.h`

**用途**: 位置、速度、力、方向等所有三维量的表示

**核心功能**:
```cpp
// 基本运算
Vec3 a(1, 2, 3), b(4, 5, 6);
Vec3 c = a + b;           // 加法
Vec3 d = a - b;           // 减法
Vec3 e = a * 2.0f;        // 标量乘法
Vec3 f = a / 2.0f;        // 标量除法

// 向量运算
float len = a.length();              // 长度
dot(a, b);                           // 点积
cross(a, b);                         // 叉积 (右手定则)
a.normalized();                      // 归一化
a.normalize();                       // 原地归一化

// 实用函数
a.project(b);                        // 投影到b上
a.reflect(normal);                   // 反射
lerp(a, b, 0.5f);                    // 线性插值
distance(a, b);                      // 两点距离

// 工厂函数
Vec3::zero();                        // (0,0,0)
Vec3::one();                         // (1,1,1)
Vec3::unit_x();                      // (1,0,0)
Vec3::unit_y();                      // (0,1,0)
Vec3::unit_z();                      // (0,0,1)
```

**物理意义**:
- **位置**: 世界坐标系中的位置向量
- **速度**: 位移对时间的导数
- **力**: 牛顿第二定律 F = ma
- **角速度**: 旋转轴×角速度大小

---

### 2. Mat3 (3×3矩阵)

**文件**: `include/attic/math/mat3.h`

**用途**: 旋转矩阵、惯性张量、坐标变换

**核心功能**:
```cpp
// 构造
Mat3 m;                              // 单位矩阵
Mat3 m(1,0,0, 0,1,0, 0,0,1);        // 按行填充
Mat3::diagonal(1, 2, 3);             // 对角矩阵
Mat3::zero();                        // 零矩阵

// 运算
Mat3 c = a * b;                      // 矩阵乘法
Vec3 v = m * Vec3(1,0,0);            // 矩阵×向量
Mat3 t = m.transpose();              // 转置
float det = m.determinant();         // 行列式
Mat3 inv = m.inverse();              // 逆矩阵

// 特殊矩阵
Mat3::outer_product(a, b);           // 外积 (a ⊗ b)
Mat3::skew_symmetric(v);             // 反对称矩阵
```

**物理意义**:
- **旋转矩阵**: 刚体的方向表示，正交矩阵
- **惯性张量**: 描述质量分布，对角化后为转动惯量
- **反对称矩阵**: 角速度ω的叉积矩阵 [ω]×v = ω×v

**数学公式**:
- 反对称矩阵: `skew_symmetric(v) = [[0,-z,y], [z,0,-x], [-y,x,0]]`
- 外积: `outer_product(a,b) = a * bᵀ`

---

### 3. Quat (四元数)

**文件**: `include/attic/math/quat.h`

**用途**: 旋转的紧凑表示，避免万向锁，适合插值

**核心功能**:
```cpp
// 构造
Quat q;                              // 单位四元数 (0,0,0,1)
Quat q(axis, angle);                 // 从轴角构造 (axis必须归一化)
Quat::from_euler(yaw, pitch, roll);  // 从欧拉角构造
Quat::from_mat3(m);                  // 从旋转矩阵构造

// 运算
Quat c = a * b;                      // 四元数乘法 (组合旋转)
Vec3 v_rot = q.rotate(v);            // 旋转向量
Quat inv = q.inverse();              // 逆四元数
Quat conj = q.conjugate();           // 共轭 (对单位四元数=逆)

// 转换
Mat3 m = q.to_mat3();                // 转旋转矩阵
float angle; Vec3 axis;
q.to_angle_axis(angle, axis);        // 提取轴角

// 插值
Quat result = a.slerp(b, 0.5f);      // 球面插值 (平滑旋转)
```

**四元数表示**:
- `q = [x, y, z, w]` 其中 `(x,y,z)` 是虚部，`w` 是实部
- 单位四元数满足: `x² + y² + z² + w² = 1`
- 旋转公式: `v' = q * [0,v] * q⁻¹`

**物理意义**:
- 刚体旋转的最小表示 (4个数 vs 矩阵9个数)
- 无万向锁问题
- SLERP提供平滑的角插值

---

## 设计特点

### 1. 内联实现 (Header-only style)
所有函数在头文件内联定义，编译器可优化内联展开，消除函数调用开销。

### 2. 命名空间 `atp`
避免命名冲突，清晰标识库来源。

### 3. GPU兼容
- 不使用STL容器
- 避免虚函数
- 纯数据结构+简单运算
- 可被CUDA/OpenCL直接编译

### 4. 易用性
- 运算符重载使代码简洁: `c = a + b` 而非 `vec3_add(a, b)`
- 工厂函数提供常用构造: `Vec3::zero()`, `Quat::identity()`
- 静态成员和友元函数保持一致性

---

## 使用示例

```cpp
#include "attic/math/vec3.h"
#include "attic/math/mat3.h"
#include "attic/math/quat.h"

using namespace atp;

// 物理仿真中的典型用法
void simulate_rigid_body() {
    // 刚体属性
    Vec3 position(0, 5, 0);           // 位置
    Vec3 velocity(1, 0, 0);            // 速度
    Quat orientation = Quat::identity(); // 方向
    Vec3 angular_velocity(0, 1, 0);    // 角速度
    
    // 旋转矩阵
    Mat3 rotation = orientation.to_mat3();
    
    // 力的计算
    Vec3 force(10, 0, 0);
    Vec3 acceleration = force / mass;
    velocity += acceleration * dt;
    position += velocity * dt;
    
    // 旋转更新 (四元数导数)
    Quat omega_quat(angular_velocity.x, angular_velocity.y, 
                    angular_velocity.z, 0);
    Quat q_dot = omega_quat * orientation * 0.5f;
    orientation = (orientation + q_dot * dt).normalized();
}
```

---

## 文件结构

```
include/attic/math/
├── vec3.h       # 三维向量
├── mat3.h       # 3x3矩阵
└── quat.h       # 四元数

src/math/
├── vec3.cpp     # (可选)非内联函数
├── mat3.cpp     # (可选)非内联函数
└── quat.cpp     # (可选)非内联函数
```

**说明**: cpp文件主要为保留结构，实际所有函数已在头文件内联实现。未来如有复杂算法可移至cpp文件。
