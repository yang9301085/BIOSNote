# BIOS_S3睡眠与唤醒流程

**来源**: 笔录 2026-03-06《BIOS S3 睡眠唤醒》

#BIOS #S3 #ACPI #BootScript #PCH

---

# S3 睡眠与唤醒流程（OS → S3 → Wake → OS）

## 核心知识点

### 进入 S3 的链路
当用户在 OS 中点击"睡眠"后，BIOS 侧的触发路径为：
1. OS 发送 SLP_SMI# → 进入 SMM
2. `AmiSbUpdateBootMode()` 覆盖默认 `DetectBootMode()`，将 BootMode 设为 `BOOT_ON_S3_RESUME`
3. 写 `S3BootScript`：保存寄存器、DIMM training 数据等到 BootScript Table
4. CPU 进入深睡（C6/C7）

### 唤醒后的 Port 80 骨架
唤醒后 BIOS 侧执行顺序（可通过 DEBUG 卡观测）：
```
E1  →  PEI_S3_BOOT_SCRIPT（进入 S3 Boot Script 执行）
     →  boot script opcode 序列（各类 memory/S3 相关操作）
D0  →  Resume from S3
E0  →  OS wake vector 准备完成
E3  →  OS waking（OS 已接管）
```

### E1 xx xx E1 循环的含义
- 第一个 E1：已进入 S3 Boot Script 阶段（`S3ResumeExecuteBootScript()` 发出）
- 中间 xx：Boot Script opcode 执行过程中的状态码
- 第二个 E1：说明 Boot Script 执行中途发生了硬复位或 CPU 异常，平台重新回到 S3 Resume 主链
- **不是** Boot Script 内部继续跑出来的（Boot Script 执行器内部不发 E1）

### 关键文件
| 阶段 | 文件 | 关键函数 |
|---|---|---|
| 进入 S3 | `S3Resume.c` | `AmiSmmControlTrigger` |
| Boot Script | `BootScriptExecute.c` | `S3ResumeExecuteBootScript()` |
| 唤醒判断 | `AmiSbUpdateBootMode.c` | AMI 覆盖 Intel 默认 BootMode |

