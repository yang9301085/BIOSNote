## CPU information 在HII中的实现：

`ASL_0804\Intel\AlderLakePlatSamplePkg\Setup\PlatformSetup.uni`

```Java

//

// CPU INFORMATION

//

#string STR_CPU_FORM_SUBTITLE                         #language en-US "Processor Information"

#string STR_CPU_FORM_SUBTITLE                         #language fr "CPU Information"

  

#string STR_PROCESSOR_VERSION_HELP                    #language en-US "Displays the Processor Type."

#string STR_PROCESSOR_VERSION_HELP                    #language fr "Displays the Processor Type."

#string STR_PROCESSOR_VERSION_STRING                  #language en-US "Type"

#string STR_PROCESSOR_VERSION_STRING                  #language fr "Type de processeur"

#string STR_PROCESSOR_VERSION_STRING                  #language es "Tipo de procesador"

#string STR_PROCESSOR_VERSION_VALUE                   #language en-US "N/A"

  

#string STR_PROCESSOR_SPEED_STRING                    #language en-US "Speed"

#string STR_PROCESSOR_SPEED_STRING                    #language fr "Vitesse du processeur"

#string STR_PROCESSOR_SPEED_STRING                    #language es "Velocidad del procesador"

#string STR_PROCESSOR_SPEED_VALUE                     #language en-US "N/A"

#string STR_PROCESSOR_SPEED_HELP                      #language en-US "Displays the Processor Speed."

#string STR_PROCESSOR_SPEED_HELP                      #language fr "Displays the Processor Speed."

  

#string STR_PROCESSOR_ID_STRING                       #language en-US "ID"

#string STR_PROCESSOR_ID_STRING                       #language fr "ID"

#string STR_PROCESSOR_ID_VALUE                        #language en-US "N/A"

#string STR_PROCESSOR_ID_HELP                         #language en-US "Displays the Processor ID."

#string STR_PROCESSOR_ID_HELP                         #language fr "Displays the Processor ID."

  

#string STR_PROCESSOR_STEPPING_STRING                 #language en-US "Stepping"

#string STR_PROCESSOR_STEPPING_STRING                 #language fr "Stepping"

#string STR_PROCESSOR_STEPPING_VALUE                  #language en-US "Unknown"

#string STR_PROCESSOR_STEPPING_HELP                   #language en-US "Displays the Processor Stepping."

#string STR_PROCESSOR_STEPPING_HELP                   #language fr "Displays the Processor Stepping."

  

#string STR_PROCESSOR_PACKAGE_STRING                  #language en-US "Package"

#string STR_PROCESSOR_PACKAGE_STRING                  #language fr "Package"

#string STR_PROCESSOR_PACKAGE_VALUE                   #language en-US "N/A"

#string STR_PROCESSOR_PACKAGE_HELP                    #language en-US "Displays the Processor Package."

#string STR_PROCESSOR_PACKAGE_HELP                    #language fr "Displays the Processor Package."

  

#string STR_PROCESSOR_MICROCODE_STRING                #language en-US "Microcode Revision"

#string STR_PROCESSOR_MICROCODE_STRING                #language fr "Microcode Revision"

#string STR_PROCESSOR_MICROCODE_VALUE                 #language en-US "Not loaded"

#string STR_PROCESSOR_MICROCODE_HELP                  #language en-US "CPU Microcode Revision"

#string STR_PROCESSOR_MICROCODE_HELP                  #language fr "CPU Microcode Revision"

  

#string STR_PROCESSOR_ATOM_COUNT_STRING               #language en-US "Number of Efficient-cores"

#string STR_PROCESSOR_CORE_COUNT_STRING               #language en-US "Number of Performance-cores"

#string STR_PROCESSOR_ATOM_COUNT_VALUE                #language en-US "xCore(s) / yThread(s)"

#string STR_PROCESSOR_CORE_COUNT_VALUE                #language en-US "xCore(s) / yThread(s)"

#string STR_PROCESSOR_COUNT_HELP                      #language en-US "Displays number of CPU cores."

  

#string STR_PROCESSOR_GT_STRING                       #language en-US "GT Info"

#string STR_PROCESSOR_GT_VALUE                        #language en-US "Not Applicable"

#string STR_PROCESSOR_GT_HELP                         #language en-US "Processor GT Info. Only valid if SNB stepping is D0 or above."

  

#string STR_EDRAM_SIZE_STRING                         #language en-US "eDRAM Size"

#string STR_EDRAM_SIZE_VALUE                          #language en-US "N/A"

#string STR_EDRAM_SIZE_HELP                           #language en-US "eDRAM Size supported by the SKU"

```

