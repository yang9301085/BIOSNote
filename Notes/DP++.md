DP++: Dual-Mode DisplayPort,DP接口可插适配器输出HDMI、DVI、VGA等信号
DP所需要的信号：
	1. 至少一组差分信号（TXP，TXN）
	2. AUX信号
	3. HPD信号
差分信号和AUX信号
![[Pasted image 20250324180426.png]]
HPD信号
![[Pasted image 20250324180612.png]]
![[Pasted image 20250324180623.png]]
![[Pasted image 20250324180633.png]]

DP转HDMI、VGA转接器：
	内置芯片可以主动把DP信号转成HDMI或者VGA
	
	
配置DP++ 所要注意的点：
![[Pasted image 20250324181142.png]]
如果DP可以显示，但DP++不行，可以检查以下几个地方：
找硬件看DDC信号，没有看下VBIOS有没有配DDC，有配DDP的话看下对应GPIO有没有复用到对应的function
DP转接器有没有问题，插上DP转接器再接上HDMI后HPD有没有被拉高，没有的话估计DP++转接器有问题

**VGA口如果用DP++配置的话会有问题，对于DP转VGA的配置的话，只需配置DP就行** 



---
当在dsc中把对应的DDC或者DDI的PCD打开之后：
`Intel\AlderLakeBoardPkg\AlderLakeNBoards\BoardVpdPcdsInit\SkuIdAdlNDdr4Rvp.dsc`
```C
  gBoardModuleTokenSpaceGuid.VpdPcdDisplayDdiConfigTable| * |{CODE(
  { 16,
    {
     DdiPortDisabled,         // DDI Port A Config : DdiPortDisabled = No LFP is Connected, DdiPortEdp = eDP, DdiPortMipiDsi = MIPI DSI
     DdiPortDisabled,    // DDI Port B Config : DdiPortDisabled = No LFP is Connected, DdiPortEdp = eDP, DdiPortMipiDsi = MIPI DSI
     DdiHpdEnable,      // DDI Port A HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdEnable,       // DDI Port B HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port C HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdEnable,      // DDI Port 1 HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdEnable,       // DDI Port 2 HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port 3 HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port 4 HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiDdcEnable,         // DDI Port A DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDdcEnable,       // DDI Port B DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,         // DDI Port C DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDdcEnable,         // DDI Port 1 DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,       // DDI Port 2 DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,         // DDI Port 3 DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable          // DDI Port 4 DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
    }
  })}
```

`Intel\AlderLakeBoardPkg\AlderLakeNBoards\Library\BoardInitLib\Pei\BoardSaInitPreMemLib.c`
code会先获取这个PCD然后给到`PcdSaDisplayConfigTable`
```C
/**
  SA Display DDI configuration init function for PEI pre-memory phase.
  @param[in]  VOID
  @retval     VOID
**/
VOID
AdlNSaDisplayConfigInit (
  VOID
  )
{
  VPD_DISPLAY_DDI_CONFIG *SaDisplayDdiConfigTable;
  SaDisplayDdiConfigTable = PcdGetPtr(VpdPcdDisplayDdiConfigTable);
  PcdSet32S (PcdSaDisplayConfigTable, (UINTN) SaDisplayDdiConfigTable->DdiConfigTable);
  PcdSet16S (PcdSaDisplayConfigTableSize, SaDisplayDdiConfigTable->Size);
  return;
}
```
`Intel\AlderLakePlatSamplePkg\Library\PeiPolicyUpdateLib\PeiSaPolicyUpdatePreMem.c`
`PcdSaDisplayConfigTable`这个PCD会给到SaDisplayConfigTable，SaDisplayConfigTable会给到`GtPreMemConfig->DdiConfiguration.DdiPortAConfig`
```C
/**
  UpdatePeiSaPolicyPreMem performs SA PEI Policy initialization
  @retval EFI_SUCCESS              The policy is installed and initialized.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicyPreMem (
  VOID
  )
{
	...
	...
	...
	//
    // Display DDI Initialization ( default Native GPIO as per board during AUTO case)
    //
    CopyMem (SaDisplayConfigTable, (VOID *) (UINTN) PcdGet32 (PcdSaDisplayConfigTable), (UINTN)PcdGet16 (PcdSaDisplayConfigTableSize));
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPortAConfig,          GtPreMemConfig->DdiConfiguration.DdiPortAConfig,       SaDisplayConfigTable[0]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPortBConfig,          GtPreMemConfig->DdiConfiguration.DdiPortBConfig,       SaDisplayConfigTable[1]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPortAHpd,             GtPreMemConfig->DdiConfiguration.DdiPortAHpd,          SaDisplayConfigTable[2]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPortBHpd,             GtPreMemConfig->DdiConfiguration.DdiPortBHpd,          SaDisplayConfigTable[3]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPortCHpd,             GtPreMemConfig->DdiConfiguration.DdiPortCHpd,          SaDisplayConfigTable[4]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPort1Hpd,             GtPreMemConfig->DdiConfiguration.DdiPort1Hpd,          SaDisplayConfigTable[5]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPort2Hpd,             GtPreMemConfig->DdiConfiguration.DdiPort2Hpd,          SaDisplayConfigTable[6]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPort3Hpd,             GtPreMemConfig->DdiConfiguration.DdiPort3Hpd,          SaDisplayConfigTable[7]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPort4Hpd,             GtPreMemConfig->DdiConfiguration.DdiPort4Hpd,          SaDisplayConfigTable[8]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPortADdc,             GtPreMemConfig->DdiConfiguration.DdiPortADdc,          SaDisplayConfigTable[9]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPortBDdc,             GtPreMemConfig->DdiConfiguration.DdiPortBDdc,          SaDisplayConfigTable[10]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPortCDdc,             GtPreMemConfig->DdiConfiguration.DdiPortCDdc,          SaDisplayConfigTable[11]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPort1Ddc,             GtPreMemConfig->DdiConfiguration.DdiPort1Ddc,          SaDisplayConfigTable[12]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPort2Ddc,             GtPreMemConfig->DdiConfiguration.DdiPort2Ddc,          SaDisplayConfigTable[13]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPort3Ddc,             GtPreMemConfig->DdiConfiguration.DdiPort3Ddc,          SaDisplayConfigTable[14]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPort4Ddc,             GtPreMemConfig->DdiConfiguration.DdiPort4Ddc,          SaDisplayConfigTable[15]);
    ...
    ...
    ...
}
```

