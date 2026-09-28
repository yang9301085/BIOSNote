# BIOS_Port80与PCIeDebugPort

**来源**: 笔录 2026-03-25《BIOS 调试库》

#BIOS #Debug #Port80 #PCIe #诊断

---

# Port 80 诊断与 PCIe Debug Port

## 核心知识点

### Port 80 是什么
x86 架构中 0x80 是 I/O 端口，用于 POST 状态码输出。
BIOS 在各阶段向 0x80 写特定值，外接 POST 卡可观测启动进度。

### AMI 的 Port 80 输出
`REPORT_STATUS_CODE(EFI_PROGRESS_CODE, EFI_SW_PEI_PC_xxx)` → 驱动层 `PostCode()` → `IoWrite8(0x80, code)`

常见 POST Code：
- PEI 阶段：`EFI_SW_PEI_PC_xxx`（0x00-0x7F）
- DXE 阶段：`EFI_SW_DXE_PC_xxx`（0x80-0xFF）

### PCIe Debug Port 配置
AMI 支持把 Port 80 I/O 周期路由到指定 PCIe Root Port。

**设置步骤：**
1. BIOS 设置 `PCH_SETUP.PcieRootPort8xhDecode = 1`
2. 设置 `MPC.P8XDE` 位（PCI offset D8h[26]）
3. 调用 `PchIoPort80DecodeSet()` 配置 DMI 寄存器
4. 设置 `GCS.RPRDID`（目标 Root Port ID）
5. 设置 `GCS.RPR` 启用路由

**注意**：DMI 锁定后无法修改 GCS 寄存器

