# OpenClaw_模型配置与Token管理

**来源**: 笔录 2026-03-13/2026-03-19/2026-03-20《OpenClaw 模型配置 / Token》

#OpenClaw #模型 #Token #配置 #API

---

# OpenClaw 模型配置与 Token 管理

## 核心知识点

### 模型配置（openclaw.json）
```json
{
  "apiProvider": "minimax",
  "apiModel": "MiniMax-M2.7",
  "apiKey": "your-key-here",
  "agents": {
    "defaults": {
      "workspace": "/root/.openclaw/workspace",
      "model": "MiniMax-M2.7"
    }
  }
}
```

### Token 优先级
Token 读取顺序（后面覆盖前面）：
1. 环境变量（`MINIMAX_API_KEY` / `XFYUN_CODING_API_KEY` 等）
2. `openclaw.json` 中的 `apiKey` 字段
3. 命令行 `--api-key` 参数

### 常见 Token 问题

**问题：401 令牌过期或验证不正确**
1. 浏览器缓存了旧的 Gateway Token → 清除 localStorage，重新获取
2. 运行的是另一个 OpenClaw 实例 → 检查进程对应的配置文件
3. systemd 服务中缺少环境变量 → 检查 service 文件的 `Environment=`

**WSL systemd 中的环境变量问题**
```bash
# ~/.bashrc 中设置（只在交互 shell 生效）
export MINIMAX_API_KEY=xxx

# systemd service 中需要显式声明
[Service]
Environment=MINIMAX_API_KEY=xxx
```

### 多模型配置
```json
{
  "apiProvider": "minimax",
  "apiModel": "MiniMax-M2.7",
  "fallbackModel": "MiniMax-M2.5"
}
```

### Codex 模型
Codex 通过 `@openai/codex` 包安装，集成在 OpenClaw subagent 中：
- 路径：`/root/.openclaw/lib/node_modules/@openai/codex`
- 通过 OpenClaw subagent 调用
- 支持多会话并发（`subagents.maxConcurrent`）

