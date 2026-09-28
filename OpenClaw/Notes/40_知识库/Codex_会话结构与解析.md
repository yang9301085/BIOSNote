# Codex_会话结构与解析

**来源**: 笔录 2026-03-13/2026-03-17《Codex 会话记录》

#Codex #会话 #JSONL #Session

---

# Codex 会话结构与 JSONL 解析

## 核心知识点

### 会话文件格式
Codex 对话存储为 JSONL（JSON Lines）格式，每行一条消息：
```json
{"type": "event_msg", "payload": {"type": "user_message", "message": {...}}}
{"type": "event_msg", "payload": {"type": "agent_message", "phase": "final_answer", "message": "..."}}
{"type": "event_msg", "payload": {"type": "tool_use", ...}}
```

### 关键 type 和 phase
| type | payload.type | 含义 |
|---|---|---|
| event_msg | user_message | 用户提问 |
| event_msg | agent_message | AI 回复 |
| event_msg | task_started | 会话启动元信息 |

| phase | 含义 |
|---|---|
| commentary | 思考过程/中间步骤 |
| final_answer | 最终回答 |

### 路径
Windows 本地：`C:\Users\yqr\.codex\sessions\`

### 过滤泛型提问
`# Context from my IDE setup` 是 Codex 初始化时注入的 VSCode 当前文件内容，**不是用户真实提问**，应过滤。

