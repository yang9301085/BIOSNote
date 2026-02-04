客户要求BIOS 默认dTPM，BIOS UI选项中设定dTPM默认后无效，刷BIOS后第一次开机仍然是PTT(fTPM)，F9 load default后变成dTPM

根本原因：

AMI有override机制，会把TPM强制设置成fTPM
``Intel\AlderLakePlatSamplePkg\Setup\MeSetup.c``

```C
/**
  Initialize ME strings.

  @param[in]  HiiHandle          Hii Handle.
  @param[in]  Class              Setup form class.
**/
VOID
InitMeInfo (
  IN EFI_HII_HANDLE HiiHandle,
  IN UINT16         Class
  )
{

//////////////////
////Other code////
//////////////////

// APTIOV_OVERRIDE_RC_START : Fix ME status
  PttHeciGetCapability (&PttCapability);
  PttHeciGetState (&PttCurrentState);
  
  InitString (
   mHiiHandle,
   STRING_TOKEN (STR_PTT_CAP_STATE_VALUE),
   L"%d / %d",
   PttCapability,
   PttCurrentState
   );

  VariableSize = sizeof (AMI_WRAPPER_SETUP);
  Status = gRT->GetVariable (
                  L"AmiWrapperSetup",
                  &gAmiWrapperSetupVariableGuid,
                  &Attributes,
                  &VariableSize,
                  &mAmiWrapperSetup
                  );  
/**
 *Mark-Start
//   // Sync ME setup data TpmDeviceSelection with Me PttCurrentState  
  if (PttCurrentState != mAmiWrapperSetup.TpmDeviceSelection) {
      mAmiWrapperSetup.TpmDeviceSelection = PttCurrentState;
      Status = gRT->SetVariable (
                      L"AmiWrapperSetup",
                      &gAmiWrapperSetupVariableGuid,
                      Attributes,
                      VariableSize,
                      &mAmiWrapperSetup
                      );
  }
 *Mark-End
 *
 */
// APTIOV_OVERRIDE_RC_END

//////////////////
////Other code////
//////////////////
}
```

将这里mark掉后刷BIOS后PTT会默认dTPM

但还会有其他问题：
刷BIOS后第一次开机后PTT选项虽然是dTPM，但TPM信息显示的还是fTPM的信息，也就是说，虽然BIOS选项生效了，但dTPM没生效

根本原因：
![[Pasted image 20251128181222.png]]

Intel(R) PTT initial power-up state
This setting determines if Intel(R) PTT is enabled on platform power-up.
当ME中这个选项打开的时候，BIOS在第一次Initial ME的时候会initial fTPM，所以虽然BIOS在DXE阶段设置了dTPM，但不会切换成dTPM


