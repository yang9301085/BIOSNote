# BIOS_ColdReset与WarmReset区别

**来源**: 笔录 2026-03-16/2026-03-17《Cold Reset 与 Warm Reset 区别》

#BIOS #Reset #ColdReset #WarmReset #PCH #CF9

---

# Cold Reset vs Warm Reset（Intel PCH CF9 寄存器）

## 核心知识点

### CF9 寄存器定义
| 名称 | 值 | 作用 |
|---|---|---|
| SOFTSTARTSTATE | 0x00 | 选择软启动状态（本身不触发复位）|
| HARDSTARTSTATE | 0x02 | 选择硬启动状态 |
| HARDRESET | 0x06 | 触发硬复位（0x00 → 0x06）|
| FULLRESET | 0x0E | 触发全复位（0x02 → 0x0E，含 power cycle）|

### Warm Reset（0x00 → 0x06）
- 从 SOFTSTARTSTATE 开始，跳过部分平台初始化
- 部分电源/时钟状态保留
- OS 层面重启（Ctrl+Alt+Del、Windows 重启）走此路径

### Cold Reset（0x02 → 0x0E）
- 从 HARDSTARTSTATE 开始，清更多状态
- 不等同于 G3 掉电上电，但重置更多电源域
- NVMe 在 Cold Reset 后找不到但 Warm Reset 后能找到的原因：
  - Cold Reset 后 Root Port LTSSM Training 需要更长时间完成
  - Warm Reset 保留了更多链路状态

### 代码位置
- Reset 实现：`AmiChipsetModulePkg/Library/AmiSbMiscLib/AmiSbMiscLib.c:226-242`
- CF9 定义：`Intel/ClientOneSiliconPkg/Pch/Include/Register/PchRegsLpc.h:276-284`

