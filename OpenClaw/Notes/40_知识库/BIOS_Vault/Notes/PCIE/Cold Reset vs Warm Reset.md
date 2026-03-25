# Cold Reset vs Warm Reset (Intel PCH CF9 寄存器)

## 核心区别

| 维度 | Warm Reset | Cold Reset |
|---|---|---|
| CF9 序列 | `0x00 -> 0x06` | `0x02 -> 0x0E` |
| 起始状态 | Soft Start State | Hard Start State |
| 复位类型 | Hard Reset | Full Reset (含 power cycle) |
| 设备状态 | 保留更多前提状态 | 更彻底清状态 |

---

## CF9 寄存器值定义

| 名称 | 值 | 作用 | 立即复位? |
|---|---:|---|---|
| `SOFTSTARTSTATE` | `0x00` | 选择软启动状态 | 否 |
| `HARDSTARTSTATE` | `0x02` | 选择硬启动状态 | 否 |
| `HARDRESET` | `0x06` | 触发硬复位 | 是 |
| `FULLRESET` | `0x0E` | 触发全复位 (含 power cycle) | 是 |

**代码位置**：
- `Intel/ClientOneSiliconPkg/Pch/Include/Register/PchRegsLpc.h:276-284`

---

## 实际路径 (典型 AMI 实现)

### Warm Reset (`EfiResetWarm`)

```
SOFTSTARTSTATE (0x00) -> HARDRESET (0x06)
```

- 代码位置: `AmiSbMiscLib.c:227`
- 先设 `SOFTSTARTSTATE`，再发 `HARDRESET`

### Cold Reset (`EfiResetCold`)

```
HARDSTARTSTATE (0x02) -> FULLRESET (0x0E)
```

- 代码位置: `AmiSbMiscLib.c:234`
- 因为 `COLD_RESET_WITH_POWER_CYCLE=1` (见 `Sb.sdl:3441`)
- 先设 `HARDSTARTSTATE`，再发 `FULLRESET`

---

## 常见误区

### 误区 1：`SOFTSTARTSTATE` = `SOFTRESET`

**错误**。`SOFTSTARTSTATE (0x00)` 只是选择起始状态，不会触发复位。

平台的 warm reset 实际是 `0x00 -> 0x06`，不是直接写 `0x04`。

### 误区 2：Cold Reset = G3 掉电上电

**不完全对**。

- Cold reset 更接近"从更干净的初始状态重来"
- 但不等于严格意义上的 G3 掉电上电
- 某些 always-on 域、resume well 是否真的掉下去，要看板级电源和 reset wiring

---

## 对 NVMe/PCIe 设备的影响

### Cold Reset 后

- M.2/PCIe 更接近"彻底冷起"状态
- 设备重新上电
- PERST#/REFCLK/CLKREQ# 重新走时序
- Root Port 重新训练链路
- **如果任何一步不稳，设备可能枚举失败**

### Warm Reset 后

- 平台/设备保留更多"非彻底冷起"状态
- 某些电源域/时钟/训练前提没有被完全打掉
- **设备更容易在早期枚举中被看见**

---

## 调试建议

如果遇到 "Cold reset 丢设备，Warm reset 正常" 的问题：

1. 检查设备上电/复位时序在 cold reset 条件下是否稳定
2. 用示波器测量 PERST#、REFCLK、CLKREQ# 信号
3. 检查 Root Port LTSSM 状态
4. 考虑在 PEI/DXE 阶段增加 PCIe 设备初始化的延迟

---

## 相关链接

- [[M.2 PCIe 设备存在检测机制]]
- [[首启自动 Reset 机制 (BDS Memory Type Information)]]

---

*来源: 2026-03-17 NVMe 首启调试会话*
