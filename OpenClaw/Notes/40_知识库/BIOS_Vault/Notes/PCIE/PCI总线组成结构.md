直接上图

![[Pasted image 20241021163522.png]]
PCI总线相关模块包括：
HOST主桥，PCI总线，PCI桥，PCI设备
## HOST主桥
* PCI总线由HOST主桥和PCI桥推出
* HOST主桥和主存储器控制器（MMC，Main Memory Controller）在同一级，所以PCI设备可以通过HOST访问MMC
* 一个CPU有几个HOST就有几个PCI总线
* 每个HOST管理一个PCI总线树，一个PCI总线树下挂的所有PCI设备属于一个总线域
## PCI总线
* PCI总线 ≠ PCI总线树
* PCI总线 ⊆ PCI总线树
* PCI总线由HOST或者PCI桥管理
* PCI桥可以扩展PCI总线
## PCI设备
* 分PCI主设备、从设备、桥设备
* PCI主从设备统称PCI Agent设备
## PCI总线信号定义
* 地址、数据、控制、仲裁、中断信号
* PCI总线是同步总线，每个设备需要CLK

* RST#复位信号
* INTA~D#中断请求信号
* PME# Power Manager Engen
* CLKRUN#
### 地址/数据信号
#### AD\[31:0]信号
* PCI总线复用地址与数据信号
#### PAR信号
* PCI使用机构校验机制确保数据和信号正确性
#### C/BE\[3:0]信号
* 复用命令和字节选通信号
* 在地址周期中输出总线命令
* 在数据周期中输出字节选通信号
* C/BE0~3与数据的字节0~3对应
* 表示字节、字、双字
* 定义多个总线事务
* ![[Pasted image 20241021172215.png]]
* ![[Pasted image 20241021172227.png]]
#### 接口控制信号
* 保证数据正常传递
#### 仲裁信号
* PCI 设备通过仲裁信号获得线权
* 只有主设备会用到
* 由REQ#、GNT#组成
* 每个主设备都有独立的仲裁信号，并与PCI总线仲裁器相连
* 总仲裁器需要保证同一时间只有一个PCI设备使用该总线
* PCI桥中也集成了仲裁器