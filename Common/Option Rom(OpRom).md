Option ROM就是在位于PCI或者ISA设备上的只读存储器，因为这个存储器不是总线标准规定一定要实现的，所以叫Option ROM（可选实现的ROM）。
Option ROM里面通常存放着用于初始化该设备的数据和代码。显卡和网卡等设备上通常带有Option ROM。简单来说，在它的开始处，总是一个固定结构的头结构，称为PnP Option ROM Header。
## UEFI 中OpRom SPEC
![[1710224793830.png]]
