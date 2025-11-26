![[Pasted image 20240401115633.png]]
![[Pasted image 20240401115643.png]]
![[Pasted image 20240401115649.png]]
![[Pasted image 20240401115656.png]]
![[Pasted image 20240401115703.png]]
![[Pasted image 20240401115710.png]]
![[Pasted image 20240401115715.png]]
![[Pasted image 20240401115723.png]]
![[Pasted image 20240401115732.png]]
Intel 平台GPIO AMI  code

AMI Code config GPIO的几种方式：
1. PCD
`Intel\AlderLakeBoardPkg\SBCVpdStructurePcd\GpioTableAdlNPreMem.dsc`
```C
# mGpioTablePreMemAdlNDdr4Rvp
[PcdsDynamicExVpd.common.SkuIdAdlNDdr4Rvp]
gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTablePreMem|*|{CODE({
  //Y240923_Enable SATA LED-[L6N10001]
  //{GPIO_VER2_LP_GPP_B14,  {GpioPadModeNative1, GpioHostOwnGpio,  GpioDirNone,    GpioOutDefault,  GpioIntDis,              GpioPlatformReset,  GpioTermNone}},//SPKR J221021+
  // PCH M.2 SSD
  {GPIO_VER2_LP_GPP_D16,  {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,  GpioIntDis, GpioPlatformReset,  GpioTermNone}},  //M2_PCH_SSD_PWREN
  {GPIO_VER2_LP_GPP_H0,   {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutLow,   GpioIntDis, GpioPlatformReset,  GpioTermNone}},  //M2_SSD_RST_N
  //X1 SLOT
  {GPIO_VER2_LP_GPP_A8,   {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutLow,   GpioIntDis, GpioPlatformReset,  GpioTermNone}},  //X1_SLOT_PWREN
  {GPIO_VER2_LP_GPP_F10,  {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutLow,   GpioIntDis, GpioPlatformReset,  GpioTermNone}},  //X1_Slot_RESET
  // TCP1
  {GPIO_VER2_LP_GPP_E20,  {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,  GpioIntDis, GpioPlatformReset, GpioTermNone}},  // TCP1_DISP_AUX_P_BIAS_GPIO
  {GPIO_VER2_LP_GPP_E21,  {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutLow,   GpioIntDis, GpioPlatformReset, GpioTermNone}},  // TCP1_DISP_AUX_P_BIAS_GPIO
  //Type-C HDMI ALS
  {GPIO_VER2_LP_GPP_A11,  {GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut,  GpioOutHigh,  GpioIntDis, GpioPlatformReset,  GpioTermNone}}, // TCP1_HDMI_ALS_PWR_EN
  //Unused pin set to HiZ
  {GPIO_VER2_LP_GPP_A17,  {GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutDefault,  GpioIntDefault,  GpioResetDefault,   GpioTermNone}},  // HiZ
  {0x0}  // terminator
})}
```
2. Head file ![[Pasted image 20250425181119.png]]
`Intel\AlderLakePlatSamplePkg\Wrapper\Library\AmiOemGpioLibSample\AmiOemGpioLibSample.h`
```C
// GPIO tables for pre-memory phase
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_INIT_CONFIG mGpioTableAdlPreMem[]=
{
        {GPIO_VER2_LP_GPP_E22, {GpioPadModeNative1,  GpioHostOwnGpio, GpioDirNone,  GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},  //DISP_AUX_P_BIAS_GPIO //J230705+
        {GPIO_VER2_LP_GPP_E23, {GpioPadModeNative1,  GpioHostOwnGpio, GpioDirNone,  GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermWpd20K}},  //DISP_AUX_N_BIAS_GPIO//J230705+
        //{GPIO_VER2_LP_GPP_D14, {GpioPadModeGpio,     GpioHostOwnGpio, GpioDirOut,   GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},
        {GPIO_VER2_LP_GPP_A12, {GpioPadModeNative1,  GpioHostOwnGpio, GpioDirIn,    GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},
        {GPIO_VER2_LP_GPP_E0 , {GpioPadModeNative1,  GpioHostOwnGpio, GpioDirIn,    GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},
        {GPIO_VER2_LP_GPP_D14,   {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv, GpioOutDefault,   GpioIntLevel|GpioIntSci,   GpioHostDeepReset,  GpioTermNone,  GpioPadConfigUnlock}}, // SPI_TPM_INT_N
        {GPIO_VER2_LP_GPP_H15, {GpioPadModeNative1,  GpioHostOwnGpio, GpioDirNone,  GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},  //Y250422_Set to DDPB_CTRLCLK
        {GPIO_VER2_LP_GPP_H17, {GpioPadModeNative1,  GpioHostOwnGpio, GpioDirNone,  GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},  //Y250422_Set to DDPB_CTRLDATA
        {0x0}
};
```
2. ELink(没用过)
![[Pasted image 20250425181011.png]]
 3. PPI
 ![[Pasted image 20250425181221.png]]
