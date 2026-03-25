# AMI StatusCode 串口输出链路

## 结论

`Sprintf_s()` 本身不会把字符串直接输出到串口。

它的职责只是把格式化后的 ASCII 文本写入调用者提供的内存缓冲区。真正负责把字符串发到串口的是：

`AmiReportStatusCode()` -> `StringStatusReport()` -> `SerialOutput()` -> `SerialPortWrite()`

## 关键调用链

### 1. 生成字符串

在 `AmiModulePkg/AmiStatusCode/StatusCodeCommon.c` 中，`CreateString()` 会根据不同的 Status Code 数据类型生成字符串。

- `ReportData()` 使用 `Sprintf_s()` 生成错误文本
- `ReportAssertString()` 使用 `Sprintf_s()` 生成 ASSERT 文本
- `ReportDebugInfo()` 使用 `AsciiBSPrint()` 生成调试文本

示例：

- `StatusCodeCommon.c:150`
- `StatusCodeCommon.c:122`

这里生成的内容都只是写入 `String` 缓冲区，还没有发生串口输出。

### 2. 进入统一状态码上报入口

不同阶段最终都会进入 `AmiReportStatusCode()`：

- PEI: `AmiModulePkg/AmiStatusCode/StatusCodePei.c:367`
- DXE/RT: `AmiModulePkg/AmiStatusCode/StatusCodeDxe.c:609`
- SMM: `AmiModulePkg/AmiStatusCode/StatusCodeSmm.c:311`

`AmiReportStatusCode()` 的关键逻辑在 `AmiModulePkg/AmiStatusCode/StatusCodeCommon.c:678` 附近：

1. 调用 `CreateString(Type, Value, Data, String)`
2. 如果 `String[0] != '\0'`
3. 调用 `StringStatusReport(PeiServices, (CHAR8*)String)`

也就是：只有先被格式化成字符串，后续字符串类处理器才会接收到它。

### 3. 字符串处理器分发

`StringStatusReport()` 位于：

- `AmiModulePkg/AmiStatusCode/StatusCodeCommon.c:538`

它的实现非常直接：

```c
for (i=0; StringList[i]; i++) StringList[i](PeiServices, String);
```

这说明它会遍历 `StringList[]` 中注册的所有字符串处理函数。

### 4. `SerialOutput` 被注册为字符串处理器

`SerialOutput` 不是被 `CreateString()` 直接调用的，而是通过 eLink 注册到字符串处理链中的。

注册位置在：

- `AmiModulePkg/AmiStatusCode/StatusCodeAmi.sdl:433`
- `AmiModulePkg/AmiStatusCode/StatusCodeAmi.sdl:434`
- `AmiModulePkg/AmiStatusCode/StatusCodeAmi.sdl:441`
- `AmiModulePkg/AmiStatusCode/StatusCodeAmi.sdl:442`
- `AmiModulePkg/AmiStatusCode/StatusCodeAmi.sdl:449`
- `AmiModulePkg/AmiStatusCode/StatusCodeAmi.sdl:450`
- `AmiModulePkg/AmiStatusCode/StatusCodeAmi.sdl:457`
- `AmiModulePkg/AmiStatusCode/StatusCodeAmi.sdl:458`

含义是：

- PEI 字符串状态码处理链注册 `SerialOutput`
- DXE 字符串状态码处理链注册 `SerialOutput`
- RT 字符串状态码处理链注册 `SerialOutput`
- SMM 字符串状态码处理链注册 `SerialOutput`

前提条件是：

`SERIAL_STATUS_SUPPORT = 1`

### 5. 真正写串口的地方

`SerialOutput()` 位于：

- `AmiModulePkg/AmiStatusCode/StatusCodeCommon.c:428`

它会先扫描字符串内容，再调用 `SerialPortWrite()`。

关键行为：

1. 把输入的 `CHAR8 *String` 当成字节流处理
2. 遇到单独的 `\n` 时，先输出前面的内容
3. 把 `\n` 转换为 `\r\n`
4. 最终通过 `SerialPortWrite()` 发送

关键代码位置：

- `StatusCodeCommon.c:449`
- `StatusCodeCommon.c:450`
- `StatusCodeCommon.c:458`

所以，真正发生硬件串口写入的是 `SerialPortWrite()`，而不是 `Sprintf_s()`。

## 串口初始化

串口初始化在 `SerialStatusInit()` 中完成：

- `AmiModulePkg/AmiStatusCode/StatusCodeCommon.c:475`
- `AmiModulePkg/AmiStatusCode/StatusCodeCommon.c:480`

其中调用了：

`SerialPortInitialize()`

这表示状态码串口输出功能依赖底层 `SerialPortLib` 实例完成 UART 初始化和发送实现。

## 一句话理解

`Sprintf_s()` 负责“拼字符串”，`StringStatusReport()` 负责“把字符串交给各个输出后端”，`SerialOutput()` 负责“把字符串改成串口可接受的换行格式并调用 SerialPortWrite 发出去”。

## 简化流程图

```text
ReportStatusCodePei / DxeRtReportStatusCode / SmmReportStatusCode
    -> AmiReportStatusCode
    -> CreateString
       -> Sprintf_s / AsciiBSPrint
    -> StringStatusReport
       -> StringList[]
       -> SerialOutput
       -> SerialPortWrite
       -> UART/Serial
```

## 补充说明

- `SerialData()` 只是声明在 `StatusCodeCommon.c` 中，当前源码里没有看到它被实际调用。
- 因此，当前这条“字符串输出到串口”的主链路应以 `SerialOutput()` 为准。
- 如果要继续往下追到具体 UART 寄存器，需要继续定位当前工程实际链接到的 `SerialPortLib` 实例。
