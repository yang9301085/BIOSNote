## issue
 TPM2.0设备找不到
![[可信计算_[12-53-29].bmp]]

### phenomenon 
正常情况下插入TPM模块，打开PTT中dTPM重启后，会显示TPM的设备版本、厂商和其他参数，
找不到设备时会显示 “未找到安全设备”

### Root Cause
ME中没有打开 TPM Over SPI Bus
```XML
<TpmOverSpiBusConfiguration label="TPM Over SPI Bus Configuration">
         <SpiOverTpmClkFreq value="14MHz" value_list="['14MHz', '25MHz', '48MHz']" label="TPM Clock Frequency" help_text="This setting determines the clock frequency setting to be used for the TPM over SPI bus." key="DescriptorPlugin:PchStraps:PCH_Strap_SPI_STCF"/>
         <SpiOverTpmBusEnable value="Yes" value_list="['No', 'Yes']" label="TPM Over SPI Bus Enabled" help_text="This setting determines the clock frequency setting to be used for the TPM over SPI bus." key="DescriptorPlugin:PchStraps:PCH_Strap_LPC_spi_strap_tos"/>
</TpmOverSpiBusConfiguration>
```

---
再追代码的时候发现以下新东西：

AmiModulePkg\\TCG2 下是TPM相关模块

他会通过这两个Variable：Tpm20Device和TpmHrdW来判断是否有TPM设备

