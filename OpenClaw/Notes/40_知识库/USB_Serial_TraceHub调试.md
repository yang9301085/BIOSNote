# USB Serial TraceHub 调试方法

**来源**: 笔录 2026-03-24/25《USB Serial TraceHub》

#BIOS #USB #调试 #TraceHub #串口

---

## 核心知识点

### USB Serial TraceHub 是什么

USB Serial TraceHub 是 Intel 平台提供的硬件调试通道，通过 USB 接口输出 BIOS 调试日志。相比传统 UART 串口，TraceHub 带宽更高（支持多端口并行输出），但配置更复杂。

**典型应用场景**：
- 没有传统 COM 口的超轻薄平台
- 需要同时输出多路调试日志（USB + Serial 混用）
- 大数据量日志（视频驱动、图形调试）

### 硬件连接

1. 找到主板上的 **USB Debug Cable** 接口（通常标注为 USB Debug）
2. 用 USB Debug Cable 连接目标机和调试主机
3. 调试主机安装 **Intel System Debugger** 或对应驱动

### BIOS 端配置

```c
// 典型 Setup 变量
PchUsbDebugEnable    // USB Debug 使能
TraceHubDebugMode    // 调试模式选择
SerialDebugPortAddr  // 串口基地址（通常是 0x3F8 或 0x2F8）
```

### 调试日志格式

BIOS 通过 `DebugLib`（如 `YqrSerialPrintLib`）输出日志：
```
[INFO] USB Init done
[DEBUG] XHCI: USB 3.0 port 2 enabled
[WARN] USB device enumeration slow
```

### 与传统 UART 串口的区别

| 维度 | UART 串口 | USB Serial TraceHub |
|---|---|---|
| 带宽 | 低（115200 bps 常见） | 高（Mbps 级） |
| 硬件需求 | COM 口 | USB Debug 口 |
| 同时多端口 | 困难 | 支持 |
| 平台依赖 | 通用 | Intel 特定 |

### 常见问题

**Q**: USB Serial TraceHub 连上了但没有输出
**A**: 检查 BIOS Setup 里 USB Debug 是否 Enable，确认驱动签名（Windows 下需要测试模式）

**Q**: 日志里出现 `Serial reg: IER:00 IIR:01 LCR:00` 后卡住
**A**: 串口初始化成功，但后续代码路径未输出日志，可能是日志开关未开启
