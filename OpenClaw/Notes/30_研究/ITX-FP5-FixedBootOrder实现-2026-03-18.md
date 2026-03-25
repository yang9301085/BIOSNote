# ITX-FP5 FixedBootOrder 实现

**平台**: AMD FP5 (ITX-FP5)  
**日期**: 2026-03-18  
**状态**: ✅ 已完成

## 需求背景

在 AMD FP5 平台上添加 FixedBootOrder 模块，允许用户在 BIOS Setup 中调整启动顺序。

**BIOS 信息**:
- M/B: ITX-FP5
- BIOS Version: JF5FBEF1
- Release Date: 2026/03/18

## 实现方案

### 功能特性
- BIOS Setup 中新增 Fixed Boot Order 设置页面
- 支持 #1 ~ #6 启动顺序调整
- 支持设备类型: USB、Network、Hard Drive、CD/DVD 等

### 关键文件
- 需求图片: `memory/requirements/ITX-FP5-FixedBootOrder-2026-03-18.jpg`

## 实现过程

1. 分析 AMD FP5 平台的启动流程
2. 添加 FixedBootOrder 模块到 BIOS
3. 在 Setup 界面添加配置入口
4. 测试验证各设备启动顺序调整功能

## 相关链接

- [[NVMe首启问题调试-2026-03-17]] - 相关平台的 NVMe 问题

## 后续事项

- [ ] 不同设备组合的启动顺序测试
- [ ] 保存/恢复默认启动顺序功能验证

---
*创建日期: 2026-03-18*