```C
suppressif 
	ideqval SETUP_DATA.Tpm20Device == 1 OR 
	ideqval SETUP_DATA.TpmHrdW == 1;
	     goto TCG_FORM_ID,
	          prompt = STRING_TOKEN(STR_TCG_FORM),
	          help = STRING_TOKEN(STR_TCG_FORM_HELP);
endif;
suppressif 
	ideqval SETUP_DATA.Tpm20Device == 0 OR 
	ideqval SETUP_DATA.TpmHrdW == 1;
         goto TCG20_FORM_ID,
              prompt = STRING_TOKEN(STR_TCG_FORM),
              help = STRING_TOKEN(STR_TCG_FORM_HELP);
endif;
suppressif 
	ideqval SETUP_DATA.TpmHrdW == 0;
         goto NO_TCG_FORM_ID,
              prompt = STRING_TOKEN(STR_TCG_FORM),
              help = STRING_TOKEN(STR_TCG_FORM_HELP);
endif;
```
他会把TPM的信息给到`PCRBitmap`这个variable里面
```C
/*

	About PCRBitmap variable
*/
typedef struct
{
    UINT32 SupportedPcrBitMap;
    UINT32 ActivePcrBitMap;
    UINT32 Reserved;
    UINT32 TpmFwVersion;
    UINT32 TpmManufacturer;
}AMITCGSETUPINFOFLAGS;

EFI_STATUS
EFIAPI
Tpm2GetCapabilityCapPCRS ()
{

    TPMS_CAPABILITY_DATA    TpmCap;
    TPMI_YES_NO             MoreData;
    EFI_STATUS              Status;
    TPMS_PCR_SELECTION      *PcrSelect;
    UINT8                   *Buffer;
    UINTN                   size = 0, i=0, j=0;
    UINT32                  SupportedBankBitMap=0;
    AMITCGSETUPINFOFLAGS    Info;
    UINT16                  hash;
    SupportedBankBitMap = 0;

    Status  = InternalTpm2GetCapability (
                  TPM_CAP_PCRS,
                  0,
                  MAX_PCR_PROPERTIES,
                  &MoreData,
                  &TpmCap);

    if(EFI_ERROR(Status))
    {
        Info.SupportedPcrBitMap = 1;
        Info.ActivePcrBitMap  = 1;
        Info.Reserved = 0;

        DEBUG(( DEBUG_INFO," SupportedPcrBitMap = %x \n",Info.SupportedPcrBitMap));
        DEBUG(( DEBUG_INFO," ActivePcrBitMap = %x \n", Info.ActivePcrBitMap));
        Status = gRT->SetVariable( L"PCRBitmap", \
                                   &gTcgInternalflagGuid, \
                                   EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE, \
                                   sizeof(AMITCGSETUPINFOFLAGS), \
                                   &Info);
                                   
        return Status;
    }


    //printbuffer((UINT8 *)&TpmCap, 0x30);
    Buffer = (UINT8 *)&TpmCap;
    PcrSelect = (TPMS_PCR_SELECTION *)(Buffer + (sizeof(UINT32)*2));
    size = SwapBytes32(*(UINT32 *)(Buffer + sizeof(UINT32)));

    DEBUG(( DEBUG_INFO," size = %x \n", size))

    //ActiveBankBitMap
    for(i=0; i<size; i++,PcrSelect++)
    {
        //printbuffer((UINT8 *)PcrSelect, 0x30);
        DEBUG(( DEBUG_INFO," PcrSelect->hash = %x \n", PcrSelect->hash));
        DEBUG(( DEBUG_INFO," PcrSelect->sizeofSelect = %x \n", PcrSelect->sizeofSelect));
        hash = SwapBytes16(PcrSelect->hash);
        switch(hash)
        {
            case 0x4:
                if(PcdGetBool(PcdAmiSha1PCRPolicy)){
                    SupportedBankBitMap |= 1;
                }
                for(j=0; j<PcrSelect->sizeofSelect; j++)
                {
                    if(PcrSelect->pcrSelect[j] ==0)continue;
                    else
                    {
                        ActiveBankBitMap |= 1;
                        break;
                    }
                }
                break;
            case 0xB:
                SupportedBankBitMap |= 2;
                for(j=0; j<PcrSelect->sizeofSelect; j++)
                {
                    if(PcrSelect->pcrSelect[j] ==0)continue;
                    else
                    {
                        ActiveBankBitMap |= 2;
                        break;
                    }
                }
                break;
            case 0xC:
                SupportedBankBitMap |= 4;
                for(j=0; j<PcrSelect->sizeofSelect; j++)
                {
                    if(PcrSelect->pcrSelect[j] ==0)continue;
                    else
                    {
                        ActiveBankBitMap |= 4;
                        break;
                    }
                }
                break;
            case 0xD:
                SupportedBankBitMap |= 8;
                for(j=0; j<PcrSelect->sizeofSelect; j++)
                {
                    if(PcrSelect->pcrSelect[j] ==0)continue;
                    else
                    {
                        ActiveBankBitMap |= 8;    // Correct for EFI_TCG2_BOOT_HASH_ALG512
                        break;
                    }
                }
                break;
            case 0x12:
                for(j=0; j<PcrSelect->sizeofSelect; j++)
                {
                    if(PcrSelect->pcrSelect[j] ==0)continue;
                    else
                    {
                        ActiveBankBitMap |= 0x10;
                        break;
                    }
                }
                SupportedBankBitMap |= 0x10;
                break;
            default:
                break;
        }
    }

    for( i=0, gNumberOfPcrBanks=0; i<16; ++i)
    {
        if( (SupportedBankBitMap & (1<<i)) )
            ++gNumberOfPcrBanks;
    }
    
    TcgSupportedBankBitMap = SupportedBankBitMap;
    Info.SupportedPcrBitMap = SupportedBankBitMap;
    Info.ActivePcrBitMap  = ActiveBankBitMap;
    Info.Reserved = 0;
    Info.TpmFwVersion = Tpm20FwVersion;
    Info.TpmManufacturer = Tpm20Manufacturer;

    DEBUG(( DEBUG_INFO," SupportedPcrBitMap = %x \n", Info.SupportedPcrBitMap));
    DEBUG(( DEBUG_INFO," ActivePcrBitMap = %x \n", Info.ActivePcrBitMap));

    Status = gRT->SetVariable( L"PCRBitmap", \
                               &gTcgInternalflagGuid, \
                               EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE, \
                               sizeof(AMITCGSETUPINFOFLAGS), \
                               &Info);

    return Status;

}
```
PCRBitmap
![[20240710124818.bmp]]
![[20240710124944.bmp]]



![[20240710124918.bmp]]

![[20240710124930.bmp]]

![[20240710124941.bmp]]

![[20240710124942.bmp]]