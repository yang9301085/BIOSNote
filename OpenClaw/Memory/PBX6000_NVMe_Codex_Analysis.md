# PBX6000 NVMe 首启问题 - Codex 深度分析

**日期**: 2026-03-19
**问题**: 清 CMOS 后首轮找不到 NVMe，warm reset 后能找到

---

## 核心结论

`PciBus.c` 本身没有 "cold boot / warm boot" 分支，但它的枚举顺序和 `Bds.c` 里的 reset 链组合起来，能解释"清 CMOS 后首轮找不到，warm 后能找到"。

---

## 关键发现

### 1. BDS 首轮 Reset 链 (最明显的软件差异)

**代码位置**: `AmiModulePkg/Bds/Bds.c`

| 行号 | 功能 |
|------|------|
| 1643-1649 | 用 `PreviousMemoryTypeInformation` 是否不存在判断 `IsFirstBoot` |
| 1767-1778 | 首轮且 runtime memory type 变化时置 `ResetRequired=TRUE` |
| 1809-1825 | 先写 CMOS `0x66=1`，再做一次 `EfiResetCold` |
| 1828-1836 | 下一轮启动发现 CMOS `0x66==1`，清零后再补一次 `EfiResetWarm` |

**流程**:
```
清 CMOS 后首轮 (IsFirstBoot=TRUE)
    ↓
BDS 检测到 runtime memory type 变化
    ↓
ResetRequired = TRUE
    ↓
写 CMOS 0x66 = 1
    ↓
EfiResetCold
    ↓
第二轮启动
    ↓
发现 CMOS 0x66 == 1
    ↓
清零 CMOS 0x66
    ↓
EfiResetWarm
    ↓
第三轮 (正常启动)
```

**关键点**: 清 CMOS 后首轮和后续 warm 轮次，软件路径不是同一次 DXE 条件。

---

### 2. PciBus 枚举顺序问题 (最可疑的差异点)

**代码位置**: `AmiModulePkg/PCI/PciBus.c`

| 行号 | 功能 |
|------|------|
| 6943-6992 | 对设备做首次 config read；只有 CRS 和 "slot implemented" 的窄场景会补重试 |
| 7413 | 先递归扫桥后设备 |
| 7522-7528 | 真正的 `PcieInitLink(dev)` 才调用 |

**支撑证据**: `AmiPciExpressLib.c:1508-1676`
- 这里会真正设置 link 属性、retrain，必要时 hot reset link

**问题机制**:
```
EnumerateBus() 首次扫描
    ↓
读取 NVMe VID/DID (此时链路还没稳)
    ↓
返回 0xFFFFFFFF → 判定无设备 → 跳过
    ↓
后续才调用 PcieInitLink() 初始化链路
    ↓
warm reset 后链路已稳定
    ↓
第二轮扫描能读到 VID/DID
```

**根因**: NVMe 挂在 root port 后面，首轮冷起时链路还没稳，config read 直接把它判成 `0xFFFFFFFF` 跳过；warm 后设备已上电、链路已稳定，才能被扫到。

---

### 3. Setup 变量差异 (第二优先级)

**代码位置**: `AmiModulePkg/PCI/PciBus.c`

| 行号 | 功能 |
|------|------|
| 11535-11566 | 首次读/建 `gPciCommonSetup`、`gPciDefaultSetup` |
| 11347-11369 | 创建缺失的 per-device PCI setup vars |

**AmiPciBusLib.c:1500-1600**: missing variable 会直接回填默认值并 `SetVariable`

**结论**: "首轮缺变量、次轮变量已存在" 是客观差异，但首轮也会拿到默认值，更像是辅助怀疑点。

---

### 4. 低优先级排除点

| 代码位置 | 功能 | 排除理由 |
|----------|------|----------|
| PciBus.c:778-838 | `DoPrepController()` platform/override/host bridge preprocess | 若板级控制 PERST#/power rail，可能导致差异 |
| PciBus.c:9033-9112 | BusReScan | 只在 bus out-of-resource 时走 |
| PciBus.c:9340-9478 | 资源提交/分配 | 无 OOR 痕迹时不相关 |

---

## 验证优先级

1. **证明清 CMOS 后是否真的经历了 `cold reset -> warm reset` 这条 BDS 链**
2. **证明 NVMe 所在 root port 在 `EnumerateBus()` 首次扫子设备时，link 是否还没 ready**
3. 看 `DoPrepController()` 有没有板级 hook 在首轮没把端口/电源准备好
4. 看 setup var 首轮创建、次轮生效是否真的改变了端口策略

---

## 建议加日志点

