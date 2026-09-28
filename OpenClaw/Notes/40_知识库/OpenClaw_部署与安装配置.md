# OpenClaw_部署与安装配置

**来源**: 笔录 2026-03-12/2026-03-13/2026-03-16/2026-03-19/2026-03-20《OpenClaw 部署安装》

#OpenClaw #部署 #安装 #WSL #配置

---

# OpenClaw 部署与安装配置

## 核心知识点

### 安装方式
```bash
# 方式1：npm 全局安装（推荐）
npm install -g openclaw@latest

# 方式2：脚本安装（跨平台）
# 项目根目录有安装脚本，检测 OS 后自动安装
# Windows: PowerShell
# Linux/WSL: Bash
```

### WSL 下的安装要点
1. WSL 中 systemd 兼容性：需要 WSL2 + 开启 systemd
2. 用户级 systemd 服务：`~/.config/systemd/user/openclaw-gateway.service`
3. 模型 API Key 放在环境变量中（建议写进 `~/.bashrc` 或 `systemd Environment=`)
4. 常见根因：Key 只写在交互 shell 的 `~/.bashrc`，systemd 服务启动时读不到

### Token 过期问题
"401 令牌过期"的原因（按概率排序）：
1. **浏览器里存的是旧 token** — 升级/重装/切换 profile 后没更新
2. **实际运行的是另一个 OpenClaw 实例** — 不同工作目录，不同 `openclaw.json`
3. **gateway.auth.token 变了** — OpenClaw 重启后 token 重新生成

解决方法：清除浏览器 localStorage，重新获取 token

### Gateway 配置
`~/.openclaw/` 目录结构：
```
~/.openclaw/
├── openclaw.json       # 主配置（模型、Token、插件）
├── workspace/          # 工作目录
├── logs/              # 日志
└── agents/            # Agent 配置
```

### 更新 OpenClaw
```bash
# npm 安装版更新
npm install -g openclaw@latest

# 检查版本
openclaw --version

# 重启 Gateway
openclaw gateway restart
```

### 常见安装问题
- `git push` 失败 → 检查网络代理 / SSH Key
- `npm install` 失败 → 检查 npm 源 / 网络
- Gateway 启动失败 → 检查端口占用 / 配置文件格式

