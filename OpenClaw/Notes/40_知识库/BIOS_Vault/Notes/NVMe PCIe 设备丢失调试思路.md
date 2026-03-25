# NVMe/PCIe 设备丢失调试思路

## 问题分层

```
gAmiNvmeControllerProtocolGuid 找不到 (表象)
    ↑
NVMe driver 没有 bind
    ↑
没有 PciIo handle
    ↑
PCI 枚举阶段看不到 endpoint (根因可能在这里)
    ↑
Root Port 状态 / 链路训练 / 设备上电时序
```

---

## 第一步：确定问题在哪一层

### 检查 PCI device list

在 Shell 或 RU 工具里看 PCI device list：

- **能看到 NVMe** → 问题在 NVMe driver 层
- **看不到 NVMe** → 问题在 PCIe 枚举层

---

## 第二步：检查 Root Port 状态

### 需要检查的寄存器

1. **BDF (Bus/Device/Function)**
   - 确认 Root Port 的位置

2. **Secondary/Subordinate Bus**
   - 是否已分配？
   - 如果还是 00/00 → bus 分配问题
   - 如果已分配但下游返回 0xFFFF → 链路/endpoint 问题

3. **Link Status**
   - 链路是否起来？
   - Link width / speed 是多少？

4. **下游 Vendor ID / Device ID**
   - 能读到有效值 → 设备存在
   - 返回 0xFFFF → 设备不存在或链路没起来

---

## 第三步：硬件时序检查

如果链路没起来，需要检查：

### 静态硬件层

| 检查项 | 说明 |
|---|---|
| `M2_CFG[3:0]` | 采样值是否正确 |
| 供电 rail | 是否到位 |
| `PERST#` | 是否按时序释放 |
| `REFCLK` | 是否送到模块 |
| `CLKREQ#` | routing/mapping 是否正确 |

### 链路层

| 检查项 | 说明 |
|---|---|---|
| LTSSM 状态 | 卡在哪个阶段？ |
| Receiver Detect | 是否通过？ |
| Link negotiation | width/speed 是否成功？ |
| VID/DID | 配置空间能否读出？ |

---

## Cold Reset 丢设备 vs Warm Reset 正常

### 典型原因

设备上电/复位时序在 cold reset 条件下不稳定。

### Cold Reset vs Warm Reset 对设备的影响

| | Warm Reset | Cold Reset |
|---|---|---|
| 设备状态 | 保留更多前提状态 | 更彻底清状态 |
| 电源/时钟 | 部分保留 | 重新初始化 |
| 链路训练 | 可能跳过某些步骤 | 完整重新训练 |

### 调试建议

1. 用示波器测量 PERST#、REFCLK、CLKREQ# 信号时序
2. 检查 M.2 插槽的电源时序
3. 检查 PERST# 信号的上拉/下拉
4. 考虑在 PEI/DXE 阶段增加 PCIe 设备初始化的延迟

---

## 调试工具

### 串口日志

在关键路径加串口打印：
- Root Port 状态
- 链路训练过程
- PCI 枚举结果

### RU 工具

- 查看 PCI device list
- 读写 PCI 配置空间

### 示波器

- 测量 PERST#、REFCLK、CLKREQ# 信号
- 检查电源上电时序

---

## 相关链接

- [[M.2 PCIe 设备存在检测机制]]
- [[Cold Reset vs Warm Reset (Intel PCH CF9 寄存器)]]
- [[首启自动 Reset 机制 (BDS Memory Type Information)]]

---

*来源: 2026-03-17 NVMe 首启调试会话*
