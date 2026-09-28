## 相关文件
- 日志
  - [修复前 log](logs/Mon-15-25-40-before-fix.log)
  - [修复后 log](logs/Mon-15-49-13-after-fix.log)
- 代码摘录
  - [DDI2 配置](snippets/AmdCpmOemTable-DDI2-Config.c)
  - [USB_C_TYPE 修复点](snippets/GfxEnumConnectors-USB_C_TYPE-Fix.c)
  - [Mapping 未传递问题](snippets/GfxEnumConnectors-GfxMappingUserConfig-OpenIssue.c)

## 问题现象
- 关机状态下同时插上 HDMI 和 DP，开机进 Win11 后两个显示器都正常。
- 进入系统后拔掉 DP，Windows 显示设置里仍保留 2 个显示器，没有变成只剩 HDMI。
- 再插回 DP，DP 显示器不自动亮。
- 但只要在 Windows 里手动修改 DP 显示器刷新率，DP 又能恢复显示。

## 初步判断
- 静态显示链路是通的，否则冷插开机不会正常显示。
- 问题更像是运行态 hotplug 事件或 driver 对该口的处理逻辑异常。
- 手动改刷新率能恢复，说明手动触发了一次 modeset / retrain。

## 实际 DDI 配置
平台在 CPM override 中把 DDI2 配成了普通 DP 口，并指定了 Aux3 / Hdp3 与 lane swap 0xB4：

来源：
- `F:\FP5IA001\FP5IA001\MandolinPkg\CrbAmdCpmOemTableOverride\AmdCpmOemTable.c`
- 见摘录 [DDI2 配置](snippets/AmdCpmOemTable-DDI2-Config.c)

关键点：
- `ConnectorTypeDP`
- `Aux3`
- `Hdp3`
- `Map0 = 0xB4`

## 修复前证据
修复前 log 里能看到两层信息：

1. CPM override 层
- 搜索 `DDI-DUMP[TableOverride-AfterGetTablePtr] DDI2`
- 可看到 `Type=0x00 Aux=2 Hdp=2 Map0=0xB4`
- 这里的 `Aux/Hdp` 是 0-based，所以 `2` 对应代码里的 `Aux3/Hdp3`

2. AGESA 枚举层
- 搜索 `DDI[2] RawCfg`
- 可看到 `Map0=0x0`
- 搜索 `StoreDisplayPathList usCaps: 100` 或 `usCaps = 0x100`
- 说明 DDI2 最终被打上了 `ATOM_ENCODER_CAP_RECORD_USB_C_TYPE`

结论：
- CPM 表里的 DDI2 静态配置是对的
- 但 AGESA 最终把 DDI2 标成了 `USB_C_TYPE`

## 根因
`AgesaModulePkg\Nbio\GFX\AmdNbioGfxRVPei\GfxEnumConnectors.c` 中原逻辑会对 `DdiCounter == 2 || DdiCounter == 3` 强制添加：

- `ATOM_ENCODER_CAP_RECORD_USB_C_TYPE (0x100)`

这导致实际为普通 DP 的 DDI2 在 Win11 / AMD driver 侧被按 USB-C 特殊口处理，运行态 hotplug 流程异常。

来源：
- 见摘录 [USB_C_TYPE 修复点](snippets/GfxEnumConnectors-USB_C_TYPE-Fix.c)

## 最小修复
只做一刀：

- 原逻辑：`if (DdiCounter == 2 || DdiCounter == 3)`
- 修复后：`if (DdiCounter == 3)`

也就是：
- 不再给 DDI2 强制加 `USB_C_TYPE`
- 只保留 DDI3

## 修复后验证
修复后 log：
- 搜索 `DDI[2] RawCfg`
- 搜索 `StoreDisplayPathList usCaps`
- 搜索 `usCaps = 0x0`

修复后现象：
- DDI2 的 `usCaps` 从 `0x100` 变成 `0x0`
- Win11 下 DP 热插拔恢复正常

## 额外发现的次要问题
还有一个独立问题，但不是本次 hotplug 恢复的决定性因素：

- CPM 表里 `DDI2 Map0 = 0xB4`
- 但 AGESA 枚举时 `DDI[2] RawCfg` 仍然是 `Map0 = 0x0`

原因在：
- `GfxMappingUserConfig()` 只拷了
  - `ConnectorType`
  - `AuxIndex`
  - `HdpIndex`
  - `LanePnInversionMask`
  - `Flags`
- 没有拷 `Mapping[0] / Mapping[1]`

来源：
- 见摘录 [Mapping 未传递问题](snippets/GfxEnumConnectors-GfxMappingUserConfig-OpenIssue.c)

结论：
- 这是一个明确存在的代码缺项
- 但它不是这次热插拔恢复的主因，因为即使它还没修，去掉 DDI2 的 `USB_C_TYPE` 后热插拔已经恢复

## 最终结论
本次 DP 热插拔失效的主因是：

**DDI2 实际为普通 DP 口，但 AGESA 将其错误标记为 `USB_C_TYPE`，导致 Win11/AMD driver 运行态 hotplug 处理异常。**

最小修复是：

**不要对 DDI2 强制添加 `ATOM_ENCODER_CAP_RECORD_USB_C_TYPE`。**

## 可直接复用的记录句子
- 现象不是“DP 口不能显示”，而是“OS 运行态 DP hotplug 状态不更新”。
- 冷插正常、改刷新率后恢复，说明静态链路正常，但运行态事件/重训练流程异常。
- 通过 log 确认 DDI2 的静态配置正确，主因是 AGESA 错误给 DDI2 加了 `USB_C_TYPE`。
- 去掉 DDI2 的 `USB_C_TYPE` 标记后，Win11 下 DP 热插拔恢复正常。

