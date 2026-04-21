# URDF/MJCF 解析器设计文档

## 1. 概述

本文档描述 attic-physics 中机器人描述文件 (URDF/MJCF) 解析器的设计和实现方案。

---

## 2. 文件格式对比

### 2.1 URDF (Unified Robot Description Format)

- **用途**: ROS 标准机器人描述格式
- **结构**: 扁平化 (link + joint 分离)
- **特点**: 
  - 连杆和关节分别定义
  - 通过 parent/child 引用建立关系
  - 支持 XACRO 宏扩展

```xml
<robot name="my_robot">
    <link name="base"/>
    <link name="link1"/>
    <joint name="joint1" type="revolute">
        <parent link="base"/>
        <child link="link1"/>
        <origin xyz="0 0.1 0"/>
        <axis xyz="0 0 1"/>
    </joint>
</robot>
```

### 2.2 MJCF (MuJoCo XML)

- **用途**: MuJoCo 物理引擎专用
- **结构**: 层次化 (body 嵌套)
- **特点**:
  - body 内嵌 joint 定义
  - 强大的默认继承机制
  - 更简洁的建模

```xml
<mujoco>
    <worldbody>
        <body name="base">
            <joint type="free"/>
            <geom type="box"/>
            <body name="link1" pos="0 0.1 0">
                <joint type="hinge"/>
                <geom type="capsule"/>
            </body>
        </body>
    </worldbody>
</mujoco>
```

---

## 3. 解析流程设计

```
┌─────────────────────────────────────────────────────────────┐
│                     输入: XML 文件/字符串                     │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                    1. 格式检测 (detect)                       │
│                  is_urdf() / is_mjcf()                       │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                    2. XML 解析 (parse)                        │
│                  tinyxml2 解析为 DOM 树                       │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│               3. 数据提取 (extract)                           │
│         遍历 DOM 提取 link/joint/body 信息                   │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│              4. 坐标系变换 (transform)                        │
│         rpy → rotation matrix, xyz → position               │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│               5. 连杆树构建 (build)                           │
│         建立 parent-child 关系, 计算 DOF                      │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                  输出: RobotModel                            │
└─────────────────────────────────────────────────────────────┘
```

---

## 4. 数据结构设计

### 4.1 解析过程中的临时结构

```cpp
// lib/physics/parser/parse_data.h

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace attic {
namespace physics {

// 临时: 连杆原始数据
struct RawLink {
    std::string name;
    std::string mesh_file;      // 视觉网格文件
    std::string collision_file; // 碰撞网格文件
    
    // 惯性数据
    float mass = 1.0f;
    Vec3 com = Vec3(0, 0, 0);    // 质心位置
    Mat3 inertia = Mat3::identity(); // 惯性张量
    
    // 几何数据 (用于碰撞检测)
    enum class ShapeType { Box, Sphere, Capsule, Cylinder, Mesh };
    ShapeType shape_type = ShapeType::Box;
    Vec3 shape_size = Vec3(0.1f, 0.1f, 0.1f);
};

// 临时: 关节原始数据
struct RawJoint {
    std::string name;
    std::string parent_link;
    std::string child_link;
    
    enum class Type { Fixed, Revolute, Prismatic, Continuous, Ball, Free };
    Type type = Type::Fixed;
    
    Vec3 origin_xyz = Vec3(0, 0, 0);     // 相对位置
    Vec3 origin_rpy = Vec3(0, 0, 0);     // 相对旋转 (roll, pitch, yaw)
    Vec3 axis = Vec3(0, 0, 1);          // 关节轴
    
    // 动力学参数
    float damping = 0.0f;
    float friction = 0.0f;
    float spring_stiffness = 0.0f;
    
    // 限位
    float lower_limit = -1e9f;
    float upper_limit = 1e9f;
    float effort_limit = 1e9f;
    float velocity_limit = 1e9f;
};

// MJCF 专用: body 树节点
struct RawBody {
    std::string name;
    std::vector<RawBody> children;
    
    // body 内嵌的 joint
    RawJoint joint;
    
    // body 内嵌的惯性
    float mass = 1.0f;
    Vec3 pos = Vec3(0, 0, 0);           // 惯性位置
    Vec3 diaginertia = Vec3(1, 1, 1);   // 对角惯性
    
    // 几何
    // ...
};

} // namespace physics
} // namespace attic
```

### 4.2 RobotModel 结构