`ASL_0804\Build\Main.hfr`

```Java
SUBTITLE(STRING_TOKEN(STR_CPU_FORM_SUBTITLE))

    text

      help  = STRING_TOKEN(STR_PROCESSOR_HELP),

      text  = STRING_TOKEN(STR_PROCESSOR_STRING),

      text  = STRING_TOKEN(STR_PROCESSOR_VALUE),

      flags = 0,

      key   = 0;

    text

      help  = STRING_TOKEN(STR_PROCESSOR_VERSION_HELP),

      text  = STRING_TOKEN(STR_PROCESSOR_VERSION_STRING),

      text  = STRING_TOKEN(STR_PROCESSOR_VERSION_VALUE),

      flags = 0,

      key   = 0;

    text

      help  = STRING_TOKEN(STR_PROCESSOR_SPEED_HELP),

      text  = STRING_TOKEN(STR_PROCESSOR_SPEED_STRING),

      text  = STRING_TOKEN(STR_PROCESSOR_SPEED_VALUE),

      flags = 0,

      key   = 0;

    text

      help  = STRING_TOKEN(STR_PROCESSOR_ID_HELP),

      text  = STRING_TOKEN(STR_PROCESSOR_ID_STRING),

      text  = STRING_TOKEN(STR_PROCESSOR_ID_VALUE),

      flags = 0,

      key   = 0;

    text

      help  = STRING_TOKEN(STR_PROCESSOR_STEPPING_HELP),

      text  = STRING_TOKEN(STR_PROCESSOR_STEPPING_STRING),

      text  = STRING_TOKEN(STR_PROCESSOR_STEPPING_VALUE),

      flags = 0,

      key   = 0;

    text

      help  = STRING_TOKEN(STR_PROCESSOR_PACKAGE_HELP),

      text  = STRING_TOKEN(STR_PROCESSOR_PACKAGE_STRING),

      text  = STRING_TOKEN(STR_PROCESSOR_PACKAGE_VALUE),

      flags = 0,

      key   = 0;

    text

      help  = STRING_TOKEN(STR_PROCESSOR_COUNT_HELP),

      text  = STRING_TOKEN(STR_PROCESSOR_ATOM_COUNT_STRING),

      text  = STRING_TOKEN(STR_PROCESSOR_ATOM_COUNT_VALUE),

      flags = 0,

      key   = 0;

    text

      help  = STRING_TOKEN(STR_PROCESSOR_COUNT_HELP),

      text  = STRING_TOKEN(STR_PROCESSOR_CORE_COUNT_STRING),

      text  = STRING_TOKEN(STR_PROCESSOR_CORE_COUNT_VALUE),

      flags = 0,

      key   = 0;

    text

      help  = STRING_TOKEN(STR_PROCESSOR_MICROCODE_HELP),

      text  = STRING_TOKEN(STR_PROCESSOR_MICROCODE_STRING),

      text  = STRING_TOKEN(STR_PROCESSOR_MICROCODE_VALUE),

      flags = 0,

      key   = 0;

    text

      help  = STRING_TOKEN(STR_PROCESSOR_GT_HELP),

      text  = STRING_TOKEN(STR_PROCESSOR_GT_STRING),

      text  = STRING_TOKEN(STR_PROCESSOR_GT_VALUE),

      flags = 0,

      key   = 0;

    text

      help  = STRING_TOKEN(STR_EDRAM_SIZE_HELP),

      text  = STRING_TOKEN(STR_EDRAM_SIZE_STRING),

      text  = STRING_TOKEN(STR_EDRAM_SIZE_VALUE),

      flags = 0,

      key   = 0;
```

