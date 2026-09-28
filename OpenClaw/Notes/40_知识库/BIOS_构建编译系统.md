# BIOS_构建编译系统

**来源**: 笔录 2026-03-04/2026-03-11《BIOS 构建编译》

#BIOS #Build #Aptio #EDK2 #编译

---

# AMI Aptio 构建编译系统

## 核心知识点

### 顶层构建入口（AMI Aptio V）
AMI BIOS 的构建入口分两类：

**命令行入口：**
- `SetVarEV.bat` — 设置环境变量（BASE_TOOLS_PATH、TOOLS_DIR、EWDK_DIR），然后分发到 `make`
- `makefile` — 顶层构建逻辑，包含 `build/rebuild/clean` 三个 target
- `Build.log` — 构建输出日志（重定向到根目录）

**VeB 工程入口：**
- `RaptorLake.veb` — VeB 工程文件（Visual Editor for BIOS）
- `Build.cif` — 把 `makefile` 注册成 VeB 里的 Build 组件
- `utilities.mak` — VeB 构建工具链，未显式设置 VEB 时自动选根目录唯一的 `*.veb`

### 典型编译错误处理

**`error LNK2001: unresolved external symbol __aullshr`**
- 原因：IA32 + `/NODEFAULTLIB` 环境下，源码对 `UINT64` 直接右移，MSVC 生成了 CRT helper `__aullshr`，但固件模块不链接 CRT
- 解决：改用 BaseLib 的 `RShiftU64()`，不再依赖 MSVC CRT

**`error C2065: undeclared identifier`**
- 原因：头文件未包含，或变量在 `ifdef DEBUG` 块里
- 排查：看是否在正确配置下编译（Debug vs Release）

### EDK2 DSC / FDF 文件结构
- `DSC`（Platform Description）：定义模块组件、库、编译选项
- `FDF`（Flash Description）：定义 Flash 布局、固件卷组织
- `!include` 指令：编译时把 inc 文件内容复制进目标文件，EDK 支持多种扩展名

### 构建流程
```
SetVarEV.bat  →  设置工具链路径
       ↓
make build     →  解析 DSC/FDF
       ↓
编译 .c → .obj  →  链接 .efi / .rom
       ↓
FitTable / GenFv  →  生成固件卷
       ↓
Flash Image  →  写入 SPI ROM
```

