# OpenClaw Gateway SIGTERM 排查

**日期**：2026-03-26
**状态**：进行中（未解决）

---

## 现象

- `openclaw gateway restart` 后进程立即被 SIGTERM 终止
- 手动 `openclaw gateway start` 同样快速终止
- `systemctl --user status openclaw` 显示状态表面正常
- 进程存活时间极短，反复尝试仍无法稳定运行

---

## 已排查

| 检查项 | 结果 |
|---|---|
| `openclaw gateway status` | 显示 down |
| `openclaw gateway start` | 进程启动后快速终止 |
| `systemctl --user status openclaw` | 表面正常，进程存活时间短 |
| journalctl --user -u openclaw | （未执行） |

---

## 待查步骤

1. `journalctl --user -u openclaw -n 50` — 查看 systemd 真实日志
2. 检查是否有外部进程主动发送 SIGTERM（watchdog、其他服务）
3. 检查 WSL2 环境下 systemd 进程管理兼容性
4. 检查 openclaw 进程 PID 文件是否异常

---

## 相关文件

- [[OpenClaw_多Agent协作模式]]
- [[OpenClaw_Skill开发与安装]]

---

**来源**：2026-03-26 Heartbeat 排查会话
