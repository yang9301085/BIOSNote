# AMI PCI Bus Driver 分析文档

## 1. 概述

### 1.1 模块信息
- **文件路径**: `AmiModulePkg/PCI/PciBus.c`
- **模块类型**: UEFI Driver
- **入口函数**: `PciBusEntryPoint`
- **版权**: Copyright (c) 1985 - 2020, American Megatrends International LLC

### 1.2 模块功能
AMI PCI Bus Driver 是一个符合 EFI 1.1 驱动模型的 PCI 总线驱动程序，负责：
- PCI 设备枚举和资源分配
- PCI I/O 协议实例管理
- PCI Express 设备支持
- PCI-X 设备支持
- 热插拔控制器支持
- IRQ 路由表生成

---

## 2. 核心数据结构

### 2.1 主要数据类型定义 (PciBus.h)

#### 2.1.1 PCI设备类型枚举 (PCI_DEVICE_TYPE)
```c
typedef enum {
    tPciUnknown=0,      // 未识别设备
    tPciRootBrg,        // PCI 根桥设备
    tPci2PciBrg,        // PCI-to-PCI 桥设备
    tPci2CrdBrg,        // PCI-to-CardBus 桥设备
    tPciHostDev,        // PCI 主桥设备
    tPciDevice,         // PCI 终端设备
    tUncompatibleDevice,// 不兼容设备
    tPciMaxDevice       // 枚举边界值
} PCI_DEVICE_TYPE;
```

#### 2.1.2 BAR类型枚举 (PCI_BAR_TYPE)
```c
typedef enum {
    tBarUnused=0,       // 未使用
    tBarBus,            // 虚拟BAR - 总线资源
    tBarIo16,           // 16位I/O资源
    tBarIo32,           // 32位I/O资源
    tBarMmio32,         // 32位MMIO资源
    tBarMmio32pf,       // 32位可预取MMIO
    tBarMmio64,         // 64位MMIO资源
    tBarMmio64pf,       // 64位可预取MMIO
    tBarIo,             // I/O资源超集
    tBarMem,            // 内存资源超集
    tBarMemPf,          // 可预取内存超集
    tBarMaxType         // 枚举边界值
} PCI_BAR_TYPE;
```

#### 2.1.3 资源类型枚举 (MRES_TYPE)
```c
typedef enum {
    rtBus=0,            // 总线资源
    rtIo16,             // 16位I/O资源
    rtIo32,             // 32位I/O资源
    rtMmio32,           // 32位MMIO资源
    rtMmio32p,          // 32位可预取MMIO
    rtMmio64,           // 64位MMIO资源
    rtMmio64p,          // 64位可预取MMIO
    rtMaxRes            // 枚举边界值
} MRES_TYPE;
```

#### 2.1.4 设备不兼容类型枚举 (PCI_INCOMPATIBILITY_TYPE)
```c
typedef enum {
    icNone,             // 设备兼容
    icBarBad,           // BAR行为异常
    icBarFixedSize,     // 固定大小BAR
    icNotBar,           // 非BAR寄存器
    icBad64BitBar,      // 64位BAR问题
    icMaxIncompatibility
} PCI_INCOMPATIBILITY_TYPE;
```

### 2.2 核心数据结构

#### 2.2.1 PCI设备私有数据结构 (PCI_DEV_INFO)
这是PCI总线驱动中最重要的数据结构，存储每个PCI设备的完整信息：

