# BIOS 缩短首次开机时间

**来源**: 笔录 2025-12《BIOS 首启优化》系列

#BIOS #性能优化 #启动时间 #PEI #BDS

---

## 核心知识点

### 首次开机时间长的原因

首次开机（First Boot）比后续开机慢的根本原因：
1. **内存训练** — 首次需要完整内存初始化（ MRC Full Training）
2. **NVRAM 初始化** — `PreviousMemoryTypeInformation` 不存在，触发 BDS Reset
3. **设备枚举** — PCI/PCIe 首次全扫描
4. **固件卷缓存** — FSP/PEI 数据未缓存

### 内存训练优化（最大头）

**MRC (Memory Reference Code) 训练级别**：
| 级别 | 耗时 | 适用场景 |
|---|---|---|
| Full Training | ~2-5s | 首次开机，冷启动 |
| Self-Refresh | ~200ms | 快速唤醒 |
| L1/L2 Cache Training | ~500ms | 折中方案 |

**优化手段**：
- 确认 `MrcL1TrainingEnable` / `MrcL2TrainingEnable` 策略
- 对非关键平台禁用完整训练
- 使用 `Fast Boot` 策略（利用 NVRAM 保存的训练结果）

### BDS Reset 避免

首启自动 Reset 机制（之前笔记有详细记录）会导致"白跑一遍"。

**跳过首启 Reset 的方法**：
1. 在制造环节预先写入 `PreviousMemoryTypeInformation` NVRAM 变量
2. 使用 `MemoryTypeInformationDisable = FALSE`（保持默认值）
3. 确保 `PcdResetOnMemoryTypeInformationChange = FALSE`

### PEI 阶段优化

- **禁用不必要的 PEIM**：检查 `PEI_CORE_ENTRY_LIST`，移除不用的驱动
- **减少 HOB 创建**：HOB 越小，后续 DXE 加载越快
- **FSP-M 优化**：选择更快的 MemoryInit 策略

### DXE 阶段优化

- **延迟加载驱动**：使用 `Dependency` 表达式让驱动按需加载
- **移除无用 Protocol**：检查 `ProtocolNotify` 回调，删除未触发的
- **SMM 优化**：减少 SMM 入口代码量

### 测量工具

**TianoCore Debug Log**：
```c
// 在 PEI 开始处加时戳
DEBUG ((EFI_D_INFO, "PEI Entry: %d ms\n", GetTimeMs()));
```
或用 Intel **Boot Duration Tool** 抓各阶段时间。

---

## 来源笔录

- 2025-12 系列《BIOS-首启优化》会话
