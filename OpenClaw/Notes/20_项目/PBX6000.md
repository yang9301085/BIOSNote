---
type: project
status: active
owner: yqr
created: 2026-03-18
updated: 2026-03-18
tags:
  - project
  - BIOS
  - UEFI
  - AMI
  - Intel
  - ElkhartLake
---

# PBX6000_sample

## 目标

- 基于Intel Elkhart Lake平台的UEFI BIOS开发项目
- 提供完整的BIOS固件解决方案，支持嵌入式和工业应用场景

## 背景

### 项目概述
这是一个基于AMI Aptio V UEFI BIOS框架的项目，目标平台为Intel Elkhart Lake (EHL) 处理器。Elkhart Lake是Intel针对嵌入式和物联网应用设计的低功耗SoC平台。

### 技术栈
| 组件 | 说明 |
|------|------|
| BIOS供应商 | AMI (American Megatrends International) |
| 平台厂商 | Intel Corporation |
| UEFI框架 | EDK2 (EFI Development Kit 2) |
| 构建工具 | AMI SDL v7.04.0320 |
| FSP版本 | Intel Elkhart Lake FSP |

## 项目基本信息

### 构建配置
| 配置项 | 值 |
|--------|-----|
| Platform Name | ElkhartLake |
| Platform GUID | 7397eaf4-e51f-2388-1992-5ae9418df16e |
| 输出目录 | Build/ElkhartLake |
| 支持架构 | IA32 / X64 |
| 构建目标 | RELEASE / DEBUG |
| Recovery Binary | 1AWHS016.rom |
| Flash定义 | Build/Platform.fdf |

### 主要Package列表 (共37个)

#### AMI核心包
| Package | 说明 |
|---------|------|
| AmiPkg | AMI核心包 |
| AmiModulePkg | AMI功能模块包 |
| AmiCompatibilityPkg | AMI兼容性包 |
| AmiChipsetModulePkg | AMI芯片组模块包 |
| AmiChipsetPkg | AMI芯片组包 |
| AmiCpuModulePkg | AMI CPU模块包 |
| AmiTsePkg | AMI Text Setup Engine包 |
| AmiSecureBootPkg | AMI安全启动包 |
| AmiTcgPkg | AMI TPM/TCG包 |
| AmiCryptoPkg | AMI加密包 |
| AmiNetworkPkg | AMI网络包 |
| AmiHidPkg | AMI HID设备包 |
| AmiI2cPkg | AMI I2C包 |
| AmiHstiPkg | AMI HSTI包 |

#### Intel平台包
| Package | 说明 |
|---------|------|
| ElkhartLakePlatSamplePkg | Elkhart Lake平台示例包 |
| ElkhartLakeBoardPkg | Elkhart Lake板级包 |
| ElkhartLakeFspPkg | Elkhart Lake FSP包 |
| ElkhartLakeFspBinPkg | Elkhart Lake FSP二进制包 |
| ClientOneSiliconPkg | Intel客户端硅片包 |

#### UEFI标准包
| Package | 说明 |
|---------|------|
| MdePkg | UEFI开发环境基础包 |
| MdeModulePkg | UEFI模块包 |
| UefiCpuPkg | UEFI CPU包 |
| CryptoPkg | 加密包 |
| IntelFrameworkPkg | Intel框架包 |
| IntelFrameworkModulePkg | Intel框架模块包 |

### 目录结构
```
PBX6000_sample/
├── AmiModulePkg/          # AMI功能模块
│   ├── PCI/               # PCI总线驱动
│   ├── ACPI/              # ACPI支持
│   ├── AHCI/              # AHCI存储
│   ├── Library/           # 库文件
│   └── ...
├── AmiPkg/                # AMI核心包
├── AmiChipsetModulePkg/   # 芯片组模块
├── AmiCpuModulePkg/       # CPU模块
├── Intel/
│   ├── ClientOneSiliconPkg/  # Intel硅片支持
│   ├── ElkhartLakeBoardPkg/  # 板级支持
│   ├── ElkhartLakeFspPkg/    # FSP支持
│   └── ElkhartLakePlatSamplePkg/ # 平台示例
├── MdePkg/                # UEFI基础
├── MdeModulePkg/          # UEFI模块
├── Build/                 # 构建输出
├── Conf/                  # 构建配置
└── DOC/                   # 文档
```

## 范围

### 包含功能
- PCI/PCIe总线枚举与资源分配
- ACPI表生成与支持
- SATA/AHCI存储支持
- USB设备支持
- 网络启动支持
- 安全启动 (Secure Boot)
- TPM支持
- Setup界面 (TSE)
- Capsule更新
- S3休眠恢复
- 多处理器支持

### 不包含功能
- 具体产品的定制功能
- OEM特定功能实现

## 技术特性

### 支持的设备类型
- PCIe Gen3/Gen4/Gen5设备
- PCI-X设备
- 热插拔设备
- SR-IOV虚拟化设备

### 固件安全特性
- Secure Boot
- TPM 2.0支持
- BIOS Guard
- Boot Guard

### Intel特定功能
- Intel FSP (Firmware Support Package)
- Intel VT-d
- Intel TXT
- Intel SGX

## 里程碑

- [x] 项目环境搭建
- [x] 基础编译测试
- [ ] 功能模块分析
- [ ] 定制开发

## 风险

| 风险 | 影响 | 缓解措施 |
|------|------|----------|
| FSP版本更新 | 可能需要适配 | 保持FSP版本跟踪 |
| 代码库较大 | 分析复杂度高 | 分模块逐步分析 |
| 硬件依赖 | 某些功能需要实际硬件 | 使用模拟器验证基础功能 |

## 相关链接

- [AMI官网](https://www.ami.com/)
- [Intel Elkhart Lake](https://www.intel.com/content/www/us/en/products/docs/embedded-processors/atom-processors/elkhart-lake-product-brief.html)
- [UEFI规范](https://uefi.org/specifications)
- [EDK2](https://github.com/tianocore/edk2)

## 分析文档

- [PCI Bus驱动分析](PciBus_Analysis.md)
