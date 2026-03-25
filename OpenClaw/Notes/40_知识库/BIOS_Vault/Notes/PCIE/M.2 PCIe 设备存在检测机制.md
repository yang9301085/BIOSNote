# M.2 PCIe 设备存在检测机制

## 核心结论

**M.2 PCIe 设备是否存在，不是靠单根脚判定，而是 "CFG 脚做模块识别 + PCIe Link Training 做最终确认"。**

---

## M.2 关键引脚作用

| 引脚 | 作用 | 能否判断设备存在 |
|---|---|---|
| `M2_CFG[3:0]` | 模块类型/主接口编码识别 | ✅ 最接近静态插入识别 |
| `CLKREQ#` | 时钟请求/低功耗配合 | ❌ 运行行为，非稳定 presence |
| `PERST#` | 主机发给设备的复位 | ❌ 主机控制，不能反推 |
| `WAKE#` | 设备唤醒事件 | ❌ 平时无动静 |
| PETp/n, PERp/n | 高速差分对 | ❌ 需示波器/协议分析仪 |

---

## 设备存在判定流程

```
1. 平台采样 M2_CFG[3:0] → 知道模块类型/主接口
        ↓
2. 给设备供电、给 REFCLK、释放 PERST#
        ↓
3. Root Port 进入 LTSSM Detect → 接收端检测
        ↓
4. 链路训练成功 → Link Up → 配置空间可读 VID/DID
        ↓
   这时才能说"PCIe 设备存在且可用"
```

---

## 各引脚详解

### 一、M2_CFG[3:0]

这 4 根最接近"插上设备后某些脚会被拉高/拉低"。

Intel PCH 文档里对这 4 根的描述是：
- `M2_SKT2_CFG0 ~ CFG3`
- 用于 **select module type and main host interface**

**含义**：
- 模块上会按规范把这几根脚通过上拉/下拉/直连方式编码
- 主板侧去采样
- 有模块插入时：呈现某个固定编码组合
- 没插模块时：落在主板默认上拉/下拉状态

**注意**：这几根脚识别的是模块类型/主接口，不是"PCIe Endpoint alive"的最终证据。

---

### 二、CLKREQ#

很多人爱拿来当"插卡检测"，但它本质不是 Presence Detect。

**实际作用**：
- 给 **L1 Substates / CLKREQ-based clock request / 动态时钟门控** 用的
- 空闲时：设备可能把 `CLKREQ#` 释放，高电平
- 需要参考时钟时：设备拉低 `CLKREQ#`

**不能作为通用可靠判据**：
1. 不是所有 M.2 PCIe 设备都必须拿它做 presence
2. 即使设备插着，也不一定一直拉低
3. 主要服务于电源管理 / refclk request

---

### 三、PERST#

这是 **主机发给设备的复位**，不是设备告诉主机"我在"。

- **低**：设备处于 reset
- **高**：允许设备工作

这根脚的高低 **不是 presence 结果**，而是 host 的控制动作。

---

### 四、WAKE#

设备往主机这边发 wake 事件用的，通常是 **open-drain / active low** 风格。

- 没事的时候通常就是释放态
- 只有设备要唤醒系统时才拉低
- 插着设备不等于它会主动动这根脚

---

### 五、PETp/n、PERp/n、REFCLKp/n

高速差分对和参考时钟。

- 不是给你拿万用表看高低电平的
- Presence 不是靠静态高低
- 最终靠的是 **接收端检测、链路训练、TS1/TS2、LTSSM 状态迁移**

---

## 常见误区

### 误区 1：把 M.2 当成标准 PCIe x16 插槽

标准 CEM 插槽常见 `PRSNT#` 语义。
**M.2 不是这套。**

### 误区 2：把 `CLKREQ#` 当 presence

它更接近 **时钟请求 / 低功耗配合信号**。
PCH 文档里说的是 "CLKREQ pin presence / mapping"，重点在 **这个 port 有没有接这根脚以及映射关系**。

### 误区 3：看到 `CFG[3:0]` 就以为一定代表"有 PCIe 设备"

不完全对。
它代表的是 **M.2 模块类型和主接口编码**，不是最终 Link Up 结论。

---

## 调试检查顺序

### 静态硬件层

1. `M2_CFG[3:0]` 采样值对不对
2. 供电 rail 到没到
3. `PERST#` 有没有按时序释放
4. `REFCLK` 有没有送到模块
5. `CLKREQ#` routing / mapping 对不对

### 链路层

6. Root Port LTSSM 卡在哪
7. Receiver Detect 是否通过
8. Link width / speed negotiation 是否失败
9. Config space 的 `VID/DID` 能不能读出来

---

## 相关链接

- [[PCI总线组成结构]]
- [[PCI]]
- [[M.2]]

---

*来源: 2026-03-17 NVMe 首启调试会话*
