# BIOS_PEI_DXE_BDS初始化链路

**来源**: 笔录 2026-03-06《BIOS 项目结构 / S3 代码流程》

#BIOS #PEI #DXE #BDS #HOB #FSP

---

# PEI → DXE → BDS 初始化链路

## 核心知识点

### SEC 阶段（Reset Vector → CAR）
- CPU 从 0xFFFFFFF0（Reset Vector）开始执行
- 运行 MicroCode + BIST 自检
- 创建临时内存存储 CAR（Cache As RAM）
- 创建系统可信根
- 调用 PEI Foundation Entry Point

### PEI 阶段（Pre-EFI Initialization）
- PEI Foundation 调度各 PEIM（PEI Module）
- 维护 Boot Mode（正常启动 / S3 Resume / Recovery）
- FSP-M 在 PEI 阶段初始化内存
- 安装 `EFI_PEI_PERMANENT_MEMORY_INSTALLED_PPI` 后开始使用物理内存
- HOB List：PEI → DXE 的信息传递载体

### DXE 阶段（Driver Execution Environment）
- PEI 通过 DXE IPL PPI 调用 DXE Foundation
- DXE 接收 HOB List
- 大量 DXE Driver 并行加载，按依赖顺序枚举总线和设备

### BDS 阶段（Boot Device Selection）
- 选择启动设备，加载启动选项
- 首启时若 runtime memory type 配额变化会触发自动 EfiResetCold

### 首启 Memory Type Information 自动 Reset
- 触发条件：IsFirstBoot == TRUE 且 `PcdResetOnMemoryTypeInformationChange == TRUE`
- BDS SaveMemoryTypeInformation() 检测配额变化
- 若变化则置 ResetRequired = TRUE，发 EfiResetCold
- **这是正常行为，不是 bug**：让新内存配额在下一轮生效

