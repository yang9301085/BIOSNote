## Intel High Definition Audio SPEC

![[high-definition-audio-specification.pdf]]

## AUDIO 电路图
### overview

![[Pasted image 20231207161534.png]]

### Realtek ALC662 

#### ALC662 Datasheet
![[ALC662-GR.pdf]]
#### Circuit Diagram
![[Pasted image 20231207161817.png]]

#### Pin Assignment

![[Pasted image 20231207162319.png]]
##### Pin description
###### Digital I/O Pins

![[Pasted image 20231207162918.png]]

###### Analog I/O Pins

![[Pasted image 20231207163021.png]]
Continue...
![[Pasted image 20231207163131.png]]



## BIOS Code中VerbTable配置 

```c
/**
  Azalia verb table header
  Every verb table should contain this defined header and followed by azalia verb commands.
**/
typedef struct {
  UINT16  VendorId;             ///< Codec Vendor ID
  UINT16  DeviceId;             ///< Codec Device ID
  UINT8   RevisionId;           ///< Revision ID of the codec. 0xFF matches any revision.
  UINT8   SdiNum;               ///< SDI number, 0xFF matches any SDI.
  UINT16  DataDwords;           ///< Number of data DWORDs following the header.
} HDA_VERB_TABLE_HEADER;


typedef struct  {
  HDA_VERB_TABLE_HEADER  Header;
  UINT32 Data[];
} HDAUDIO_VERB_TABLE;

/**
	makefile and ELink Gen OEM_HDA_VERB_TABLE
	{0x10EC, 0x0662, 0xFF, 0xFF, 60} for HDAUDIO_VERB_TABLE->Header
	ACL_662_MIC_1_FRONT_SURR_EAPD for HDAUDIO_VERB_TABLE->Data
*/
#define OEM_HDA_VERB_TABLE {{0x8086, 0x280F, 0xFF, 0xFF, 18}, OemHdaVerbTableDisplayAudio}, {{0x10EC, 0x0700, 0xFF, 0xFF, 644}, OemHdaVerbTblSample}, {{0x10EC, 0x0662, 0xFF, 0xFF, 60}, ACL_662_MIC_1_FRONT_SURR_EAPD},
```



