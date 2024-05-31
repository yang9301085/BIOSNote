```
........Serial reg: IER:00 IIR:01 LCR:00 MCR:00 LSR:60
Serial initialization is successful
FSP-T: CAR Init
PrintPeiCoreEntryPointParam in PlatformInit
FspHobList - 0x0
StartOfRange - 0xFEF00000
EndOfRange - 0xFEFFFF00

FSP Wrapper BootFirmwareVolumeBase - 0xFFEE0000
FSP Wrapper BootFirmwareVolumeSize - 0x120000
FSP Wrapper TemporaryRamBase       - 0xFEF00000

FSP Wrapper TemporaryRamSize       - 0xFFF00
FSP Wrapper PeiTemporaryRamBase    - 0xFEF00000
FSP Wrapper PeiTemporaryRamSize    - 0x7FF80
FSP Wrapper StackBase              - 0xFEF7FF80
FSP Wrapper StackSize              - 0x7FF80
FSP CPUID                          - 0x000B06F5
FSP PatchID                        - 0x00000032

SecStartupPhase2() Stack Base: 0xFEF7FF80, Stack Size: 0x7FF80
PeiCore.Entry(FFF1C07C)

Register PPI Notify: EfiPeiSecurity2
Install PPI: 8C8CE578-8A3D-4F1C-9935-896185C32DD3
Install PPI: 5473C07A-3DCB-4DCA-BD6F-1E9689E7349A
The 0th FV start address is 0x000FFEE0000, size is 0x00120000, handle is 0xFFEE0000
Register PPI Notify: EfiPeiFirmwareVolumeInfo
Register PPI Notify: EfiPeiFirmwareVolumeInfo2
Install PPI: EfiPeiLoadFile
Register PPI Notify: PeiSecPerformance
Install PPI: EfiTemporaryRamDone
Install PPI: EfiSecPlatformInformation
Install PPI: TopOfTemporaryRam
Install PPI: FC4DD4F2-179E-41F8-9D6D-FAD6F9D7B8B9
Install PPI: PeiSecPerformance
Notify: PPI: PeiSecPerformance, Peim notify entry point: FFFF9000
SecGetPerformance
FPDT: SEC Performance Hob ResetEnd = 100586066
DiscoverPeimsAndOrderWithApriori(): Found 0x2F PEI FFS files in the 0th FV
Loading PEIM E9DD7F62-25EC-4F9D-A4AB-AAD20BF59A10
StatusCodePei.Entry(FFF9AB60)
Install PPI: AmiDebugService

```