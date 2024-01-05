* 通过ACPI Table 中MCFG 获取到Memory Base Address: *0x8000 0000*
![[ACPI_Table.png]]

* 在Memory中输入对应地址，可以发现此地址为PCI BUS 的初始地址
![[PCI_and_MemoryAdd_80000000.png|669]]

* PCI B0D0F0 初始地址0x8000 0000 + 0x8000 (4k byte) 为下一 Device 地址
![[BaseAdd_addto8000.png]]
* PCI B0D1F0初始地址0x8000 8000 +0x1000 (512 byte)为下一 Function 地址
![[D1F0AddtoD1F1.png]]

* PCI B0D0F0初始地址0x8000 0000+0x100000 () 为下一 BUS 地址，但此案例中没有对应BUS，所以直接跳到BUS 3的位置（0x100000 * 3）
![[B0D0F0addtoB3D0F0.png]]
