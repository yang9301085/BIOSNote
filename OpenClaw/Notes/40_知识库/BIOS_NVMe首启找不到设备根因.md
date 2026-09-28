# BIOS_NVMe首启找不到设备根因

**来源**: 笔录 2026-03-16/2026-03-19/2026-03-20《NVMe 调试》

#BIOS #NVMe #PCIe #枚举 #ColdReset

---

# NVMe 首启找不到设备：根因分析

## 核心知识点

### 问题现象
- 清 CMOS 后首次 Cold Reset 找不到 NVMe
- 进入 Shell 或 BIOS Setup 后做一次 Warm Reset → NVMe 出现
- S5 关机再开机 → 依然找不到

### 根因：PCIe 枚举时序问题

`PciBus.c` 中 `EnumerateBus()` 的执行顺序：
1. **先扫子总线**（L7407-7413）：读取所有下游设备的 VID/DID
2. **后做 PCIe Link 初始化**（L7522-7527）：`PcieInitLink()`
3. **没有 Rescan 机制**

Cold Reset 后问题链：
```
Cold Reset
  → Root Port LTSSM Training 未完成
  → EnumerateBus() 读 VID/DID → 0xFFFFFFFF
  → 判"无设备"，不再挂载 gAmiNvmeControllerProtocolGuid
  → PcieDoubleCheckCard() 检查的是 Root Port Slot（SlotImpl），不是 NVMe 本身
```

### 关键代码位置
| 操作 | 位置 |
|---|---|
| 枚举 VID/DID | `PciBus.c:6943-6992` |
| Link 初始化 | `PciBus.c:7522-7527` |
| 设备安装 | `PciBus.c:10765-10823` |
| 二次检查 | `AmiPciExpressLib.c:126-170` |

### Warm Reset 能解决的原因
Warm Reset 保留了链路状态，Training 结果在，第二次枚举时设备已 Link Up。

