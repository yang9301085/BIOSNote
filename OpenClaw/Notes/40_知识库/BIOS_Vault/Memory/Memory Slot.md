对应Intel Desktop code
* 问题现象是：主板只有一个内存插槽，但任务管理器中显示两个
![[2916d95590e75d0ebde52f22fddf3a5.png]]
`Intel\ClientOneSiliconPkg\SystemAgent\Include\ConfigBlock\MemoryDxeConfig.h`中有相关memory slot 的设定
![[Pasted image 20240402144248.png]]
`Intel\AlderLakePlatSamplePkg\Library\DxePolicyUpdateLib\DxeSaPolicyUpdate.c`中可以把用不到的slot关掉

![[Pasted image 20240715113643.png]]
对应：X99 Code
`GrantleyPkg\Platform\Dxe\MemorySubClass\MemorySubClass.c`
```C
EFI_STATUS
EFIAPI
MemorySubClassEntryPoint(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_HII_HANDLE HiiHandle;
    EFI_STATUS Status;
    EFI_HOB_GUID_TYPE *GuidHob;
    struct SystemMemoryMapHob *SystemMemoryMap;
    UINT8 Socket, Ch, Dimm; // AptioV Server Override : Removed the Chstring as it is not used.
    BOOLEAN DimmPresent;
    UINT8 haNum;

    EFI_SMBIOS_PROTOCOL *Smbios;
    SMBIOS_TABLE_TYPE16 *Type16Record;
    SMBIOS_TABLE_TYPE17 *Type17Record;
    SMBIOS_TABLE_TYPE19 *Type19Record;
    SMBIOS_TABLE_TYPE20 *Type20Record;
    EFI_SMBIOS_HANDLE MemArraySmbiosHandle;
    EFI_SMBIOS_HANDLE MemArrayMappedAddrSmbiosHandle;
    EFI_SMBIOS_HANDLE MemDevSmbiosHandle;
    EFI_SMBIOS_HANDLE MemDevMappedAddrSmbiosHandle;
    UINTN TableSize;
    UINT8 NumSlots;
    UINT64 TotalMemorySize;
    UINT64 DimmStartingAddressMb;
    UINT64 DimmEndingAddressMb;
    UINT32 MemoryCapacity;
    UINT32 MemoryDeviceSize; // AptioV Server Override: Modified for supporting 64Gb Dimm's
    UINT16 MemorySpeed;
    UINT16 MemoryTotalWidth;
    UINT16 MemoryDataWidth;
    CHAR8 *OptionalStrStart;
    UINTN DeviceLocatorStrLen = 0; // AptioV server override
    UINTN BankLocatorStrLen = 0;   // AptioV server override
    UINTN ManufactureStrLen = 0;   // AptioV server override
    UINTN SerialNumberStrLen = 0;  // AptioV server override
    UINTN AssertTagStrLen = 0;     // AptioV server override
    UINTN PartNumberStrLen = 0;    // AptioV server override
    EFI_STRING_ID DeviceLocator;
    EFI_STRING_ID BankLocator;
    EFI_STRING_ID Manufacturer;
    EFI_STRING_ID SerialNumber;
    EFI_STRING_ID AssetTag;
    EFI_STRING_ID PartNumber;
    EFI_STRING DeviceLocatorStr = NULL; // AptioV server override
    EFI_STRING BankLocatorStr = NULL;   // AptioV server override
    EFI_STRING ManufactureStr = NULL;   // AptioV server override
    EFI_STRING SerialNumberStr = NULL;  // AptioV server override
    EFI_STRING AssertTagStr = NULL;     // AptioV server override
    EFI_STRING PartNumberStr = NULL;    // AptioV server override
    UINTN StringBufferSize;
    EFI_STRING StringBuffer;
    CHAR16 StringBuffer2[SMBIOS_STRING_MAX_LENGTH];
    UINT32 MemInfoTotalMem, MemInfoTotalNodeMem; // AptioV server override Adding MemInfoTotalNodeMem
    UINT16 MemInfoMemFreq;
    UINT8 Index = 0;
    UINT64 MemoryAddress;
    UINT64 Type20StartingAddr = 0, Type20EndAddress; // AptioV server override: Filling SMBIOS Type 20 information
    UINT64 RowLength;
    UINT8 HaCh = 0, Node = 0;
    UINT32 BspCpuidSignature;
    UINT32 RegEax, RegEbx, RegEcx, RegEdx;
    //
    // AptioV server override : START - Filling Type 17,20 details
    //
    EFI_STRING StringBufferAsset;
    CHAR16 *TempString = NULL;
    UINT8 j = 0;
    UINT8 Data8 = 0;
    UINT16 ManfData = 0;
    JEDEC_MF_ID *IdTblPtr;
    UINT64 MemoryArrayMappedEndAddress = 0;
    UINT8 PartitionRowPosition = 0;
    //
    // AptioV server override : END
    //

    //
    // AptioV Server Override Start : Update Partition width in Type 19 Structure.
    //
    UINT8 PartitionWidth = 0;
    EFI_SMBIOS_TABLE_HEADER *Record;
    EFI_SMBIOS_HANDLE SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    UINT8 Type;
    UINT8 MaxIndex;
    //
    // AptioV Server Override End : Update Partition width in Type 19 Structure.
    //
    EFI_PLATFORM_TYPE_PROTOCOL *PlatformType;
    UINT8 PlatformMaxSocket;
    UINT8 PlatformDimmsPerChannel;
    PLATFORM_SLOT_INFO_PROTOCOL *PlatformInfo;
    EFI_HANDLE PlatformInfoHandle = NULL;
    UINT8 NoofChannelsPerHA; // AptioV Server Override.
    UINT8 NoofHAPerSocket;   // AptioV Server Override.

    //
    // Allocate Buffers
    //
    StringBufferSize = (sizeof(CHAR16)) * SMBIOS_STRING_MAX_LENGTH;
    StringBuffer = AllocatePool(StringBufferSize); // AptioV Server Override: AllocateZeroPool has been replaced with AllocatePool since memory is already cleared everytime inside the loop.
    ASSERT(StringBuffer != NULL);

    PlatformInfo = AllocateZeroPool(sizeof(PLATFORM_SLOT_INFO_PROTOCOL));

    //
    // AptioV server override : START - Allocate for Asset tag
    //
    StringBufferAsset = AllocatePool(StringBufferSize);
    ASSERT(StringBufferAsset != NULL);
    TempString = AllocatePool(StringBufferSize);
    ASSERT(TempString != NULL);
    //
    // AptioV server override : End
    //
    GuidHob = GetFirstGuidHob(&gEfiMemoryMapGuid);
    if (GuidHob == NULL)
    {
        FreePool(PlatformInfo); // AptioV server override - Cpp Check
        return EFI_NOT_FOUND;
    }

    SystemMemoryMap = (struct SystemMemoryMapHob *)GET_GUID_HOB_DATA(GuidHob);

    //
    // Locate the platform type protocol
    //
    Status = gBS->LocateProtocol(
        &gEfiPlatformTypeProtocolGuid,
        NULL,
        &PlatformType);

    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "Cannot locate platform type protocol\n"));
        FreePool(PlatformInfo); // AptioV server override - Cpp Check
        return EFI_NOT_FOUND;
    }
    switch (PlatformType->BoardId)
    {
#ifdef GRANGEVILLE_FLAG
    case TypeBeverlyCoveCRB:
    case TypeBeverlyCoveSODimmCRB:
    case TypeTrabucoCanyon:
    case TypePiestewaPeak:
    case TypeCamelbackMountain:
        PlatformMaxSocket = 1;
        PlatformDimmsPerChannel = 2;
        break;
#else
    case TypeHedtSKU1CRB:
    case TypeHedtSKU1CRBFab2:
    case TypeHedtEv:
    case TypeHedtDDR3:
        //Y240430_Change memory slot to 4-s
        // PlatformMaxSocket = 2;
        // PlatformDimmsPerChannel = 3;
        PlatformMaxSocket = 1;//Y240617
        PlatformDimmsPerChannel = 2;
        //Y240430_Change memory slot to 4-e
        break;

    case TypeIncaCityCRB:
    case TypeIncaCitySRP:
        PlatformMaxSocket = 4;
        PlatformDimmsPerChannel = 3;
        break;

    case TypeMayanCityCRB:
    case TypeMayanCitySRP:
    case TypeWildcatPass:
        PlatformMaxSocket = 2;
        PlatformDimmsPerChannel = 3;
        break;

    case TypeMayanCityCRBDDR3:
    case TypeMayanCitySRPDDR3:
        PlatformMaxSocket = 2;
        PlatformDimmsPerChannel = 2;
        break;

    case TypeYakimaCityCRB:
    case TypeYakimaCitySRP:
    case TypeYakimaCitySTHI:
        PlatformMaxSocket = 2;
        PlatformDimmsPerChannel = 3;
        break;

    case TypeAztecCityCRBSRP:
    case TypeAztecCitySTHI:
    case TypeHalfWidthCitySRP:
        PlatformMaxSocket = 2;
        PlatformDimmsPerChannel = 2;
        break;
#endif

    default:
        PlatformMaxSocket = 2;
        PlatformDimmsPerChannel = 3;
        break;
    }

    PlatformInfo->PlatformMaxSocket = PlatformMaxSocket;
    PlatformInfo->PlatformDimmsPerChannel = PlatformDimmsPerChannel;
    //
    // Check if MRC code has set the PCDs for platform or not
    //
    MemInfoTotalMem = (UINT32)(SystemMemoryMap->memSize << 6);
    MemInfoMemFreq = SystemMemoryMap->memFreq;
    if ((MemInfoTotalMem == 0) || (MemInfoMemFreq == 0))
    {
        FreePool(PlatformInfo); // AptioV server override - Cpp Check
        return EFI_NOT_FOUND;
    }

    //
    // Locate dependent protocols
    //
    Status = gBS->LocateProtocol(&gEfiSmbiosProtocolGuid, NULL, &Smbios);
    ASSERT(!EFI_ERROR(Status));

    //
    // Add our default strings to the HII database. They will be modified later.
    //
    HiiHandle = HiiAddPackages(
        &mEfiMemorySubClassDriverGuid,
        NULL,
        MemorySubClassStrings,
        NULL,
        NULL);

    ASSERT(!EFI_ERROR(Status));

    //
    // Get BSP CPU ID
    // Shift out the stepping
    //
    AsmCpuid(0x01, &RegEax, &RegEbx, &RegEcx, &RegEdx);
    BspCpuidSignature = (RegEax >> 4) & 0x0000FFFF;

    //
    // Convert Total Memory size from MB to Bytes
    //
    TotalMemorySize = (UINT64)LShiftU64(MemInfoTotalMem, 20);
    DEBUG((EFI_D_INFO, "Total Memory Size = %dGB\n", MemInfoTotalMem >> 10));
    DEBUG((EFI_D_INFO, "Memory Speed = %dMHz\n", MemInfoMemFreq));


    NumSlots = PlatformMaxSocket * MAX_CH * PlatformDimmsPerChannel;
#ifndef GRANGEVILLE_FLAG
    //
    // Accomodate platforms with asymmetrical DIMM configurations
    //
    if (PlatformType->BoardId == TypeMayanCitySRP)
    {
        //
        // Only 2 DIIMMs per ch on socket 1
        // Subtract 1 DIMM for each channel
        //
        NumSlots -= 4;
    }

    if ((PlatformType->BoardId == TypeYakimaCityCRB) ||
        (PlatformType->BoardId == TypeYakimaCitySRP) ||
        (PlatformType->BoardId == TypeYakimaCitySTHI))
    {
        //
        // No ch 0 for either socket
        // Subtract 3 DIMMs for ch 0 on 2 sockets
        //
        NumSlots -= 6;
    }
#endif
#if defined(GRANGEVILLE_FLAG) || defined(DE_SKU)
    NumSlots = MAX_SOCKET_DE * MAX_CH_DE * 2; // 2 DRR4 SODIMM slots per Channel in DE SKU
#endif
    PlatformInfo->NumSlots = NumSlots;
    // AptioV server Override start
    //  PlatformInfo->PlatformMaxNode = PlatformMaxSocket * MAX_HA;
    //  PlatformInfo->NodeInfo[Node].numChSupp = SystemMemoryMap->NodeInfo[Node].NumChPerHA;

    // One Type 16 structure for each memory controller
    NoofChannelsPerHA = SystemMemoryMap->NodeInfo[0].NumChPerHA;
    NoofHAPerSocket = SystemMemoryMap->NoHAPerSocket;
    NumSlots = NoofChannelsPerHA * PlatformDimmsPerChannel; // Calculate the Number of slots according to platform.

    PlatformInfo->PlatformMaxNode = PlatformMaxSocket * NoofHAPerSocket; // Plartform Total HA count should depends on PlatformMaxSocket * no of HA per socket.

    // Commented as  Num of slots and Memory Capacity are calculated in the node loop.
    //  MemoryCapacity = (UINT32) (NumSlots * MAX_DIMM_SIZE); // in GB
    for (Node = 0; Node < PlatformMaxSocket * MAX_HA; Node++)
    { // Loop for each node.

        // Suppress the Node 1 and Node 3 for 1HA per socket case.
        if (NoofHAPerSocket == 1)
        {
            if (Node % 2)
                continue;
        }

        PlatformInfo->NodeInfo[Node].numChSupp = (MAX_CH / NoofHAPerSocket); // Total no of channel supported by this HA.
        // AptioV Server Override End

        //
        // Report top level Physical Memory Array to Type 16 SMBIOS Record
        //

        Type16Record = AllocatePool(sizeof(SMBIOS_TABLE_TYPE16) + 1 + 1);
        ZeroMem(Type16Record, sizeof(SMBIOS_TABLE_TYPE16) + 1 + 1);

        Type16Record->Hdr.Type = EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY;
        Type16Record->Hdr.Length = sizeof(SMBIOS_TABLE_TYPE16);
        Type16Record->Hdr.Handle = 0;
        Type16Record->Location = MemoryArrayLocationSystemBoard;
        Type16Record->Use = MemoryArrayUseSystemMemory;

        //
        // AptioV Server Override Start : Program ECC in Type16 structure depending upon System Memory Map
        //
        if (SystemMemoryMap->EccEn)
        {
            Type16Record->MemoryErrorCorrection = MemoryErrorCorrectionMultiBitEcc;
        }
        else
        {
            Type16Record->MemoryErrorCorrection = MemoryErrorCorrectionNone;
        }

        //
        // AptioV Server Override End : Program ECC in Type16 structure depending upon System Memory Map
        //
        Type16Record->MemoryErrorInformationHandle = 0xFFFE;

        //
        // AptioV Server Override Start : Calculate the MemoryCapacity according to the Platform Type.
        //
        if ((PlatformType->BoardId == TypeMayanCitySRP) && (Node > 1))
        {
            //
            // Only 2 DIIMMs per ch on socket 1
            // Subtract 1 DIMM for each channel
            //
            MemoryCapacity = (UINT64)((NumSlots - 2) * MAX_DIMM_SIZE) << 20; // in KB
            Type16Record->NumberOfMemoryDevices = (UINT16)NumSlots - 2;
        }
        else if ((PlatformType->BoardId == TypeYakimaCityCRB) ||
                 (PlatformType->BoardId == TypeYakimaCitySRP) ||
                 (PlatformType->BoardId == TypeYakimaCitySTHI) && !(Node & 1))
        {
            //
            // No ch 0 for either socket
            // Subtract 3 DIMMs for ch 0 on 2 sockets
            //
            MemoryCapacity = (UINT64)((NumSlots - 3) * MAX_DIMM_SIZE) << 20; // in KB
            Type16Record->NumberOfMemoryDevices = (UINT16)NumSlots - 3;
        }
        else
        {
            MemoryCapacity = (UINT64)(NumSlots * MAX_DIMM_SIZE) << 20; // in KB
            Type16Record->NumberOfMemoryDevices = (UINT16)NumSlots;
        }
        Type16Record->NumberOfMemoryDevices = 2;//Y240715_memory_slot
        //
        // AptioV Server Override End : Calculate the MemoryCapacity according to the Platform Type.
        //

        //
        //  AptioV Server Override Start : For capacity over 2 TB, use extended maximum capacity. Set maximum capacity
        //  to 2 TB to tell OS to look at extended maximum capacity instead.If Max capacity is greater than 2 TB, store
        //  in Extended Maximum Capacity else store in Max Capacity.
        //
        if (MemoryCapacity >= 0x80000000)
        {
            Type16Record->MaximumCapacity = 0x80000000;
            Type16Record->ExtendedMaximumCapacity = MemoryCapacity << 10; // Extended Max capacity should be stored in bytes.
        }
        else
        {
            Type16Record->MaximumCapacity = (UINT32)MemoryCapacity; // Max capacity should be stored in kilo bytes.
            Type16Record->ExtendedMaximumCapacity = 0;
        }
        //
        // AptioV Server Override End : if Max capacity is greater than 2 TB store
        // in Extended Maximum Capacity else store in Max Capacity.
        //

        //
        // Don't change it. This handle will be referenced by Type 17 and Type 19 SMBIOS Records
        //
        MemArraySmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
        Status = Smbios->Add(Smbios, NULL, &MemArraySmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *)Type16Record);
        FreePool(Type16Record);
        ASSERT_EFI_ERROR(Status);

        // AptioV server override start- Calculate the memory size in this node
        MemInfoTotalNodeMem = (UINT32)(SystemMemoryMap->NodeInfo[Node].MemSize << 6);

        MemoryArrayMappedEndAddress += (UINT64)LShiftU64(MemInfoTotalNodeMem, 10); // in KB
        // AptioV server override end- Calculate the memory size in this node
        if (MemInfoTotalNodeMem)
        { // AptioV server override : Publish the Type 19 structure only if any dimm's are populated in that node.

            //
            // Report Memory Array Mapped Address to Type 19 SMBIOS Record
            //

            Type19Record = AllocatePool(sizeof(SMBIOS_TABLE_TYPE19) + 1 + 1);
            ZeroMem(Type19Record, sizeof(SMBIOS_TABLE_TYPE19) + 1 + 1);
            Type19Record->Hdr.Type = EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS;
            Type19Record->Hdr.Length = sizeof(SMBIOS_TABLE_TYPE19);
            Type19Record->Hdr.Handle = 0;
            //
            // AptioV Server Override Start : Extended start and Extended end addresses support in Type 19 Structure.
            //
            //  Type19Record->StartingAddress = 0;
            //  Type19Record->EndingAddress = (UINT32) (RShiftU64 (TotalMemorySize, 10) - 1); // in KB
            if (MemoryArrayMappedEndAddress >= 0xFFFFFFFF)
            {
                //
                // End Address is greater than 4 TB so filling the "Start Address" and "End Address" as 0xFFFFFFFF and
                // storing actual address in "Extended Start" and "Extended End" Addresses
                //
                Type19Record->StartingAddress = 0xFFFFFFFF;
                Type19Record->EndingAddress = 0xFFFFFFFF;
                Type19Record->ExtendedStartingAddress = Type20StartingAddr << 10;              // Extended Start address is stored in bytes so left shift by 10.;
                Type19Record->ExtendedEndingAddress = (MemoryArrayMappedEndAddress << 10) - 1; // Extended End address is stored in bytes so left shift by 10.
            }
            else
            {
                //
                // End Address is less than 4 TB then store actual address in "Start Address" and "End Address"
                //
                Type19Record->StartingAddress = (UINT32)Type20StartingAddr;
                Type19Record->EndingAddress = (UINT32)(MemoryArrayMappedEndAddress - 1);
                Type19Record->ExtendedStartingAddress = 0;
                Type19Record->ExtendedEndingAddress = 0;
            }
            //
            // AptioV Server Override End : Extended start and Extended end addresses support in Type 19 Structure.
            //
            Type19Record->MemoryArrayHandle = MemArraySmbiosHandle;
            // Type19Record->PartitionWidth = 1; //AptioV Server Override : Update the Partition width depending upon no DIMM's present.

            //
            // Don't change it. This handle will be referenced by Type 20 SMBIOS Records
            //
            MemArrayMappedAddrSmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
            Status = Smbios->Add(Smbios, NULL, &MemArrayMappedAddrSmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *)Type19Record);
            FreePool(Type19Record);
            ASSERT_EFI_ERROR(Status);
        } // AptioV server override : Publish the Type 19 structure only if any dimm's are populated in that node.

        //
        // Accumulators for Type 20 structures
        //
        DimmStartingAddressMb = DimmEndingAddressMb = 0;

        //
        // Building Type 17 structures
        // Loop for each memory node
        //
        // AptioV server Override start
        /*
          for (Node = 0; Node < PlatformMaxSocket * MAX_HA; Node++) {
            MemorySpeed = 0;

        #ifdef SDP_FLAG
            MAX_MemorySpeed = 0;
        #endif
        */
        MemorySpeed = 0;
        // AptioV server Override end
        Socket = Node; // assume socket=node
        haNum = NODE_TO_MC(Node);

        if ((BspCpuidSignature == CPU_FAMILY_HSX) ||
            (BspCpuidSignature == CPU_FAMILY_BDX_DE) ||
            (BspCpuidSignature == CPU_FAMILY_BDX))
        {

            // These CPUIDs = 2 nodes per socket
            //
            Socket = Node >> 1;
        }

        for (Ch = 0; Ch < MAX_CH; Ch++)
        {
            //Y240715_memory_slot-Start
            if(Ch==0||Ch==2){
                continue;
            }
            //Y240715_memory_slot-End
            HaCh = Ch; // init array ptrs with Ch //AptioV server override
            if (SystemMemoryMap->NodeInfo[Node].NumChPerHA == 2)
            { // 2 HA case.
                if (((haNum == 0) && (Ch >= 2)) || ((haNum == 1) && (Ch < 2)))
                    continue; // skip ha0ch2,3 and ha1ch 0,1
                // 2ha : ch0,1,2,3 -> ha0ch0,1 and Ha1ch0,1
                if (haNum == 1)
                {
                    HaCh = Ch - 2;
                }
            }

            for (Dimm = 0; Dimm < PlatformDimmsPerChannel; Dimm++)
            {

                DimmPresent = TRUE; // assume DIMM found

                if ((BspCpuidSignature == CPU_FAMILY_HSX) ||
                    (BspCpuidSignature == CPU_FAMILY_BDX_DE) ||
                    (BspCpuidSignature == CPU_FAMILY_BDX))
                {

                    // For odd-numbered nodes, change ch 0 -> ch 2 and ch 1 -> ch 3
                    //  to get correct strings from DIMM string array
                    //
                    // AptioV server Override start
                    /* if (Node & 1) {
                       ChString += 2;
                     }*/
                    if (NoofChannelsPerHA == 2)
                    {
                        //
                        // Change the channel for the second memory controller.
                        // channel 0 -> channel 2 ; channel 1 -> channel 3
                        //

                        //
                        // Suppress the channels 2 and 3 for Node 0
                        // and channels 0 and 1 for Node1.
                        //
                        if (Node % 2)
                        {
                            if (Ch < 2)
                            {
                                Ch = Ch + 2;
                            }
                            HaCh = Ch % 2;
                        }
                        else
                        {
                            // Suppressing the channels 2 and 3 for IMC 0.
                            if (Ch > 1)
                            {
                                continue;
                            }
                            HaCh = Ch;
                        }
                    }
                    else
                    {
                        HaCh = Ch;
                    }
                }
                else
                {
                    Socket = Node;
                    HaCh = Ch;
                }
                // AptioV server Override end
#ifndef GRANGEVILLE_FLAG
                //
                // Accomodate platforms with asymmetrical DIMM configurations
                //
                if (PlatformType->BoardId == TypeMayanCitySRP)
                {
                    if (Socket == 1)
                    {
                        if (Dimm >= 2)
                        {
                            continue;
                        }
                    }
                }

                if ((PlatformType->BoardId == TypeYakimaCityCRB) ||
                    (PlatformType->BoardId == TypeYakimaCitySRP) ||
                    (PlatformType->BoardId == TypeYakimaCitySTHI))
                {
                    if (Ch == 0)
                    {
                        continue;
                    }
                }
#endif

#if defined(GRANGEVILLE_FLAG) || defined(DE_SKU)

                if (
                    (PlatformType->BoardId == TypeBeverlyCoveCRB) ||
                    (PlatformType->BoardId == TypeBeverlyCoveSODimmCRB) ||
                    (PlatformType->BoardId == TypeCamelbackMountain) ||
                    (PlatformType->BoardId == TypeTlaquepaque) ||
                    (PlatformType->BoardId == TypeTrabucoCanyon))
                {

                    // We should skip the Node =1  for  DE. As only one socket is valid. 1 socket -> 2 channels

                    if (SystemMemoryMap->NodeInfo[Node].Enabled == 0)
                    {
                        continue;
                    }
                }
#endif

                if (SystemMemoryMap->NodeInfo[Node].Enabled == 0 ||
                    SystemMemoryMap->NodeInfo[Node].ChannelInfo[HaCh].Enabled == 0 ||
                    SystemMemoryMap->NodeInfo[Node].ChannelInfo[HaCh].DimmInfo[Dimm].Present == 0)
                {
                    DimmPresent = FALSE;
                }

                DeviceLocator = mDimmToDevLocator[(Socket * 12) + (Ch * 3) + Dimm];
                //
                // AptioV Server Override Start : Fix for Invalid banklocator coming for 1HA case.
                //
                if (NoofHAPerSocket == 1)
                {
                    BankLocator = mDimmToBankLocator[Node >> 1];
                }
                else
                {
                    BankLocator = mDimmToBankLocator[Node]; // Display the node in the bank locator.
                }
                //
                // AptioV Server Override End : Fix for Invalid banklocator coming for 1HA case.
                //
                AssetTag = STRING_TOKEN(STR_MEMORY_SUBCLASS_UNKNOWN);
                SerialNumber = STRING_TOKEN(STR_MEMORY_SUBCLASS_UNKNOWN);
                PartNumber = STRING_TOKEN(STR_MEMORY_SUBCLASS_UNKNOWN);

                PlatformInfo->SocketSupportedBitmap |= (1 << Socket);
                PlatformInfo->NodeInfo[Node].Valid = TRUE;
                PlatformInfo->NodeInfo[Node].ChannelSupportedBitmap |= (1 << HaCh);
                PlatformInfo->NodeInfo[Node].ChannelInfo[HaCh].Valid = TRUE;
                PlatformInfo->NodeInfo[Node].ChannelInfo[HaCh].DimmSlotSupportedBitmap |= (1 << Dimm);
                PlatformInfo->NodeInfo[Node].ChannelInfo[HaCh].numDimmSlotsSupp = SystemMemoryMap->MemSetup.socket[Socket].ddrCh[Ch].numDimmSlots; // AptioV Server Override : Ch should be considered if Socket is used.

                if (DimmPresent)
                {
                    //  AptioV server override start: Update Manufacturer String, Serial No. and Part No.
                    //  Update Partition width in Type 19 structure.
                    //  Partition width is no of Dimms present so increament for every Dimm present.
                    //  This will be added in Type 19 structure
                    PartitionWidth++;
                    //
                    // Update Manufacturer String
                    //
                    ManfData = 0;
                    ManfData = SystemMemoryMap->NodeInfo[Node].ChannelInfo[HaCh].DimmInfo[Dimm].SPDMMfgId;
                    IdTblPtr = BankTable;
                    ZeroMem(TempString, StringBufferSize);
                    for (j = 0; j < 37; j++)
                    {
                        if ((IdTblPtr[j].VendorId == ManfData) || (IdTblPtr[j].VendorId == 0xff00))
                        {
                            CopyMem(TempString, IdTblPtr[j].ManufacturerString, (2 * StrLen(IdTblPtr[j].ManufacturerString)));
                            break;
                        }
                    }
                    Manufacturer = HiiSetString(HiiHandle, 0, TempString, NULL);
                    //
                    // Update Serial No
                    //
                    ZeroMem(StringBuffer2, StringBufferSize);
                    ZeroMem(StringBuffer, StringBufferSize);
                    for (j = 0; j < 4; j++)
                    {
                        ZeroMem(TempString, StringBufferSize);
                        if (SystemMemoryMap->NodeInfo[Node].ChannelInfo[HaCh].DimmInfo[Dimm].SPDModSN[j] <= 0x0F)
                        {
                            StrCpy(TempString, L"0");
                        }
                        UnicodeValueToString(
                            StringBuffer2,
                            RADIX_HEX,
                            SystemMemoryMap->NodeInfo[Node].ChannelInfo[HaCh].DimmInfo[Dimm].SPDModSN[j],
                            2);
                        StrCat(TempString, StringBuffer2);
                        StrCat(StringBuffer, TempString);
                    }
                    SerialNumber = HiiSetString(HiiHandle, 0, StringBuffer, NULL);

                    //
                    // Update Part No
                    //
                    Data8 = 0;
                    ZeroMem(StringBuffer, StringBufferSize);
                    ZeroMem(TempString, StringBufferSize);

                    if (SystemMemoryMap->NodeInfo[Node].DramType == SPD_TYPE_DDR3)
                    {
                        // APTIOV_SERVER_OVERRIDE_START : To allow maximum characters for Part number
                        MaxIndex = SPD_MODULE_PART;
                        // APTIOV_SERVER_OVERRIDE_END : To allow maximum characters for Part number
                    }
                    else
                    {
                        // APTIOV_SERVER_OVERRIDE_START : To allow maximum characters for Part number
                        MaxIndex = SPD_MODULE_PART_DDR4;
                        // APTIOV_SERVER_OVERRIDE_END : To allow maximum characters for Part number
                    }

                    for (j = 0; j < MaxIndex; j++)
                    {
                        Data8 = SystemMemoryMap->NodeInfo[Node].ChannelInfo[HaCh].DimmInfo[Dimm].SPDModPart[j];
                        if (Data8 == 0)
                            Data8 = 0x20;
                        TempString[0] = FilterCharacter(Data8);
                        StringBuffer[j] = TempString[0];
                    }
                    PartNumber = HiiSetString(HiiHandle, 0, StringBuffer, NULL);
                    //
                    // AptioV server override: End
                    //
                    MemoryDeviceSize = SystemMemoryMap->NodeInfo[Node].ChannelInfo[HaCh].DimmInfo[Dimm].DimmSize; // in 64MB
                    MemoryDeviceSize = MemoryDeviceSize << 6;                                                     // in MB
                    DimmEndingAddressMb = DimmStartingAddressMb + MemoryDeviceSize;
                    DEBUG((EFI_D_INFO, "Socket - 0x%x Channel - 0x%x Dimm - 0x%x Dimm Size = %dGB\n", Socket, Ch, Dimm, MemoryDeviceSize >> 10));

                    //
                    // AptioV server override start - Reference index converted to speed
                    //
                    // MemorySpeed = MemInfoMemFreq;
                    switch (SystemMemoryMap->NodeInfo[Node].MemFreq)
                    {
                    case DDR_800:
                        MemorySpeed = 800;
                        break;
                    case DDR_1000:
                        MemorySpeed = 1000;
                        break;
                    case DDR_1067:
                        MemorySpeed = 1066;
                        break;
                    case DDR_1200:
                        MemorySpeed = 1200;
                        break;
                    case DDR_1333:
                        MemorySpeed = 1333;
                        break;
                    case DDR_1400:
                        MemorySpeed = 1400;
                        break;
                    case DDR_1600:
                        MemorySpeed = 1600;
                        break;
                    case DDR_1800:
                        MemorySpeed = 1800;
                        break;
                    case DDR_1867:
                        MemorySpeed = 1866;
                        break;
                    case DDR_2000:
                        MemorySpeed = 2000;
                        break;
                    case DDR_2133:
                        MemorySpeed = 2133;
                        break;
                    case DDR_2200:
                        MemorySpeed = 2200;
                        break;
                    case DDR_2400:
                        MemorySpeed = 2400;
                        break;
                    case DDR_2600:
                        MemorySpeed = 2600;
                        break;
                    case DDR_2667:
                        MemorySpeed = 2666;
                        break;
                    case DDR_2800:
                        MemorySpeed = 2800;
                        break;
                    case DDR_2933:
                        MemorySpeed = 2933;
                        break;
                    case DDR_3000:
                        MemorySpeed = 3000;
                        break;
                    case DDR_3200:
                        MemorySpeed = 3200;
                        break;
                    default:
                        MemorySpeed = 0;
                        break;
                    }
                    //
                    // AptioV server override: End
                    //

                    ///
                    /// @todo Need a non-hard-coded way to get these
                    ///
                    MemoryTotalWidth = 72;
#ifdef SDP_FLAG
                    MemoryDataWidth = 64;
#else
                    MemoryDataWidth = 72;
#endif
                }
                else
                {
                    StrCpy(StringBuffer, L"NO DIMM");
                    Manufacturer = HiiSetString(HiiHandle, 0, StringBuffer, NULL);
#ifdef SDP_FLAG
                    MAX_MemorySpeed = 0;
#endif
                    // AptioV server override: Start - Add serial number and part number to Hii Database
                    //
                    SerialNumber = HiiSetString(HiiHandle, 0, StringBuffer, NULL);
                    PartNumber = HiiSetString(HiiHandle, 0, StringBuffer, NULL);
                    //
                    // AptioV server override: End
                    //
                    MemoryDeviceSize = 0;
                    MemorySpeed = 0;
                    MemoryTotalWidth = 0;
                    MemoryDataWidth = 0;
                }
                //
                // AptioV Server Override Start: Fill DeviceLocator field with No DIMM when Dimm is not present.
                //
                if (!DimmPresent)
                {
                    StrCpy(StringBuffer, L"NO DIMM");
                    DeviceLocator = HiiSetString(HiiHandle, 0, StringBuffer, NULL);
                }
                //
                // AptioV Server Override End: Fill DeviceLocator field with NO DIMM when Dimm is not present.
                //
                //
                // Get strings from HII database
                //

                DeviceLocatorStr = HiiGetPackageString(&mEfiMemorySubClassDriverGuid, DeviceLocator, NULL);
                //
                // AptioV server override start - Asserting if string is NULL
                //
                if (DeviceLocatorStr == NULL)
                {
                    DeviceLocatorStr = AllocatePool(sizeof(CHAR16) * SMBIOS_STRING_MAX_LENGTH);
                    ASSERT(DeviceLocatorStr != NULL);
                    StrCpy(DeviceLocatorStr, L"Details could not be retrieved");
                }
                //
                // AptioV server override end.- Asserting if string is NULL
                //
                DeviceLocatorStrLen = StrLen(DeviceLocatorStr);
                ASSERT(DeviceLocatorStrLen <= SMBIOS_STRING_MAX_LENGTH);

                //
                // AptioV server override start - Update Asset Tag
                //
                if (DimmPresent)
                {
                    StrCpy(TempString, L"_AssetTag");
                    ZeroMem(StringBufferAsset, StringBufferSize);
                    StrCat(StringBufferAsset, DeviceLocatorStr);
                    StrCat(StringBufferAsset, TempString);
                    AssetTag = HiiSetString(HiiHandle, 0, StringBufferAsset, NULL);
                }
                else
                {
                    StrCpy(StringBuffer, L"NO DIMM");
                    AssetTag = HiiSetString(HiiHandle, 0, StringBuffer, NULL);
                }
                //
                // AptioV server override : End
                //
                //
                // AptioV Server Override Start: Fill BankLocator field with No DIMM when Dimm is not present.
                //
                if (!DimmPresent)
                {
                    StrCpy(StringBuffer, L"NO DIMM");
                    BankLocator = HiiSetString(HiiHandle, 0, StringBuffer, NULL);
                }
                //
                // AptioV Server Override End: Fill BankLocator field with No DIMM when Dimm is not present.
                //
                BankLocatorStr = HiiGetPackageString(&mEfiMemorySubClassDriverGuid, BankLocator, NULL);
                //
                // AptioV server override start - Asserting if string is NULL
                //
                if (BankLocatorStr == NULL)
                {
                    BankLocatorStr = AllocatePool(sizeof(CHAR16) * SMBIOS_STRING_MAX_LENGTH);
                    ASSERT(BankLocatorStr != NULL);
                    StrCpy(BankLocatorStr, L"Details could not be retrieved");
                }
                //
                // AptioV server override end - Asserting if string is NULL
                //
                BankLocatorStrLen = StrLen(BankLocatorStr);
                ASSERT(BankLocatorStrLen <= SMBIOS_STRING_MAX_LENGTH);

                ManufactureStr = HiiGetPackageString(&mEfiMemorySubClassDriverGuid, Manufacturer, NULL);
                //
                // AptioV server override start - Asserting if string is NULL
                //
                if (ManufactureStr == NULL)
                {
                    ManufactureStr = AllocatePool(sizeof(CHAR16) * SMBIOS_STRING_MAX_LENGTH);
                    ASSERT(ManufactureStr != NULL);
                    StrCpy(ManufactureStr, L"Details could not be retrieved");
                }
                //
                // AptioV server override end - Asserting if string is NULL
                //
                ManufactureStrLen = StrLen(ManufactureStr);
                ASSERT(ManufactureStrLen <= SMBIOS_STRING_MAX_LENGTH);

                SerialNumberStr = HiiGetPackageString(&mEfiMemorySubClassDriverGuid, SerialNumber, NULL);
                //
                // AptioV server override : START - Asserting if string is NULL
                //
                if (SerialNumberStr == NULL)
                {
                    SerialNumberStr = AllocatePool(sizeof(CHAR16) * SMBIOS_STRING_MAX_LENGTH);
                    ASSERT(SerialNumberStr != NULL);
                    StrCpy(SerialNumberStr, L"Details could not be retrieved");
                }
                //
                // AptioV server override : END - Asserting if string is NULL
                //
                SerialNumberStrLen = StrLen(SerialNumberStr);
                ASSERT(SerialNumberStrLen <= SMBIOS_STRING_MAX_LENGTH);

                AssertTagStr = HiiGetPackageString(&mEfiMemorySubClassDriverGuid, AssetTag, NULL);
                //
                // AptioV server override : START - Asserting if string is NULL
                //
                if (AssertTagStr == NULL)
                {
                    AssertTagStr = AllocatePool(sizeof(CHAR16) * SMBIOS_STRING_MAX_LENGTH);
                    ASSERT(AssertTagStr != NULL);
                    StrCpy(AssertTagStr, L"Details could not be retrieved");
                }
                //
                // AptioV server override : END - Asserting if string is NULL
                //
                AssertTagStrLen = StrLen(AssertTagStr);
                ASSERT(AssertTagStrLen <= SMBIOS_STRING_MAX_LENGTH);

                PartNumberStr = HiiGetPackageString(&mEfiMemorySubClassDriverGuid, PartNumber, NULL);
                //
                // AptioV server override : START - Asserting if string is NULL
                //
                if (PartNumberStr == NULL)
                {
                    PartNumberStr = AllocatePool(sizeof(CHAR16) * SMBIOS_STRING_MAX_LENGTH);
                    ASSERT(PartNumberStr != NULL);
                    StrCpy(PartNumberStr, L"Details could not be retrieved");
                }
                //
                // AptioV server override : END - Asserting if string is NULL
                //
                PartNumberStrLen = StrLen(PartNumberStr);
                ASSERT(PartNumberStrLen <= SMBIOS_STRING_MAX_LENGTH);

                //
                // Report Memory Device to Type 17 SMBIOS Record
                //

                TableSize = sizeof(SMBIOS_TABLE_TYPE17) + DeviceLocatorStrLen + 1 + BankLocatorStrLen + 1 + ManufactureStrLen + 1 + SerialNumberStrLen + 1 + AssertTagStrLen + 1 + PartNumberStrLen + 1 + 1;
                Type17Record = AllocatePool(TableSize);
                ZeroMem(Type17Record, TableSize);
                Type17Record->Hdr.Type = EFI_SMBIOS_TYPE_MEMORY_DEVICE;
                Type17Record->Hdr.Length = sizeof(SMBIOS_TABLE_TYPE17);
                Type17Record->Hdr.Handle = 0;
                Type17Record->MemoryArrayHandle = MemArraySmbiosHandle;
                Type17Record->MemoryErrorInformationHandle = 0xFFFE;
                Type17Record->TotalWidth = MemoryTotalWidth;
                Type17Record->DataWidth = MemoryDataWidth;
                // AptioV Server Override : Store the size of device either in size or extended size element depending upon Memory device size.
                // Type17Record->Size = MemoryDeviceSize; // in MB
                if (DimmPresent)
                {
                    switch (SystemMemoryMap->NodeInfo[Node].DimmType + 1)
                    {
                    case SPD_RDIMM:
                        Type17Record->FormFactor = MemoryFormFactorDimm; // AptioV Server Override: RDIMM is type of DIMM. So, use the apt value of DIMM instead of RIM.
                        break;
                    case SPD_UDIMM:
                        Type17Record->FormFactor = MemoryFormFactorDimm;
                        break;
                    case SPD_SODIMM:
                        Type17Record->FormFactor = MemoryFormFactorSodimm;
                        break;
                    default:
                        Type17Record->FormFactor = MemoryFormFactorDimm;
                    }
                    // Type17Record->FormFactor = MemoryFormFactorDimm;
                }
                else
                {
                    Type17Record->FormFactor = MemoryFormFactorUnknown; // AptioV Server Override: When No Dimm is present, Formfactor is set as Unknown.
                }
                Type17Record->DeviceLocator = 1;
                Type17Record->BankLocator = 2;
                if (DimmPresent)
                { // AptioV Server Override
                    if (SystemMemoryMap->NodeInfo[Node].DramType == SPD_TYPE_DDR3)
                    {
                        Type17Record->MemoryType = MemoryTypeDdr3;
                    }
                    else
                    {
                        // TODO: 130709 Hard coded value needs to be replaced with MemoryTypeDdr4.
                        // Will be enumerated in MdePkg\Include\IndustryStandard\SmBios.h with
                        // SmBios specification update.
                        Type17Record->MemoryType = MemoryTypeDdr4; // Memory Type DDR4 = 0x1a, SMBIOS spec specific.
                    }
                    Type17Record->TypeDetail.Synchronous = 1;
                }
                else
                {                                                 // AptioV Server Override
                    Type17Record->MemoryType = MemoryTypeUnknown; // AptioV Server Override: When No Dimm is present, Memory type is set as Unknown.
                    Type17Record->TypeDetail.Unknown = 1;         // AptioV Server Override: When No Dimm is present, TypeDetail is set as Unknown.
                }
                // AptioV server override start
                Type17Record->Speed = SystemMemoryMap->NodeInfo[Node].ChannelInfo[HaCh].DimmInfo[Dimm].DimmSpeed; // AptioV Server Override: Updating capable speed of the device in MHz
                Type17Record->ConfiguredMemoryClockSpeed = MemorySpeed;
                // Store the size of device either in size or extended size element depending upon Memory device size.
                // Size field can only hold up to 32GB - 1MB, because setting bit 15
                // indicates the value is in KB rather than MB. If the DIMM size is
                // that large or larger, use the ExtendedSize field.
                //
                if (MemoryDeviceSize >= 0x7fff)
                {
                    Type17Record->Size = 0x7fff;                   // max value
                    Type17Record->ExtendedSize = MemoryDeviceSize; // in MB
                }
                else
                {
                    Type17Record->Size = MemoryDeviceSize; // in MB
                }
                /*
                #ifdef SDP_FLAG
                        switch (SystemMemoryMap->NodeInfo[Node].commonTck) {
                          case DDR_800_TCK_MIN:
                            MAX_MemorySpeed = 800;
                            break;
                          case DDR_1000_TCK_MIN:
                            MAX_MemorySpeed = 1000;
                            break;
                          case DDR_1067_TCK_MIN:
                            MAX_MemorySpeed = 1066;
                            break;
                          case DDR_1200_TCK_MIN:
                            MAX_MemorySpeed = 1200;
                            break;
                          case DDR_1333_TCK_MIN:
                            MAX_MemorySpeed = 1333;
                            break;
                          case DDR_1400_TCK_MIN:
                            MAX_MemorySpeed = 1400;
                            break;
                          case DDR_1600_TCK_MIN:
                            MAX_MemorySpeed = 1600;
                            break;
                          case DDR_1800_TCK_MIN:
                            MAX_MemorySpeed = 1800;
                            break;
                          case DDR_1867_TCK_MIN:
                            MAX_MemorySpeed = 1866;
                            break;
                          case DDR_2000_TCK_MIN:
                            MAX_MemorySpeed = 2000;
                            break;
                          case DDR_2133_TCK_MIN:
                            MAX_MemorySpeed = 2133;
                            break;
                          case DDR_2200_TCK_MIN:
                            MAX_MemorySpeed = 2200;
                            break;
                          case DDR_2400_TCK_MIN:
                            MAX_MemorySpeed = 2400;
                            break;
                          case DDR_2600_TCK_MIN:
                            MAX_MemorySpeed = 2600;
                            break;
                          case DDR_2667_TCK_MIN:
                            MAX_MemorySpeed = 2666;
                            break;
                          case DDR_2800_TCK_MIN:
                            MAX_MemorySpeed = 2800;
                            break;
                          case DDR_2933_TCK_MIN:
                            MAX_MemorySpeed = 2933;
                            break;
                          case DDR_3000_TCK_MIN:
                            MAX_MemorySpeed = 3000;
                            break;
                          case DDR_3200_TCK_MIN:
                            MAX_MemorySpeed = 3200;
                            break;
                        }
                        Type17Record->Speed = MAX_MemorySpeed;
                        Type17Record->ConfiguredMemoryClockSpeed = MemorySpeed*2;
                  // AptioV server override end*/
                if (DimmPresent)
                {
                    Type17Record->Attributes = SystemMemoryMap->NodeInfo[Node].ChannelInfo[HaCh].DimmInfo[Dimm].NumRanks;
                }
                else
                {
                    Type17Record->Attributes = 0;
                }
                // AptioV server override start
                // #else
                //         Type17Record->Attributes = Dimm + 1;
                // #endif
                //  AptioV server override end
                Type17Record->Manufacturer = 3;
                Type17Record->SerialNumber = 4;
                Type17Record->AssetTag = 5;
                Type17Record->PartNumber = 6;
                //        Type17Record->Attributes = Dimm + 1; // AptioV server override

                OptionalStrStart = (CHAR8 *)(Type17Record + 1);
                UnicodeStrToAsciiStr(DeviceLocatorStr, OptionalStrStart);
                UnicodeStrToAsciiStr(BankLocatorStr, OptionalStrStart + DeviceLocatorStrLen + 1);
                UnicodeStrToAsciiStr(ManufactureStr, OptionalStrStart + DeviceLocatorStrLen + 1 + BankLocatorStrLen + 1);
                UnicodeStrToAsciiStr(SerialNumberStr, OptionalStrStart + DeviceLocatorStrLen + 1 + BankLocatorStrLen + 1 + ManufactureStrLen + 1);
                UnicodeStrToAsciiStr(AssertTagStr, OptionalStrStart + DeviceLocatorStrLen + 1 + BankLocatorStrLen + 1 + ManufactureStrLen + 1 + SerialNumberStrLen + 1);
                UnicodeStrToAsciiStr(PartNumberStr, OptionalStrStart + DeviceLocatorStrLen + 1 + BankLocatorStrLen + 1 + ManufactureStrLen + 1 + SerialNumberStrLen + 1 + AssertTagStrLen + 1);

                //
                // Don't change it. This handle will be referenced by Type 20 SMBIOS Records
                //
                MemDevSmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
                Status = Smbios->Add(Smbios, NULL, &MemDevSmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *)Type17Record);
                FreePool(DeviceLocatorStr);
                FreePool(BankLocatorStr);
                FreePool(ManufactureStr);
                FreePool(SerialNumberStr);
                FreePool(AssertTagStr);
                FreePool(PartNumberStr);
                FreePool(Type17Record);
                ASSERT_EFI_ERROR(Status);

                if (DimmPresent)
                {

                    if ((MAX_SOCKET * MAX_CH * MAX_DIMM) <= Index)
                    {
                        DEBUG((EFI_D_INFO, "Asserting the System Index Bufferflow"));

                        ASSERT_EFI_ERROR(EFI_INVALID_PARAMETER);
                    }

                    MemoryAddress = LShiftU64(SystemMemoryMap->Element[Index].BaseAddress, MEM_ADDR_SHFT_VAL);
                    RowLength = LShiftU64(MemoryDeviceSize, 20);

                    //
                    // Report Memory Device Mapped address to Type 20 SMBIOS Record
                    //
                    Type20Record = AllocatePool(sizeof(SMBIOS_TABLE_TYPE20) + 1 + 1);
                    ZeroMem(Type20Record, sizeof(SMBIOS_TABLE_TYPE20) + 1 + 1);

                    Type20Record->Hdr.Type = EFI_SMBIOS_TYPE_MEMORY_DEVICE_MAPPED_ADDRESS;
                    Type20Record->Hdr.Length = sizeof(SMBIOS_TABLE_TYPE20);
                    Type20Record->Hdr.Handle = 0;
                    //
                    //  AptioV Server Override Start
                    //
                    //  Support Extended start and Extended end address in Type 20 Structure.
                    //      Type20Record->StartingAddress = (UINT32) (DimmStartingAddressMb << 10);    // in kB
                    //      Type20Record->EndingAddress = (UINT32) ((DimmEndingAddressMb << 10) - 1);  // in kB
                    Type20EndAddress = Type20StartingAddr + (MemoryDeviceSize << 10);
                    if (Type20EndAddress >= 0xFFFFFFFF)
                    {
                        //
                        // End Address is greater than 4 TB so filling the "Start Address" and "End Address" as 0xFFFFFFFF and
                        // storing actual address in "Extended Start" and "Extended End" elements
                        //
                        Type20Record->StartingAddress = 0xFFFFFFFF;
                        Type20Record->EndingAddress = 0xFFFFFFFF;
                        Type20Record->ExtendedStartingAddress = Type20StartingAddr << 10; // Extended Start address is stored in bytes so left shift by 10.
                        DEBUG((EFI_D_INFO, "Memory Start address  = %x \n", Type20Record->ExtendedStartingAddress));
                        Type20StartingAddr += MemoryDeviceSize << 10;
                        Type20Record->ExtendedEndingAddress = (Type20StartingAddr << 10) - 1; // Extended End address is stored in bytes so left shift by 10.
                        DEBUG((EFI_D_INFO, "Memory Ending address  = %x \n", Type20Record->ExtendedEndingAddress));
                    }
                    else
                    {
                        //
                        // End Address is less than 4 TB then store actual address in "Start Address" and "End Address" elements.
                        //
                        Type20Record->StartingAddress = (UINT32)Type20StartingAddr;
                        DEBUG((EFI_D_INFO, "Memory Start address  = %x \n", Type20Record->StartingAddress));
                        Type20StartingAddr += MemoryDeviceSize << 10;
                        Type20Record->EndingAddress = (UINT32)(Type20StartingAddr - 1);
                        DEBUG((EFI_D_INFO, "Memory Ending address  = %x \n", Type20Record->EndingAddress));
                        Type20Record->ExtendedStartingAddress = 0;
                        Type20Record->ExtendedEndingAddress = 0;
                    }
                    // Programming the "MemoryDevice" and "MemoryArrayMappedAddress" handles correctly.
                    // Type20Record->MemoryDeviceHandle = MemArrayMappedAddrSmbiosHandle;
                    // Type20Record->MemoryArrayMappedAddressHandle = MemDevSmbiosHandle;
                    Type20Record->MemoryDeviceHandle = MemDevSmbiosHandle;
                    Type20Record->MemoryArrayMappedAddressHandle = MemArrayMappedAddrSmbiosHandle;

                    //
                    //  Update PartitionRowPosition in Type20 structure
                    //
                    // We get whole row from one device
                    // Type20Record->PartitionRowPosition = 0xFF;
                    Type20Record->PartitionRowPosition = 1;

                    DimmStartingAddressMb = DimmEndingAddressMb;

                    MemDevMappedAddrSmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
                    Status = Smbios->Add(Smbios, NULL, &MemDevMappedAddrSmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *)Type20Record);
                    FreePool(Type20Record);
                    Index++;
                }

            } // End of DIMM for loop
        }     // End of Channel for loop

        //  Update Partition Width in Type 19 structure,one for each memory controller(NODE)
        // Publish the Type 19 structure only if any dimm's are populated in that node.
        if (MemInfoTotalNodeMem)
        {
            Type = 19;
            Status = Smbios->GetNext(Smbios, &SmbiosHandle, &Type, &Record, NULL);
            Type19Record = (SMBIOS_TABLE_TYPE19 *)Record;
            Type19Record->PartitionWidth = PartitionWidth;
            PartitionWidth = 0;
        }
    } // End of Node for loop
      //
      //  AptioV server override end
      //
    gBS->InstallProtocolInterface(
        &PlatformInfoHandle,
        &gPlatformSlotInfoProtocolGuid,
        EFI_NATIVE_INTERFACE,
        PlatformInfo);

    gBS->FreePool(StringBuffer);
    //
    // AptioV server override start : Filling Type17 info
    //
    gBS->FreePool(StringBufferAsset);
    gBS->FreePool(TempString);
    //
    // AptioV server override end
    //
    return Status;
}
```
Windows中资源管理器内存插槽显示的问题和SMBIOS的Type16，Type17相关
SMBIOS Type16
Type16
```C
///
/// Physical Memory Array (Type 16).
///
/// This structure describes a collection of memory devices that operate 
/// together to form a memory address space. 
///
typedef struct {
  SMBIOS_STRUCTURE          Hdr;
  UINT8                     Location;                       ///< The enumeration value from MEMORY_ARRAY_LOCATION.
  UINT8                     Use;                            ///< The enumeration value from MEMORY_ARRAY_USE.
  UINT8                     MemoryErrorCorrection;          ///< The enumeration value from MEMORY_ERROR_CORRECTION.
  UINT32                    MaximumCapacity;
  UINT16                    MemoryErrorInformationHandle;
  UINT16                    NumberOfMemoryDevices;
  //
  // Add for smbios 2.7
  //
  UINT64                    ExtendedMaximumCapacity;
} SMBIOS_TABLE_TYPE16;
```

