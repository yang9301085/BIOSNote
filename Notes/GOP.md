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
现象：LVDS屏最低亮度时黑屏

PWM 调光
![[dd73b69743a590ddb714c7afd4a27ab.png]]

![[1720514132179.png]]
对应VBIOS设置 
![[Pasted image 20240709163641.png]]

---
### HDMI Port 1 无显

B760上有两个on board HDMI port (DP_HDMI+HDMI only)，DP_HDMI port可以在BIOS下显示，在OS下实现Hut Plug，HDMI 1 port在BIOS&OS下无显无功能
对应线路图：
![[Pasted image 20240709164217.png]]
![[Pasted image 20240709164430.png]]
![[Pasted image 20240709164452.png]]
![[Pasted image 20240709164519.png]]
![[Pasted image 20240710091450.png]]
![[Pasted image 20240710091520.png]]
CPU中对应Display port
![[Pasted image 20240710091730.png]]
![[Pasted image 20240710091814.png]]
#### Root Cause:
VBIOS 中没有设置HDMI 1 port
#### Debug:
HE load HDMI 相关GPIO \[GPP_I3],\[GPP_R12],\[GPP_R13]，检查确认GPIO复用功能正确
但是<BR>HDMI线插拔 HDMI 1port时发现HPD\[GPP_I3]无变化，\[GPP_R12],\[GPP_R13]的RX位置也为0
插拔DP_HDMI port时，相关GPIO有变化，并且\[GPP_R15]\[GPP_R16]的RX位置为1
怀疑VBIOS没有设置正确
检查VBIOS设置发现VBIOS中没有对HDMI 1Port进行设置
把HDMI 1 port设置进VBIOS后HDMI 1 port口功能正常
![[Pasted image 20240710161350.png]]