`Intel\ClientOneSiliconPkg\IpBlock\Graphics\LibraryPrivate\PeiDisplayInitLibGen13\PeiDisplayInitLib.c`

在这个Code中会根据`GtPreMemConfig->DdiConfiguration.DdiPortADdc`把不同的DDC参数传到`GpioEnableDpInterface`中
```C
  ///
  /// Enable DDP CTRLCLK and CTRLDATA pins OR TBT RX and TX pins
  ///
  
  //
  // DDI Port A
  //
  if (GtPreMemConfig->DdiConfiguration.DdiPortADdc == DdiDdcEnable) {
    GpioEnableDpInterface (GpioDdpA);
  }
  
  //
  // DDI Port B
  //
  if (GtPreMemConfig->DdiConfiguration.DdiPortBDdc == DdiDdcEnable) {
    GpioEnableDpInterface (GpioDdpB);
  }

  //
  // DDI Port C
  //
  if (GtPreMemConfig->DdiConfiguration.DdiPortCDdc == DdiDdcEnable) {
    GpioEnableDpInterface (GpioDdpC);
  }

  //
  // DDI Port 1
  //
  if (GtPreMemConfig->DdiConfiguration.DdiPort1Ddc == DdiDdcEnable) {
    GpioEnableDpInterface (GpioDdp1);
  }

  //
  // DDI Port 2
  //
  if (GtPreMemConfig->DdiConfiguration.DdiPort2Ddc == DdiDdcEnable) {
    GpioEnableDpInterface (GpioDdp2);
  }

  //
  // DDI Port 3
  //
  if (GtPreMemConfig->DdiConfiguration.DdiPort3Ddc == DdiDdcEnable) {
    GpioEnableDpInterface (GpioDdp3);
  }

  //
  // DDI Port 4
  //
  if (GtPreMemConfig->DdiConfiguration.DdiPort4Ddc == DdiDdcEnable) {
    GpioEnableDpInterface (GpioDdp4);
  }
```


`Intel\ClientOneSiliconPkg\IpBlock\Gpio\LibraryPrivate\PeiDxeSmmGpioPrivateLib\GpioNativePrivateLib.c`
`GpioEnableDpInterface`中的`GpioGetDdpPins`会根据不同的DDC参数计算GPIO的数值
```C
/**
  This function sets DDP pins into native mode
  @param[in]  DdpInterface   DDPx interface
  @retval Status
**/
EFI_STATUS
GpioEnableDpInterface (
  IN  GPIO_DDP            DdpInterface
  )
{
  EFI_STATUS               Status;
  UINTN                    Index;
  GPIO_PAD_NATIVE_FUNCTION *DdpGpio;

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }

  GpioGetDdpPins (
    DdpInterface,
    &DdpGpio
    );

  if (DdpGpio == NULL) {
    return EFI_UNSUPPORTED;
  }
  
  for (Index = 0; Index < PCH_GPIO_DDP_NUMBER_OF_PINS; Index++) {
    Status = GpioSetPadMode (DdpGpio[Index].Pad, DdpGpio[Index].Mode);
    if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
    }

    Status = GpioConfigurePadIoStandby (DdpGpio[Index].Pad, DdpGpio[Index].IosState, DdpGpio[Index].IosTerm);
    
    if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
    }
  }

  return EFI_SUCCESS;
}
```

