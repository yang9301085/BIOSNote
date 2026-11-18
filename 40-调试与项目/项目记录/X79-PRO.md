---
type: project
status: active
owner: yqr
created: 2026-03-19
updated: 2026-03-19
tags:
  - project
  - BIOS
  - X79
  - SPI-Flash
---

> [!note] 历史学习记录
> 本文来自 2026 年 3 月的笔记与会话摘录，2026-11-18 提取归档。技术结论、寄存器定义和项目状态按原记录保留，使用前需结合对应平台源码、手册及实测核对。


# X79-PRO V3.1

## 基本信息

| 项目 | 值 |
|------|-----|
| 版本 | V3.1 |
| BIOS版本 | X79SMA11 |
| 状态 | 活跃 |

## 当前需求

### 加复旦微 FM25Q64AI3 SPI Flash 支持

**芯片信息**:
| 参数 | 值 |
|------|-----|
| 型号 | FM25Q64AI3 |
| 厂商 | 复旦微 (Fudan Microelectronics) |
| 容量 | 64Mbit (8MB) |
| 接口 | SPI |

## 待办

- [ ] 查找 FM25Q64AI3 数据手册
- [ ] 确认 SPI Flash 驱动模块位置
- [ ] 添加芯片 JEDEC ID 到支持列表
- [ ] 测试验证

## 相关链接

- [[40-调试与项目/项目记录/X79G.md|X79G]] - 同平台项目