```c
typedef struct _PCI_DEV_INFO {
    EFI_PCI_IO_PROTOCOL      PciIo;          // PCI I/O协议实例
    PCI_BUS_OVERRIDE_DATA    BusOvrData;     // 总线覆盖数据
    EFI_DEVICE_PATH_PROTOCOL *DevicePath;    // 设备路径

    // 父设备信息
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *RbIo;   // 根桥I/O协议
    struct _PCI_DEV_INFO    *ParentBrg;       // 父桥指针
    PCI_HOST_INFO           *HostData;        // 主机数据指针

    // 设备特定信息
    PCI_DEVICE_TYPE         Type;            // 设备类型
    BOOLEAN                 Started;         // 设备已启动
    BOOLEAN                 Assigned;        // 资源已分配
    BOOLEAN                 Discovered;      // 资源已发现
    BOOLEAN                 Enumerated;      // 设备已枚举
    PCI_CFG_ADDR            Address;         // PCI地址
    EFI_HANDLE              Handle;          // 设备句柄
    PCI_DEV_ID              DevVenId;        // 设备/厂商ID
    PCI_DEV_CLASS           Class;           // 设备类型代码
    UINT8                   IrqPinReg;       // IRQ引脚寄存器
    PCI_BAR                 Bar[PCI_MAX_BAR_NO+1]; // BAR数组

    // 设备特性
    UINT64                  Capab;           // 能力标志
    UINT64                  Attrib;          // 属性标志
    UINTN                   PmiCapOffs;      // PM能力偏移

    // 扩展特性
    PCIE_DATA               *PciExpress;     // PCIe数据
    PCIX_DATA               *PciX;           // PCI-X数据
    PCI_HPC_INFO            *HotPlug;        // 热插拽数据

    // 错误标志
    BOOLEAN                 Incompatible;    // 不兼容设备
    BOOLEAN                 RbError;         // 根桥错误
    BOOLEAN                 RomBarError;     // ROM BAR错误
    BOOLEAN                 DebugPort;       // 调试端口

    // SDL数据
    AMI_SDL_PCI_DEV_INFO    *AmiSdlPciDevData;
    UINTN                   SdlDevCount;
    UINTN                   *SdlIdxArray;
    UINTN                   SdlDevIndex;

    // IRQ路由
    PCI_IRQ_PIC_ROUTE       *PicIrqEntry;
    PCI_IRQ_APIC_ROUTE      *ApicIrqEntry;

    // 多功能设备支持
    PCI_DEV_INFO            *Func0;
    UINTN                   FuncCount;
    PCI_DEV_INFO            **DevFunc;

    // ROM相关
    UINTN                   EfiRomCount;
    PCI_ROM_IMAGE_DATA      **EfiRomImages;
    PCI_LOAD_FILE_DATA      LoadFileData;

    // Setup数据
    PCI_DEVICE_SETUP_DATA   DevSetup;
    AMI_ROM_POLICY_DATA     *AmiRomPolicy;
} PCI_DEV_INFO;
```

#### 2.2.2 PCI桥扩展数据结构 (PCI_BRG_EXT)
存储PCI桥设备的额外信息：

```c
typedef struct _PCI_BRG_EXT {
    PCI_BAR                  Res[rtMaxRes];  // 资源需求
    PCI_BAR                  Pad[rtMaxRes];  // 填充资源
    BRG_ALIGN_INFO           Align[rtMaxRes];// 对齐信息
    BRG_RES_ORDER            BarOrder[rtMaxRes];// BAR顺序
    EFI_HPC_PADDING_ATTRIBUTES PaddAttr;     // 填充属性
    UINT64                   AllocAttr;      // 分配属性

    // 子设备列表
    UINTN                    ChildCount;     // 子设备数量
    PCI_DEV_INFO             **ChildList;    // 子设备列表

    BOOLEAN                  Padded;         // 已填充标志
    PCI_BRG_CNT_REG          BrgCmdReg;      // 桥命令寄存器
} PCI_BRG_EXT;
```

#### 2.2.3 PCI主机信息结构 (PCI_HOST_INFO)
PCI子系统根结构：

