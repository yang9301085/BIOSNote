# OpenClaw_飞书机器人开发

**来源**: 笔录 2026-03-18/2026-03-20《飞书接入 / 三省六部》

#OpenClaw #飞书 #Feishu #Bot #API

---

# 飞书机器人开发与接入

## 核心知识点

### 飞书自建应用接入流程
1. 登录飞书开放平台 → 创建企业自建应用
2. 拿到 `App ID`（cli_xxx 格式）和 `App Secret`
3. 开启 Bot 能力（应用功能 → 机器人）
4. 配置权限（im:message:send_as_bot、im:message.receive_v1）
5. 配置事件订阅 → 添加 `im.message.receive_v1`

### OpenClaw 飞书插件配置
```bash
openclaw plugins install @openclaw/feishu
```
在 `~/.openclaw/config.yaml` 或环境变量中配置：
```yaml
feishu:
  app_id: cli_xxxxxxxxxxxxx
  app_secret: xxxxxxxxxxxxxxxxxxxx
  bot_name: 你的机器人名
```

### 飞书消息接收链路
```
用户发消息
  → 飞书服务器
  → 事件订阅（im.message.receive_v1）
  → OpenClaw 飞书插件
  → Gateway /chat/completions
  → OpenClaw 主 Agent
  → 回复 → 飞书 Bot 发送
```

### OpenClaw 三省六部 Skill
- 自定义多 Agent 协作框架
- 把任务分成多条 Lane 并行跑：
  - Lane 1（baseline）：梳理项目结构基线
  - Lane 2（differential）：对比正常/异常找差异
  - Lane 3（hypothesis）：验证故障假设
- 适用场景：复杂 BIOS 调试、多模块协同分析

### 常见接入问题
- 收不到消息 → 检查事件订阅是否开启
- Bot 不回复 → 检查 App Secret / Token
- 权限不足 → 确认已添加 `im:message` 相关权限

