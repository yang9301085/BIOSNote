# Display Configuration Tool

![[DisCon_UserGuide.pdf]]
# GOP in BIOS Code

```
#GOP-start
TOKEN
    Name  = "OEM_PLATFROM_1_INTEL_GOP_VBT_BIN_FILE"
    Value  = "OemPkg/ITX_N100_2L/Vbt.bin"
    Help  = "This file guid is followed gIntelSiliconPkgTokenSpaceGuid.PcdIntelGraphicsVbtFileGuid"
    TokenType = Expression
    TargetMAK = Yes
    TargetFDF = Yes
End

TOKEN
    Name  = "OEM_INTEL_GOP_VBT_BIN_FILE"
    Value  = "OemPkg/ITX_N100_2L/Vbt.bin"
    TokenType = Expression
    TargetMAK = Yes
    TargetFDF = Yes
End
#GOP-end
```
# Issues

PWM 调光
![[dd73b69743a590ddb714c7afd4a27ab.png]]