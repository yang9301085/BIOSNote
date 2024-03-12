# PEI 阶段流程概览
![[Pasted image 20240306212552.png]]
# PEI Foundation
主要完成一下工作：
- 调用每个PEIM
- 维护Boot Mode
- 初始化内存
- 调用DXE loader
# Pre-EFI Initialization Modules(PEIM)
- PEI 阶段所要运行的特殊驱动
# PEI Dispatcher(PEI 调度器)
- 主要在各个PEIM之间做调度
# PEIM-to-PEIM Interface (PPI)
- PEI Dispatcher 在各个PEIM之间调度所要用到的工具，本质上是一个结构体
- 主要包含以下PPI：
	- CPU I/O PPI
	- PCI Configuration PPI
	- Stall PPI
	- PEI Variable PPI
	- SEC platform Information PPI 
# Firmware Volumes(FV)
  -  PEIM存在FV中，PEI阶段将在FV中找到这些PEIM供PEI Foundation使用
# PEI Services Table
- PEI Foundation 会在系统中建立PEI Service Table给PEIM使用
- 因为在编译阶段PEI Foundation和临时内存都是未知的，所以每个PEIM 的entry point和PPI都会传入PEI Services Table这个参数
## PEI Services
- PEI Service 在PEI 阶段建立并在PEI之后的阶段(DXE、BDS .etc)仍然可用
- 主要有以下功能
	- 管理Boot mode
	- 管理内存
	- 管理FFS
	- 管理PPI Database
	- 建立Hand-Off Block(HOB)
![[Pasted image 20240306223051.png]]


# PEI 阶段内存管理
- 当PEI install完EFI_PEI_PERMANENT_MEMORY_INSTALLED_PPI之后，PEI开始使用物理内存
- 当PEI初始化并且测试完物理内存后，PEI将使用InstallPeiMemory( )去使用内存，从而PEI Foundation将初始化HOB
# PEI to DXE
- PEI Foundation 使用DXE Initial Program Load(IPL) PPI 调用 DXE Foundation
- DXE IPL PPI 传递HOB list给到DXE foundation 
## HOB(Hand-Off Block)
HOB Type：
![[Pasted image 20240306230619.png]]





  