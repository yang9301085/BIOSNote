相关知识点：
[[LoadImage&StartImage]]
客户需求在开机过程中按F9进WinRE
也就是这个界面
![[c0d56aa28ead11696ced6222451f137a.jpg]]

微软有相关说明：
![[添加硬件恢复按钮以启动 Windows RE _ Microsoft Learn.pdf]]


> 你仍然需要在 UEFI 固件启动顺序列表的末尾，为恢复创建一个静态启动设备条目。
此启动设备条目应指向 ESP 上文件夹 `\EFI\Microsoft\Boot` 中的默认 Windows 启动管理器 (bootmgfw.efi)。
启动设备条目必须指定 `/RecoveryBCD` 参数。

也就是说： bootmgfw.efi是可以接受参数的
我在shell下也试了下这个命令：
`EFI\Microsoft\Boot\bootmgfw.efi /RecoveryBCD
执行后会跳到WinRE页面

所以思路很清晰

开机POST ---> 按F9 ---> UEFI执行跳转WinRE命令

按键监听和Hook 函数AMI都已经写好了，只要写具体实现就可以

先给TOKEN打开，然后设置按键F9
```C
TOKEN
    Name  = "SETUP_OEM_KEY1_ENABLE"
    Value  = "1"
    Help  = "Enable or disable the SETUP_OEM_KEY1"
    TokenType = Boolean
    TargetH = Yes
End

TOKEN
    Name  = "SETUP_OEM_KEY1_SCAN"
    Value  = "EFI_SCAN_F9"
    Help  = "Set to one of the defined constants from EFI_SIMPLE_TEXT_INPUT protocol"
    TokenType = Expression
    TargetH = Yes
    Token = "SETUP_OEM_KEY1_ENABLE" "!=" "0"
