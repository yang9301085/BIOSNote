# Video_GOP与显示输出

**来源**: 笔录 2026-03-25《BIOS 调试库 / Video GOP》

#BIOS #GOP #Video #显示 #FrameBuffer

---

# GOP 与显示输出

## 核心知识点

### UEFI GOP（Graphics Output Protocol）
GOP = BIOS 提供给 OS 的图形输出接口。
- BIOS 在 DXE 阶段安装 GOP 协议
- OS bootloader 通过 GOP 获取分辨率、模式集、FrameBuffer 地址

### GOP 核心接口
```c
EFI_GRAPHICS_OUTPUT_PROTOCOL {
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE  *Mode;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE  QueryMode(...);
    EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE   SetMode(...);
    EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT       Blt(...); // 块传输（画图）
};
```

### BIOS UI 的显示路径
```
BIOS UI（Skia/Custom）
  → FrameBuffer（或 GOP Blt）
  → 显存地址
  → 显示器输出
```

### 独立显卡 vs 集成显卡输出
- **集成显卡（iGPU）**：PCH 内置显示输出，UEFI GOP 由 iGPU BIOS 提供
- **独立显卡（dGPU）**：PCIe 插槽，GOP 由 dGPU VBIOS 提供
- 多显示器时，BIOS 需要管理 GOP Handle 选择

### GOP 与 Skia 的关系
- Skia 在 BIOS UI 中绑定的 Surface → FrameBuffer
- `GOP->Blt()` 可用于将 Skia 渲染结果输出到屏幕
- 有些平台用 OEM 定制显示驱动，不走标准 GOP

### VBT（Video BIOS Table）
- Intel 平台：VBT = 描述板载显示端口（LVDS/eDP/HDMI/DP）信息的表
- 由 VBIOS 在 POST 时传递给 BIOS
- BIOS 据此配置显示输出端口和顺序

