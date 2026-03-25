## 设备存在检测

详见 [[M.2 PCIe 设备存在检测机制]]

**核心结论**：M.2 PCIe 设备是否存在，不是靠单根脚判定，而是 "CFG 脚做模块识别 + PCIe Link Training 做最终确认"。

---

## DAS/DSS* 信号

DAS（驱动器活动信号）由SSD驱动，以指示正在进行访问。
![[Pasted image 20240906152554.png]]

![[Pasted image 20240906152603.png]]
![[Pasted image 20240906152623.png]]
![[Pasted image 20240906153335.png]]
![[Pasted image 20240906153359.png]]