# BIOS_SPI_Flash与PCH启动时序

**来源**: 笔录 2026-03-25《BIOS 固件镜像 / SPI Flash》

#BIOS #SPI #Flash #ROM #PCH #启动

---

# SPI Flash 与 PCH 启动时序

## 核心知识点

### 谁先访问 SPI Flash？
**PCH 先于 CPU 访问 SPI Flash。**

- PCH 内部的 BFV（Boot Firmware Volume）启动向量在复位后即可访问 ROM
- CPU 还在做 BIST 时，PCH 已经可以读写 SPI Flash
- CPU 在 SEC 阶段才真正开始执行 BIOS 代码，但代码本身已由 PCH 搬运到临时 RAM

### BIOS 启动流程（PCH 视角）
```
PCH 上电
  ↓
读取 SPI Flash 0xFFFFFFFC（Reset Vector）→ 获取 BFV 起始地址
  ↓
CPU 开始执行 SEC 代码（0xFFFFFFF0）
  ↓
MicroCode 加载 → CAR 初始化
  ↓
FSP-M（Memory Init）→ 内存初始化
  ↓
FSP-S（Silicon Init）→ CPU 和 PCH 初始化
  ↓
PEI Foundation → DXE Foundation → BDS → OS Loader
```

### VSCC 寄存器（PCH 访问 Flash 的参数表）
每块 Flash 芯片有自己的电压/时序参数：
```
Flash 型号（JEDEC ID）
  → 对应 VSCC 参数（LVSCC / UVSCC）
  → PCH 按参数配置 SPI 控制器时序
```
`SpiIdentify.c`：根据 Flash 芯片的 JEDEC ID 查询对应 VSCC 参数，确保不同型号 Flash 都能正确访问。

