# OpenCode 智能体配置

本项目配置了三个plan/subagent模式的智能体：Sherlock（福尔摩斯）、Sisyphus（西西弗斯）和Code Reviewer（代码审查员）。

## 智能体列表

### 🔍 Sherlock (福尔摩斯)
**文件**: `.opencode/agents/sherlock.md`
**模式**: `plan` (主智能体)

**角色**: C++代码侦探
- 像福尔摩斯一样敏锐地分析代码问题
- 教授现代C++17/20/23最佳实践
- 发现内存泄漏、UB、性能瓶颈

**专长**:
- 代码审查与Bug发现
- 内存安全与RAII
- 模板元编程
- 并发编程

**工具权限**:
- ✅ Context7文档查询 (C++标准库)
- ❌ 不编辑文件
- ❌ 不运行命令

---

### ⛰️ Sisyphus (西西弗斯)
**文件**: `.opencode/agents/sisyphus.md`
**模式**: `plan` (主智能体)

**角色**: LLVM/MLIR编译器工程师
- 像西西弗斯一样不断追求编译器优化的极致
- 设计编译器架构和Dialect
- 生成和优化LLVM IR/MLIR

**专长**:
- LLVM IR设计与Pass开发
- MLIR Dialect创建
- JIT编译器实现
- 代码生成优化

**工具权限**:
- ✅ Context7文档查询 (LLVM/MLIR)
- ❌ 不编辑文件
- ❌ 不运行命令

---

### 🔎 Code Reviewer (代码审查员)
**文件**: `.opencode/agents/code-reviewer.md`
**模式**: `subagent` (子智能体)

**角色**: 代码质量专家
- 像资深工程师审查PR一样严格
- 全面检查代码质量、安全性和最佳实践
- 提供建设性的改进建议

**专长**:
- 内存安全与资源管理
- 未定义行为检测
- 线程安全分析
- 性能优化建议
- 代码风格和可维护性

**工具权限**:
- ✅ Context7文档查询
- ❌ 不编辑文件
- ❌ 不运行命令

**使用方式**: 通常由其他智能体调用
```
@sherlock 完成这段代码后，请@code-reviewer审查一下。
```

## 使用方法

### 1. 配置opencode.json

在项目根目录创建或编辑 `opencode.json`:

```json
{
  "$schema": "https://opencode.ai/config.json",
  "agent": {
    "sherlock": {
      "description": "C++代码侦探",
      "mode": "plan",
      "prompt": "{file:.opencode/agents/sherlock.md}",
      "model": "anthropic/claude-sonnet-4-20250514",
      "temperature": 0.2,
      "tools": {
        "write": false,
        "edit": false,
        "bash": false,
        "context7_query-docs": true,
        "context7_resolve-library-id": true
      }
    },
    "sisyphus": {
      "description": "LLVM/MLIR编译器工程师",
      "mode": "plan",
      "prompt": "{file:.opencode/agents/sisyphus.md}",
      "model": "anthropic/claude-sonnet-4-20250514",
      "temperature": 0.2,
      "tools": {
        "write": false,
        "edit": false,
        "bash": false,
        "context7_query-docs": true,
        "context7_resolve-library-id": true
      }
    },
    "code-reviewer": {
      "description": "代码审查专家",
      "mode": "subagent",
      "prompt": "{file:.opencode/agents/code-reviewer.md}",
      "model": "anthropic/claude-sonnet-4-20250514",
      "temperature": 0.1,
      "tools": {
        "write": false,
        "edit": false,
        "bash": false,
        "context7_query-docs": true
      }
    }
  }
}
```

### 2. 激活智能体

在OpenCode中，按 `Tab` 键切换智能体，或使用命令：

```
/agent sherlock
/agent sisyphus
```

### 3. 使用示例

**与Sherlock对话**:
```
@sherlock 请帮我分析这段代码的内存安全性

@sherlock 解释std::move和完美转发
```

**与Sisyphus对话**:
```
@sisyphus 如何创建一个MLIR Dialect？

@sisyphus 设计一个JIT编译器架构
```

**调用Code Reviewer审查代码**:
```
@code-reviewer 请审查这段刚体动力学代码
[代码片段]

@sherlock 实现这个功能后，请@code-reviewer审查代码质量。
```

## 关键特性

### Plan/Subagent模式 (只读)
Sherlock和Sisyphus是`plan`模式（主智能体），Code Reviewer是`subagent`模式（子智能体）：
- ✅ 提供详细的代码审查报告
- ✅ 生成完整的代码示例
- ✅ 解释概念和原理
- ✅ 查询Context7官方文档
- ❌ 不直接编辑用户文件
- ❌ 不运行shell命令

### Context7集成

**Sherlock可查询**:
- `/cplusplus/standard` - C++标准库
- `/boost/boost` - Boost库
- `/google/googletest` - Google Test

**Sisyphus可查询**:
- `/llvm/llvm` - LLVM核心
- `/llvm/mlir` - MLIR框架
- `/llvm/clang` - Clang前端

**Code Reviewer可查询**:
- `/cplusplus/standard` - C++标准库
- `/llvm/llvm` - LLVM核心
- `/google/styleguide` - Google代码规范

## 注意事项

1. **重启生效**: 修改`opencode.json`后需完全退出OpenCode再重启
2. **温度设置**: 两个智能体都使用`temperature: 0.2`，保证回答的确定性
3. **模型选择**: 默认使用`claude-sonnet-4-20250514`，平衡性能和成本
4. **安全限制**: plan模式确保不会意外修改代码，所有建议都以文本形式提供

## 配置文件说明

智能体配置文件使用Markdown格式，包含：
- **YAML Frontmatter**: 元数据配置（mode, model, tools等）
- **Markdown内容**: 系统提示词和详细说明

这种格式比纯JSON更灵活，可以包含更详细的角色设定和示例。

## 扩展

要创建新的智能体，复制模板并修改：

```bash
cp .opencode/agents/sherlock.md .opencode/agents/my-agent.md
```

然后编辑`my-agent.md`中的内容，并在`opencode.json`中添加配置。
