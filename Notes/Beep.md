
# 蜂鸣器

platform : ADL-N

![[Pasted image 20240516164627.png]]
![[Pasted image 20240516164612.png]]
![[Pasted image 20240516165322.png]]

给 61 port的bit 1 置 1 时 蜂鸣器会叫一下
![[Pasted image 20240516165301.png]]
![[Pasted image 20240516170654.png]]
# Issue 
platform : ADL-N
Beep not work when unplug memory
正常情况下应该响3声
RC : GPP B14 没有设置内部pull-down
![[Pasted image 20240516165446.png]]
RF : 把GPP B14设置为内部pull-down
```C
//{GPIO_VER2_LP_GPP_B14,  {GpioPadModeNative1, GpioHostOwnGpio,  GpioDirNone,    GpioOutDefault,  GpioIntDis,              GpioPlatformReset,  GpioTermNone}},//SPKR J221021+
{GPIO_VER2_LP_GPP_B14, {GpioPadModeNative1, GpioHostOwnGpio, GpioDirNone, GpioOutDefault, GpioIntDis, GpioPlatformReset, GpioTermWpd20K}},// SPKR Pull down 20
```

# BIOS Code
```C
/**
    Simple Status Code handler that generates beep codes for a certain types of
    progress and error codes defined by the BEEP_PROGRESS_CODES_MAP and BEEP_ERROR_CODES_MAP mapping tables.

    @param PeiServices - pointer to the PEI Boot Services table
    @param Type the type and severity of the error that occurred
    @param Value the Class, subclass and Operation that caused the error

    @retval
        EFI_STATUS
**/
EFI_STATUS BeepStatus(
    IN VOID *PeiServices,
    IN EFI_STATUS_CODE_TYPE Type, IN EFI_STATUS_CODE_VALUE Value
)
{
    UINT32 CodeTypeIndex = STATUS_CODE_TYPE(Type) - 1;
    UINT8 i,n;

    if (CodeTypeIndex >= sizeof(BeepStatusCodes)/sizeof(STATUS_CODE_TO_BYTE_MAP*) ) return EFI_SUCCESS;

    n = FindByteCode(BeepStatusCodes[CodeTypeIndex],Value);

    if (n>0)
    {
        for (i=0; i<n; i++)
        {
            AmiBeep(29366*2,400000);
            MicroSecondDelay(100000);
        }
    }

    if (CodeTypeIndex == 1) // This is an error code, not progress, or debug
        MicroSecondDelay(1000000);// Delay 1 second to separate error codes one from another
    return EFI_SUCCESS;

}

/**
    Generates sound of beep of the specified frequency and duration using legacy PC/AT speaker.
    
    @param Frequency sound frequency in hundredth of Hertz units.
    @param Duration sound duration in microseconds.
**/
VOID AmiBeep(
    IN UINT32           Frequency,
    IN UINT32           Duration
    )
{
    /** THIS IS HW DEPENDENT. PORTING MAY BE REQUIRED. **/
    UINT16    Divider;
    Divider = (UINT16)((119318200 + Frequency/2)/Frequency);

    //
    // Set up channel 1 timer (used for delays)
    //
    IoWrite8(0x43,0x54);
    IoWrite8(0x41,0x12);

    //
    // Set up channel 2 timer (used by speaker)
    //
    IoWrite8(0x43,0xb6);
    IoWrite8(0x42,(UINT8)Divider);
    IoWrite8(0x42,(UINT8)(Divider>>8));

    //
    // Turn the speaker on
    //
    IoWrite8(0x61,IoRead8(0x61)|3);

    //
    // Delay
    //
    MicroSecondDelay(Duration);
    
    //
    // Turn off the speaker
    //
    IoWrite8(0x61,IoRead8(0x61)&0xfc);

}
```


