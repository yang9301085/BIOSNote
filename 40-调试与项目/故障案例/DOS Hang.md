DOS hang在进DOS界面

![[DOS Hang-01.jpg]]

在无IO的板子上，要关闭KBC 不然会hang

```SDL
TOKEN
    Name  = "PS2Ctl_SUPPORT"
    Value  = "0"
    Help  = "Main switch to enable PS2 Controller support in the project."
    TokenType = Boolean
    TargetEQU = Yes
    TargetMAK = Yes
End

TOKEN
    Name  = "KBC_SUPPORT"
    Value  = "0"
    Help  = "Enable/Disable KBC support"
    TokenType = Boolean
    TargetEQU = Yes
    TargetMAK = Yes
    TargetH = Yes  
End
```