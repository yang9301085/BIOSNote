// Source:
// F:\FP5IA001\FP5IA001\AgesaModulePkg\Nbio\GFX\AmdNbioGfxRVPei\GfxEnumConnectors.c
// Hotplug fix: stop marking DDI2 as USB_C_TYPE

    if (DdiCounter == 3) {
      StoreDisplayPathList->usCaps |= (DdiComplexData[DdiCounter].Type.Ddi.DdiData.Flags | ATOM_ENCODER_CAP_RECORD_USB_C_TYPE);
    }

