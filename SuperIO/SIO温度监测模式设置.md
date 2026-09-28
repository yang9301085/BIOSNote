# TMPIN2 读取三极管温度配置笔记

## 1. 问题背景

板上原理图中：
![[原理图 - Q30 2N3904 接法.png]]
* **Q30 = 2N3904**
* **B 极与 C 极短接**
* **E 极接 GND**
* **B/C 节点连接到 EC / Super I/O 的 TMPIN2**

该接法不是把三极管当放大器或开关使用，而是将其作为：

* **Diode-connected Transistor**
* **Thermal Diode**
* **结温传感器**

供硬件监控模块读取温度。

---

## 2. 关键概念

### 2.1 diode-connected transistor 是什么

所谓 **diode-connected**，就是把三极管按二极管方式连接。

对于 **NPN 三极管**：

* **Base 和 Collector 短接**
* 对外只剩两个端子：

  * **B/C**
  * **E**

这样从外部看，它的行为近似一个二极管，主要利用的是：

* **B-E 结的正向压降 Vbe**

---

### 2.2 为什么能测温

在固定偏置电流下，三极管的 **B-E 结压降 Vbe** 会随温度升高而下降，经验规律约为：

* **-2 mV / °C**

因此 EC / Super I/O 可通过：

1. 给 TMPIN 外接的结温器件施加小偏置电流
2. 测量 TMPIN 节点电压
3. 用内部 ADC 采样
4. 按 Thermal Diode 模式换算温度

实现温度检测。

---

## 3. 原理图结论

该板中 Q30 的接法说明：

* **TMPIN2 对应的外部温度传感器是三极管结温传感器**
* 不应按 **Thermal Resistor mode** 配置
* 应按 **Thermal Diode mode** 配置

---

## 4. datasheet 关键寄存器

### 寄存器


![[ADC Temperature Channel Enable Register 寄存器表.png]]
* **ADC Temperature Channel Enable Register**
* **Index = 51h**
* **Default = 00h**

### datasheet 说明

TMPIN1 ~ TMPIN3 **不能同时**启用：

* **Thermal Resistor mode**
* **Thermal Diode mode**

也就是说，同一个 TMPIN 通道只能二选一。

---

## 5. Index 51h 位定义总结

### Thermal Diode mode

* **bit0**: TMPIN1 enable
* **bit1**: TMPIN2 enable

### Thermal Resistor mode

* **bit3**: TMPIN1 enable
* **bit4**: TMPIN2 enable

---

## 6. 本次正确配置

由于板上 **TMPIN2 接的是 diode-connected 2N3904**，因此应配置为：

* **0x51[1] = 1**
  启用 TMPIN2 的 **Thermal Diode mode**

* **0x51[4] = 0**
  禁止 TMPIN2 的 **Thermal Resistor mode**

---

## 7. 本次实际修改结果

已修改为：

* **bit4 = 0**
* **bit1 = 1**

结论：

> **TMPIN2 已被配置为读取 diode-connected transistor（三极管结温传感器）**

这是与原理图硬件连接一致的正确配置。

---

## 8. 配置逻辑说明

如果只从 TMPIN2 角度理解：

### 正确

* `bit1 = 1`
* `bit4 = 0`

### 错误

* `bit1 = 0, bit4 = 1`
  会把 TMPIN2 当成热敏电阻输入

* `bit1 = 1, bit4 = 1`
  不允许，datasheet 明确说明不能同时启用两种模式

* `bit1 = 0, bit4 = 0`
  TMPIN2 温度通道未启用

---

## 9. 推荐的软件写法

采用**读改写**方式，只改 TMPIN2 相关位，避免误伤其它 bit。

```c
val = ReadReg(0x51);
val &= ~(1 << 4);   // TMPIN2: disable Thermal Resistor mode
val |=  (1 << 1);   // TMPIN2: enable Thermal Diode mode
WriteReg(0x51, val);
```

---

## 10. 这一步的意义

完成该配置后，EC / Super I/O 会把 TMPIN2 当成：

* **Thermal Diode**
* **Diode-connected Transistor**

进行采样，而不是按热敏电阻分压模型解释。

这意味着后续 TMPIN2 温度值应来自：

* **Q30 的 B-E 结压降 Vbe**

---

## 11. 后续验证建议

### 11.1 读取温度寄存器

继续确认 TMPIN2 对应的温度读数寄存器，例如 datasheet 中类似：

* **TMPIN2 Temperature Reading Register**
* 常见可能为 **Index 2Ah**

### 11.2 确认全局监控模块已开启

除了 0x51 外，还要确认：

* HWM / ADC 总使能已开启
* conversion 已 start
* monitor function 已启用

否则模式配对了，ADC 也可能没有真正工作。

### 11.3 做物理验证

可以对 Q30 做简单热刺激测试：

* 手指按住附近
* 轻微热风吹
* 观察 TMPIN2 温度是否上升

如果读数随加热上升，说明配置和硬件链路基本正确。

---

## 12. 最终结论

本次问题的结论是：

> 板上 TMPIN2 外接的是一颗 **diode-connected 2N3904**，因此必须将 **Index 51h 的 bit1 置 1、bit4 清 0**，使 TMPIN2 工作在 **Thermal Diode mode**，这样 EC / Super I/O 才会按三极管结温传感器方式读取温度。