#PXE进不去
看MAC地址有没有刷
关V-TD: `STR_SA_VTD_PROMPT`
```C
      //
      // Vt-d BIOS Option
      //
      suppressif  ideqval SETUP_VOLATILE_DATA.VTdAvailable == 0;
        grayoutif ideqval ME_SETUP_STORAGE.RemoteSessionActive == 1;
            oneof varid    = SA_SETUP.EnableVtd,
              questionid  = AUTO_ID(SA_VTD_QUESTION_ID),
              prompt      = STRING_TOKEN(STR_SA_VTD_PROMPT),
              help        = STRING_TOKEN(STR_SA_VTD_HELP),
              option text = STRING_TOKEN (STR_ENABLED_STRING),  value = 1, flags = RESET_REQUIRED;
              option text = STRING_TOKEN (STR_DISABLED_STRING), value = 0, flags = DEFAULT | MANUFACTURING | RESET_REQUIRED;
            endoneof;
        endif; //RemoteSessionActive
      endif; //VTdAvailable
```

8111H网卡配置
```C
ELINK
    Name  = "OPROM(20,10EC,8168,OemPkg\\RTLPXE\\rtegpxe8111.lom)"
    Parent  = "CSM_OPROM_LIST"
    InvokeOrder = AfterParent
End

FFS_FILE
    Name  = "OnBoardLanRTL8111HEfi"
    FD_AREA  = "FV_MAIN"
    FILE_Stmt  = "OemPkg\RTLPXE\RtkUndiDxe64.txt"
End

TOKEN
    Name  = "RT8111_ON_BOARD_BAD_ROM_BAR"
    Value  = "1"
    Help  = "Includes RT8111 Expansion ROM BAR into Bad PCI Devices List, for defective On-Board Chip with embedded Option ROM."
    TokenType = Boolean
    TargetH = Yes
    Range  = "ON or OFF default is OFF"
End
```

![[RtkUndiDxe.efi|RTKPXEDriver]]

![[rtegpxe8111.lom|RTXPXEROMfile]]