
> [!note] 历史学习记录
> 本文来自 2026 年 3 月的笔记与会话摘录，2026-11-18 提取归档。技术结论、寄存器定义和项目状态按原记录保留，使用前需结合对应平台源码、手册及实测核对。

# RevolutionBIOSUI

## 项目概述

- **类型**: UEFI BIOS 项目
- **平台**: Intel Raptor Lake（第13代酷睿）
- **BIOS 厂商**: AMI (American Megatrends)
- **框架**: EDK2 + AMI Aptio 5.x
- **主板型号**: B760M4S1

## 编译方式

```batch
SetVarEV.bat rebuild    # 完整重建
SetVarEV.bat build      # 增量构建
SetVarEV.bat clean      # 清理
```

## 工具路径

```
F:\Aptio_5.x_TOOLS_46\Aptio_5.x_TOOLS_46
```

## 相关笔记

- [[README.md|BIOS 知识库]]
- [[SEC|SEC 阶段]]
- [[PEI|PEI 阶段]]
- [[AMITse|AMI TSE 架构]]

## 项目路径

```
F:\RevolutionBIOSUI
```