```c
typedef struct _PCI_HOST_INFO {
    PCI_DEV_INFO             *VgaDev;        // VGA设备指针
    PCI_DEV_INFO             *PriIde;        // 主IDE设备
    PCI_DEV_INFO             *SecIde;        // 从IDE设备

    // SDL数据
    AMI_SDL_PCI_DEV_INFO     *HbSdlData;
    UINTN                    HbSdlIndex;

    // 根桥列表
    UINTN                    RbCount;
    PCI_DEV_INFO             **RootBridges;

    // 协议指针
    EFI_HANDLE               HostHandle;
    EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *ResAllocProt;
    EFI_PCI_PLATFORM_PROTOCOL *PlatformProt;
    EFI_INCOMPATIBLE_PCI_DEVICE_SUPPORT_PROTOCOL *IncompDevProt;
    EFI_PCI_HOT_PLUG_INIT_PROTOCOL *HpInitProt;
    EFI_PCI_OVERRIDE_PROTOCOL *PciOverrideProt;

    // 热插拔控制器
    UINTN                    RhpcCount;
    PCI_HPC_INFO             **RhpcList;

    // 状态标志
    BOOLEAN                  Updated;
    BOOLEAN                  Enumerated;
    BOOLEAN                  EnumerationCompleteFlag;
} PCI_HOST_INFO;
```

#### 2.2.4 PCIe数据结构 (PCIE_DATA)
存储PCIe设备特定信息：

```c
typedef struct _PCIE_DATA {
    UINT16                   PcieOffs;       // PCIe能力偏移
    PCIE_PCIE_CAP_REG        PcieCap;        // PCIe能力寄存器
    PCIE_DEV_CAP_REG         DeviceCap;      // 设备能力
    PCIE_LNK_CAP_REG         LinkCap;        // 链路能力
    PCIE_SLT_CAP_REG         SlotCap;        // 插槽能力
    PCIE_ROOT_CAP_REG        RootCap;        // 根能力

    // 初始化标志
    BOOLEAN                  LinkSet;
    BOOLEAN                  DevSet;
    BOOLEAN                  SlotSet;

    // 链路参数
    UINT16                   MaxPayload;
    UINT16                   MaxL0Lat;
    UINT16                   MaxL1Lat;
    UINT8                    CurrLnkSpeed;
    UINT8                    CurrLnkWidth;

    // 扩展数据指针
    PCIE2_DATA               *Pcie2;         // Gen2数据
    PCIE3_DATA               *Pcie3;         // Gen3数据
    PCIE4_DATA               *Pcie4;         // Gen4数据
    PCIE5_DATA               *Pcie5;         // Gen5数据
    VC_DATA                  *VcData;        // 虚拟通道
    PCIE_SRIOV_DATA          *SriovData;     // SR-IOV数据
    PCIE2_ARI_DATA           *AriData;       // ARI数据
    PCIE2_ACS_DATA           *AcsData;       // ACS数据
    PCIE2_ATS_DATA           *AtsData;       // ATS数据

    PCIE1_SETUP_DATA         Pcie1Setup;     // Setup配置
    PCI_DEV_INFO             *Owner;         // 所属设备
} PCIE_DATA;
```

---

## 3. 协议接口

### 3.1 EFI_DRIVER_BINDING_PROTOCOL

驱动绑定协议实例，实现UEFI驱动模型：

```c
EFI_DRIVER_BINDING_PROTOCOL gPciBusDriverBinding = {
    PciBusSupported,    // Supported检测函数
    PciBusStart,        // Start启动函数
    PciBusStop,         // Stop停止函数
    版本号,
    NULL,               // Image Handle
    NULL                // DriverBindingHandle
};
```

#### 3.1.1 PciBusSupported
检查是否支持该控制器：
- 验证设备路径有效性
- 检查EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL是否存在
- 返回EFI_SUCCESS表示支持

#### 3.1.2 PciBusStart
启动PCI总线驱动：
1. 定位Boot Script协议
2. 定位AMI Board Info协议
3. 初始化Setup数据
4. 调用PciEnumerateAll进行枚举
5. 创建IRQ路由表
6. 安装扩展PCI总线协议
7. 启动PCI设备

#### 3.1.3 PciBusStop
停止PCI总线驱动：
- 释放子设备句柄
- 关闭协议

### 3.2 EFI_PCI_IO_PROTOCOL

PCI I/O协议提供以下功能：

