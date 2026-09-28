# BIOS_DDR时序与SPD

**来源**: 笔录 2026-03-17《BIOS DDR 时序》

#BIOS #DDR #SPD #Memory #时序

---

# DDR 内存时序与 SPD

## 核心知识点

### DDR4 时序参数（常见格式：8-8-8-24）
| 参数 | 全称 | 含义 |
|---|---|---|
| tCL | CAS Latency | 收到列地址到数据输出的延迟，最重要 |
| tRCD | RAS to CAS Delay | 行地址到列地址的延迟 |
| tRP | Row Precharge Timing | 行预充电时间 |
| tRAS | Min RAS Active | 最小行激活时间 |

### 为什么时序按 tCK 计算
大部分 DDR4 时序参数的实际值 = f(tCK)，SPD 存的是基准值，由 tCK 计算得出。

### SPD 时间基准
- **MTB（Medium Timebase）**：中等粒度基准
- **FTB（Fine Timebase）**：细粒度基准

计算公式：
```
tXX = tXX(MTB) * MTB + tXX(FTB) * FTB
```

### 首启 Memory Type Information Reset
当 runtime memory type 配额变化时：
- 若 `PcdResetOnMemoryTypeInformationChange == TRUE`
- BDS 在首启时强制发 EfiResetCold，让新配额在下一轮生效
- 这是 Intel 推荐的行为，不是 bug

