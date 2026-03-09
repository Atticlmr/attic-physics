# Log 日志系统使用指南

## 1. 概述

attic-physics 提供了一个简单的日志系统，支持以下级别：

| 级别 | 值 | 说明 |
|------|-----|------|
| DEBUG | 0 | 调试信息，详细输出 |
| INFO | 1 | 普通信息，正常运行日志 |
| WARN | 2 | 警告信息 |
| ERROR | 3 | 错误信息 |

默认级别为 **INFO**，只显示 INFO/WARN/ERROR。

---

## 2. 在代码中使用

### 2.1 基本用法

```cpp
#include "attic/core/log/log.h"

int main() {
    // 设置日志级别和输出
    attic::log::set_level(attic::log::INFO);
    attic::log::set_output(stdout);
    
    // 使用日志宏
    ATTIC_INFO("程序启动");
    ATTIC_DEBUG("这是一条调试信息");
    ATTIC_WARN("警告：某个值超过预期");
    ATTIC_ERROR("错误：内存分配失败");
    
    return 0;
}
```

### 2.2 格式化输出

支持类似 `printf` 的格式化：

```cpp
ATTIC_INFO("创建物体: name=%s, mass=%.2f", name.c_str(), mass);
ATTIC_DEBUG("位置: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
```

---

## 3. 命令行参数控制

### 3.1 在 main() 中解析 -v 参数

```cpp
// test/cpp/test_core.cpp
#include <iostream>
#include <cstring>
#include "attic/core/log/log.h"

int main(int argc, char* argv[]) {
    // 解析命令行参数
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            attic::log::set_level(attic::log::DEBUG);
            attic::log::set_output(stdout);
            break;
        }
    }
    
    // 程序代码
    ATTIC_INFO("程序启动");
    ATTIC_DEBUG("调试信息");
    
    return 0;
}
```

### 3.2 运行效果

```bash
# 默认 (无参数) - 只显示 INFO 及以上
$ ./test_core
[INFO] 程序启动

# 带 -v 参数 - 显示所有日志
$ ./test_core -v
[INFO] 程序启动
[DEBUG] [test_core.cpp:15] 调试信息
```

---

## 4. 封装初始化函数 (推荐)

在大型项目中，可以封装一个初始化函数：

```cpp
// lib/core/log.cpp
#include "attic/core/log/log.h"

void init_logging(int argc, char* argv[]) {
    attic::log::Level level = attic::log::INFO;
    FILE* output = stderr;
    
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            level = attic::log::DEBUG;
            output = stdout;
        } else if (strcmp(argv[i], "-q") == 0 || strcmp(argv[i], "--quiet") == 0) {
            level = attic::log::WARN;
        }
    }
    
    attic::log::set_level(level);
    attic::log::set_output(output);
}
```

使用：

```cpp
int main(int argc, char* argv[]) {
    init_logging(argc, argv);
    
    ATTIC_INFO("启动");
    ATTIC_DEBUG("调试");
    
    return 0;
}
```

---

## 5. 快速参考

| 任务 | 代码 |
|------|------|
| 包含头文件 | `#include "attic/core/log/log.h"` |
| 打印信息 | `ATTIC_INFO("message")` |
| 打印调试 | `ATTIC_DEBUG("message")` |
| 打印警告 | `ATTIC_WARN("message")` |
| 打印错误 | `ATTIC_ERROR("message")` |
| 设置级别 | `log::DEBUG)` |
| 设置输出 | `log::set_level(log::set_output(stdout)` |
| 启用 DEBUG | 运行程序时加 `-v` 参数 |

---

## 6. 日志级别说明

| 级别 | 使用场景 |
|------|----------|
| DEBUG | 开发调试信息，如变量值、函数调用、循环进度 |
| INFO | 正常运行信息，如程序启动、加载完成、重要状态变化 |
| WARN | 异常但可继续运行，如参数越界被修正、性能警告 |
| ERROR | 严重错误，如内存分配失败、文件无法打开 |

**建议**：
- DEBUG 只在开发时使用，上线时默认关闭
- INFO 用于记录关键事件
- WARN/ERROR 用于记录问题，必须输出