`ASL_0804\Intel\AlderLakePlatSamplePkg\Setup\CpuSetup.c`

```C
/**

  Initialize CPU strings.

  

  @param[in] EFI_HII_HANDLE   HiiHandle

  @param[in] UINT16           Class

**/

VOID

InitCPUStrings (

  EFI_HII_HANDLE HiiHandle,

  UINT16         Class

  )

{

  EFI_EVENT             SetupNvramCallbackEvt;

  VOID                  *SetupNvramCallbackReg;

  CONST CHAR8           *RevisionTableString = NULL;

  //InitBootFrequencyDefault (NULL, NULL);

  if ((Class == MAIN_FORM_SET_CLASS) || (Class == ADVANCED_FORM_SET_CLASS)) {

    DEBUG ((DEBUG_INFO, "<InitCPUStrings>"));

    gHiiHandle  = HiiHandle;

  

    //InitBootFrequencyDefault (NULL, NULL);

    InitTurboRatioDefault (NULL, NULL);

    InitTxtAcheckDefault ();

    InitDCDInfo ();

    InitCPUInfo (NULL, NULL);

///

///

// APTIOV_OVERRIDE_RC_START : For AMI setup style

#if 0

    SetupNvramCallbackEvt = EfiCreateProtocolNotifyEvent (

                              &gSetupNvramUpdateGuid,

                              TPL_CALLBACK,

                              CpuSetupCallback,

                              NULL,

                              &SetupNvramCallbackReg

                              );

#else

    SetupNvramCallbackEvt = EfiCreateProtocolNotifyEvent (

                              &gAmiTseNVRAMUpdateGuid,

                              TPL_CALLBACK,

                              CpuSetupCallback,

                              NULL,

                              &SetupNvramCallbackReg

                              );

#endif

// APTIOV_OVERRIDE_RC_END

    ASSERT (SetupNvramCallbackEvt != NULL);

  }

  

  if (Class == MAIN_FORM_SET_CLASS) {

    RevisionTableString = GetRevisionTable ();

    InitString (

      HiiHandle,

      STRING_TOKEN (STR_PROCESSOR_STEPPING_VALUE),

      L"%a",

      RevisionTableString

      );

  }

  InitCpuVrConfig (); ///< Display CPU VR config default programmed values.

#if FixedPcdGetBool(PcdITbtEnable) == 1

  OverrideCpuCxLimitForTcssIom ();

#endif // FixedPcdGetBool(PcdITbtEnable) == 1

}
```

`ASL_0804\Intel\ClientOneSiliconPkg\Fru\AdlCpu\LibraryPrivate\BaseCpuInfoFruLib\BaseCpuInfoFruLib.c`

