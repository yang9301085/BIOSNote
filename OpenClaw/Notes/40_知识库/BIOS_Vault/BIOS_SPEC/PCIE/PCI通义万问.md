## PCIE memory bar的SIZE 有要求最低1M还是4k吗？ 记得有这个要求，找不到了

单就BAR 寄存器读写属性而言， BAR寄存器队除了SPACE INDICATION, MEMORY TYPE, 
PREFETCHABLE 这些特殊字集外， 其他都可以通过设置BAR MASK 的方式来指指示BAR SIZE, 
在此基础上， PCIE BASE SPEC 中对MEMORY BAR 的最小SIZE 有要求， 但没有4KB 或1MB 这
两个要求。
在不同的PCI BASE SPEC 中， 对MEMORY BAR 的最小SIZE 要求不同， 比如PCIE 5.0 中，
BAR SIZE 最低可以为16B, 在PCIE 6.2 中， 特别指明MEMROY BAR SIZE 最小变为128B.
此外，从PCIE 3.0 开始， 如果MEMORY BAR SIZE 需求小于4KB, 协议建议BAR SIZE 按照
4KB 给，但非硬性要求， 基于该建议， SYNOPSYS PCIE IP 的MEMORY BAR SIZE 有最低4KB
的要求， 以减小ADDR DECODER 的位宽。
至于最低1MB 的限制， 应该是说MEMORY BASE, LIMIST REGISTER, 这两个寄存器有低12B默认为0，
即存在最低1MB 的限制。 

---

## 在枚举之过后， EP 设备BDF 的值还可以动态变吗？

EP 的BUS NUMBER 和DEVICE NUMBER 可以动态变化， EP 应该根据接收到的CFGWR 请求中的信息
动态更新自已所在的BUS NUMBER 和DEVICE NUMBER, 至于FUNCITON NUMBER, 这个是EP 自己定的。
在枚举之后HOST 能够知道有哪些可用的FUNCTION, 除非打算重新枚举， 不建议动态改FUNCTION NUMBER.
附协议原文： 
NOTE that bus number and device number may be changed at run time, and so it is necessary to re-capture
this information with each and every configuration write request. 

---

### SYNOPSYS PCIE 仿真时可以跳过LINK TRAINING吗？

SYNOPSYS PCIE IP 及VIO 均支持该操作：
SYNOPSYS PCIE IP 是支持SKIP LINK TRAINING 的，可以通过配置PORT LOGIC 中的PORT FORCE LINK 寄
存器来实现， 直接把LINK 的LTSSM 状态FORCE 为L0即可。
SYNOPSYS PCIE VIP 也支持跳过LINK TRAINING, 配置VIP 的L_CFG.SKIP_INTIAL_LINK_TRAINING = 1
即可。
需要注意的是， 上述配置中VIP 对链路速率有要求， DUT 对链路速率没有要求， 对于VIP ,考虑到GEN3及以上速率
时需要OS 来做DESKEW, SKIP LINK INITIAL TRAINING 仅在GEN1, GEN2 速率下有效， 对GEN3及以上速率无效。
对于DUT , 只要通过寄存器配置了FORCE_EN =1及LINK_STATE=L0即可促使DUT 的LTSSM 跳转到L0.
跳过LINK TRAINING 需要链路两端均支持才可以， 如果链路对端不支持跳过LINK TRAINING, 只配置DUT 是没有用的。

---

## PCIE 的INBOUND, OUTBOUND 该怎么理解？

个人理解 ，INBOUND 和OUTBOUND 是请求事务（控制流）进出的方向， 可以把PCIE 接口理解为一道门， 
INBOUND  是进门， OUTBOUND 是出门。 设备接在主机上， 从主机角度， INBOUND 是设备访问（读或写）主机， OUTBOUND 是主机访问设备；从设备角度，INBOUND 是主机访问设备，OUTBOUND 是设备访问主机。
也有人认为这里的方向是针对数据流而言的， 对主机而言，INBOUND 是设备写主机或者设备响应主机读请求。对设备而言， INBOUND 是主机写设备或主机响应设备读请求， OUTBOUND 是设备写主机或设备响应主机读请求。
为避免歧义， 在提到INBOUND, OUTBOUND 时最好带上名词， 比如INBOUND ADDRESS TRANSLATION, INBOUND REQUEST 等等， 并结合上下语境分析其具体意义。

---

## 为什么ZERO WRITE 的TLP 内还需要携带1DW 的DATA?

对于ZERO BYTE TRANSFER, 其TLP 中LENGTH 值为1而非0， 为0 表示1024DW, 既然 
LENGTH=1， 那么按照相关规则ZERO WRITE TLP 内就应该填充1DW 的DATA, 对端收到
之后不用即可， 同理，收到ZERO READ后， 回复的CPLD 中也应携带1DW DATA. 