End
```

在`AmiTsePkg\SkiaPkg\BootOnlyTseBin\BootOnlyTseBinCommonOem.c`中会监听按键然后走不同的bootflow
```C
VOID CheckForKey (EFI_EVENT Event, VOID *Context)
{
	Status = pKeyCodeProtocol->ReadEfikey( pKeyCodeProtocol, &AmiKey );
//other code
	#if SETUP_OEM_KEY1_ENABLE
            else if (CheckOEMKey(&AmiKey, SETUP_OEM_KEY1_UNICODE, SETUP_OEM_KEY1_SCAN,
                     SETUP_OEM_KEY1_EFIKEY, SETUP_OEM_KEY1_SHIFT, SETUP_OEM_KEY1_TOGGLE))
                gBootFlow = BOOT_FLOW_CONDITION_OEM_KEY1;
#endif
//other code
}
```

然后在`AmiTsePkg\Core\em\AMITSE\bootflow.c`中会根据不同的bootflow执行不同的callback
```C
static BOOT_FLOW _gBootFlowTable[] =
{
//  { Condition,                                    PageClass,              PageSubClass,   PageFormID, ControlNumber,  MessageBoxToken,    MessageBoxTimeout,  GotoPageOnEntry,    ProceedBooting, InfiniteLoop,                               LaunchShell,    DoNotEnterSetup,    CallbackFunction },
    { BOOT_FLOW_CONDITION_NORMAL,                   0,                      0,              0,          0,              0,                  0,                  FALSE,              TRUE,           BOOT_FLOW_NORMAL_INFINITE_LOOP,             BOOT_FLOW_NORMAL_LAUNCH_DEFAULT_BOOTIMAGE,  FALSE,              NULL },
    { BOOT_FLOW_CONDITION_ERROR,                    ERROR_MANAGER_KEY_ID,   0,              1,          0,              0,                  0,                  TRUE,               FALSE,          FALSE,                                      TRUE,                                       FALSE,              NULL },
    { BOOT_FLOW_CONDITION_RECOVERY,                 0x40,                   0,              1,          0,              0,                  0,                  TRUE,               TRUE,           FALSE,                                      TRUE,                                       FALSE,              NULL },
    { BOOT_FLOW_CONDITION_PCI_OUT_OF_RESOURCE,      0x79,                   0,              1,          0,              0,                  0,                  TRUE,               TRUE,           FALSE,                                      TRUE,                                       FALSE,              NULL },
    { BOOT_FLOW_CONDITION_FIRST_BOOT,               MAIN_FORM_SET_CLASS,    0,              MAIN_MAIN,  0,              0,                  0,                  TRUE,               TRUE,           FALSE,                                      TRUE,                                       FALSE,              NULL },
    { BOOT_FLOW_CONDITION_OS_UPD_CAP,               0,                      0,              0,          0,              0,                  0,                  FALSE,              TRUE,           FALSE,                                      FALSE,                                      FALSE,              &OsUpdateCapsuleWrap },
    { BOOT_FLOW_HOTKEY_BOOT,                        0,                      0,              0,          0,              0,                  0,                  FALSE,              TRUE,           FALSE,                                      FALSE,                                      FALSE,              &LaunchHotKeyBootOption },
#if SETUP_OEM_KEY1_ENABLE

    { BOOT_FLOW_CONDITION_OEM_KEY1,                 0,                      0,              0,          0,              0,                  0,                  FALSE,              TRUE,           FALSE,                                      FALSE,                                      FALSE,              &OemKey1HookHook },
#endif
#if SETUP_OEM_KEY2_ENABLE
    { BOOT_FLOW_CONDITION_OEM_KEY2,                 0,                      0,              0,          0,              0,                  0,                  FALSE,              TRUE,           FALSE,                                      FALSE,                                      FALSE,              &OemKey2HookHook },
#endif
#if SETUP_OEM_KEY3_ENABLE
    { BOOT_FLOW_CONDITION_OEM_KEY3,                 0,                      0,              0,          0,              0,                  0,                  FALSE,              TRUE,           FALSE,                                      FALSE,                                      FALSE,              &OemKey3HookHook },
#endif
#if SETUP_OEM_KEY4_ENABLE
    { BOOT_FLOW_CONDITION_OEM_KEY4,                 0,                      0,              0,          0,              0,                  0,                  FALSE,              TRUE,           FALSE,                                      FALSE,                                      FALSE,              &OemKey4HookHook },
#endif
#if SETUP_BBS_POPUP_ENABLE
    { BOOT_FLOW_CONDITION_BBS_POPUP,                0,                      0,              0,          0,              0,                  0,                  FALSE,              TRUE,           FALSE,                                      FALSE,                                      FALSE,              &DoPopup },
#endif
    { BOOT_FLOW_CONDITION_OEM_KEY_CALLBACK,         0,                      0,              0,          0,              0,                  0,                  FALSE,              TRUE,           FALSE,                                      FALSE,                                      FALSE,              NULL }, // Callback is null and on OEMkey press it will updated.
    { BOOT_FLOW_CONDITION_NO_SETUP,                 0,                      0,              0,          0,              0,                  0,                  FALSE,              TRUE,           TRUE,                                       FALSE,                                      TRUE,               NULL },
#if FAST_BOOT_SUPPORT
    { BOOT_FLOW_CONDITION_FAST_BOOT,                0,                      0,              0,          0,              0,                  0,                  FALSE,              TRUE,           FALSE,                                      TRUE,                                       TRUE,               &FBBootFlow },
#endif
    { BOOT_FLOW_CONDITION_SECURITY,                MAIN_FORM_SET_CLASS,    0,              SECURITY_MAIN,  0,           0,                  0,                  TRUE,               TRUE,           FALSE,                                      TRUE,                                       FALSE,              NULL },
    // this MUST be the last entry in the boot flow table
    { BOOT_FLOW_CONDITION_NULL,                     0,                      0,              0,          0,              0,                  0,                  FALSE,              TRUE,           FALSE,                                      TRUE,                                       FALSE,              NULL }
};
```
所以
```C
{ BOOT_FLOW_CONDITION_OEM_KEY1, 0, 0, 0, 0, 0, 0, FALSE, TRUE, FALSE, FALSE, FALSE, &OemKey1HookHook },
```
`OemKey1HookHook`就是要实现F9跳转WinRE的地方

`AmiTsePkg\SkiaPkg\BootOnlyTseBin\BootOnlyTseBinCommonOem.c`

```C
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	OemKey1Hook
//
// Description:	This function is a hook called when user activates
//              configurable post hot key 1. This function is
//              available as ELINK. Generic implementation is empty.
//              OEMs may choose to use different logic here.
//
// Input:		bootFlowPtr: Boot flow entry that triggered this call
//
// Output:		always EFI_SUCCESS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS	OemKey1Hook ( BOOT_FLOW *bootFlowPtr )
{
	EFI_STATUS Status = EFI_SUCCESS;
	//
	// TODO:: Add hook
    //Y251203_Add WINRE Test-Start
    TseSerialDebugPrint("[F9_OemKey1Hook] Start\n");
    // Entry trace with function name and parameter
    TseSerialDebugPrint("[%a] Entry\n", 
                        __FUNCTION__);
    EFI_HANDLE Handle = NULL;
    EFI_DEVICE_PATH_PROTOCOL *EspDp = NULL;
    EFI_LOADED_IMAGE_PROTOCOL *WINREImage=NULL;
    UINT32 Size=0;
    CHAR16* WinReOptionalData= L"/RecoveryBCD";
    UINT32  OptSize  = (UINT32) StrSize (WinReOptionalData);

    UINTN OptionSize, BootOrderSize,i;
    UINT16 *BootOrder=NULL;
    EFI_LOAD_OPTION *Option=NULL;
    CHAR16 BootVarName[15]; //Boot0000

    Status = GetEfiVariable(
        L"BootOrder", &gEfiGlobalVariableGuid, NULL, &BootOrderSize, (VOID**)&BootOrder
    );

    if (EFI_ERROR(Status)){
        TseSerialDebugPrint("[%a] ERROR: GetEfiVariable BootOrder failed!\n", __FUNCTION__);
        TseSerialDebugPrint("[%a]   Status = %r\n", __FUNCTION__, Status);
         return Status;
    }
    TseSerialDebugPrint("Entrying BootOrder loop-Start\n");
    for(i=0; i< BootOrderSize/sizeof(UINT16); i++){
        UnicodeSPrint(BootVarName, 15*sizeof(CHAR16), L"Boot%04X",BootOrder[i]);
        TseSerialDebugPrint("BootVarName[%d] is [%a]\n",i,BootVarName);
        Status = GetEfiVariable(
            BootVarName, &gEfiGlobalVariableGuid, NULL, &OptionSize, (VOID**)&Option
        );
        if (EFI_ERROR(Status)){
            TseSerialDebugPrint("[%a] ERROR: GetEfiVariable BootVarName failed!\n", __FUNCTION__);
            TseSerialDebugPrint("[%a]   Status = %r\n", __FUNCTION__, Status);
            TseSerialDebugPrint("Trying lowercase BootVarName...\n");
            // Workaround for non-UEFI specification complaint OS.
            // Some OS create BootXXXX variables using lower case letters A-F.
            // Search for lower case boot option variable if no upper case variable found.
            UnicodeSPrint(BootVarName, 15*sizeof(CHAR16), L"Boot%04x",BootOrder[i]);
            Status=GetEfiVariable(
                BootVarName, &gEfiGlobalVariableGuid, NULL, &OptionSize,(VOID**)&Option
            );
            if (EFI_ERROR(Status)) {
                TseSerialDebugPrint("[%a] ERROR: GetEfiVariable lowercase BootVarName also failed!\n", __FUNCTION__);
                TseSerialDebugPrint("[%a]   Status = %r\n", __FUNCTION__, Status);
                continue;
            }
            TseSerialDebugPrint("Lowercase BootVarName[%d] is [%a]\n",i,BootVarName);
        }
        TseSerialDebugPrint("Trying to boot BootOrder[%d]: Boot%04X\n",i,BootOrder[i]);
        Boot(Option,BootOrder[i],OptionSize);
    }
    TseSerialDebugPrint("Entrying BootOrder loop-End\n");
    pBS->FreePool(BootOrder);
    pBS->FreePool(Option);
    // Exit trace with status
    TseSerialDebugPrint("[%a] Exit, Status=%r\n", 
                        __FUNCTION__, 
                        Status);
    TseSerialDebugPrint("[F9_OemKey1Hook] End\n");
    //Y251203_Add WINRE Test-End
	return Status;
}
```

其实麻烦的地方在于loadimage需要device path这个protocol
所以直接把BDS模块和boot相关的函数挪过来，因为AMI 的bds模块没做lib，所以想用头文件反倒麻烦

```C
/**
 * Attempt to boot the device associated with the passed EFI_LOAD_OPTION.
 *
 * @param BootOption Pointer to the EFI_LOAD_OPTION that will attempt to be booted
 * @param Number The BootXXXX number that will attempt to be booted
 * @param Size The size of the optional data in the BootOption
 *
 * @retval EFI_UNSUPPORTED the device cannot be booted
 */
