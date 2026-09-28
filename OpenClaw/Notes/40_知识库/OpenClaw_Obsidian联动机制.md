# OpenClaw_Obsidian联动机制

**来源**: 笔录 2026-03-13《OpenClaw Obsidian 同步》

#OpenClaw #Obsidian #笔记 #插件

---

# OpenClaw 与 Obsidian 联动机制

## 核心知识点

### 三类联动动作

**1. 聊天联动**
- Obsidian 侧边栏 → OpenClaw 插件 → 消息发到 `http://127.0.0.1:18789/v1/chat/completions`
- 勾选 "Include current note" 时，当前进度笔记内容一起发送

**2. 文件动作联动**
当 OpenClaw 回复含动作块时，插件执行：
- `createFile` - 新建文件
- `updateFile` - 更新文件
- `appendToFile` - 追加内容

**3. 记忆与知识库**
- OpenClaw 的 `memory_search` 扫描 `obsidian-notes/` 目录
- 每次对话可自动写入会话笔记到 Obsidian
- 笔录路径：`obsidian-notes/5_笔录/`

### 插件配置文件
- `data.json` → API 地址、对话历史
- `main.js` → HTTP 请求构造逻辑

### 常见问题
- "同步不过来" → 检查 WSL 网络到 Windows localhost 的可达性
- "连不上" → 检查 token 是否过期

