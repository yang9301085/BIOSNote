# BIOS_调试打印链路

**来源**: 笔录 2026-03-16/2026-03-24《BiosSerialPrintf / YqrSerialPrintLib》

#BIOS #Debug #Serial #PEI #DXE

---

# BIOS 调试打印链路（BiosSerialPrintf / YqrSerialPrintLib）

## 核心知识点

### BiosSerialPrintf 的调用链（AMi 框架）
```
Sprintf_s → 格式化到内存缓冲区（不是串口输出！）
           → StatusCodeCommon.c 中的 SerialOutput() → 硬件串口
```

### BIOS 三类调试打印路径

**路径1：PEI 阶段（Port 80 / Serial）**
- `PeiDebugServicePrint()`
- 需要 `DEBUG_SERVICE_PRINT_FUNCTION` 签名：`EFI_STATUS(IN CHAR8 *String, IN UINTN ErrorLevel)`

**路径2：DXE/SMM 阶段（StatusCode）**
- `ReportStatusCode()` → `PeiReportStringStatus()` → `SerialOutput()`
- ELINK 头中 `SerialOutput` 挂在 `PeiReportStringStatus` 上

**路径3：YqrSerialPrintLib（推荐跨模块方案）**
- 封装成独立库，可在任意 DXE 模块中调用
- 不依赖 AMI 框架签名
- 接口：`YqrSerialPrintf(String, ErrorLevel)`

### 函数签名对比
| 函数 | 签名 | 类型 |
|---|---|---|
| `SerialOutput` | `(IN VOID **PeiServices, IN CHAR8 *String)` | STRING_FUNCTION |
| `PeiDebugServicePrint` | `(IN CHAR8 *String, IN UINTN ErrorLevel)` | DEBUG_SERVICE_PRINT_FUNCTION |
| `YqrSerialPrintf` | `(IN CHAR8 *Fmt, ...)` | 自定义，无框架依赖 |