EFI_STATUS
Boot (
  EFI_LOAD_OPTION *BootOption,
  UINT16          Number,
  UINTN           Size
  )
{
    CHAR16 *WinReOptionalData = L"/RecoveryBCD";
    UINT32 OptSize            = (UINT32)StrSize(WinReOptionalData);

    TseSerialDebugPrint("Boot function Start\n");

    EFI_DEVICE_PATH_PROTOCOL *Dp;
    Dp = (EFI_DEVICE_PATH_PROTOCOL *)(
             // skip the header
             (UINT8 *)(BootOption + 1)
             // skip the string
           + (Wcslen((CHAR16 *)(BootOption + 1)) + 1) * sizeof(CHAR16)
         );

    TseSerialDebugPrint("Boot Device Path Type: %x, SubType: %x\n", Dp->Type, Dp->SubType);

    if (Dp->Type != BBS_DEVICE_PATH) {
        TseSerialDebugPrint("Booting EFI Device Path\n");

        // 原来的 pOptions 没用了，保留一行便于以后切换：
        // UINT8 *pOptions = (UINT8*)Dp + BootOption->FilePathListLength;

        return BootEfi(Dp, Number, WinReOptionalData, OptSize);
    }

#if CSM_SUPPORT
    else {
        return BootLegacy(Dp, Number);
    }
    TseSerialDebugPrint("Boot function End\n");
#else
    return EFI_UNSUPPORTED;
#endif
}