```cpp
// include/attic/physics/robot/robot_model.h

#pragma once

#include <vector>
#include <string>
#include <memory>

#include "attic/core/math/vec3.h"
#include "attic/core/math/mat3.h"
#include "attic/core/math/quat.h"

namespace attic {
namespace physics {

enum class JointType {
    Fixed,      // 固定关节 (0 DOF)
    Revolute,   // 旋转关节 (1 DOF)
    Prismatic,  // 移动关节 (1 DOF)
    Ball,       // 球关节 (3 DOF) - 用四元数表示
    Free        // 自由关节 (6 DOF) - 位置 + 四元数
};

enum class BodyType {
    Static,     // 静止物体
    Dynamic     // 动态物体
};

// 单个连杆
struct Link {
    std::string name;
    
    int parent_index = -1;        // 父连杆索引 (-1 表示根)
    std::vector<int> child_indices; // 子连杆索引
    
    // 惯性 (在连杆坐标系下)
    float mass = 1.0f;
    Mat3 inertia = Mat3::identity();  // 3x3 惯性张量
    Vec3 com = Vec3(0, 0, 0);         // 质心相对位置
    
    // 关节 (连接本连杆与父连杆)
    JointType joint_type = JointType::Fixed;
    Vec3 joint_axis = Vec3(0, 0, 1);  // 旋转/移动轴
    Vec3 joint_origin_pos = Vec3(0, 0, 0);  // 关节相对父连杆的位置
    Quat joint_origin_rot = Quat::identity(); // 关节相对父连杆的旋转
    
    // 动力学参数
    float damping = 0.0f;
    float friction = 0.0f;
    
    // 限位
    float lower_limit = -1e9f;
    float upper_limit = 1e9f;
};

// 机器人模型
class RobotModel {
public:
    std::string name;
    std::vector<Link> links;
    
    // 计算得到的 DOF 信息
    int dof = 0;                    // 自由度数量
    std::vector<int> q_index;       // 每个关节对应的 q 索引
    
    // 根连杆索引
    int root_index = 0;
    
    // 辅助函数
    int find_link_by_name(const std::string& name) const;
    int get_parent(int link_index) const;
    std::vector<int> get_children(int link_index) const;
    
private:
    std::unordered_map<std::string, int> name_to_index_;
};

} // namespace physics
} // namespace attic
```

---

## 5. 实现方案

### 5.1 目录结构

```
include/attic/physics/
├── robot/
│   ├── robot_model.h       # 机器人模型定义
│   └── robot_model.cpp
├── parser/
│   ├── robot_parser.h      # 解析器接口
│   └── robot_parser.cpp    # 实现
└── integrators/
    └── ...

lib/physics/
├── robot/
│   └── robot_model.cpp
├── parser/
│   ├── robot_parser.cpp    # 解析器实现
│   ├── urdf_parser.cpp    # URDF 专用解析
│   └── mjcf_parser.cpp    # MJCF 专用解析
└── ...

third_party/
└── tinyxml2/              # XML 解析库 (header-only)
```

### 5.2 类设计

```cpp
// include/attic/physics/parser/robot_parser.h

#pragma once

#include <string>
#include <memory>
#include "attic/physics/robot/robot_model.h"

namespace attic {
namespace physics {

// 解析结果
struct ParseResult {
    std::shared_ptr<RobotModel> model;
    std::string error;
    bool success = false;
    
    explicit operator bool() const { return success; }
};

// 解析器类
class RobotParser {
public:
    // 从文件加载
    static ParseResult load_file(const std::string& path);
    
    // 从字符串加载
    static ParseResult parse_string(const std::string& xml);
    
    // 自动检测格式
    static ParseResult parse(const std::string& input);
    
    // URDF 专用
    static ParseResult parse_urdf(const std::string& xml);
    
    // MJCF 专用
    static ParseResult parse_mjcf(const std::string& xml);
    
private:
    // 格式检测
    static bool is_urdf(const std::string& xml);
    static bool is_mjcf(const std::string& xml);
    
    // 工具函数
    static Vec3 parse_vec3(const std::string& str);
    static Mat3 parse_rotation(const std::string& rpy_str);
    static JointType parse_joint_type(const std::string& type_str);
    static float parse_float(const std::string& str, float default_val = 0.0f);
};

} // namespace physics
} // namespace attic
```

---

## 6. 实现步骤

### Phase 1: 基础结构 (1-2 天)

1. 创建目录结构
2. 实现 `RobotModel` 数据结构
3. 集成 tinyxml2

### Phase 2: URDF 解析器 (2-3 天)

1. 实现 XML 解析骨架
2. 解析 `<link>`: 质量、惯性、几何
3. 解析 `<joint>`: 类型、轴、限位
4. 构建连杆树
5. 坐标系变换

### Phase 3: MJCF 解析器 (2-3 天)

1. 解析 `<body>` 树结构
2. 处理 body 内嵌的 joint/inertia
3. 处理 `<default>` 继承机制
4. 转换为 RobotModel

### Phase 4: 测试与验证 (1-2 天)

1. 加载 MuJoCo 示例模型 (Atlas, Humanoid)
2. 对比 urdf_to_mjcf 转换结果
3. 单元测试

---

## 7. 参考资源

### 7.1 官方文档