```c
VOID AmiInstallOemHdaVerbTables (
    IN CONST EFI_PEI_SERVICES   **PeiServices,
    IN SI_POLICY_PPI        *SiPolicy,
    IN VOID                 *FspsUpd
)
{
    EFI_STATUS              Status;
    HDAUDIO_VERB_TABLE      *HdaVerbTable;
    SB_HDA_VERB_TABLE       *VerbTblSourcePtr = NULL;
    UINT8                   Index, TotalVerbTableNumber;
    UINT32                  NumOfData;
    HDAUDIO_CONFIG          *HdAudioConfig;
    UINT32                  VerbTableArray[32];
    UINT32                  *VerbTablePtr;
#ifdef OEM_HDA_VERB_TABLE_INSTALL
    BOOLEAN                 PrintHdaVerbTables = PRINT_OEM_HDA_VERB_TABLE_SUPPORT;
#endif

    DEBUG ((DEBUG_INFO, "AmiInstallOemHdaVerbTables() Start\n"));
    HdAudioConfig = NULL;

#if FixedPcdGetBool(PcdFspModeSelection) == 0    
    Status = GetConfigBlock ((VOID *) SiPolicy, &gHdAudioConfigGuid, (VOID *) &HdAudioConfig);
    if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_WARN, "Fail to get HdAudioConfig, Skip pass Verb Table.\n"));
        return;
    }

#endif    

    /*******************************************************************/

    /** Customers can override this function to pass their Verb Table **/

    /*******************************************************************/

    Status = SbHdaVerbTableOverride( PeiServices, &VerbTblSourcePtr, &TotalVerbTableNumber );
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_INFO, "TotalVerbTableNumber = %x\n", TotalVerbTableNumber));

    if((VerbTblSourcePtr == NULL) || (TotalVerbTableNumber == 0)) {
        DEBUG ((DEBUG_INFO, "Verb Table Source is NULL!! \n"));
        DEBUG ((DEBUG_INFO, "AmiInstallOemHdaVerbTables() End \n"));
        return;
    } // end if

    for (Index = 0 ; Index < TotalVerbTableNumber ; Index++)
    {
        NumOfData = VerbTblSourcePtr[Index].VerbTableHeader.DataDwords;
        Status = (*PeiServices)->AllocatePool (
                                     PeiServices,
                                     (sizeof (HDA_VERB_TABLE_HEADER)) + (sizeof (UINT32) * NumOfData),
                                     (VOID**)&HdaVerbTable
                                     );
        ASSERT_EFI_ERROR (Status);
        (*PeiServices)->CopyMem (
                            &HdaVerbTable->Header,
                            &VerbTblSourcePtr[Index].VerbTableHeader,
                            sizeof (HDA_VERB_TABLE_HEADER)
                            );
        DEBUG ((DEBUG_INFO, "HdaVerbTable->Header = 0x%08X\n", HdaVerbTable->Header));
        (*PeiServices)->CopyMem (
                            &HdaVerbTable->Data[0],
                            VerbTblSourcePtr[Index].VerbPtr,
                            sizeof (UINT32) * NumOfData
                            );
        DEBUG ((DEBUG_INFO, "HdaVerbTable->Data[0] = 0x%08X\n", HdaVerbTable->Data[0]));

#if defined (INTEL_DISPLAY_AUDIO_VERB_TABLE_SUPPORT) && (INTEL_DISPLAY_AUDIO_VERB_TABLE_SUPPORT == 1)
        if (HdaVerbTable->Header.VendorId == 0x8086) {
            if (HDMI_CODEC_PORT_B == 0) {
                HdaVerbTable->Data[4] |= 0x40;
            }
            if (HDMI_CODEC_PORT_C == 0) {
                HdaVerbTable->Data[8] |= 0x40;
            }
            if (HDMI_CODEC_PORT_D == 0) {
                HdaVerbTable->Data[12] |= 0x40;
            }
        }
#endif
        VerbTableArray[Index] = (UINT32) &HdaVerbTable->Header;
    } // end for loop

    Status = (*PeiServices)->AllocatePool (
                                 PeiServices,
                                 sizeof (UINT32) * TotalVerbTableNumber,
                                 (VOID**)&VerbTablePtr
                                 );
    ASSERT_EFI_ERROR (Status);

    (*PeiServices)->CopyMem (
                        VerbTablePtr,
                        VerbTableArray,
                        sizeof (UINT32) * TotalVerbTableNumber
                        );

    //
    // Pass the address to HdAudioConfig.
    //
    // cppcheck-suppress nullPointer
    UPDATE_POLICY (((FSPS_UPD *)FspsUpd)->FspsConfig.PchHdaVerbTablePtr, HdAudioConfig->VerbTablePtr, (UINT32) VerbTablePtr);

    // cppcheck-suppress nullPointer
    UPDATE_POLICY (((FSPS_UPD *)FspsUpd)->FspsConfig.PchHdaVerbTableEntryNum, HdAudioConfig->VerbTableEntryNum, TotalVerbTableNumber);


#ifdef OEM_HDA_VERB_TABLE_INSTALL
    //
    // Print OEM HDA VerbTables (Optional).
    //    
    if (PrintHdaVerbTables) AmiPrintOemHdaVerbTables (HdAudioConfig, FspsUpd);
#endif

    DEBUG ((DEBUG_INFO, "AmiInstallOemHdaVerbTables() End\n"));

}
```


