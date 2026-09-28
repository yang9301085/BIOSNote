# SPI Flash 镜像组织与 FD 区域

**来源**: 笔录 2026-03-25《BIOS-固件镜像》

#BIOS #SPI #Flash #固件镜像 #FlashDescriptor

---

## 核心知识点

### SPI Flash 分区结构

以 Intel PCH 为例，SPI Flash 典型布局：

```
+---------------------------+ 0x0000_0000
|   Flash Descriptor (FD)   | ~0x10000 (64KB) - 只读
+---------------------------+ 0x0001_0000
|        ME Region          | ~0x100000 (1MB) - Intel Management Engine
+---------------------------+ 0x0011_0000
|       BIOS Region         | ~0x700000 (7MB) - 主 BIOS 固件
+---------------------------+ 0x0081_0000
|       GbE Region          | ~0x10000 - 千兆网卡 MAC
+---------------------------+ 0x0082_0000
|    Platform Data          | 保留区
+---------------------------+ 0x0083_0000
|       Reserved            |
+---------------------------+
```

### Flash Descriptor（FD）区域

FD 是整个 SPI Flash 的"目录"，记录了：
- 各 Flash 区域的基址和大小
- 访问权限（读/写/擦除）
- SPI 控制器配置

**关键寄存器**（PCH 寄存器）：
- `RCBA + FD`（Flash Descriptor）— FD 有效性判断
- `SPIBAR + FLVALCOD` — Flash Valid Signature 检测

**FD 损坏的症状**：
- 读取 SPI Flash 返回 `0xFF` 或 `0x00`
- BIOS 无法找到启动设备
- 串口输出 `Flash Region read Permission FF`（正常情况有具体值）

### BIOS Region 内部结构

BIOS Region 本身包含多个子区域：
```
BIOS Region
├── Boot Block              # 启动块（最小引导代码）
├── FFS (Firmware File System)
│   ├── PEI Core           # PEI 核心
│   ├── PEIM              # PEI 模块
│   ├── DXE Core          # DXE 核心
│   └── DXE Driver        # DXE 驱动
├── NVRAM                  # 非易失存储（变量区）
└── SMM SMRAM             # System Management Mode RAM
```

### FSP-M 内存初始化时机

FSP-M 在 PEI 阶段早期被调用：
```
SEC Phase
  → FSP-M (MemoryInit)   ← 初始化内存控制器
  → PEI Foundation
  → PEIM (MemoryInit)    ← 传递 HOB 给后续模块
  → DXE Phase
```

**关键输出**：
- `FspHobList` — FSP 传递给 BIOS 的 HOB 列表指针
- `FspBootFirmwareVolumeInfo` — BFV（Boot Firmware Volume）信息

### 固件镜像打包工具

- **FSP 打包**: Intel FSP Tool（或 FSP build script）
- **BIOS 镜像**: AMI APTIO Build（`build.py` + DSC/FDF）
- **FD 编程**: Intel Flash Programming Tool（CHIPSAT 或同壳工具）

### 调试：Flash 读取异常

常见日志：
```
Flash_Region_read_Permission_FF
```
这通常**不是错误**，而是 FD 在描述该区域权限时的正常编码。需要结合具体区域判断（ME Region vs BIOS Region）。