| 函数 | 功能 |
|------|------|
| PciIoPollMem | 轮询内存区域 |
| PciIoPollIo | 轮询I/O区域 |
| PciIoMemRead/Write | 内存读写 |
| PciIoIoRead/Write | I/O读写 |
| PciIoConfigRead/Write | 配置空间读写 |
| PciIoCopyMem | 内存复制 |
| PciIoMap/Unmap | DMA映射 |
| PciIoAllocateBuffer/FreeBuffer | 缓冲区分配/释放 |
| PciIoFlush | 刷新DMA缓冲区 |
| PciIoGetLocation | 获取设备位置(Bus/Dev/Func) |
| PciIoAttributes | 属性操作 |
| PciIoGetBarAttributes/SetBarAttributes | BAR属性操作 |

### 3.3 AMI_PCI_EXT_PROTOCOL (扩展协议)

AMI扩展PCI总线协议提供额外功能：

```c
typedef struct _AMI_PCI_EXT_PROTOCOL {
    AMI_PCI_EXT_IS_PCI_EXPRESS    IsPciExpress;      // 检查PCIe设备
    AMI_PCI_EXT_IS_PCI_X          IsPciX;            // 检查PCI-X设备
    AMI_PCI_EXT_IS_P2P_BRG        IsPci2PciBridge;   // 检查P2P桥
    AMI_PCI_EXT_IS_CRD_BRG        IsPci2CrdBridge;   // 检查CardBus桥
    AMI_PCI_EXT_IS_REG_DEVICE     IsPciDevice;       // 检查普通设备
    AMI_PCI_EXT_GET_CLASS_CODES_INFO GetClassCodesInfo; // 获取类别码
    AMI_PCI_EXT_GET_PCI_PIC_IRQ   GetPciPicIrq;      // 获取PIC IRQ
    AMI_PCI_EXT_GET_PCI_APIC_IRQ  GetPciApicIrq;     // 获取APIC IRQ
    EFI_HANDLE                    PciExtHanle;
} AMI_PCI_EXT_PROTOCOL;
```

GUID: `{0xf42a009d, 0x977f, 0x4f08, 0x94, 0x40, 0xbc, 0xa5, 0xa3, 0xbe, 0xd9, 0xaf}`

---

## 4. 核心函数分析

### 4.1 入口函数 (PciBusEntryPoint)

```c
EFI_STATUS EFIAPI PciBusEntryPoint(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
)
```

**功能**: 初始化PCI总线驱动

**执行流程**:
1. 调用InitAmiLib初始化AMI库
2. 获取CPU Cache Line Size
3. 获取调试端口信息(如果启用)
4. 安装驱动绑定协议和组件名称协议
5. 注册DxeSmmReadyToLock回调
6. 注册IoMmu协议就绪回调

### 4.2 枚举函数 (PciEnumerateAll)

```c
EFI_STATUS EFIAPI PciEnumerateAll(EFI_HANDLE Controller)
```

**功能**: 枚举所有PCI设备

**执行流程**:
1. 初始化PCI数据库(如果未初始化)
2. 遍历所有主机桥
3. 发送枚举开始通知(EfiPciHostBridgeBeginEnumeration)
4. 发送总线分配开始通知(EfiPciHostBridgeBeginBusAllocation)
5. 枚举每个根桥下的设备
6. 分配资源
7. 发送枚举完成通知

### 4.3 设备能力获取 (GetDeviceCapabilities)

```c
EFI_STATUS EFIAPI GetDeviceCapabilities(PCI_DEV_INFO *Dev)
```

**功能**: 检测PCI设备能力

**检测项目**:
- I/O空间支持
- 内存空间支持
- 总线主控支持
- VGA调色板侦听支持
- ISA I/O支持
- VGA支持
- IDE主/从支持

### 4.4 能力指针获取 (GetPciCapPtrs)

```c
EFI_STATUS EFIAPI GetPciCapPtrs(PCI_DEV_INFO *Device)
```

**功能**: 收集设备能力指针

