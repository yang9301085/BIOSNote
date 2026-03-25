# Cold Reset vs Warm Reset vs Power Cycle (Intel 平台定义)

> 收录日期: 2026-03-18

---

## 核心定义 (Intel PCH 文档)

- **Warm Reset** = Host Reset **without** Power Cycle
- **Cold Reset** = Host Reset **with** Power Cycle
- **Power Cycle** = 掉电再上电 (reset 的属性/动作，不是 reset 类型名)

---

## 触发方式 (CF9 寄存器)

| Reset 类型 | CF9 值 | Global Reset Bit | 触发方式 |
|---|---:|---|---|
| Warm Reset | `0x06` | 0 | SYS_RESET# + CF9[3]=0, TCO WDT 第二次超时 |
| Cold Reset | `0x0E` | 0 | SYS_RESET# + CF9[3]=1, CSME 发起 |

---

## 硬件特征对比

| 项目 | Warm Reset | Cold Reset |
|---|---|---|
| 是否掉主电 | 否 | 是 |
| `PLTRST#` | 会断言 | 会断言 + 电源域 cycle |
| 状态保留 | 相对更多 | 更少 |
| CSME/strap 影响 | 视策略而定 | 更容易重新采样/重启 |
| BIOS 观感 | 快速重启 | 更像完整重启 |

---

## Reset 握手机制

当 reset 被触发时：

1. PCH 向 CPU 发 warning message
2. CPU 完成 outstanding memory cycles，内存放安全状态
3. CPU 回 acknowledge
4. PCH 收到 ack 后断言 `PLTRST#`
5. 若 4 秒内无 ack → 升级为 Global Reset with power-cycle

---

## BIOS 流程差异

### Warm Reset

- 从 reset vector 重新进来
- SEC → PEI → DXE → BDS 重新跑一遍
- 可能走 fast path (跳过某些 training/memory init)
- 某些 scratch/handoff 信息可能保留

### Cold Reset

- 更接近"新上电启动"
- 走完整初始化路径
- 某些 strap 重新采样
- 某些锁定位、sticky bit 被清空

---

## Reset Cause 识别 (PMC 寄存器)

### `HPR_CAUSE0` - Host Partition Reset Cause

| 字段 | 含义 |
|---|---|
| `HRWPC` | host reset with power cycle |
| `HRWOPC` | host reset without power cycle |
| `MI_HRPC` | CSME initiated host reset with power cycle |
| `MI_HR` | CSME initiated host reset without power cycle |
| `TCO_WDT` | TCO Watchdog timeout |
| `SYSRST_ES` | System reset via eSPI |

### Global Reset Cause

可查看上次 global reset 是 thermal、power button override、eSPI type7/8、host reset promoted to global reset 等。

---

## Global Reset

**Global Reset 比 host reset 更大** = host + CSME partition 一起 reset

- 除了 RTC well 和 suspend well 的部分信息，PCH 功能基本都 reset
- 某些 catastrophic error 会触发 global reset

---

## UEFI `ResetSystem()` 对应关系

| UEFI API | 语义 |
|---|---|
| `EfiResetCold` | system-wide reset, tantamount to system power cycle |
| `EfiResetWarm` | processor 回初始态，pending cycles 不被破坏 |

---

## 代码位置索引

- Reset 实现: `AmiChipsetModulePkg/Library/AmiSbMiscLib/AmiSbMiscLib.c:226-242`
- CF9 定义: `Intel/ClientOneSiliconPkg/Pch/Include/Register/PchRegsLpc.h:276-284`
- Power Cycle 配置: `AmiChipsetModulePkg/Sb.sdl` 里的 `COLD_RESET_WITH_POWER_CYCLE`

---

## 工程落地要点

1. **别把 cold/warm 只理解成 UEFI API 名字** — 在 Intel 平台里是 power cycle 属性的区别
2. **别把 power cycle 当成 reset 的同义词** — 它是 reset 的附加属性/电源动作
3. **平台判断要看 PMC cause 寄存器，不要靠猜** — PEI 早期读 `HPR_CAUSE0`
4. **boot path 都是重新进 SEC/PEI/DXE/BDS** — 区别在 reset cause、power domain 是否 cycle、哪些状态被保留

---

## 相关问题追踪

- **PBX6000 NVMe 首启问题**: Cold reset 后找不到 NVMe，warm reset 后可以找到
  - 根因方向: cold reset / S5 开机的电源域/时序差异
  - 详见: `memory/2026-03-18.md`
