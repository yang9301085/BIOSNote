# BIOS 平台配置项（Intel EHL/FSP）

**来源**: 笔录 2026-03-11《BIOS-配置项》

#BIOS #Intel #EHL #FSP #平台配置

---

## 核心知识点

### 1. Intel FSP 架构

FSP（Framebuffer Software Package）是 Intel 提供的初始化固件包，封装了 CPU 和 PCH 的初始化代码。BIOS 开发商（IBV）通过调用 FSP API 完成底层初始化。

**FSP 三大组件**：
- **FSP-M**: Memory Init — 内存控制器初始化
- **FSP-T**: Silicon Init — PCH 初始化（USB、PCIe 等）
- **FSP-S**: Silicon Init — 更多 PCH 组件（可选）

**调用顺序**：`FSP-M（内存）→ FSP-T（基础PCH）→ FSP-S（如有）→ PEI Foundation → DXE`

### 2. CrbPkg 目录结构（Intel EHL）

典型 CrbPkg 结构：
```
CrbPkg/
├── Intel/CrbPkg/           # CRB 板级包
│   ├── Include/            # 头文件
│   ├── Library/            # CRB 专用库
│   └── FspmUpd.h / FspTUpd.h  # FSP 配置表
├── PlatformPkg/            # 平台包
│   ├── CrbBoard/          # 板级配置
│   └── Policy/            # 策略库
```

### 3. PCH 配置项（Pcd）

BIOS 通过 PCD（Platform Configuration Database）管理平台配置。常见配置项：
- `PcdLpcUsb20Enable` — USB 2.0 使能
- `PcdPcieAspm` — PCIe ASPM 电源管理
- `PcdSiClockConfiguration` — 时钟配置

配置方式：
1. **DSC 文件**中 `PcdsDynamicVpd` 或 `PcdsDynamicExVpd`
2. **FDF 文件**中指定 Flash 区域映射
3. **VPD（Vital Product Data）** — 可变持久配置

### 4. 平台启动顺序（Boot Order）

Fixed Boot Order 由 `BootOrderPolicy` 驱动：
- **AMI 方案**: `FixedBootOrder` 库读取 NVRAM 变量 `BootOrder` / `BootNext`
- **位置**: `AmiModulePkg/.../FixedBootOrder.c`
- **优先级**: USB → SATA → NVMe → PCIe → Network

### 5. 镜像组织（Flash Map）

SPI Flash 分区典型布局：
```
+------------------------+ 0x0
| FD Region (Descriptor) | 只读，描述其他区域
+------------------------+ ~0x100000
| BIOS Region            | 主 BIOS 代码
+------------------------+ 
| ME Region              | Intel ME 固件
+------------------------+
| GBe Region             | 千兆网卡 MAC
+------------------------+
```

---

## 来源笔录

- 2026-03-11《BIOS-配置项》— 分析 F:\RevolutionBIOSUI 平台/镜像组织
- 2025-12-15/25 平台配置项系列
