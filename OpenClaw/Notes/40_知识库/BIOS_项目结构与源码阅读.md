# BIOS 项目结构与源码阅读方法

**来源**: 笔录 2026-03-11《BIOS-项目结构》系列

#BIOS #UEFI #项目架构 #源码阅读

---

## 核心知识点

### EDK II 项目标准目录结构

```
Project/
├── ProjectName.dsc          # Build 描述文件（组件声明）
├── ProjectName.fdf          # Flash 布局文件（区域划分）
├── PlatformPkg/             # 平台包（板级支持）
│   ├── Library/            # 平台专用库
│   ├── Include/            # 平台头文件
│   ├── CrbBoard/          # CRB（Customer Reference Board）配置
│   └── Policy/            # 策略库
├── IntelFsp2Pkg/           # Intel FSP 封装包
├── MdePkg/                 # Intel MDE（Module Development Environment）
│   ├── Include/           # UEFI/PI 标准头文件
│   └── Library/           # 通用库（Timer, Debug, ReportStatusCode）
├── AmiModulePkg/           # AMI 专用模块包
├── Build/                   # 编译输出目录
└── Conf/                   # 构建配置文件（target.txt, tools_def.txt）
```

### DSC 文件解析

DSC（Platform Build Description File）声明了：
- `Defines` — 编译变量（TARGET, ARCH, TOOLCHAIN）
- `LibraryClasses` — 全局库实例（链接到所有模块）
- `Pcds` — 平台配置数据库
- `Components` — 所有要编译的模块（.inf 文件列表）

**关键概念**：
- `LibraryClasses` 优先级高于 INF 内声明的默认库
- 多个库同名时， DSC 中后声明的覆盖先声明的

### INF 文件结构

INF（Module INF File）描述单个 UEFI 模块：
```ini
[Defines]
  INF_VERSION    = 0x00010005
  BASE_NAME      = MyDriver
  FILE_GUID      = xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
  MODULE_TYPE    = DXE_DRIVER

[Sources]
  MyDriver.c

[Packages]
  MdePkg/MdePkg.dec

[LibraryClasses]
  BaseLib
  DebugLib
```

### FDF 文件解析

FDF（Flash Description File）描述 Flash 布局：
```ini
[FV.FVBIOS]
FfsGuids = xxx { ... }   # FV 文件组
File = xxx.fv { ... }    # 固件卷

[Section]
FILE = ModuleName { ... } # 单个模块的 Flash 位置
```

### AMI BIOS 特有目录

```
AmiModulePkg/              # AMI 公共模块
  ├── Library/
  │   └── YqrSerialPrintLib/   # 串口打印库（启仁自研）
  ├── Include/
  └── Graphics/           # 图形相关

AmiChipsetModulePkg/       # 芯片组模块
AmiCryptoPkg/              # 加密模块
```

### 源码阅读顺序建议

1. **先看 DSC+FDF**：了解整体镜像结构和编译范围
2. **再看平台包**：PlatformPkg/CrbBoard/ 得到板级配置
3. **最后深入模块**：按启动顺序 SEC→PEI→DXE→BDS 追踪关键模块
4. **定位问题**：用 `grep -r "关键字" .` 快速找代码位置

### PBX6000 项目结构（Intel Elkhart Lake）

```
PBX6000_sample/
├── Intel/                  # Intel 官方包（FSP, MDE）
├── PlatformPkg/           # PBX6000 平台包
│   ├── CrbBoard/         # CRB 配置
│   └── Policy/           # 启动策略
├── AmiModulePkg/         # AMI 模块
├── AmiChipsetModulePkg/ # 芯片组模块
└── Build/                # 编译产物
```