使用 `BiosSerialPrintf` 打印：

1. **BDS 层**:
   - `BdsProceedToBootCallback()` 入口
   - `ResetRequired` 值
   - CMOS `0x66` 值
   - 实际 reset 类型

2. **PCI 枚举层**:
   - `EnumerateBus()` 里 NVMe 上游 port 的第一次 `PciCfg32(VID/DID)` 返回值
   - `PcieDoubleCheckCard()` 是否进入
   - `EnumerateBus()` 递归扫桥前后
   - `PcieInitLink()` 调用前后

**定性标志**: 抓到"首轮扫描时没设备，随后 link init/retrain 才发生；第二轮 warm 扫描时设备已能回 VID/DID"

---

## 相关函数索引

### PCIe Link 管理
- `PcieInitLink()` - AmiPciExpressLib.c:1478
- `PcieRetrainLink()` - AmiPciExpressLib.c:922
- `PcieDisableLink()` - AmiPciExpressLib.c:786
- `PcieDoubleCheckCard()` - AmiPciExpressLib.c:117
- `PcieProbeDevice()` - AmiPciExpressLib.c:2745
- `PcieCheckIntegratedDevice()` - AmiPciExpressLib.c:2910

### Link Training 参数
- `LnkTrRetry` - 重试次数 (默认 5)
- `LnkTrTimeout` - 超时时间 (默认 1000us)
- `LnkDisable` - 链路禁用控制

### Gen2/Gen3 特性
- `HwAutoSpeedDis` - 硬件自动速度禁用
- `HwAutoWdtDis` - 硬件自动宽度禁用

---

## 分析方法论

### 分层定位思路

```
现象: NVMe 找不到
    ↓
NVMe driver 没有 bind
    ↓
没有 PciIo handle
    ↓
PCI 枚举阶段看不到 endpoint
    ↓
Root Port 状态 / 链路训练 / 设备上电时序
```

### 时序分析关键点

1. **软件时序**: 枚举发生在 link init 之前
2. **硬件时序**: cold reset 后链路稳定需要时间
3. **Reset 链**: BDS 的 cold reset → warm reset 链提供了"第二次机会"

---

## Multi-Agent Triad 分析 (2026-03-19)

### 三个子代理结论

**Baseline Mapper** (PciBus.c 主流程分析):
- `PciBus.c` 对 NVMe 没有专门分支，当成普通 PCIe endpoint 处理
- 主链: `PciBusStart -> PciEnumerateAll -> StartPciDevices -> StartPciChildDevice -> InstallPciDevice`
- 关键边界点: `EnumerateBus()` 首次 config read、`PcieDoubleCheckCard()`、`QueryPciDevice()` PCIe 初始化序列

**Differential Comparator** (BDS reset 链分析):
- BDS 会在首轮后制造额外 reset 链
- `Bds.c:1643-1649`: 用 `PreviousMemoryTypeInformation` 判断 `IsFirstBoot`
- `Bds.c:1767-1778`: runtime memory type 变化时置 `ResetRequired=TRUE`
- `Bds.c:1809-1825`: 写 CMOS `0x66=1` → `EfiResetCold`
- `Bds.c:1828-1836`: 发现 `0x66==1` → 清零 → `EfiResetWarm`

**Hypothesis Verifier** (假设验证):
1. **高置信度**: 冷启动时 NVMe ready 太慢，`PcieDoubleCheckCard()` retry 路径太窄
   - 默认 `LnkTrRetry=5`, `LnkTrTimeout=1000us`，总等待量级只有几毫秒
   - M.2 场景常见 `SlotImpl=0` 或 `PresenceDet=0`，retry 根本不走
2. **高置信度**: 链路初始化发生在"扫子总线之后"，成功后没有 rescan
   - `PciBus.c:7413` 先递归 `EnumerateBus()`
   - `PciBus.c:7522-7527` 才调 `PcieInitLink()`
   - 没有"链路起来后重新扫一次"的正常路径

### 最小验证打点

**4 个关键点**:
1. `EnumerateBus()` 首次读 VID/DID
2. `PcieDoubleCheckCard()`
3. `PcieInitLink()`
4. `InstallPciDevice()`

**BiosSerialPrintf 实现提醒**:
- `AmiModulePkg/Pci/PciBus.inf` 目前还没有 `YqrSerialPrintLib`
- 参考: `AmiModulePkg/Bds/Bds.c`, `AmiModulePkg/Nvme/NvmeBus.c`

---

*整理自 Codex 会话记录 rollout-2026-03-19T10-20-34 和 rollout-2026-03-19T10-14-34*
