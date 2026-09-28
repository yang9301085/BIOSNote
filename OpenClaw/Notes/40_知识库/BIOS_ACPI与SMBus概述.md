# BIOS_ACPI与SMBus概述

**来源**: 笔录 2026-03-06《S3 睡眠唤醒》（部分内容）

#BIOS #ACPI #SMBus #SMM #电源管理

---

# ACPI 与 SMBus 概述

## 核心知识点

### ACPI（Advanced Configuration and Power Interface）
ACPI = 操作系统与 BIOS 之间的电源管理和配置接口。

**ACPI 表（从 BIOS 传递到 OS）：**
| 表 | 内容 |
|---|---|
| RSDP | Root System Description Pointer |
| RSDT/XSDT | 指向其他 ACPI 表的指针 |
| FADT | ACPI Fixed Hardware Feature Flags |
| DSDT | Differentiated System Description Table（含 AML 代码）|
| SSDT | Secondary System Description Table |
| MCFG | PCI Configuration Space |
| HPET | High Precision Event Timer |

**S0 / S3 / S5 状态：**
- **S0**：正常工作状态
- **S3**（Suspend to RAM）：内存保持，CPU 断电
- **S5**（Soft Off）：完全断电，只有 RTC 供电

### SMM（System Management Mode）
CPU 的最高权限模式，由 SMI（System Management Interrupt）触发：

**进入 SMM 的常见路径：**
- ACPI SLP_SMI#：OS 发送睡眠请求
- GPIO 触发 SMI
- BIOS 内部事件（温度告警等）

**SMM 与 S3 的关系：**
```
OS 发送 SLP_SMI# → CPU 进入 SMM
  → SMM Handler 保存上下文到 S3 Boot Script
  → 写 ACPI SLP_TYP = S3
  → CPU 进入深睡
唤醒时 → SMM Handler 从 Boot Script 恢复 → 返回 OS
```

### SMBus（System Management Bus）
- 类似 I2C 的串行总线
- 用于连接主板传感器（温度、电压、风扇）
- BIOS 通过 SMBus 读取 DIMM SPD、电源模块信息

### SMBIOS
System Management BIOS，表格式存储硬件信息：
- BIOS 版本、制造商
- 系统序列号、UUID
- 内存插槽信息
- 主板信息

**BIOS 中获取 SMBIOS 信息：**
```c
EFI_SMBIOS_PROTOCOL *Smbios;
gBS->LocateProtocol(&gEfiSmbiosProtocolGuid, ..., &Smbios);
// 遍历 SMBIOS 表获取信息
```

