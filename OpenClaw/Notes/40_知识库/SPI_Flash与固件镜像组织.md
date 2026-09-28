# SPI_Flash与固件镜像组织

**来源**: 笔录 2026-03-25《BIOS 固件镜像 / SPI Flash》

#BIOS #SPI #Flash #固件 #ROM #镜像

---

# SPI Flash 与固件镜像组织

## 核心知识点

### SPI Flash 上电时序（PCH 视角）
```
PCH 上电
  ↓
读取 SPI Flash 0xFFFFFFFC（Reset Vector）→ 获取 BFV 起始地址
  ↓
CPU 开始执行 SEC 代码（0xFFFFFFF0）
  ↓
MicroCode 加载 → CAR 初始化（Cache As RAM）
  ↓
FSP-M → 内存初始化
  ↓
FSP-S → PCH/CPU Silicon 初始化
  ↓
PEI → DXE → BDS → OS Loader
```

### Flash Descriptor（描述符）
SPI Flash 第一个 4KB 区域为 Flash Descriptor：
- 包含各 region 的访问权限（BIOS / ME / GbE / 平台数据）
- PCH 根据 Descriptor 配置 SPI controller 的 decode 范围
- Descriptor 本身不可被软件修改（由 Intel 工厂写入）

### VSCC 寄存器（JEDEC ID → 参数表）
每块 Flash 芯片有自己的 JEDEC ID：
```
Flash JEDEC ID → SpiIdentify.c 查询 → VSCC 参数（LVSCC/UVSCC）
                                    ↓
                           PCH 按参数配置 SPI 时序
```
- `LVSCC`：Lower VSCC（低位地址范围参数）
- `UVSCC`：Upper VSCC（高位地址范围参数）

### 固件卷（FV = Firmware Volume）
固件镜像内部由多个 FV 组成：
- **FV_MAIN**：主固件卷（PEI + DXE）
- **FV_SECURITY**：安全相关模块
- **FV_BB**：Boot Block

### FIT Table（Firmware Interface Table）
- Intel 平台特有，位于固件镜像固定偏移
- 定义 FSP-M / FSP-S / ACM 等组件的入口和位置
- BIOS 在 POST 阶段读取 FIT 做初始化调度

### AMI Flash 工具
- `AmiFlashLib`：BIOS 中访问 Flash 的库
- `Flash擦写`：通过 SPI 控制器写入 Flash 区块
- `SpiIdentify.c`：Flash 芯片识别