```c
ELINK
    Name  = "{{0x10EC,   0x0662, 0xFF, 0xFF, 60}, ACL_662_MIC_1_FRONT_SURR_EAPD}," #Y231107_yqr- Add RTK 662 verb table -Add
    Parent  = "OEM_HDA_VERB_TABLE"
    Help  = " HDA Verb Table"
    Token = "INSTALL_OEM_HDA_VERB_TABLE" "=" "1"
    Token = "CUSTOMER_SUPPORT" "=" "6" 
    InvokeOrder = AfterParent
End

TOKEN
    Name  = "INSTALL_OEM_HDA_VERB_TABLE"
    Value  = "1"
    TokenType = Boolean
    TargetEQU = Yes
    TargetH = Yes
End

TOKEN
    Name  = "PRINT_OEM_HDA_VERB_TABLE_SUPPORT"
    Value  = "1"
    Help  = "Print Oem Hda Verb Tables for debug bios."
    TokenType = Integer
    TargetH = Yes
    Token = "INSTALL_OEM_HDA_VERB_TABLE" "=" "1"
End

ELINK
    Name  = "-D OEM_HDA_VERB_TABLE_INSTALL"
    Parent  = "*_*_*_CC_FLAGS"
    Type  = "BuildOptions"
    InvokeOrder = AfterParent
    Token = "INSTALL_OEM_HDA_VERB_TABLE" "=" "1"
End
```

```c
UINT32 ACL_662_MIC_1_FRONT_SURR_EAPD[]={
//
// Rear Audio Verb Table 0x10EC0662
//
// (NID 01h)
//HDA Codec Subsystem ID  : 0x10EC0662
0x00172062,
0x00172106,
0x001722EC,
0x00172310,
//===== Pin Widget Verb-table =====
//Widget node 0x01 :
0x0017FF00,
0x0017FF00,
0x0017FF00,
0x0017FF00,
//Pin widget 0x12 - DMIC
0x01271C00,
0x01271D00,
0x01271E00,
0x01271F40,
//Pin widget 0x14 - FRONT (Port-D)
0x01471C20,
0x01471D40,
0x01471E01,
0x01471F01,
//Set EAPD pin State is high
0x01470C02,
0x014F0C00,
//Pin widget 0x15 - SURR (Port-A)
0x01571C10,
0x01571D01,
0x01571E17,
0x01571F90,
0x01570C02,
0x015F0C00,
//Set EAPD pin State is high
// 0x01570C02,
//Pin widget 0x16 - CEN/LFE (Port-G)
0x01671CF0,
0x01671D11,
0x01671E11,
0x01671F41,
//Pin widget 0x18 - MIC1 (Port-B)
0x01871C30,
0x01871D90,
0x01871EA1,
0x01871F01,
//Pin widget 0x19 - MIC2 (Port-F)
0x01971C3F,
0x01971D90,
0x01971EA1,
0x01971F02,
//Pin widget 0x1A - LINE1 (Port-C)
0x01A71CF0,
0x01A71D11,
0x01A71E11,
0x01A71F41,
//Pin widget 0x1B - LINE2 (Port-E)
0x01B71C1F,
0x01B71D40,
0x01B71E21,
0x01B71F02,
//Pin widget 0x1C - CD-IN
0x01C71CF0,
0x01C71D11,
0x01C71E11,
0x01C71F41,
//Pin widget 0x1D - BEEP-IN
0x01D71C05,
0x01D71D80,
0x01D71E42,
0x01D71F40,
//Pin widget 0x1E - S/PDIF-OUT
0x01E71CF0,
0x01E71D11,
0x01E71E11,
0x01E71F41,
};
```

### VerbTable  结构

![[Pasted image 20231207172116.png]]

eg：
```C
//Pin widget 0x14 - FRONT (Port-D)
0x01471C20,
0x01471D40,
0x01471E01,
0x01471F01,
```

![[Pasted image 20231207173354.png]]

* NID 和Verb的说明
![[Pasted image 20231207173707.png]]
Verb ID `71C`  / `71D` / `71E` / `71F` 这4个Bytes是为Codec配置Default 值，共32 bits
![[Pasted image 20231207174709.png]]

这4个Bytes对应的是 Configuration Default register Data Structure
![[Pasted image 20231207174859.png]]


这一部分配置可以用Realtek的 HDACfg tool来配置：
![[Pasted image 20231207180929.png]]