```

```C

EFI_STATUS
GetESPDevicePath (
  EFI_DEVICE_PATH_PROTOCOL **Dp
  )
{
    EFI_STATUS               Status;
    EFI_DEVICE_PATH_PROTOCOL *DevicePath;
    EFI_HANDLE               *HandleBuffer;
    UINTN                     HandleCount;
    UINTN                     Index;
    EFI_BLOCK_IO_PROTOCOL    *BlockIo;
    EFI_DEVICE_PATH_PROTOCOL *EspDp = NULL;

    Status = pBS->LocateHandleBuffer(
                   ByProtocol,
                   &gEfiBlockIoProtocolGuid,
                   NULL,
                   &HandleCount,
                   &HandleBuffer
                 );
    if (EFI_ERROR(Status)) {
        return Status;
    }

    for (Index = 0; Index < HandleCount; Index++) {
        Status = pBS->HandleProtocol(
                       HandleBuffer[Index],
                       &gEfiBlockIoProtocolGuid,
                       (VOID **)&BlockIo
                     );
        if (EFI_ERROR(Status)) {
            continue;
        }

        if (BlockIo->Media->BlockSize == 512) {
            Status = pBS->HandleProtocol(
                           HandleBuffer[Index],
                           &gEfiDevicePathProtocolGuid,
                           (VOID **)&DevicePath
                         );
            if (EFI_ERROR(Status)) {
                continue;
            }

            EspDp = DevicePath;
            break;
        }
    }

    gBS->FreePool(HandleBuffer);
    *Dp = EspDp;
    return EFI_SUCCESS;
}