**处理的能力类型**:
- PMI (电源管理接口)
- PCI Express
- 热插拔
- PCI-X

### 4.5 设备属性操作 (DeviceAttributes)

```c
EFI_STATUS EFIAPI DeviceAttributes(
    PCI_DEV_INFO *Dev,
    UINT64 Attributes,
    BOOLEAN Set,
    EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION Operation
)
```

**功能**: 设置或重置PCI设备属性

**支持的属性**:
- EFI_PCI_IO_ATTRIBUTE_IO
- EFI_PCI_IO_ATTRIBUTE_MEMORY
- EFI_PCI_IO_ATTRIBUTE_BUS_MASTER
- EFI_PCI_IO_ATTRIBUTE_VGA_*
- EFI_PCI_IO_ATTRIBUTE_IDE_*

### 4.6 SDL数据匹配 (FindMatchingSdlDev)

```c
EFI_STATUS EFIAPI FindMatchingSdlDev(PCI_DEV_INFO *Dev)
```

**功能**: 为设备查找匹配的SDL数据

**执行流程**:
1. 检查SDL数据模式
2. 获取PCI设备缓冲区
3. 匹配设备地址或槽位
4. 更新设备SDL数据指针
5. 设置固定资源和ROM策略

### 4.7 IRQ路由表创建 (CreateDevIrqEntry)

```c
EFI_STATUS EFIAPI CreateDevIrqEntry(PCI_DEV_INFO *Dev)
```

**功能**: 为设备创建IRQ路由表条目

**处理内容**:
- PIC IRQ路由信息
- APIC IRQ路由信息
- 从SDL数据获取IRQ映射

---

## 5. 全局变量

### 5.1 主要全局变量

| 变量名 | 类型 | 说明 |
|--------|------|------|
| gPciHostList | PCI_HOST_INFO** | PCI主机列表 |
| gPciHostArray | PCI_HOST_INFO* | PCI主机数组 |
| gHostCnt | UINTN | 主机计数 |
| gPciDefaultSetup | PCI_SETUP_DATA* | 默认Setup数据 |
| gPciCommonSetup | PCI_COMMON_SETUP_DATA* | 通用Setup数据 |
| gS3SaveState | EFI_S3_SAVE_STATE_PROTOCOL* | S3启动脚本协议 |
| gPciInitProtocol | AMI_BOARD_INIT_PROTOCOL* | PCI初始化协议 |
| gIoMmuProtocol | EDKII_IOMMU_PROTOCOL* | IOMMU协议 |
| gPciOutOfRes | BOOLEAN | 资源不足标志 |
| mMaxBusFound | UINT8 | 最大总线号 |

### 5.2 不兼容设备列表

全局变量 `gAmiBadPciDevList` 包含已知的不兼容PCI设备：

| 厂商ID | 设备ID | 问题类型 | 说明 |
|--------|--------|----------|------|
| 0x9004 | 0xFFFF | icBarBad | Adaptec设备I/O BAR问题 |
| 0x9005 | 0xFFFF | icBarBad | Adaptec设备I/O BAR问题 |
| 0x1077 | 0x1216 | icBarFixedSize | AMI Mega RAID固定BAR |
| 0x8086 | 0x283E | icNotBar | ICH8 SMBus控制器 |
| 0x10EC | 0x8168 | icBarFixedSize | RealTek 8111DP |
| 0x10DE | 0x01D3 | icBad64BitBar | NVIDIA 7200 GS |

---

## 6. 依赖库

| 库名称 | 用途 |
|--------|------|
| AmiPciBusLib | PCI总线通用功能 |
| AmiPciExpressLib | PCIe通用功能 |
| AmiPciExpressGen2Lib | PCIe Gen2支持 |
| AmiPciExpressGen3Lib | PCIe Gen3支持 |
| AmiPciExpressGen4Lib | PCIe Gen4支持 |
| AmiPciExpressGen5Lib | PCIe Gen5支持 |
| AmiPciHotPlugLib | 热插拔支持 |
| AmiSriovLib | SR-IOV支持 |
| AmiSdlLib | SDL数据处理 |
| DxeServicesTableLib | DXE服务表 |

