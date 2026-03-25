# 来电开机功能
硬件层：
![[Pasted image 20240314165424.png]]
SIO的PME#会连到GPIO13（X99 Platform）
![[Pasted image 20240314171824.png]]
![[Pasted image 20240314171908.png]]
![[Pasted image 20240314171938.png]]

* SuperIO关于ACPI POWER LOSS功能配置的描述

![[Pasted image 20240314155252.png]]
![[Pasted image 20240314163729.png]]

![[Pasted image 20240314163834.png]]


```C
void PowerB_Last_state()
{

    UINT8 bData;
    
    if (Power_Loss_status == 2)
    {
    
        IoWrite8(0x2e, 0x87);
        IoWrite8(0x2e, 0x87);
        
        IoWrite8(0x2e, 0x07);
        IoWrite8(0x2f, 0x0a);
        
        IoWrite8(0x2e, 0xe4);
        bData |= BIT6;
        bData |= BIT5;
        IoWrite8(0x2f, bData);
        
        IoWrite8(0x2e, 0xE7);
        bData &= ~BIT4;
        IoWrite8(0x2f, bData);
        
        IoWrite8(0x2e, 0xE6);
        bData |= BIT4;
        IoWrite8(0x2f, bData);

        IoWrite8(0x2e, 0xaa);

    }

}
```