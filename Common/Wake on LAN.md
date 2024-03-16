# 硬件层：

网卡可以监控网口中的唤醒帧、魔术包或Re-LinkOk，并在发生这种包或事件时通过LANWAKEB引脚通知PCH的WAKE引脚，WAKE# 收到信号后发出 [[PME]] 触发SMI从而唤醒系统
![[Pasted image 20240312162345.png]]

网卡的LANWAKE#
![[Pasted image 20240312161535.png]]

LANWAKE#连到PCH的WAKE#上
![[Pasted image 20240312161939.png]]
![[Pasted image 20240312162038.png]]
![[Pasted image 20240312164928.png]]
![[Pasted image 20240314113647.png]]


# BIOS 配置 Wake on LAN 

![[Pasted image 20240312150519.png]]

* 根据PCI SPEC中找到对应位置  

![[Wake on LAN-20240104151429512.webp|1007]]

首先遍历PCI设备找到LAN ，访问LAN的Power Managment，打开PMEE 
![[Pasted image 20240314104231.png]]

```C
void Enable_LAN_PME(void)
{
    UINT8    u8temp,tempPCIBUS,tempPCIDEV,tempPCIFUN,temp_reg;
    UINT32   u32temp;
    /**
		深度遍历查找LAN
    */
    for(tempPCIBUS=0; tempPCIBUS < Max_Search_BUS ;tempPCIBUS++)
    {
        for(tempPCIDEV=0; tempPCIDEV < Max_Search_DEV ; tempPCIDEV++)
        {
            for(tempPCIFUN=0; tempPCIFUN < Max_Search_FUN ;tempPCIFUN++)
            {
	            //0x08位置为Reversion ID 和 Class Code位置
                u32temp = READ_PCI32(
	                tempPCIBUS,
	                tempPCIDEV,
	                tempPCIFUN,
	                0x08
	            );
	            //PCI SPCE 规定 Class code为02时为Internet Device
                if((u32temp & 0xFFFF0000)==0x02000000)
                {
	                //找到0x34位置，此位置为capabilities Pointer                                      
                    temp_reg = READ_PCI8(
	                    tempPCIBUS,
	                    tempPCIDEV,
	                    tempPCIFUN,
	                    0x34
	                );
	                //通过capabilities Pointer指针找到Capability ID
                    u8temp = READ_PCI8(
	                    tempPCIBUS,
	                    tempPCIDEV,
	                    tempPCIFUN,
	                    temp_reg
	                );
	                //当Capability ID为0x01时，
	                //此位置为PCI Power management Interface
                    if(u8temp == 1)
                    {
	                    //找到PME Enable位置
                        u8temp = READ_PCI8(
	                        tempPCIBUS,
	                        tempPCIDEV,
	                        tempPCIFUN,
	                        temp_reg+5
	                    );
	                    //将PME_EN 置 1
                        u8temp = u8temp|0x01;
                        //写入对应位置
                        WRITE_PCI8(
	                        tempPCIBUS,
	                        tempPCIDEV,
	                        tempPCIFUN,
	                        temp_reg+5,
	                        u8temp
                        );
                    }
                }
            }
        }
    }
}
```

* 然后再打开PM IO中的寄存器
```C
// Enable PCI-E PME Wake-Up function
VOID PCI_PCIE_PME_Wakeup()
{
    UINT32 adata1;
    UINT32 adata2;
    UINT32 adata3;

    Enable_LAN_PME();
    
    adata1 = IoRead32(PM_BASE_ADDRESS + 0x00); // PM1_EN_STS
    adata1 |= BIT14 + BIT15;
    adata2 = IoRead32(PM_BASE_ADDRESS + 0x6C); // GPE0_STS
    adata3 = IoRead32(PM_BASE_ADDRESS + 0x7C); // GPE0_EN

    // Set PCI/PCI-E/Lan PME event to enable
    if (WakeUp_PCIELAN_Event == 1)
    {
        adata1 &= ~BIT30;                         // Enable PCIE event
        /*
	        PCI_EXP_EN
	        PEM_B0_EN
	        LAN_WAKE_EN
        */
        adata3 = adata3 | (BIT9 + BIT13 + BIT16);
    }
    else
    {
        adata1 |= BIT30;                   // Disable PCIE event
        adata3 &= ~(BIT9 + BIT13 + BIT16); 
    }
    // Set PCI/PCI-E/Lan PME event to enable
    if (WakeUp_PCIELAN_Event == 1)
    {
	    /* 
		    clear PCI event
		    PCI_EXP_STS
		    PME_B0_STS
	    */
        adata2 = adata2 | (BIT9 + BIT13); 
    }
    else
    {
        adata2 &= ~(BIT9 + BIT13); // Disable PCI event
    }

    IoWrite32(PM_BASE_ADDRESS + 0x00, adata1);
    IoWrite32(PM_BASE_ADDRESS + 0x34, 0xFFFFFFFF);
    IoWrite16(PM_BASE_ADDRESS + 0x6C, adata2);
    IoWrite16(PM_BASE_ADDRESS + 0x7C, adata3);
}
```