```

```C
EFI_DEVICE_PATH_PROTOCOL *
BuildBootmgfwDevicePath (
  EFI_DEVICE_PATH_PROTOCOL *EspDp
  )
{
    EFI_DEVICE_PATH_PROTOCOL *FilePathNode;
    EFI_DEVICE_PATH_PROTOCOL *BootmgfwDp;
    // UINTN Length;

    FilePathNode = (EFI_DEVICE_PATH_PROTOCOL *)EfiLibAllocateZeroPool(
                     sizeof(EFI_DEVICE_PATH_PROTOCOL) +
                     StrSize(L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi")
                   );
    if (FilePathNode == NULL) {
        return NULL;
    }

    FilePathNode->Type      = MEDIA_DEVICE_PATH;
    FilePathNode->SubType   = MEDIA_FILEPATH_DP;
    FilePathNode->Length[0] =
      (UINT8)(sizeof(EFI_DEVICE_PATH_PROTOCOL) +
              StrSize(L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi"));
    FilePathNode->Length[1] = 0;

    StrCpyS(
      (CHAR16 *)(FilePathNode + 1),
      StrSize(L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi") / sizeof(CHAR16),
      L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi"
      );

    BootmgfwDp = AppendDevicePathNode(EspDp, FilePathNode);
    return BootmgfwDp;
}

```

```C

/**
 * Attempt to Efi boot the device associated with the passed Device path
 *
 * @param Dp The device path of the device that will attempt to be booted
 * @param Number The BootXXXX of the device that will attempt to be booted
 * @param pOptions The optional parameters to pass to the boot device
 * @param Size The size of the optional parameters
 *
 * @note If booting the device is successful, the control will never be returned
 * to this function from the ->StartImage call
 */
EFI_STATUS
BootEfi (
  EFI_DEVICE_PATH_PROTOCOL *Dp,
  UINT16                    Number,
  VOID                     *pOptions,
  UINT32                    Size
  )
{
    TseSerialDebugPrint(
      "BootEfi: >>> Start, Boot%04x, Dp=%p, Options=%p, Size=%u\n",
      Number, (VOID *)Dp, pOptions, Size
      );

    EFI_STATUS                Status;
    EFI_HANDLE                Handle;
    EFI_LOADED_IMAGE_PROTOCOL *Image;

    Status = pBS->LoadImage(TRUE, TheImageHandle, Dp, NULL, 0, &Handle);
    TseSerialDebugPrint(
      "BootEfi: First LoadImage(Dp=%p) -> Status=0x%lx\n",
      (VOID *)Dp, (UINT64)Status
      );

    // If LoadImage has failed, try resolving short device path starting with HD device path node.
    if (EFI_ERROR(Status) &&
        Dp != NULL &&
        Dp->Type == MEDIA_DEVICE_PATH &&
        Dp->SubType == MEDIA_HARDDRIVE_DP)
    {
        TseSerialDebugPrint(
          "BootEfi: LoadImage failed, Dp is short HD node, try _DiscoverPartition\n"
          );

        EFI_DEVICE_PATH_PROTOCOL *FullDp = _DiscoverPartition(Dp);
        TseSerialDebugPrint(
          "BootEfi: _DiscoverPartition(%p) -> FullDp=%p\n",
          (VOID *)Dp, (VOID *)FullDp
          );

        if (FullDp != NULL) {
            Dp     = FullDp;
            Status = pBS->LoadImage(TRUE, TheImageHandle, Dp, NULL, 0, &Handle);
            TseSerialDebugPrint(
              "BootEfi: Second LoadImage(FullDp=%p) -> Status=0x%lx\n",
              (VOID *)Dp, (UINT64)Status
              );
        }
    }

    // If LoadImage has failed, try the default OS loader path.
    if (EFI_ERROR(Status)) {
        TseSerialDebugPrint(
          "BootEfi: LoadImage still failed, try default OS loader path (%s)\n",
          "EFI_REMOVABLE_MEDIA_FILE_NAME"
          );

        UINT8 FileNodeBuffer[
          sizeof(EFI_DEVICE_PATH_PROTOCOL) +
          sizeof(EFI_REMOVABLE_MEDIA_FILE_NAME)
        ];
        EFI_DEVICE_PATH_PROTOCOL *FileNode =
          (EFI_DEVICE_PATH_PROTOCOL *)FileNodeBuffer;

        FileNode->Type      = MEDIA_DEVICE_PATH;
        FileNode->SubType   = MEDIA_FILEPATH_DP;
        FileNode->Length[0] =
          (UINT8)(sizeof(EFI_DEVICE_PATH_PROTOCOL) +
                  sizeof(EFI_REMOVABLE_MEDIA_FILE_NAME));
        FileNode->Length[1] = 0;

        StrnCpyS(
          (CHAR16 *)(FileNode + 1),
          sizeof(EFI_REMOVABLE_MEDIA_FILE_NAME) / sizeof(CHAR16),
          EFI_REMOVABLE_MEDIA_FILE_NAME,
          sizeof(EFI_REMOVABLE_MEDIA_FILE_NAME) / sizeof(CHAR16)
          );

        TseSerialDebugPrint(
          "BootEfi: Append file path node '%s' to Dp=%p\n",
          "EFI_REMOVABLE_MEDIA_FILE_NAME", (VOID *)Dp
          );

        Dp = DPAddNode(Dp, FileNode);
        TseSerialDebugPrint(
          "BootEfi: DPAddNode -> New Dp=%p\n",
          (VOID *)Dp
          );

        Status = pBS->LoadImage(TRUE, TheImageHandle, Dp, NULL, 0, &Handle);
        TseSerialDebugPrint(
          "BootEfi: Third LoadImage(With default path) -> Status=0x%lx\n",
          (UINT64)Status
          );

        pBS->FreePool(Dp);

        if (EFI_ERROR(Status)) {
            TseSerialDebugPrint(
              "BootEfi: All LoadImage attempts failed, return 0x%lx\n",
              (UINT64)Status
              );
            return Status;
        }
    }

    Status = pBS->HandleProtocol(
                    Handle,
                    &gEfiLoadedImageProtocolGuid,
                    (VOID **)&Image
                  );
    TseSerialDebugPrint(
      "BootEfi: HandleProtocol(LoadedImage) -> Status=0x%lx, Image=%p\n",
      (UINT64)Status, (VOID *)Image
      );

    if (EFI_ERROR(Status)) {
        TseSerialDebugPrint(
          "BootEfi: HandleProtocol failed, return 0x%lx\n",
          (UINT64)Status
          );
        return Status;
    }

    TseSerialDebugPrint(
      "BootEfi: ImageCodeType=0x%x (expect EfiLoaderCode=%d)\n",
      Image->ImageCodeType, EfiLoaderCode
      );

    if (Image->ImageCodeType != EfiLoaderCode) {
        TseSerialDebugPrint(
          "BootEfi: Image is not EfiLoaderCode, return EFI_UNSUPPORTED\n"
          );
        return EFI_UNSUPPORTED;
    }

    if (Size) {
        TseSerialDebugPrint(
          "BootEfi: Set LoadOptions, Size=%u, Options=%p\n",
          Size, pOptions
          );
        Image->LoadOptionsSize = Size;
        Image->LoadOptions     = pOptions;
    } else {
        TseSerialDebugPrint(
          "BootEfi: No LoadOptions provided (Size=%u, Options=%p)\n",
          Size, pOptions
          );
    }

    TseSerialDebugPrint("BootEfi: Call ReadyToBoot(%04x)\n", Number);
    ReadyToBoot(Number);
    PERF_END(NULL, "BDS", NULL, 0);

    TseSerialDebugPrint(
      "BootEfi: <<< Calling StartImage(), Handle=%p\n",
      Handle
      );

    Status = pBS->StartImage(Handle, NULL, NULL);
    TseSerialDebugPrint(
      "BootEfi: StartImage returned! Status=0x%lx\n",
      (UINT64)Status
      );

    return Status;
}

```

```C
/**
 * Attempt to boot to the legacy boot device associated with the Device path and
 * the BootXXXX variable in Number.
 *
 * @param Dp Pointer to the device path that will attempt to be booted (BBS device path)
 * @param Number The BootXXXX number that will attempt to be booted
 *
 * @retval EFI_NOT_FOUND The legacy boot device did not exist
 *
 * @note If this function is successful, control will never be returned back to the function
 * from the LegacyBios->LegacyBoot function call.
 */
EFI_STATUS
BootLegacy (
  EFI_DEVICE_PATH_PROTOCOL *Dp,
  UINT16                    Number
  )
{
    UINTN i, Old = MAX_UINTN, New = MAX_UINTN, Priority = MAX_UINTN;

    BBS_BBS_DEVICE_PATH *BbsEntry = (BBS_BBS_DEVICE_PATH *)Dp;
    EFI_STATUS           Status;
    EFI_LEGACY_BIOS_PROTOCOL *LegacyBios;
    UINT16               HddCount;
    UINT16               BbsCount;
    HDD_INFO            *HddInfo;
    BBS_TABLE           *BbsTable;

    // Legacy Boot
    Status = pBS->LocateProtocol(
                   &gEfiLegacyBiosProtocolGuid,
                   NULL,
                   (VOID **)&LegacyBios
                 );
    if (EFI_ERROR(Status)) {
        return Status;
    }

    LegacyBios->GetBbsInfo(
      LegacyBios,
      &HddCount,
      &HddInfo,
      &BbsCount,
      &BbsTable
      );
    if (!BbsCount) {
        return EFI_NOT_FOUND;
    }

    for (i = 0; i < BbsCount; i++) {
        if (BbsTable[i].BootPriority == BBS_IGNORE_ENTRY ||
            BbsTable[i].BootPriority == BBS_DO_NOT_BOOT_FROM)
        {
            continue;
        }

        if (!BbsTable[i].BootPriority && Old == (UINTN)-1) {
            Old = i;
        }

        if (BbsTable[i].DeviceType == BbsEntry->DeviceType &&
            BbsTable[i].BootPriority < Priority)
        {
            Priority = BbsTable[i].BootPriority;
            New      = i;
        }
    }

    if (New == (UINTN)-1) {
        return EFI_NOT_FOUND;
    }

    if (Old != (UINTN)-1) {
        BbsTable[Old].BootPriority = BbsTable[New].BootPriority;
    }
    BbsTable[New].BootPriority = 0;

    ReadyToBoot(Number);
    PERF_END(NULL, "BDS", NULL, 0);

    return LegacyBios->LegacyBoot(
                         LegacyBios,
                         (BBS_BBS_DEVICE_PATH *)Dp,
                         0,
                         0
                       );
}

```

```C
/**
 * Update the L"BootCurrent" NVRAM variable and signal a ready-to-boot event for the BootXXXX
 * variable associated with the passed OptionNumer.
 *
 * @param OptionNumber The BootXXXX number that will attempt to be booted
 */
VOID
ReadyToBoot (
  UINT16 OptionNumber
  )
{
    // signal EFI_EVENT_SIGNAL_READY_TO_BOOT
    EFI_EVENT  ReadyToBootEvent;
    EFI_STATUS Status;

    if (OptionNumber != MAX_UINT16) {
        pRS->SetVariable(
              L"BootCurrent",
              &gEfiGlobalVariableGuid,
              EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
              sizeof(OptionNumber),
              &OptionNumber
              );
    }

    Status = CreateReadyToBootEvent(
               TPL_CALLBACK,
               NULL,
               NULL,
               &ReadyToBootEvent
             );
    if (!EFI_ERROR(Status)) {
        pBS->SignalEvent(ReadyToBootEvent);
        pBS->CloseEvent(ReadyToBootEvent);
    }

    //J221011+S
    {
        //9F0->F1;A00->F2;A10->F3;A20->F4
        //J221123-      MmioWrite32(0xE06A09F0, 0x84000201); //GPP_F1
    }
    //J221011+E

    PROGRESS_CODE(DXE_READY_TO_BOOT);

    //J221123+S
#if AUTO_LEGACY_VBIOS_SUPPORT
    IoWrite8(0x70, 0x53);
    IoWrite8(0x71, 0);
#endif
    //J221123+e
}

```