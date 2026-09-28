# BIOS_Intel_EHL平台分析

**来源**: 笔录 2026-03-11/2026-03-20《BIOS 平台分析》

#BIOS #Intel #EHL #ElkhartLake #PCH

---

# Intel Elkhart Lake（EHL）平台分析

## 核心知识点

### 平台架构概述
- Intel Elkhart Lake = 面向嵌入式/工业的 SoC 平台
- 代号：EHL / JSL / JHL
- 集成 PCH，功耗优化，适合物联网、POS、工业主板
- BIOS 使用 AMI Aptio V + EDK2 + Intel FSP

### 关键模块目录
- `AmiModulePkg/` — AMI 通用模块（PCI、StatusCode、BDS等）
- `Intel/ClientOneSiliconPkg/` — Intel 客户端 PCH 硅片模块
- `OemPkg/` — ODM/OEM 定制代码
- `FSP/` — Intel FSP 封装包（MemoryInit / SiliconInit）

### PCIe 平台枚举（EHL 特定）
EHL 平台 PCIe 枚举使用 `PeiPcieRpInitLib.c`，关键逻辑：
```
临时分配子总线号
  ↓
写下游 VendorID → 配置空间
  ↓
读回 VID/DID → 判断设备是否存在（0xFFFFFFFF = 无设备）
  ↓
有效设备挂到父桥 ChildList
```

### PCH GPIO 在 EHL 上的配置
GPIO 配置四种方式（优先级从高到低）：
1. **PPI 方式** — PEI 阶段通过 PEI PPI 接口配置
2. **ELink 方式** — 通过 ELink 机制链接
3. **Head file 方式** — 头文件定义 GPIO_INIT_CONFIG 表
4. **PCD 方式** — DSC 文件中配置 `PcdsDynamicExVpd`

### FSP 在 EHL BIOS 中的角色
- **FSP-M**：内存初始化（PEI 阶段调用，输出 HOB）
- **FSP-S**：Silicon 初始化（CPU、PCH、Peripherals）
- **FSP-T**：可选的 Silicon 初始化封装包
- 调用流程：SEC → FSP-M → PEI → FSP-S → DXE → BDS

### S3 Resume 特定路径（EHL）
`AmiSbUpdateBootMode()` 覆盖 Intel 默认的 `DetectBootMode()`，在 EHL 平台下：
- 把 BootMode 强制设为 `BOOT_ON_S3_RESUME`
- 影响 BDS 阶段内存类型分配逻辑
- 与 `PcdResetOnMemoryTypeInformationChange` 配合工作

