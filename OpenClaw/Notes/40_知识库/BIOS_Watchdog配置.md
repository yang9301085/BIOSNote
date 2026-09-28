# BIOS_Watchdog配置

**来源**: 笔录 2026-03-04《BIOS 看门狗》

#BIOS #Watchdog #WDT #SIO #安全

---

# Watchdog Timer 配置（以 ITE8659 为例）

## 核心知识点

### 三种检测模式
| 模式 | 检测方式 | 精度 |
|---|---|---|
| Time-out | 设定间隔内未收到 MCU 信号则复位 | 基础 |
| Window | 未收到信号或收到多个信号都触发复位 | 较高 |
| Q&A | MCU 发送预定数据，WDT 验证是否匹配 | 最高 |

### ITE8659 SIO WDT 配置寄存器
| 寄存器 | 位 | 功能 |
|---|---|---|
| 0x72[5] | WDT timeout value extra select | 超时时间额外选择 |
| 0x72[6] | WDT Output through KRST Enable | 通过 KRST 输出 |
| 0x72[7] | 秒/分计时选择 | 0=秒，1=分 |
| 0x73-0x74 | WDT timeout value | 超时值 |

### 配置流程
1. **进入配置模式**：SIO 特定序列（写 0x87 两遍）
2. **激活 WDT**：设置 LDN = WDT device（0x07）
3. **配置超时**：0x72/0x73 设置值
4. **启动 WDT**：0x72[0] = 1
5. **喂狗**：周期性向 0x72 写特定值清计数器

### 应用场景
汽车电子、热水器、厨房设备等需要高安全性的嵌入式系统。

