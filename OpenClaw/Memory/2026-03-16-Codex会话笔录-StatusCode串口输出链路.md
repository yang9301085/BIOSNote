# Codex 会话笔录 - AMI StatusCode 串口输出链路分析

**会话时间：** 2026-03-16 11:50 - 12:00
**项目路径：** `f:\PBX6000_sample`
**会话文件：** `C:\Users\yqr\.codex\sessions\2026\03\16\rollout-2026-03-16T11-50-23-019cf4c4-2d12-7210-a2d5-9858669bb650.jsonl`

---

## 用户问题

分析 `Sprintf_s` 函数如何输出到串口

## 分析过程

### 1. 搜索 `Sprintf_s` 的定义和调用位置

Codex 在 `AmiCompatibilityPkg/Library/StdLibC.c` 中找到了 `Sprintf_s` 的实现：
- 行 1139：实现逻辑
- 行 1351：函数定义 `UINTN EFIAPI Sprintf_s(char *Buffer, UINTN BufferSize, char *Format, ...)`

**关键发现：** `Sprintf_s` 只是往 `Buffer` 里逐字节拷贝，没有任何串口 I/O 操作。

### 2. 追踪真正的串口输出链路

Codex 发现真正的串口输出链路如下：

```
ReportStatusCodePei / DxeRtReportStatusCode / SmmReportStatusCode
    -> AmiReportStatusCode
    -> CreateString
       -> Sprintf_s / AsciiBSPrint  (只生成字符串，不输出)
    -> StringStatusReport
       -> StringList[]  (遍历注册的字符串处理器)
       -> SerialOutput
       -> SerialPortWrite
       -> UART/Serial
```

### 3. 关键代码位置

| 函数 | 文件位置 |
|------|----------|
| `Sprintf_s` 实现 | `AmiCompatibilityPkg/Library/StdLibC.c:1351` |
| `AmiReportStatusCode` | `AmiModulePkg/AmiStatusCode/StatusCodeCommon.c:678` |
| `CreateString` | `AmiModulePkg/AmiStatusCode/StatusCodeCommon.c:199` |
| `StringStatusReport` | `AmiModulePkg/AmiStatusCode/StatusCodeCommon.c:538` |
| `SerialOutput` | `AmiModulePkg/AmiStatusCode/StatusCodeCommon.c:428` |
| `SerialPortWrite` 调用 | `AmiModulePkg/AmiStatusCode/StatusCodeCommon.c:449,450,458` |
| `SerialStatusInit` | `AmiModulePkg/AmiStatusCode/StatusCodeCommon.c:475,480` |

### 4. SerialOutput 的注册机制

`SerialOutput` 通过 eLink 注册到字符串处理链中，配置在 `StatusCodeAmi.sdl`：

- 行 433-434：PEI 字符串状态码处理链注册 `SerialOutput`
- 行 441-442：DXE 字符串状态码处理链注册 `SerialOutput`
- 行 449-450：RT 字符串状态码处理链注册 `SerialOutput`
- 行 457-458：SMM 字符串状态码处理链注册 `SerialOutput`

前提条件：`SERIAL_STATUS_SUPPORT = 1`

## 结论

1. **`Sprintf_s` 只负责"生成字符串"** — 把格式化后的 ASCII 文本写入调用者提供的内存缓冲区
2. **真正输出到串口的是** `StringStatusReport -> SerialOutput -> SerialPortWrite` 这条链
3. **`SerialOutput` 的职责**：把字符串改成串口可接受的换行格式（`\n` -> `\r\n`），然后调用 `SerialPortWrite` 发出去

## 产出文档

Codex 已将分析结果整理成文档：
- **文件路径：** `E:\Note\BIOSNote\BIOS\Notes\AmiStatusCode 串口输出链路.md`
- **文件大小：** 4529 字节

## 后续可追

如需继续往下追到具体 UART 寄存器，需要定位当前工程实际链接到的 `SerialPortLib` 实例。
