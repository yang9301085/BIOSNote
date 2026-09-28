# OpenClaw_多Agent协作模式

**来源**: 笔录 2026-03-13/2026-03-19《OpenClaw Codex 多Agent协作》

#OpenClaw #Codex #Agent #多Agent

---

# OpenClaw 多 Agent 协作模式

## 核心知识点

### OpenClaw Agent 架构
- `main`：主对话 Agent，直接与用户交互
- `subagent`：子 Agent，并行处理子任务
- 通过 `sessions_spawn` 创建子会话
- 子 Agent 结果通过 `sessions_yield` 返回

### 多 Agent 协作典型场景：三省六部
把复杂任务分成多条"线"（Lane）并行跑：
- **Baseline Mapper**：梳理当前代码/项目结构基线
- **Differential Comparator**：对比正常 vs 异常场景，找差异点
- **Hypothesis Verifier**：验证故障假设，给出具体证据

### Token 管理
- `MAXTokens` 控制单次回复上限
- `compact` 操作压缩历史上下文
- 会话上下文接近 90%（约 83k/92k token）时容易出问题

### 记忆机制
- `MEMORY.md`：跨会话长期记忆（主 session 专用）
- `memory/*.md`：每日笔记，按日期归档
- `memory_search`：语义搜索长期记忆
- **注意**：多 Agent 场景下各 Agent 工作区记忆不互通，需显式共享