```C


typedef struct {

  UINT32  CPUID;

  UINT8   CpuSku;

  CHAR8   *String;

} CPU_REV;


GLOBAL_REMOVE_IF_UNREFERENCED CONST CPU_REV  mProcessorRevisionTable[] = {

#if FixedPcdGetBool(PcdAdlSSupport) == 1

  {CPUID_FULL_FAMILY_MODEL_RAPTORLAKE_DT_HALO    + EnumRplA0, EnumCpuTrad,    "A0"},

  {CPUID_FULL_FAMILY_MODEL_RAPTORLAKE_DT_HALO    + EnumRplB0, EnumCpuTrad,    "B0"},

  {CPUID_FULL_FAMILY_MODEL_RAPTORLAKE_2_DT_HALO  + EnumRpl2C2, EnumCpuTrad,   "C2"},

  {CPUID_FULL_FAMILY_MODEL_RAPTORLAKE_2_DT_HALO  + EnumRpl2H0, EnumCpuTrad,   "H0"},

#endif

#if FixedPcdGetBool(PcdRplpSupport) == 1

  {CPUID_FULL_FAMILY_MODEL_RAPTORLAKE_MOBILE     + EnumRplJ0, EnumCpuUlt,     "J0"},

  {CPUID_FULL_FAMILY_MODEL_RAPTORLAKE_MOBILE     + EnumRplQ0, EnumCpuUlt,     "Q0"},

#endif

#if (FixedPcdGetBool(PcdAdlSSupport) == 1)

  {CPUID_FULL_FAMILY_MODEL_ALDERLAKE_DT_HALO     + EnumAdlA0, EnumCpuTrad,    "A0"},

  {CPUID_FULL_FAMILY_MODEL_ALDERLAKE_DT_HALO     + EnumAdlB0, EnumCpuTrad,    "B0"},

  {CPUID_FULL_FAMILY_MODEL_ALDERLAKE_DT_HALO     + EnumAdlC0, EnumCpuTrad,    "C0"},

  {CPUID_FULL_FAMILY_MODEL_ALDERLAKE_DT_HALO     + EnumAdlD0, EnumCpuTrad,    "D0"},

  {CPUID_FULL_FAMILY_MODEL_ALDERLAKE_DT_HALO     + EnumAdlG0, EnumCpuTrad,    "G0"},

  {CPUID_FULL_FAMILY_MODEL_ALDERLAKE_DT_HALO     + EnumAdlH0, EnumCpuTrad,    "H0"},

#endif

  {CPUID_FULL_FAMILY_MODEL_ALDERLAKE_MOBILE      + EnumAdlJ0, EnumCpuUlt,     "J0"},

  {CPUID_FULL_FAMILY_MODEL_ALDERLAKE_MOBILE      + EnumAdlK0, EnumCpuUlt,     "K0"},

  {CPUID_FULL_FAMILY_MODEL_ALDERLAKE_MOBILE      + EnumAdlL0, EnumCpuUlt,     "L0"},

  {CPUID_FULL_FAMILY_MODEL_ALDERLAKE_MOBILE      + EnumAdlQ0, EnumCpuUlt,     "Q0"},

  {CPUID_FULL_FAMILY_MODEL_ALDERLAKE_MOBILE      + EnumAdlR0, EnumCpuUlt,     "R0"},

  {CPUID_FULL_FAMILY_MODEL_ALDERLAKE_MOBILE      + EnumAdlQ0, EnumCpuUlx,     "Q0"},

  {CPUID_FULL_FAMILY_MODEL_ALDERLAKE_MOBILE      + EnumAdlR0, EnumCpuUlx,     "R0"},

  {CPUID_FULL_FAMILY_MODEL_ALDERLAKE_MOBILE      + EnumAdlS0, EnumCpuUlx,     "S0"}

};



/**

  Returns Revision Table string

  

  @param[in]   CpuId

  

  @retval      Character pointer of Revision Table string

**/

CONST CHAR8*

GetRevisionTableString (

  UINT32                CpuId

  )

{

  UINTN                 Index = 0;

  UINTN                 Count;

  CPU_SKU               CpuSku;

  

  CpuSku = GetCpuSku ();

  Count = ARRAY_SIZE (mProcessorRevisionTable);

  

  for (Index = 0; Index < Count; Index++) {

    if ((CpuId == mProcessorRevisionTable[Index].CPUID) && CpuSku == (mProcessorRevisionTable[Index].CpuSku)) {

      return mProcessorRevisionTable[Index].String;

    }

  }

  return NULL;

}
```
