# IBV LOGO：
![[Pasted image 20231220114350.png]]
**这个Token可以override**

* 所有的FV自己设定的大小和实际所占用的大小都会在build log里面显示出来


FV Space Information
FV_MAIN [99%Full] 24100864 (0x16fc000) total, 24100528 (0x16fbeb0) used, 336 (0x150) free
FV_LOGOROMHOLE [93%Full] 196608 (0x30000) total, 183848 (0x2ce28) used, 12760 (0x31d8) free
FVBLUETOOTHDXEUNCOMPACT [2%Full] 4096 (0x1000) total, 120 (0x78) used, 3976 (0xf88) free
FVNETWORKDXEUNCOMPACT [99%Full] 2576384 (0x275000) total, 2573488 (0x2744b0) used, 2896 (0xb50) free
FVWIFIDXEUNCOMPACT [2%Full] 4096 (0x1000) total, 120 (0x78) used, 3976 (0xf88) free
FVCRYPTODXEUNCOMPACT [92%Full] 16384 (0x4000) total, 15128 (0x3b18) used, 1256 (0x4e8) free
FV_NETWORK [99%Full] 2793472 (0x2aa000) total, 2791032 (0x2a9678) used, 2440 (0x988) free
FV_BB_AFTER_MEMORY [99%Full] 512000 (0x7d000) total, 510808 (0x7cb58) used, 1192 (0x4a8) free
NVRAM [100%Full] 196608 (0x30000) total, 196608 (0x30000) used, 0 (0x0) free
FV_MAIN_WRAPPER [36%Full] 7442432 (0x719000) total, 2684432 (0x28f610) used, 4758000 (0x4899f0) free
FV_NETWORK_WRAPPER [28%Full] 2949120 (0x2d0000) total, 846744 (0xceb98) used, 2102376 (0x201468) free
FV_BB_AFTER_MEMORY_WRAPPER [37%Full] 589824 (0x90000) total, 220640 (0x35de0) used, 369184 (0x5a220) free
FV_FSP_S [51%Full] 720896 (0xb0000) total, 368752 (0x5a070) used, 352144 (0x55f90) free
FV_BB [56%Full] 524288 (0x80000) total, 298064 (0x48c50) used, 226224 (0x373b0) free
FV_DATA_BACKUP [83%Full] 1052672 (0x101000) total, 880640 (0xd7000) used, 172032 (0x2a000) free
FV_DATA [100%Full] 1138688 (0x116000) total, 1138688 (0x116000) used, 0 (0x0) free
FV_BB1_BACKUP [42%Full] 262144 (0x40000) total, 111056 (0x1b1d0) used, 151088 (0x24e30) free
FV_BB1 [42%Full] 262144 (0x40000) total, 111056 (0x1b1d0) used, 151088 (0x24e30) free
FV_BB_AFTER_MEMORY_BACKUP [37%Full] 589824 (0x90000) total, 220640 (0x35de0) used, 369184 (0x5a220) free
FV_FSP_S_BACKUP [51%Full] 720896 (0xb0000) total, 368752 (0x5a070) used, 352144 (0x55f90) free
FV_BB_BACKUP [56%Full] 524288 (0x80000) total, 298064 (0x48c50) used, 226224 (0x373b0) free

* 如果Logo文件过大的话，在build的时候会报错FV Size 。。。。。。
* 改这两个Token扩大logo的FV size
```C
FFS_FILE
	Name  = "AmiTseLogoFfsFdfFileStatement"
	FD_AREA  = "FV_LOGOROMHOLE"
	FILE_Stmt  = "AmiTsePkg/Core/em/AMITSE/Logoffs.txt"
	Token = "TSE_ROMHOLE_SUPPORT" "=" "1"
End

FD_AREA
	Name  = "FV_LOGOROMHOLE"
	TYPE  = "StandAlone"
	FD_INFO  = "AMIROM"
	Size  = "$(ROMHOLE_BLOCK_SIZE) * $(ROMHOLE_NUMBER_OF_BLOCK)"
	Attributes  = "0x7ffe"
	Guid  = "E54D9684-2735-43ef-A379-30F2F592BA10"
	Token = "TSE_ROMHOLE_SUPPORT" "=" "1"
End

TOKEN
	Name  = "ROMHOLE_BLOCK_SIZE"
	Value  = "0x10000"
	Help  = "Size of Block used for ROMHOLE"
	TokenType = Integer
	TargetMAK = Yes
	TargetH = Yes
End

TOKEN
	Name  = "ROMHOLE_NUMBER_OF_BLOCK"
	Value  = "2"
	Help  = "Number of Block used for ROMHOLE"
	TokenType = Integer
	TargetMAK = Yes
	TargetH = Yes
End
```

* 在显示logo的同时显示状况圈
```C
TOKEN
    Name  = "CONTRIB_BGRT_TABLE_TO_ACPI"
    Value  = "1"
    Help  = "Set to Contribute the BGRT table to ACPI."
    TokenType = Integer
    TargetH = Yes
End
```
![[Pasted image 20231220114821.png]]
* 显示客户Logo
* AmiCompatibilityPkg\Setup\AmiTse.sd
![[Pasted image 20231220114858.png]]
![[Pasted image 20231220114903.png]]
![[Pasted image 20231220114907.png]]