* READ_PCI 系列 函数 的定义
```C

#define READ_PCI8(Bx, Dx, Fx, Rx)           ReadPci8(Bx, Dx, Fx, Rx)

#define READ_PCI16(Bx, Dx, Fx, Rx)          ReadPci16(Bx, Dx, Fx, Rx)

#define READ_PCI32(Bx, Dx, Fx, Rx)          ReadPci32(Bx, Dx, Fx, Rx)

#define WRITE_PCI8(Bx, Dx, Fx, Rx, bVal)    WritePci8(Bx, Dx, Fx, Rx, bVal)

#define WRITE_PCI16(Bx, Dx, Fx, Rx, wVal)   WritePci16(Bx, Dx, Fx, Rx, wVal)

#define WRITE_PCI32(Bx, Dx, Fx, Rx, dVal)   WritePci32(Bx, Dx, Fx, Rx, dVal)

//----------------------------------------------------------------------------

// Standard PCI Access Routines, No Porting Required.

//----------------------------------------------------------------------------


//<AMI_PHDR_START>

//----------------------------------------------------------------------------

//

// Procedure:   ReadPci8

//

// Description: This function reads an 8bits data from the specific PCI

//              register.

//

// Input:       Bus - PCI Bus number.

//              Dev - PCI Device number.

//              Fun - PCI Function number.

//              Reg - PCI Register number.

//

// Output:      UINT8

//----------------------------------------------------------------------------

//<AMI_PHDR_END>

UINT8 ReadPci8 (
    IN UINT8        Bus,
    IN UINT8        Dev,
    IN UINT8        Fun,
    IN UINT16       Reg )
{
    if (Reg >= 0x100) {
        return MMIO_READ8(SB_PCIE_CFG_ADDRESS(Bus, Dev, Fun, Reg));
    } else {
        IoWrite32(0xcf8, \
                BIT31 | (Bus << 16) | (Dev << 11) | (Fun << 8) | (Reg & 0xfc));
        return IoRead8(0xcfc | (UINT8)(Reg & 3));
    }
}

  

//<AMI_PHDR_START>

//----------------------------------------------------------------------------

//

// Procedure:   ReadPci16

//

// Description: This function reads a 16bits data from the specific PCI

//              register.

//

// Input:       Bus - PCI Bus number.

//              Dev - PCI Device number.

//              Fun - PCI Function number.

//              Reg - PCI Register number.

//

// Output:      UINT16

//----------------------------------------------------------------------------

  

//<AMI_PHDR_END>

UINT16 ReadPci16 (
    IN UINT8        Bus,
    IN UINT8        Dev,
    IN UINT8        Fun,
    IN UINT16       Reg )
{
    if (Reg >= 0x100) {
        return MMIO_READ16(SB_PCIE_CFG_ADDRESS(Bus, Dev, Fun, Reg));
    } else {
        IoWrite32(0xcf8, \
                BIT31 | (Bus << 16) | (Dev << 11) | (Fun << 8) | (Reg & 0xfc));
        return IoRead16(0xcfc | (UINT8)(Reg & 2));
    }
}

  

//<AMI_PHDR_START>

//----------------------------------------------------------------------------

//

// Procedure:   ReadPci32

//

// Description: This function reads a 32bits data from the specific PCI

//              register.

//

// Input:       Bus - PCI Bus number.

//              Dev - PCI Device number.

//              Fun - PCI Function number.

//              Reg - PCI Register number.

//

// Output:      UINT32

//----------------------------------------------------------------------------

//<AMI_PHDR_END>

UINT32 ReadPci32 (
    IN UINT8        Bus,
    IN UINT8        Dev,
    IN UINT8        Fun,
    IN UINT16       Reg )
{
    if (Reg >= 0x100) {
        return MMIO_READ32(SB_PCIE_CFG_ADDRESS(Bus, Dev, Fun, Reg));
    } else {
        IoWrite32(0xcf8, \
                BIT31 | (Bus << 16) | (Dev << 11) | (Fun << 8) | (Reg & 0xfc));
        return IoRead32(0xcfc);
    }
}
```

* 关于IO空间 0xCF8 （PCI Configuration Address Port）
![[Wake on LAN-20240104153939337.webp|685]]

## Register Fields for PM_CAP/CON_STATUS_REG
| **Field Name**     | **Bit Offset** |
| ------------------ | -------------- |
| POWER_STATE        | 0              |
| RSVDP_2            | 2              |
| NO_SOFT_RST        | 3              |
| RSVDP_4            | 4              |
| PME_ENABLE         | 8              |
| DATA_SELECT        | 9              |
| DATA_SCALE         | 13             |
| PME_STATUS         | 15             |
| RSVDP_16           | 16             |
| B2_B3_SUPPORT      | 22             |
| BUS_PWR_CLK_CON_EN | 23             |
| DATA_REG_ADD_INFO  | 24             |
![[Wake on LAN-20240104175053475.webp|647]]

![[Wake on LAN-20240104175201952.webp|638]]

![[Wake on LAN-20240104175224099.webp|928]]


![[Wake on LAN-20240104175126812.webp|718]]

![[Wake on LAN-20240104175405364.webp|813]]

![[Wake on LAN-20240104175421255.webp|761]]