Type17
```C
typedef struct {
  UINT16    Reserved        :1;
  UINT16    Other           :1;
  UINT16    Unknown         :1;
  UINT16    FastPaged       :1;
  UINT16    StaticColumn    :1;
  UINT16    PseudoStatic    :1;
  UINT16    Rambus          :1;
  UINT16    Synchronous     :1;
  UINT16    Cmos            :1;
  UINT16    Edo             :1;
  UINT16    WindowDram      :1;
  UINT16    CacheDram       :1;
  UINT16    Nonvolatile     :1;
  UINT16    Registered      :1;
  UINT16    Unbuffered      :1;
  UINT16    LrDimm          :1;
} MEMORY_DEVICE_TYPE_DETAIL;

///
/// Memory Device (Type 17).
///
/// This structure describes a single memory device that is part of 
/// a larger Physical Memory Array (Type 16).
/// Note:  If a system includes memory-device sockets, the SMBIOS implementation 
/// includes a Memory Device structure instance for each slot, whether or not the 
/// socket is currently populated.
///
typedef struct {
  SMBIOS_STRUCTURE          Hdr;
  UINT16                    MemoryArrayHandle;
  UINT16                    MemoryErrorInformationHandle;
  UINT16                    TotalWidth;
  UINT16                    DataWidth;
  UINT16                    Size;
  UINT8                     FormFactor;                     ///< The enumeration value from MEMORY_FORM_FACTOR.
  UINT8                     DeviceSet;
  SMBIOS_TABLE_STRING       DeviceLocator;
  SMBIOS_TABLE_STRING       BankLocator;
  UINT8                     MemoryType;                     ///< The enumeration value from MEMORY_DEVICE_TYPE.
  MEMORY_DEVICE_TYPE_DETAIL TypeDetail;
  UINT16                    Speed;
  SMBIOS_TABLE_STRING       Manufacturer;
  SMBIOS_TABLE_STRING       SerialNumber;
  SMBIOS_TABLE_STRING       AssetTag;
  SMBIOS_TABLE_STRING       PartNumber;
  //
  // Add for smbios 2.6
  //  
  UINT8                     Attributes;
  //
  // Add for smbios 2.7
  //
  UINT32                    ExtendedSize;
  UINT16                    ConfiguredMemoryClockSpeed;
  //
  // Add for smbios 2.8.0
  //
  UINT16                    MinimumVoltage;
  UINT16                    MaximumVoltage;
  UINT16                    ConfiguredVoltage;
} SMBIOS_TABLE_TYPE17;
```


SMBIOS SPEC
![[SMBIOS_V_3_7_1.pdf]]