| 资源 | 链接 |
|------|------|
| URDF 规范 | http://wiki.ros.org/urdf/XML |
| URDF Model 教程 | http://wiki.ros.org/urdf/Tutorials |
| MJCF 规范 | https://mujoco.readthedocs.io/en/latest/XMLreference.html |
| MuJoCo 模型转换 | https://mujoco.readthedocs.io/en/latest/URDF.html |

### 7.2 开源项目

| 项目 | 说明 |
|------|------|
| [ROS urdf](http://docs.ros.org/api/urdf/html/) | ROS 官方 URDF 解析器 (C++) |
| [urdfdom](https://github.com/ros/urdfdom) | URDF 数据结构实现 |
| [mujoco](https://github.com/google-deepmind/mujoco) | MuJoCo 官方 (含 MJCF 解析) |
| [urdfpy](https://github.com/mmatl/urdfpy) | Python URDF 解析器 |

### 7.3 依赖库

| 库 | 用途 | 大小 |
|---|------|------|
| tinyxml2 | XML 解析 (header-only) | ~2KB |
| pugixml | XML 解析 (header-only) | ~40KB |
| rapidxml | XML 解析 (header-only) | ~2KB |

推荐: **tinyxml2** (ROS 也在用)

---

## 8. 坐标系变换公式

### 8.1 RPY → 旋转矩阵

```cpp
Mat3 rpy_to_rotation(float roll, float pitch, float yaw) {
    float cr = cos(roll), sr = sin(roll);
    float cp = cos(pitch), sp = sin(pitch);
    float cy = cos(yaw), sy = sin(yaw);
    
    // R = R_z(yaw) * R_y(pitch) * R_x(roll)
    return Mat3(
        cy*cp,  cy*sp*sr - sy*cr,  cy*sp*cr + sy*sr,
        sy*cp,  sy*sp*sr + cy*cr,  sy*sp*cr - cy*sr,
        -sp,    cp*sr,             cp*cr
    );
}
```

### 8.2 字符串解析

```cpp
// "1.0 2.0 3.0" → Vec3(1, 2, 3)
Vec3 parse_vec3(const std::string& str) {
    std::istringstream iss(str);
    Vec3 v;
    iss >> v.x >> v.y >> v.z;
    return v;
}
```

---

## 9. 测试用例

### 9.1 简单 URDF

```xml
<robot name="test_robot">
    <link name="base">
        <inertial>
            <mass value="1.0"/>
            <inertia ixx="0.001" ixy="0" ixz="0" iyy="0.001" iyz="0" izz="0.001"/>
        </inertial>
    </link>
    <link name="link1">
        <inertial>
            <mass value="0.5"/>
            <inertia ixx="0.0001" ixy="0" ixz="0" iyy="0.0001" iyz="0" izz="0.0001"/>
        </inertial>
    </link>
    <joint name="joint1" type="revolute">
        <parent link="base"/>
        <child link="link1"/>
        <origin xyz="0 0.1 0"/>
        <axis xyz="0 0 1"/>
        <limit lower="-3.14" upper="3.14"/>
    </joint>
</robot>
```

预期结果:
- 2 个连杆
- 1 个旋转关节 (1 DOF)
- 总 DOF = 1

### 9.2 加载 MuJoCo 模型

```cpp
// 加载 MuJoCo 示例模型
auto result = RobotParser::load_file("model/humanoid.xml");
// 或
auto result = RobotParser::load_file("model/ur5e.urdf");
```

---

## 10. 后续扩展

### 10.1 支持的功能

| 功能 | 优先级 | 说明 |
|------|--------|------|
| 基本 URDF 解析 | P0 | 连杆 + 关节 |
| MJCF 解析 | P0 | body 树结构 |
| XACRO 支持 | P1 | 宏展开 (可调用 external xacro) |
| 碰撞几何 | P1 | 解析 `<collision>` |
| 视觉几何 | P2 | 解析 `<visual>`, 加载 mesh |
| 传感器 | P2 | IMU, 力传感器 |
| 执行器 | P2 | motor, actuator |

### 10.2 与物理引擎集成

```cpp
// 解析后直接创建物理世界
ParseResult result = RobotParser::load_file("ur5e.urdf");
if (result) {
    World world;
    for (const auto& link : result.model->links) {
        Body* body = world.create_body();
        body->set_mass(link.mass);
        body->set_inertia(link.inertia);
        // ...
    }
}
```

---

## 11. 总结

| 项目 | 方案 |
|------|------|
| 存放位置 | `lib/physics/parser/` + `include/attic/physics/parser/` |
| XML 库 | tinyxml2 (header-only) |
| 解析顺序 | 格式检测 → XML 解析 → 数据提取 → 坐标系变换 → 构建模型 |
| 验证方法 | 对比 MuJoCo urdf_to_mjcf 输出 |

解析器是机器人仿真的第一步，实现后即可加载各种机器人模型进行动力学仿真。
