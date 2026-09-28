# OpenClaw_飞书接入配置

**来源**: 笔录 2026-03-18《飞书接入》

#OpenClaw #飞书 #Feishu #Bot

---

# 飞书接入配置步骤

## 核心知识点

### 准备清单
1. OpenClaw 已安装并可正常运行 `gateway`
2. 飞书开放平台账号（有"创建/发布企业自建应用"权限）
3. 飞书 Bot 应用（App ID + App Secret）

### 配置步骤
```bash
# 1. 安装飞书插件
openclaw plugins install @openclaw/feishu

# 2. 在飞书开放平台创建自建应用
#    登录 https://open.feishu.cn/
#    创建应用 → 拿到 App ID (cli_xxx) 和 App Secret

# 3. 开启 Bot 能力
#    应用功能 → 机器人 → 开启

# 4. 配置权限
#    im:message:receive_v1（接收消息）
#    im:message:send_as_bot（发送消息）

# 5. 订阅消息事件
#    事件订阅 → im.message.receive_v1
```

### OpenClaw 侧配置
在 `~/.openclaw/config.yaml` 或环境变量中配置飞书插件参数。