`Intel\ClientOneSiliconPkg\IpBlock\Gpio\LibraryPrivate\PeiDxeSmmGpioPrivateLib\GpioNativePrivateLibVer2.c`

```C
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchPDdpInterfacePins[][PCH_GPIO_DDP_NUMBER_OF_PINS] =
{
  {// DDP1
    {GPIO_VER2_LP_GPP_E18, GpioPadModeNative1, GpioIosStateHizRx0, GpioIosTermSame},// DDP1_CTRLCLK
    {GPIO_VER2_LP_GPP_E19, GpioPadModeNative1, GpioIosStateHizRx0, GpioIosTermSame} // DDP1_CTRLDATA
  },
  {// DDP2
    {GPIO_VER2_LP_GPP_E20, GpioPadModeNative1, GpioIosStateHizRx0, GpioIosTermSame},// DDP2_CTRLCLK
    {GPIO_VER2_LP_GPP_E21, GpioPadModeNative1, GpioIosStateHizRx0, GpioIosTermSame} // DDP2_CTRLDATA
  },
  {// DDP3
    {GPIO_VER2_LP_GPP_D9,  GpioPadModeNative2, GpioIosStateHizRx0, GpioIosTermSame},// DDP3_CTRLCLK
    {GPIO_VER2_LP_GPP_D10, GpioPadModeNative2, GpioIosStateHizRx0, GpioIosTermSame} // DDP3_CTRLDATA
  },
  {// DDP4
    {GPIO_VER2_LP_GPP_D11, GpioPadModeNative2, GpioIosStateHizRx0, GpioIosTermSame},// DDP4_CTRLCLK
    {GPIO_VER2_LP_GPP_D12, GpioPadModeNative2, GpioIosStateHizRx0, GpioIosTermSame} // DDP4_CTRLDATA
  },
  {// DDPA
    {GPIO_VER2_LP_GPP_E22, GpioPadModeNative1, GpioIosStateMasked, GpioIosTermSame},// DDPA_CTRLCLK
    {GPIO_VER2_LP_GPP_E23, GpioPadModeNative1, GpioIosStateMasked, GpioIosTermSame} // DDPA_CTRLDATA
  },
  {// DDPB
    {GPIO_VER2_LP_GPP_H15, GpioPadModeNative1, GpioIosStateMasked, GpioIosTermSame},// DDPB_CTRLCLK
    {GPIO_VER2_LP_GPP_H17, GpioPadModeNative1, GpioIosStateMasked, GpioIosTermSame} // DDPB_CTRLDATA
  },
  {// DDPC
    {GPIO_VER2_LP_GPP_A21, GpioPadModeNative1, GpioIosStateMasked, GpioIosTermSame},// DDPC_CTRLCLK
    {GPIO_VER2_LP_GPP_A22, GpioPadModeNative1, GpioIosStateMasked, GpioIosTermSame} // DDPC_CTRLDATA
  }
};


/**
  This function provides DDPx interface pins
  @param[in]  DdpInterface   DDPx interface
  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetDdpPins (
  IN  GPIO_DDP                    DdpInterface,
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable
  )
{
  UINT32  DdpInterfaceIndex;

  switch (DdpInterface) {
    case GpioDdp1:
    case GpioDdp2:
    case GpioDdp3:
    case GpioDdp4:

      if (IsPchLp () || IsPchH () || IsPchP () || IsPchN ()) {
        DdpInterfaceIndex = DdpInterface - GpioDdp1;
      } else {
        goto Error;
      }
      break;
      
    case GpioDdpA:
    case GpioDdpB:
    case GpioDdpC:

      if (IsPchLp () || IsPchH () || IsPchP () || IsPchN ()) {
        DdpInterfaceIndex = (DdpInterface - GpioDdpA) + 4;
      } else {
        goto Error;
      }
      break;
    default:
      goto Error;
  }

  if (IsPchP () || IsPchN ()) {
    if (DdpInterfaceIndex < ARRAY_SIZE (mPchPDdpInterfacePins)) {
      *NativePinsTable = mPchPDdpInterfacePins[DdpInterfaceIndex];
      return;
    }
  } else if (IsPchLp ()) {
    if (DdpInterfaceIndex < ARRAY_SIZE (mPchLpDdpInterfacePins)) {
      *NativePinsTable = mPchLpDdpInterfacePins[DdpInterfaceIndex];
      return;
    }
  } else if (IsPchH ()) {
    if (DdpInterfaceIndex < ARRAY_SIZE (mPchHDdpInterfacePins)) {
      *NativePinsTable = mPchHDdpInterfacePins[DdpInterfaceIndex];
      return;
    }
  }
  
Error:
  *NativePinsTable = NULL;
  ASSERT (FALSE);
}
```