# BIOS_M2_PCIe设备检测机制

**来源**: 笔录 2026-03-19《PCIe 枚举》

#BIOS #PCIe #M.2 #LTSSM #检测

---

# M.2 PCIe 设备存在检测机制

## 核心知识点

### M.2 接口关键信号的作用
| 信号 | 作用 | 能否判断设备存在 |
|---|---|---|
| M2_CFG[3:0] | 模块类型/主接口编码 | ✅ 最接近静态插入识别 |
| CLKREQ# | 时钟请求/低功耗配合 | ❌ 运行行为，非稳定 presence |
| PERST# | 主机发给设备的复位 | ❌ 主机控制 |
| PETp/n, PERp/n | 高速差分对 | ❌ 需协议分析仪 |

### 检测完整流程
```
1. 采样 M2_CFG[3:0] → 知道模块类型/主接口
        ↓
2. 供电 + REFCLK + 释放 PERST#
        ↓
3. Root Port LTSSM 进入 Detect 状态
        ↓
4. Receiver Detect：通过检测端接电阻确认有设备
        ↓
5. 链路训练 → Configuration → L0
        ↓
   VID/DID 可读 → 设备真正"可用"
```

### LTSSM 关键状态
- **Detect**：检测对方是否存在
- **Polling**：协商速率和宽度
- **Configuration**：交换 lane mapping
- **L0**：链路正常工作状态
- **L0s/L1**：省电状态

### 常见误区
- M.2 无 PRSNT# 引脚，不能用标准 PCIe 插槽方式判断 presence
- CFG[3:0] 不是"有 PCIe 设备"的结论，只是模块类型编码

