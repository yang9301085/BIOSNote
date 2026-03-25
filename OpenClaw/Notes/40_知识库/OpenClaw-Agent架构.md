---
title: OpenClaw-Agent架构
date: 2026-03-20
type: knowledge
status: stable
owner: 启仁
tags:
  - OpenClaw
  - Agent
  - architecture
  - feishu
created: 2026-03-20
updated: 2026-03-20
---

# OpenClaw Agent 架构知识卡片

> [!info] 来源
> 2026-03-20 发现，启仁通过 `ls /root/.openclaw/agents/` 和 `workspace-*/SOUL.md` 交叉验证得出。

---

## Agent 发现情况

| Agent ID | 角色 | 说明 | 来源 |
|----------|------|------|------|
| `main` | 太子（实际） | SOUL.md 被 install.sh 替换成"太子"人格，飞书消息进它 | `install.sh` |
| `taizi` | 太子（Edict 专用） | Edict 项目注册的专属 Agent，理论上应接飞书消息 | Edict 配置 |
| `codex` | 未知 | 不在 Edict 原装 11 个里，来历不明 | — |
| `zhongshu` | 中书省 | 三省略六部系统成员 | 自建 |
| `menxia` | 门下省 | 三省略六部系统成员 | 自建 |
| `shangshu` | 尚书省 | 三省略六部系统成员 | 自建 |

---

## 问题：两个"太子"

飞书 Bot 消息路由到 `main`，但 Edict 流程里期望的是 `taizi`。

**原因**：`install.sh` 没有完全清理旧的 `main` 配置，直接把 `main` 的 SOUL.md 替换成了"太子"人格。

---

## 配置位置

| 路径 | 内容 |
|------|------|
| `/root/.openclaw/agents/` | 各 Agent 的根目录 |
| `*/SOUL.md` | Agent 人格定义 |
| `*/AGENTS.md` | Agent 工作空间说明 |
| `workspace-*/` | Agent 的 workspace |

---

## 三省略六部系统架构

```
皇上(启仁)
    ↓ 飞书消息
太子(分拣) → 旨意路由
    ↓
中书省(起草) → 方案制定
    ↓
门下省(审议) → 「准奏」/「封驳」
    ↓
尚书省(派发) → 分发给六部
    ↓
六部(执行) → 兵部/户部/吏部/礼部/工部/刑部
```

**当前状态**: 2026-03-20 首次完整运转，JJC-20260320-001 周报任务走通全链路。

---

## 关联

- 日记: [[2026-03-20 工作日记]]
