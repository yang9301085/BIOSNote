// Source:
// F:\FP5IA001\FP5IA001\AgesaModulePkg\Nbio\GFX\AmdNbioGfxRVPei\GfxEnumConnectors.c
// Open issue: Mapping[0]/Mapping[1] are not copied into Engine[] here

  do {
    Engine[Index].Type.Ddi.DdiData.ConnectorType = DdiConfig[Index].Ddi.ConnectorType;
    Engine[Index].Type.Ddi.DdiData.AuxIndex = DdiConfig[Index].Ddi.AuxIndex;
    Engine[Index].Type.Ddi.DdiData.HdpIndex = DdiConfig[Index].Ddi.HdpIndex;
    Engine[Index].Type.Ddi.DdiData.LanePnInversionMask = DdiConfig[Index].Ddi.LanePnInversionMask;
    Engine[Index].Type.Ddi.DdiData.Flags = DdiConfig[Index].Ddi.Flags;

    if (DdiConfig[Index].Flags == DESCRIPTOR_TERMINATE_LIST) {
      LocalFlag = TRUE;
    }