---

## 7. 协议依赖

### 7.1 生成的协议
- gEfiPciIoProtocolGuid
- gEfiDriverBindingProtocolGuid
- gEfiBusSpecificDriverOverrideProtocolGuid
- gAmiExtPciBusProtocolGuid
- gEfiLoadFile2ProtocolGuid
- gEfiComponentName2ProtocolGuid
- gEfiPciEnumerationCompleteProtocolGuid
- gAmiOptionRomPolicyProtocolGuid

### 7.2 消费的协议
- gEfiPciHostBridgeResourceAllocationProtocolGuid
- gEfiPciRootBridgeIoProtocolGuid
- gEfiDevicePathProtocolGuid
- gEfiIncompatiblePciDeviceSupportProtocolGuid
- gEfiS3SaveStateProtocolGuid
- gEfiPciHotPlugInitProtocolGuid
- gEfiPciPlatformProtocolGuid
- gEfiPciOverrideProtocolGuid
- gEdkiiIoMmuProtocolGuid

---

## 8. 流程图

### 8.1 PCI设备枚举流程

```
PciBusStart()
    │
    ├─→ 初始化Setup数据
    │
    ├─→ PciEnumerateAll()
    │       │
    │       ├─→ InitPciDb() 初始化数据库
    │       │
    │       ├─→ DoPciNotify(EfiPciHostBridgeBeginEnumeration)
    │       │
    │       ├─→ DoPciNotify(EfiPciHostBridgeBeginBusAllocation)
    │       │
    │       ├─→ 遍历根桥
    │       │       │
    │       │       └─→ 枚举总线上的设备
    │       │               │
    │       │               ├─→ 读取配置空间
    │       │               ├─→ GetDeviceCapabilities()
    │       │               ├─→ GetPciCapPtrs()
    │       │               ├─→ FindMatchingSdlDev()
    │       │               └─→ CreateDevIrqEntry()
    │       │
    │       ├─→ 资源分配
    │       │
    │       └─→ DoPciNotify(EfiPciHostBridgeEndEnumeration)
    │
    ├─→ CreateIrqTables()
    │
    ├─→ 安装扩展协议
    │
    └─→ StartPciDevices()
```

---

## 9. 关键宏定义

```c
// PCI地址组装
#define PCI_ASSEMBLE_ADDR(b,d,f,r) \
    ((UINT64)((((UINTN)b)<<24)+(((UINTN)d)<<16)+(((UINTN)f)<<8)+((UINTN)r)))

// 驱动签名
#define AMI_PCI_SIG 0x24494350494d4124 // '$AMIPCI$'

// VGA属性
#define PCI_VGA_ATTRIBUTES (EFI_PCI_IO_ATTRIBUTE_VGA_PALETTE_IO | \
                           EFI_PCI_IO_ATTRIBUTE_VGA_MEMORY | \
                           EFI_PCI_IO_ATTRIBUTE_VGA_IO)
```

---

## 10. 注意事项

### 10.1 内存管理
- 使用MallocZ分配零初始化内存
- 使用pBS->FreePool释放内存
- 注意检查返回指针是否为NULL

### 10.2 配置空间访问
- 使用PciCfg8/16/32/64函数访问配置空间
- PCIe设备支持扩展配置空间(0x100-0xFFF)
- 普通PCI设备配置空间限制在0xFF以内

### 10.3 资源分配
- 支持固定资源分配模式
- 支持动态资源分配模式
- 支持资源填充(Padding)机制

### 10.4 S3休眠支持
- 支持保存BAR配置到启动脚本
- 支持VGA S3重发布

---

## 11. 版本信息

- 主版本: PCI_BUS_MAJOR_VER
- 次版本: PCI_BUS_MINOR_VER
- 修订版本: PCI_BUS_REVISION

---

*文档生成日期: 2026-03-18*
*基于源码版本: AMI Aptio*
