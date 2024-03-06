# 串口 UART

SuperIO 型号：ITE8316
虽然配置串口不会碰到这些寄存器，但也需要了解
UART 寄存器:
![[Pasted image 20240301181541.png]]
DLAB: Divisor Latch Access Bit. Because some registers share the same address, it accesses a different register by this bit is 0 or 1.
几个比较重要的寄存器：
RBR/TBR： 也叫Data Register,TX/RX传输数据通过这个寄存器 
LSR: 线路状态寄存器，表明当前通信的状态
![[Pasted image 20240305091659.png]]
![[Pasted image 20240305091719.png]]
Bit0(Data Ready)为 1 时表明接收数据
Bit5 表明传输数据

