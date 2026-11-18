---
source_type: ChatGPT Page
source_page_id: page_06e489ffecac8191bdd5a88c69eb5825
source_url: https://chatgpt.com/space/page_06e489ffecac8191bdd5a88c69eb5825
source_updated_at: "2026-11-04T08:23:40.443926Z"
synced_on: 2026-11-18
tags:
  - IT8772E
  - COM
  - ACPI
  - WinDbg
---

# IT8772E COM 故障调试知识与 WinDbg 命令解析

[原始 Page](https://chatgpt.com/space/page_06e489ffecac8191bdd5a88c69eb5825) · [[40-调试与项目/故障案例/IT8772E COM 快速启动故障定位与解决记录.md|故障定位与解决记录]]

整理日期：2026-11-04。基于 ITX-FP5C 的 COM 快速启动故障实录。本文用于解释当时的命令与判断方法，不是要求重新执行所有历史步骤。故障已在新 ROM 上验收 PASS。

## 1. 先建立系统链路

```text
BIOS 生成 DSDT/SSDT 与 CPM 数据
        ↓
Windows ACPI 加载命名空间，执行资源/电源方法
        ↓
Serial 驱动使用 I/O 3F8 与 IRQ4
        ↓
UART 发出中断 → I/O APIC GSI4 → CPU Vector → ISR → 完成 IRP
```

这条链中的各层不能互相替代：设备存在不等于方法存在；ACPI 资源正确不等于硬件 RTE 正确；UART 有中断待处理不等于 ISR 已执行；WRITE 返回 pending 不等于最终完成。

| 术语 | 本例含义 |
| --- | --- |
| ASL / AML | ASL 是源码，AML 是 OS 解释器执行的字节码；源码宏在预处理后可能变成 M264 之类短名字 |
| DSDT / SSDT | ACPI 定义表；SSDT 补充命名空间。表被缓存不保证每个 Scope 和 Method 都成功建立 |
| OperationRegion / Field | 描述硬件或内存访问区域以及字段，不是普通 C 全局变量 |
| `_CRS` / `_PRS` / `_SRS` | 当前资源、可能资源、OS 选定资源后的设置方法 |
| `_STA` / `_DIS` | 设备状态、禁用方法；关闭测试程序也可能间接引发设备电源变化 |
| `_PTS` / `_WAK` | 睡眠准备与唤醒控制方法；MPTS/MWAK 是平台内部辅助方法，不是 ACPI 标准保留方法 |
| IRQ / GSI / Vector | 设备中断编号、系统中断输入编号、CPU 中断向量；本例 IRQ4→GSI4→Vector70 |
| ISR / IRP | 中断服务例程、Windows I/O 请求；要绑定同一个请求和设备上下文观察 |

`_WAK` 不是每次冷开机必定执行的方法。ACPI 规范定义其参数为退出的睡眠状态，返回值为包含唤醒状态信息的 Package；S5 开机不能简单等同于 S3/S4 恢复。本例旧现场确实传 0，新现场传 4，应保留实测差异，不能把 0 当方法编号，也不能据此宣布“快速启动固定传 0”。见 [ACPI 系统级控制方法](https://uefi.org/htmlspecs/ACPI_Spec_6_4_html/07_Power_and_Performance_Mgmt/oem-supplied-system-level-control-methods.html)。

## 2. 三个执行位置必须分清

| 位置 | 标志 | 可以做什么 |
| --- | --- | --- |
| 调试主机 PowerShell/CMD | `PS ...>` 等 | 管理文件、计算哈希、编译、git；不能直接执行 WinDbg 扩展 |
| WinDbg 内核命令行 | `0: kd>`、`2: kd>` | `r`、`k`、`ba`、`!amli`、脚本调用 |
| AML 调试器 | `AMLI(? for help)->` | AML 的 `g`、`q`、单步等；不是 WinDbg 命令解释器 |

AMLI 提示符下 `q` 是 Shell to Kernel Debugger；这时内核 `g` 可能先返回 AMLI，随后 AMLI `g` 才继续 AML。内核提示符下的 `q` 不是相同含义，不要混用。官方说明：[基本 AML 调试](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/basic-aml-debugging)。

```text
AMLI(? for help)-> q
0: kd> $$><C:\Users\Public\snapshot.wds
0: kd> g
AMLI(? for help)-> g
```

是否需要最后一步取决于实际提示符。把 `$$><...` 输入 AMLI 曾引出错误请求，不能据此判断固件出现了新的功能故障。

## 3. 日志、暂停原因和调用栈

```text
.logfile
.logopen /t C:\Users\Public\irq4-debug.txt
.time
.lastevent
r rip
k 0n12
```

| 命令 | 解析与用途 |
| --- | --- |
| `.logfile` | 查看是否已有日志打开，避免忘记保存现场 |
| `.logopen /t 文件` | 将后续调试输出写到主机文件；`/t` 自动附加时间等后缀，实际文件名以回显为准 |
| `.logclose` | 结束当前日志；不是结束目标调试 |
| `.time` | 记录调试会话时间和运行时间。主机与目标时间可能不同，需结合事件顺序 |
| `.lastevent` | 看最后事件是异常、断点还是模块加载 |
| `r rip` | 读取当前指令指针 |
| `k 0n12` | 显示最多 12 个栈帧；`0n` 明确十进制。原记录 `k 12` 在默认十六进制下数值是 18 |
| `g` | 继续执行；会改变观察时序，不属于只读快照 |

`nt!DebugService2+0x5` 并不自动意味着崩溃。本案打开 BurnInTest 或加载 WdNisDrv 时有符号加载/调试服务暂停；`DbgLoadImageSymbols → MiDriverLoadSucceeded` 栈说明模块加载事件。DirectIo64 的 CI 策略提示是另一类事件，不等于 `_WAK` 缺名。

`Failed to write breakin packet`、`HOST cannot communicate` 表示 KD 通信问题。先检查 USB KD 重连，不能用失联窗口证明目标没有执行某函数。

## 4. 模块、符号与反汇编

```text
lmvm nt
lmvm ACPI
.reload /s
.reload /f ACPI.sys
x nt!*Interrupt*Restore*
uf nt!HalpInterruptRestoreController
uf ACPI!ACPIDeviceCompleteGenericPhase
```

| 命令 | 参数与结果 |
| --- | --- |
| `lmvm ACPI` | 列指定模块详细信息：地址区间、映像大小、校验和、符号路径；pdb symbols 表示已加载符号 |
| `lm a 地址` | 查询哪个已知模块覆盖该地址；无结果不一定是非法代码，也可能模块列表尚未刷新 |
| `.reload /s` | 重新装载系统符号相关信息；本案刷新后函数指针地址被识别为 pci.sys |
| `.reload /f ACPI.sys` | 立即尝试载入 ACPI 模块符号；不要把 deferred 当作已经失败 |
| `x 模块!通配符` | 搜符号，例如找到 RestoreAllControllerState，而不是猜函数名称 |
| `ln 地址` | 显示最近符号；还要看偏移，不能把“最近”误当精确函数入口 |
| `u 地址 L20` | 从地址反汇编，默认 L20 是十六进制 0x20 条指令 |
| `uf 模块!函数` | 按符号反汇编整个函数，用于分析分支、参数与调用关系 |
| `dq nt!HalAcpiDispatchTable+0x20 L2` | 读取两个 64 位函数指针；再用 ln 查看指向哪里 |
| `ln poi(nt!HalPrivateDispatchTable+0x130)` | 先取指针值，再解析目标符号；本例识别到 StopLegacyUsbInterrupts |

指针偏移和函数布局属于当前内核版本的逆向结果，不是稳定公开接口。离线下载的驱动若校验和不同，只能提供线索，必须与目标机 `uf` 核对。`Unable to load image ... Win32 error 2` 也不能单独证明该驱动未加载。

## 5. 断点类型、状态和限制

```text
bl
ba e 1 nt!HalpInterruptRestoreAllControllerState
bl
```

| 命令 | 解释 |
| --- | --- |
| `bl` | 列原生断点。条目中的 e=enabled、d=disabled；旁边 Disable/Clear 是界面动作链接 |
| `bp 地址` | 软件断点，通常通过改写指令字节实现 |
| `bu 模块!符号` | 可延期解析的符号软件断点；并不是硬件断点 |
| `ba e 1 符号` | 硬件执行断点；e 表示 execute，执行断点大小为 1 |
| `ba0 e 1 符号` | 指定断点编号 0，编号紧接 ba |
| `ba w 4 地址` | 监视目标对 4 字节区域的写入，适合 WorkDone 字段；地址需满足对齐等限制 |
| `ba w 1 地址` | 监视 1 字节标志，例如 HalpInterruptPicStateIntact |
| `bd 0` / `be 0` | 禁用/启用原生断点 0 |
| `bc 0` | 删除原生断点 0；删除前先看 bl，避免清掉不属于本次的断点 |
| `gc` | 在断点命令里按原来的执行方式继续，适合条件不匹配时放行 |

x64 调试寄存器槽位有限，本案使用少量硬件断点组合。曾在恢复期间使用较多 HAL 软件断点后出现独立 0x109 问题，不能将该调试干扰当作 COM 原故障。硬件断点也不是任何暂停位置都能成功设置：`SetContext failed` 时，bl 中显示 enabled 不能证明目标已成功装载调试寄存器。参考 [ba 命令](https://learn.microsoft.com/en-us/windows-hardware/drivers/debuggercmds/ba--break-on-access-) 与 [0x109 说明](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/bug-check-0x109---critical-structure-corruption)。

跨关机/恢复后要重新 `bl` 核验。本案实际出现过断点列表清空；不要承诺硬件或软件断点必定跨电源转换保留。

### 5.1 为什么要按请求地址筛选

以下是**语法示例**，不是可以在未知现场直接运行的脚本；必须先从当前 AML Context 获得并验证 `$t7`，且确认断点编号可用。

```text
ba0 e 1 ACPI!ACPIDeviceCompleteGenericPhase ".if (@r9 == @$t7) { .echo MATCHING WAK; r edx; r r9; k 8 } .else { gc }"
```

`@r9 == @$t7` 只捕获同一个电源请求。Windows x64 的前四个整数/指针参数通常由 RCX、RDX、R8、R9 传入；本函数的 EDX 为状态、R9 为请求上下文，是结合当前反汇编确认的，不能推广成所有函数的固定含义。

```text
r @$t6 = @edx
```

这是保存观测值到调试器伪寄存器。相反，`r edx=0` 会修改目标 CPU 状态；旧 ROM 的一次因果实验用了它，新 ROM 验收绝不能用它冒充自然成功。

## 6. 内存读取、进制与链表

| 写法 | 含义 |
| --- | --- |
| `db 地址 L10` | 读 0x10 个 byte，共 16 字节 |
| `dd 地址 Le` | 读 0xE 个 DWORD，共 56 字节 |
| `dq 地址 L2` | 读两个 QWORD，共 16 字节 |
| `poi(地址)` | 按目标指针宽度解引用；本例 x64 为 8 字节 |
| `by(地址)` / `dwo(地址)` | 在 MASM 表达式里读取 byte / DWORD |
| `r @$t0 = poi(符号)` | 把读到的值存到调试器临时伪寄存器 t0 |
| `? 表达式` | 计算表达式；注意默认进制和当前表达式解释器 |
| `@rcx` | 表达式中的 CPU 寄存器；`@$t0` 是伪寄存器 |

数字通常按当前 radix 解析。用 `0x` 明确十六进制，用 `0n` 明确十进制；L 后是元素数，不是普遍的字节数。寄存器转储中的反引号只用于分隔 64 位地址。

当时读取 HAL 保存数据：

```text
r @$t0 = poi(nt!HalpRegisteredInterruptControllers)
dq @$t0 L4
dq @$t0+0x108 L2
```

本版本 controller+0x108 是链表头；节点 +0x14/+0x18 是区间界限，+0x28 指向每项 0x38 字节的保存状态，+0x30 指向每项 0x10 字节的辅助状态。以包含 GSI4 的区间计算对应项，不能假定链表第一个节点就是 IRQ0 起始区间。早期负编号节点是为什么需要遍历而不是直接偏移的原因。

有界遍历示例，前提是 t0/t1 已在当前会话正确建立：

```text
r @$t2 = poi(@$t1)
.for (r @$t3 = 0; @$t3 < 0x10; r @$t3 = @$t3 + 1) { .if (@$t2 == (@$t0+0x108)) { .break }; dq @$t2 L7; r @$t2 = poi(@$t2) }
```

`t2` 从下一个节点开始；到链表头退出，并额外限制最多 16 次，防止损坏链表无限循环。当时将 `&&` 放入该 MASM 条件得到 Numeric expression missing，改成嵌套 `.if` 判断；不要直接把 C 语言条件表达式搬进调试器。

## 7. I/O APIC、虚拟内存与物理内存

```text
!ioapic
db nt!HalpInterruptPicStateIntact L1
db nt!HalpResumeFromHibernate L1
```

`!ioapic` 读取控制器路由，关注正确控制器的 Inti04。输出行末 `m` 表示 masked。RTE 低 DWORD 的 bits7:0 是 Vector、bit16 是 mask；本例故障低值 000100FF 是 VectorFF 且被屏蔽，正常低值 00000970 是 Vector70 且未屏蔽。正常值还包含投递模式等信息，不能只改 mask 位就假定路由正确。

```text
!pte ffffd8800eb07018
db ffffd8800eb07018 L20
!db dd88709f L1
```

以上是**旧现场示例地址**。`!pte` 检查虚拟地址映射；普通 db/dd/dq 读虚拟地址；扩展 `!db` 读物理内存。CPNV 在 AMLI 中显示的地址经检查可作为映射后的虚拟地址读取，而 M128 字段值 dd887038 是后续被 AML 用作 SystemMemory 区域的地址。两层不能混淆。

`M049(M128,0x67)` 经 `M011` 建立临时 OperationRegion 读取一个字节，因此在确认物理指针后读取主表 +0x67，验证分支条件。表头 `$CPM/$A25/$A27` 用于核对指针指向的内容。不是看到一个像地址的数就可以任意用 `!db` 扫描。

## 8. AMLI 查看命名空间和执行现场

```text
!amli dns \_WAK
!amli u \_WAK
!amli dns \MWAK
!amli u \MWAK
!amli dns \_SB.M264
!amli lc
!amli r
```

| 命令 | 说明 |
| --- | --- |
| `!amli dns 路径` | 查询命名空间对象：Method、FieldUnit、Package、Device 等；对象类型决定后续解释 |
| `!amli dns /s 路径` | 递归列命名空间，输出可能很大；优先查精确路径 |
| `!amli dns 地址` | 按已知命名空间对象地址查询，例如找到 Field 的父区域 |
| `!amli u 路径` | 反汇编 AML 方法，查看调用和条件分支 |
| `!amli ln 地址` | 将 AML 代码地址定位到方法及偏移 |
| `!amli lc` | 列 AML Context；桌面空闲没有活动上下文不代表 ACPI 错误 |
| `!amli r` | 显示当前 AML Context、Arg、Local、RetObj、下一条 AML 等 |
| `!amli r 上下文地址` | 查看指定上下文；本例按扩展接受的裸十六进制地址输入，不加 0x 前缀 |

`AMLI_DBGERR: object ... not found` 若来自主动 dns 查询，只证明该次查询不存在；运行期间失败还需结合错误上下文与回调。`Failed to find symbol of the address` 也可能是传入对象类型或地址不适合解析，不能直接当运行故障。

命名空间查询的递归选项与地址形式见 [!amli dns 官方说明](https://learn.microsoft.com/en-us/windows-hardware/drivers/debuggercmds/-amli-dns)。

失败后的 `pbOp` 指向 MWAK 调用之后，不证明后面的 ShiftLeft/SWAK 已执行：解释器可能已推进指令指针再因子调用失败回退。要看 Context 状态、RetObj、错误跟踪和完成码。

### 8.1 AML 断点和输出开关

```text
!amli bp \_WAK
!amli bl
!amli set dbgbrkon
```

| 命令 | 用途 |
| --- | --- |
| `!amli bp \_WAK` | 在 AML 方法设置断点，捕获实际调用参数 |
| `!amli bl` | 列 AML 断点；编号与 WinDbg 的 bl/ba 编号是两个体系 |
| `!amli bd 0` / `!amli be 0` | 禁用/启用 AML 断点 0 |
| `!amli bc 0` | 清除 AML 断点 0，不是原生 bc 0 |
| `!amli set` | 查看 AML 调试选项 |
| `dbgbrkon` / `dbgbrkoff` | 开关 AML 调试器进入机制；结合 AML 断点使用 |
| `errbrkon` / `errbrkoff` | 开关遇 AML 错误时进入调试器 |
| `spewon` / `spewoff` | 开关 AML 调试输出 |
| `verboseon` / `verboseoff` | 增加方法等详细输出 |
| `traceon` / `traceoff` | 开关解释执行跟踪，输出量很大 |
| `nesttraceon` / `nesttraceoff` | 控制嵌套方法跟踪，与 trace 配合 |
| `logon` / `logoff` | 控制解释器事件日志；不同于 .logopen 的主机文本日志 |

各开关通过 `!amli set 选项` 使用，结束后恢复原设置，不要长期保留全量跟踪。详见 [!amli set](https://learn.microsoft.com/en-us/windows-hardware/drivers/debuggercmds/-amli-set)。目标请求执行的 `!amli err 0x78 ...;g` 中 0x78 是 AML 调试器错误分类，不是 NTSTATUS C0000034；不能混用编号体系。未知错误码应查对应说明和上下文，不能见数字就猜缺失对象。

## 9. WAK 的三种状态不要混淆

| 状态 | 本例观测 | 意义 |
| --- | --- | --- |
| AML Arg0 | 旧 0，新 4 | 调用参数，不是成功码 |
| AML RetObj | 新 ROM Package(0,0) | 方法正常返回的数据 |
| ACPI 回调 EDX | 旧 C0000034，新 0 | 解释执行/异步完成的 NTSTATUS |
| 请求 WorkDone/Status | 旧 1→2/C0000034，新 1→4/0 | 当前 Windows 内部请求状态机的结果 |
| HAL 状态与 RTE | 标志 0→1，IRQ4 未屏蔽 | 后续中断恢复完成的证据 |

`Return(WAKP)` 的 Package 不是 `EDX` 的替代物。找不到对象时可能没有执行到 Return。对当前请求结构 +0xD0、+0xD4、+0x100 的解释来自本次反汇编，不是跨 Windows 版本稳定 ABI。

一个有效抓取策略是：AML 入口暂停 → 得到 CallBackContext → 在回调按 R9 匹配 → 写监视请求 WorkDone → 写监视 HAL 状态标志 → 读取硬件路由 → 释放后功能测试。阶段结束及时删除请求地址断点，避免对象释放复用后误触发。

## 10. ACPI 表导出与离线核对

```text
!acpicache 1
.writemem C:\Users\Public\irq4-ssdt-probe-20261104.aml ffffa48114fef018 L5419
```

第二行仅是旧现场成功命令示例，地址和长度须来自当前缓存表。`.writemem` 把目标虚拟内存写成**调试主机文件**，L5419 是 0x5419 字节，不是 5419 十进制；没有修改目标内存。导出后核对 ACPI Header Length 与整表 8 位校验和，再离线反编译。

早先批量导出出现 Unable to create file，随后简单公共目录路径成功；这证明当时创建文件失败，不证明目标表损坏，也不能概括为所有带引号路径均无效。

PowerShell 中离线工作示例：

```powershell
& 'F:\Aptio_5.x_TOOLS_46\Aptio_5.x_TOOLS_46\BuildTools\iasl6.exe' -e Common.aml -d OtherHotplug.aml
Get-FileHash -Algorithm SHA256 'F:\1AVSZ015\IXFP5C06.rom'
```

`-e` 提供外部表供方法原型解析，`-d` 反编译；文件名需替换成实际导出件。多个表一起 -da 出现 AE_ALREADY_EXISTS，可能是离线组合/外部对象问题，不能直接当成目标 Windows 的加载错误。旧 iASL5 不能直接代替 C 预处理器解析项目宏，必须按构建方式预处理完整 Common，再编译，不是仅编译 Function.asi 片段。

## 11. 串口、设备和端口命令

| 命令模式 | 用途与判断限制 |
| --- | --- |
| `!devnode` | 检查 PnP 设备树和资源相关上下文；先对应 PNP0501 的完整实例 |
| `!devstack 设备对象地址` | 查看设备栈，区分 ACPI PDO 与 serial FDO |
| `!devobj 设备对象地址` | 看设备对象及扩展等信息 |
| `!irp IRP地址` | 看当前栈位置、MajorFunction、完成状态，避免把 IOCTL 当 WRITE |
| `dt nt!_IRP 地址` | 按符号类型查看 IRP 字段；结构必须与当前模块符号匹配 |
| `dt nt!_KINTERRUPT 地址` | 查看 Vector、Connected、ISR 等；软件对象不保证硬件路由有效 |
| `ib 端口` | 读一个 I/O byte；读 UART 某些寄存器有副作用 |
| `ob 端口 值` | 写 I/O byte，会修改硬件；不要视为无扰动快照 |

SIO Index=2Eh、Data=2Fh。历史采用的 IT8772E 配置模式进入序列是向 2Eh 依次写 87h、01h、55h、55h；退出是 `ob 0x2e 0x02` 后 `ob 0x2f 0x02`，不能把两条都写到 2Eh。需要选择 LDN1 时先保存原 LDN，结束恢复。若停在 AML 已进入配置模式并持锁的位置，不要再随意插入进入/退出操作。

UART 寄存器：base+1 为 IER（DLAB=0 时）、+2 为 IIR、+3 为 LCR、+4 为 MCR、+5 为 LSR。读取 IIR 可能应答 THRE，读取 LSR 可能清错误状态，读取 RBR 会消耗数据。因此连续扫描端口会改变现场；先记录时序和取样目的。

## 12. 试过但没有提供有效证据的命令

```text
.load rcdrkd.dll
!rcdrkd.rcdrloglist ACPI
!rcdrkd.rcdrlogdump ACPI
```

`.load` 加载调试器扩展，后两条尝试列出和转储驱动 recorder 日志。本案返回找不到日志头、无 autologger 支持、Empty log，因此改用实际断点/AML trace。不能将空日志写成“ACPI 没有出错”。

如果出现真正 bugcheck，可使用 `.bugcheck`、`!analyze -v` 并核对栈；本案 0x109 调试干扰与原始 COM 失败分开记录，避免追错问题。

## 13. 脚本组织和执行约定

用户要求超过两步的调试操作放入 `.wds`，用一条命令执行：

```text
$$><C:\Users\Public\irq4-acpi-stage27-newrom-verify-20261104.wds
```

`$$><` 将脚本作为命令块读取并执行，行分隔会参与命令块处理，注意 `.if/.for` 与引号中的分号。`$><` 也是脚本执行形式，解析差异见 [WinDbg 脚本文件](https://learn.microsoft.com/en-us/windows-hardware/drivers/debuggercmds/using-script-files)。本次统一用经现场验证的 `$$><`。

设计脚本应做到：开始/结束 `.echo` 标记；开头检查日志和上下文；把只读快照、布置断点、修改状态分开；是否包含 g 在结尾明确；退出时只清理自己设置的断点。涉及结构地址的脚本必须先验证当前请求类型、状态和地址，不能从历史日志复制旧指针。

可重复使用的轻量只读快照内容：

```text
.echo ===== IRQ4 READ ONLY SNAPSHOT BEGIN =====
.logfile
.lastevent
bl
!amli bl
!amli lc
db nt!HalpInterruptPicStateIntact L1
db nt!HalpResumeFromHibernate L1
!ioapic
.echo ===== END - TARGET REMAINS PAUSED =====
```

前提是目标已连接暂停、当前内核有相应符号。它不执行 AML 方法，不修改 CPU 状态，也不自动继续；`!ioapic` 会读取控制器。不要把该快照的正常结果单独当成 `_WAK` 全流程成功，仍需对应时序证据。

### 13.1 本次脚本阶段索引

| 阶段 | 解决的问题 |
| --- | --- |
| stage1–3 | ACPI/HAL 调用线索、完成路径与 IFR 可用性 |
| stage4–8 | WAK 入口、上下文、恢复断点、匹配回调；首次抓到 C0000034 |
| stage9–13 | 失败后上下文、MWAK 名称与依赖、CPNV 字段、CPM 物理表内容 |
| stage14/14b–15 | 导出缓存表、检查父 Scope 与同表其他对象 |
| stage16–18 | 释放调试、Setup/命名空间审计及实验准备；以各脚本头部实际操作为准 |
| stage19–22 | 受控一次成功状态实验，验证 IRQ4 与 COM 能恢复 |
| stage23–26 | 再次复现并跟踪缺失依赖，确定首个错误和保护范围 |
| stage27–30 | 新 ROM 实时 AML、自然回调成功、HAL 自动恢复、清理断点与最终测试 |

历史文件保存在调试主机 `C:\Users\Public\irq4-acpi-stage*.wds`。编号表示调查顺序，不是所有脚本都必须串行重跑；尤其旧请求地址、写 EDX 的实验脚本不能用于新 ROM 验收。

## 14. 下次排查可以复用的判断原则

1. 先固定最小复现路径，再做正常/异常对照。

2. 把设备实例、请求地址、回调上下文绑定起来；名称相同和时间接近都不够。

3. 同时观察配置、软件状态、硬件寄存器和实际功能；四者可能不一致。

4. 缺名错误先定位完整路径，再核对表加载和分支，不按最后一条 Debug 字符串猜根因。

5. 用最小干预实验验证因果，随后撤掉干预验证正式修复。

6. `CondRefOf` 只防止调用不存在的对象，不捕获方法内部错误；不要把局部 workaround 描述成通用容错机制。

7. 最终验收同时需要自然成功回调、正确硬件路由和串口 PASS；没有调试器强制成功才算本次修复证据。

## 关联提交与本地归档

[COM 修复提交 10bf489](http://127.0.0.1:3000/gitea/1AVSZ015/commit/10bf48994f06f08fe9e5eb83b5e0e05df9ce8b53)；[版本固定的命令手册源码](http://127.0.0.1:3000/gitea/1AVSZ015/src/commit/10bf48994f06f08fe9e5eb83b5e0e05df9ce8b53/docs/debug/2026-11-04-windbg-amli-command-guide.md)。Gitea 地址为本地服务，需在可访问该服务的环境打开。
