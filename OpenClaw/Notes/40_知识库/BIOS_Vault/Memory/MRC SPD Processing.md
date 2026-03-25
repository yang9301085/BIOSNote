* Intel MRC 中get SPD中信息并通过计算写入Memory Timing
```C
/** @file
  By passing in a SPD data structure and platform support values, an output
  structure is populated with DIMM configuration information.

@copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2021 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#include "MrcSpdProcessing.h"
#include "MrcCommonTypes.h"
#include "MrcDdr5.h"
#include "MrcLpddr5.h"
#include <Library/IoLib.h> //Jessie_debug+


//Y240131_yqr_define-Start
UINT32 g_DDR4tCLKArr[2]={0};
//Y240131_yqr_define-End

#ifdef MRC_DEBUG_PRINT
const char  UnknownString[]    = "unknown";
const char  Ddr4String[]       = "DDR4";
const char  Ddr5String[]       = "DDR5";
const char  RdimmString[]      = "RDIMM";
const char  UdimmString[]      = "UDIMM";
const char  SodimmString[]     = "SO-DIMM";
const char  Sodimm72String[]   = "72 bit SO-DIMM";
const char  StdString[]        = "Standard";
const char  CustomString[]     = "Custom";
const char  Xmp1String[]       = "XMP1";
const char  Xmp2String[]       = "XMP2";
const char  Xmp3String[]       = "XMP3";
const char  User4String[]      = "USER4";
const char  User5String[]      = "USER5";
const char  XmpNameString[]    = "  Profile%d Name:";
const char  XpString[]         = "  XMP profile %u is %sabled and recommended channel config: %u DIMM per channel\n";
const char  ErrorString[]      = "ERROR: Unsupported ";
const char  SpdValString[]     = "SPD value: ";
const char  IsSupString[]      = " is supported";
const char  NotSupString[]     = " is not supported";
const char  TimeBaseString[]   = "Timebase (MTB/FTB)";
const char  tAAString[]        = "CAS Latency Time (tAAmin)";
const char  tCKString[]        = "SDRAM Cycle Time (tCKmin)";
const char  tWRString[]        = "Write recovery time (tWRmin)";
const char  tRCDString[]       = "RAS# to CAS# delay time (tRCDmin)";
const char  tRRDString[]       = "Row active to row active delay time (tRRDmin)";
const char  tRPString[]        = "Row precharge delay time (tRPmin)";
const char  Lpddr4String[]     = "LPDDR4";
const char  Lpddr4xString[]    = "LPDDR4X";
const char  Lpddr5String[]     = "LPDDR5";
const char  Lpddr5xString[]     = "LPDDR5x";
const char  LpDimmString[]     = "LP-DIMM";
const char  MemoryDownString[] = "Memory Down";
const char  DoubleSize1DpcString[] = "Double Size 1DPC Module";
const char  DoubleSize2DpcString[] = "Double Size 2DPC Module";
const char  tRPabString[]      = "Row precharge delay time for all banks (tRPab)";
const char  tRASString[]       = "Active to precharge delay time (tRASmin)";
const char  tRCString[]        = "Active to active/refresh delay time (tRCmin)";
const char  tRFCString[]       = "Refresh recovery delay time (tRFCmin)";
const char  tRFCpbString[]     = "Per Bank refresh recovery delay time (tRFCpb)";
const char  tWTRString[]       = "Internal write to read command delay time (tWTRmin)";
const char  tRTPString[]       = "Internal read to precharge delay time (tRTPmin)";
const char  tFAWString[]       = "Active to active/refresh delay time (tFAWmin)";
const char  tREFIString[]      = "Average Periodic Refresh Interval (tREFImin)";
const char  tCWLString[]       = "CAS Write Latency (tCWLmin)";
const char  NmodeString[]      = "Command rate mode (Nmode)";
const char  VoltageString[]    = "Module voltage (mVolts)";
const char  VddString[]        = "VDD";
const char  VddqString[]       = "VDDQ";
const char  VppString[]        = "VPP";
const char  BestCaseString[]   = "Best case value for profiles 0-";
const char  ProfileString[]    = "Profile";
const char  HeaderString[]     = "Profile Controller Channel Dimm Value";
const char  tRRDSString[]      = "Row active to row active delay time (tRRD_Smin)";
const char  tRRDLString[]      = "Row active to row active delay time (tRRD_Lmin)";
const char  tRFC2String[]      = "Refresh recovery delay time (tRFC2min)";
const char  tRFC4String[]      = "Refresh recovery delay time (tRFC4min)";
const char  tWTRLString[]      = "Internal write to read command delay time (tWTR_L)";
const char  tWTRSString[]      = "Internal write to read command delay time (tWTR_S)";
const char  tCCDLSString[]     = "CAS to CAS delay for same bank group (tCCD_L)";
const char  ChPerSdramPkgStr[] = "Channels per SDRAM Package";

const char  RrcString[][3]     = {
                                     " A", " B", " C", " D", " E", " F", " G", " H", " J", " K",
                                     " L", " M", " N", " P", " R", " T", " U", " V", " W", " Y",
                                     "AA", "AB", "AC", "AD", "AE", "AF", "AG", "AH", "AJ", "AK",
                                     "AL", "ZZ", "AM", "AN", "AP", "AR", "AT", "AU", "AV", "AW",
                                     "AY", "BA", "BB", "BC", "BD", "BE", "BF", "BG", "BH", "BJ",
                                     "BK", "BL", "BM", "BN", "BP", "BR", "BT", "BU", "BV", "BW",
                                     "BY", "CA", "CB", "ZZ"};
#endif // MRC_DEBUG_PRINT

typedef enum {
  MrcDensity256Mb = MRC_SPD_SDRAM_DENSITY_256Mb,
  MrcDensity512Mb = MRC_SPD_SDRAM_DENSITY_512Mb,
  MrcDensity1Gb   = MRC_SPD_SDRAM_DENSITY_1Gb,
  MrcDensity2Gb   = MRC_SPD_SDRAM_DENSITY_2Gb,
  MrcDensity4Gb   = MRC_SPD_SDRAM_DENSITY_4Gb,
  MrcDensity8Gb   = MRC_SPD_SDRAM_DENSITY_8Gb,
  MrcDensity16Gb  = MRC_SPD_SDRAM_DENSITY_16Gb,
  MrcDensity32Gb  = MRC_SPD_SDRAM_DENSITY_32Gb,
  MrcDensity12Gb  = MRC_SPD_LPDDR_SDRAM_DENSITY_12Gb,
  MrcDensity24Gb  = MRC_SPD_LPDDR_SDRAM_DENSITY_24Gb,
  MrcDensity3Gb   = MRC_SPD_LPDDR_SDRAM_DENSITY_3Gb,
  MrcDensity6Gb   = MRC_SPD_LPDDR_SDRAM_DENSITY_6Gb,
  MrcDensity18Gb  = MRC_SPD_LPDDR_SDRAM_DENSITY_18Gb,
  MrcDensity48Gb  = 13,
  MrcDensity64Gb  = 14,
  MrcDensityMax
} MRC_SDRAM_DENSITY;

GLOBAL_REMOVE_IF_UNREFERENCED const UINT32 SdramCapacityTable[MrcDensityMax] = {
  (256 / 8),   // 256Mb (32MB)
  (512 / 8),   // 512Mb (64MB)
  (1024 / 8),  // 1Gb   (128MB)
  (2048 / 8),  // 2Gb   (256MB)
  (4096 / 8),  // 4Gb   (512MB)
  (8192 / 8),  // 8Gb   (1024MB)
  (16384 / 8), // 16Gb  (2048MB)
  (32768 / 8), // 32Gb  (4096MB)
  (12288 / 8), // 12Gb  (1536MB)
  (24576 / 8), // 24Gb  (3072MB)
  (3072 / 8),  // 3Gb   (384MB)
  (6144 / 8),  // 6Gb   (768MB)
  (18432 / 8), // 18Gb  (2304MB)
  (49152 / 8), // 48Gb  (6144MB)
  (65536 / 8)  // 64Gb  (8192MB)
};
                                                                   // Ratio | Ratio
                                                                   // 133   | 100
GLOBAL_REMOVE_IF_UNREFERENCED const TRangeTable FreqTable[] = {
  { 0xFFFFFFFF,            fInvalid, MRC_FREQ_INVALID           }, //----------------
  { MRC_DDR_800_TCK_MIN,  f800,     MRC_FREQ_133 | MRC_FREQ_100 }, //   6   |   8
  { MRC_DDR_1000_TCK_MIN, f1000,                   MRC_FREQ_100 }, //       |  10
  { MRC_DDR_1067_TCK_MIN, f1067,    MRC_FREQ_133                }, //   8   |
  { MRC_DDR_1100_TCK_MIN, f1100,                   MRC_FREQ_100 }, //       |  11
  { MRC_DDR_1200_TCK_MIN, f1200,    MRC_FREQ_133 | MRC_FREQ_100 }, //   9   |  12
  { MRC_DDR_1300_TCK_MIN, f1300,                   MRC_FREQ_100 }, //       |  13
  { MRC_DDR_1333_TCK_MIN, f1333,    MRC_FREQ_133                }, //  10   |
  { MRC_DDR_1400_TCK_MIN, f1400,                   MRC_FREQ_100 }, //       |  14
  { MRC_DDR_1467_TCK_MIN, f1467,    MRC_FREQ_133                }, //  11   |
  { MRC_DDR_1500_TCK_MIN, f1500,                   MRC_FREQ_100 }, //       |  15
  { MRC_DDR_1600_TCK_MIN, f1600,    MRC_FREQ_133 | MRC_FREQ_100 }, //  12   |  16
  { MRC_DDR_1700_TCK_MIN, f1700,                   MRC_FREQ_100 }, //       |  17
  { MRC_DDR_1733_TCK_MIN, f1733,    MRC_FREQ_133                }, //  13   |
  { MRC_DDR_1800_TCK_MIN, f1800,                   MRC_FREQ_100 }, //       |  18
  { MRC_DDR_1867_TCK_MIN, f1867,    MRC_FREQ_133                }, //  14   |
  { MRC_DDR_1900_TCK_MIN, f1900,                   MRC_FREQ_100 }, //       |  19
  { MRC_DDR_2000_TCK_MIN, f2000,    MRC_FREQ_133 | MRC_FREQ_100 }, //  15   |  20
  { MRC_DDR_2100_TCK_MIN, f2100,                   MRC_FREQ_100 }, //       |  21
  { MRC_DDR_2133_TCK_MIN, f2133,    MRC_FREQ_133                }, //  16   |
  { MRC_DDR_2200_TCK_MIN, f2200,                   MRC_FREQ_100 }, //       |  22
  { MRC_DDR_2267_TCK_MIN, f2267,    MRC_FREQ_133                }, //  17   |
  { MRC_DDR_2300_TCK_MIN, f2300,                   MRC_FREQ_100 }, //       |  23
  { MRC_DDR_2400_TCK_MIN, f2400,    MRC_FREQ_133 | MRC_FREQ_100 }, //  18   |  24
  { MRC_DDR_2500_TCK_MIN, f2500,                   MRC_FREQ_100 }, //       |  25
  { MRC_DDR_2533_TCK_MIN, f2533,    MRC_FREQ_133                }, //  19   |
  { MRC_DDR_2600_TCK_MIN, f2600,                   MRC_FREQ_100 }, //       |  26
  { MRC_DDR_2667_TCK_MIN, f2667,    MRC_FREQ_133                }, //  20   |
  { MRC_DDR_2700_TCK_MIN, f2700,                   MRC_FREQ_100 }, //       |  27
  { MRC_DDR_2800_TCK_MIN, f2800,    MRC_FREQ_133 | MRC_FREQ_100 }, //  21   |  28
  { MRC_DDR_2900_TCK_MIN, f2900,                   MRC_FREQ_100 }, //       |  29
  { MRC_DDR_2933_TCK_MIN, f2933,    MRC_FREQ_133                }, //  22   |
  { MRC_DDR_3000_TCK_MIN, f3000,                   MRC_FREQ_100 }, //       |  30
  { MRC_DDR_3067_TCK_MIN, f3067,    MRC_FREQ_133                }, //  23   |
  { MRC_DDR_3100_TCK_MIN, f3100,                   MRC_FREQ_100 }, //       |  31
  { MRC_DDR_3200_TCK_MIN, f3200,    MRC_FREQ_133 | MRC_FREQ_100 }, //  24   |  32
  { MRC_DDR_3300_TCK_MIN, f3300,                   MRC_FREQ_100 }, //       |  33
  { MRC_DDR_3333_TCK_MIN, f3333,    MRC_FREQ_133                }, //  25   |
  { MRC_DDR_3400_TCK_MIN, f3400,                   MRC_FREQ_100 }, //       |  34
  { MRC_DDR_3467_TCK_MIN, f3467,    MRC_FREQ_133                }, //  26   |
  { MRC_DDR_3500_TCK_MIN, f3500,                   MRC_FREQ_100 }, //       |  35
  { MRC_DDR_3600_TCK_MIN, f3600,    MRC_FREQ_133 | MRC_FREQ_100 }, //  27   |  36
  { MRC_DDR_3700_TCK_MIN, f3700,                   MRC_FREQ_100 }, //       |  37
  { MRC_DDR_3733_TCK_MIN, f3733,    MRC_FREQ_133                }, //  28   |
  { MRC_DDR_3800_TCK_MIN, f3800,                   MRC_FREQ_100 }, //       |  38
  { MRC_DDR_3867_TCK_MIN, f3867,    MRC_FREQ_133                }, //  29   |
  { MRC_DDR_3900_TCK_MIN, f3900,                   MRC_FREQ_100 }, //       |  39
  { MRC_DDR_4000_TCK_MIN, f4000,    MRC_FREQ_133 | MRC_FREQ_100 }, //  30   |  40
  { MRC_DDR_4100_TCK_MIN, f4100,                   MRC_FREQ_100 }, //       |  41
  { MRC_DDR_4133_TCK_MIN, f4133,    MRC_FREQ_133                }, //  31   |
  { MRC_DDR_4200_TCK_MIN, f4200,                   MRC_FREQ_100 }, //       |  42
  { MRC_DDR_4267_TCK_MIN, f4267,    MRC_FREQ_133                }, //  32   |
  { MRC_DDR_4300_TCK_MIN, f4300,                   MRC_FREQ_100 }, //       |  43
  { MRC_DDR_4400_TCK_MIN, f4400,    MRC_FREQ_133 | MRC_FREQ_100 }, //  33   |  44
  { MRC_DDR_4500_TCK_MIN, f4500,                   MRC_FREQ_100 }, //       |  45
  { MRC_DDR_4533_TCK_MIN, f4533,    MRC_FREQ_133                }, //  34   |
  { MRC_DDR_4600_TCK_MIN, f4600,                   MRC_FREQ_100 }, //       |  46
  { MRC_DDR_4667_TCK_MIN, f4667,    MRC_FREQ_133                }, //  35   |
  { MRC_DDR_4700_TCK_MIN, f4700,                   MRC_FREQ_100 }, //       |  47
  { MRC_DDR_4800_TCK_MIN, f4800,    MRC_FREQ_133 | MRC_FREQ_100 }, //  36   |  48
  { MRC_DDR_4900_TCK_MIN, f4900,                   MRC_FREQ_100 }, //       |  49
  { MRC_DDR_4933_TCK_MIN, f4933,    MRC_FREQ_133                }, //  37   |
  { MRC_DDR_5000_TCK_MIN, f5000,                   MRC_FREQ_100 }, //       |  50
  { MRC_DDR_5067_TCK_MIN, f5067,    MRC_FREQ_133                }, //  38   |
  { MRC_DDR_5100_TCK_MIN, f5100,                   MRC_FREQ_100 }, //       |  51
  { MRC_DDR_5200_TCK_MIN, f5200,    MRC_FREQ_133 | MRC_FREQ_100 }, //  39   |  52
  { MRC_DDR_5300_TCK_MIN, f5300,                   MRC_FREQ_100 }, //       |  53
  { MRC_DDR_5333_TCK_MIN, f5333,    MRC_FREQ_133                }, //  40   |
  { MRC_DDR_5400_TCK_MIN, f5400,                   MRC_FREQ_100 }, //       |  54
  { MRC_DDR_5467_TCK_MIN, f5467,    MRC_FREQ_133                }, //  41   |
  { MRC_DDR_5500_TCK_MIN, f5500,                   MRC_FREQ_100 }, //       |  55
  { MRC_DDR_5600_TCK_MIN, f5600,    MRC_FREQ_133 | MRC_FREQ_100 }, //  42   |  56
  { MRC_DDR_5700_TCK_MIN, f5700,                   MRC_FREQ_100 }, //       |  57
  { MRC_DDR_5733_TCK_MIN, f5733,    MRC_FREQ_133                }, //  43   |
  { MRC_DDR_5800_TCK_MIN, f5800,                   MRC_FREQ_100 }, //       |  58
  { MRC_DDR_5867_TCK_MIN, f5867,    MRC_FREQ_133                }, //  44   |
  { MRC_DDR_5900_TCK_MIN, f5900,                   MRC_FREQ_100 }, //       |  59
  { MRC_DDR_6000_TCK_MIN, f6000,    MRC_FREQ_133 | MRC_FREQ_100 }, //  45   |  60
  { MRC_DDR_6100_TCK_MIN, f6100,                   MRC_FREQ_100 }, //       |  61
  { MRC_DDR_6133_TCK_MIN, f6133,    MRC_FREQ_133                }, //  46   |
  { MRC_DDR_6200_TCK_MIN, f6200,                   MRC_FREQ_100 }, //       |  62
  { MRC_DDR_6267_TCK_MIN, f6267,    MRC_FREQ_133                }, //  47   |
  { MRC_DDR_6300_TCK_MIN, f6300,                   MRC_FREQ_100 }, //       |  63
  { MRC_DDR_6400_TCK_MIN, f6400,    MRC_FREQ_133 | MRC_FREQ_100 }, //  48   |  64
  { MRC_DDR_6500_TCK_MIN, f6500,                   MRC_FREQ_100 }, //       |  65
  { MRC_DDR_6533_TCK_MIN, f6533,    MRC_FREQ_133                }, //  49   |
  { MRC_DDR_6600_TCK_MIN, f6600,                   MRC_FREQ_100 }, //       |  66
  { MRC_DDR_6667_TCK_MIN, f6667,    MRC_FREQ_133                }, //  50   |
  { MRC_DDR_6700_TCK_MIN, f6700,                   MRC_FREQ_100 }, //       |  67
  { MRC_DDR_6800_TCK_MIN, f6800,    MRC_FREQ_133 | MRC_FREQ_100 }, //  51   |  68
  { MRC_DDR_6900_TCK_MIN, f6900,                   MRC_FREQ_100 }, //       |  69
  { MRC_DDR_6933_TCK_MIN, f6933,    MRC_FREQ_133                }, //  52   |
  { MRC_DDR_7000_TCK_MIN, f7000,                   MRC_FREQ_100 }, //       |  70
  { MRC_DDR_7067_TCK_MIN, f7067,    MRC_FREQ_133                }, //  53   |
  { MRC_DDR_7100_TCK_MIN, f7100,                   MRC_FREQ_100 }, //       |  71
  { MRC_DDR_7200_TCK_MIN, f7200,    MRC_FREQ_133 | MRC_FREQ_100 }, //  54   |  72
  { MRC_DDR_7300_TCK_MIN, f7300,                   MRC_FREQ_100 }, //       |  73
  { MRC_DDR_7333_TCK_MIN, f7333,    MRC_FREQ_133                }, //  55   |
  { MRC_DDR_7400_TCK_MIN, f7400,                   MRC_FREQ_100 }, //       |  74
  { MRC_DDR_7467_TCK_MIN, f7467,    MRC_FREQ_133                }, //  56   |
  { MRC_DDR_7500_TCK_MIN, f7500,                   MRC_FREQ_100 }, //       |  75
  { MRC_DDR_7600_TCK_MIN, f7600,    MRC_FREQ_133 | MRC_FREQ_100 }, //  57   |  76
  { MRC_DDR_7700_TCK_MIN, f7700,                   MRC_FREQ_100 }, //       |  77
  { MRC_DDR_7733_TCK_MIN, f7733,    MRC_FREQ_133                }, //  58   |
  { MRC_DDR_7800_TCK_MIN, f7800,                   MRC_FREQ_100 }, //       |  78
  { MRC_DDR_7867_TCK_MIN, f7867,    MRC_FREQ_133                }, //  59   |
  { MRC_DDR_7900_TCK_MIN, f7900,                   MRC_FREQ_100 }, //       |  79
  { MRC_DDR_8000_TCK_MIN, f8000,    MRC_FREQ_133 | MRC_FREQ_100 }, //  60   |  80
  { MRC_DDR_8100_TCK_MIN, f8100,                   MRC_FREQ_100 }, //       |  81
  { MRC_DDR_8133_TCK_MIN, f8133,    MRC_FREQ_133                }, //  61   |
  { MRC_DDR_8200_TCK_MIN, f8200,                   MRC_FREQ_100 }, //       |  82
  { MRC_DDR_8267_TCK_MIN, f8267,    MRC_FREQ_133                }, //  62   |
  { MRC_DDR_8300_TCK_MIN, f8300,                   MRC_FREQ_100 }, //       |  83
  { MRC_DDR_8400_TCK_MIN, f8400,    MRC_FREQ_133 | MRC_FREQ_100 }, //  63   |  84
  { MRC_DDR_8500_TCK_MIN, f8500,                   MRC_FREQ_100 }, //       |  85
  { MRC_DDR_8533_TCK_MIN, f8533,    MRC_FREQ_133                }, //  64   |
  { MRC_DDR_8600_TCK_MIN, f8600,                   MRC_FREQ_100 }, //       |  86
  { MRC_DDR_8667_TCK_MIN, f8667,    MRC_FREQ_133                }, //  65   |
  { MRC_DDR_8700_TCK_MIN, f8700,                   MRC_FREQ_100 }, //       |  87
  { MRC_DDR_8800_TCK_MIN, f8800,    MRC_FREQ_133 | MRC_FREQ_100 }, //  67   |  88
  { MRC_DDR_8900_TCK_MIN, f8900,                   MRC_FREQ_100 }, //       |  89
  { MRC_DDR_8933_TCK_MIN, f8933,    MRC_FREQ_133                }, //  67   |
  { MRC_DDR_9000_TCK_MIN, f9000,                   MRC_FREQ_100 }, //       |  90
  { MRC_DDR_9067_TCK_MIN, f9067,    MRC_FREQ_133                }, //  68   |
  { MRC_DDR_9100_TCK_MIN, f9100,                   MRC_FREQ_100 }, //       |  91
  { MRC_DDR_9200_TCK_MIN, f9200,    MRC_FREQ_133 | MRC_FREQ_100 }, //  69   |  92
  { MRC_DDR_9300_TCK_MIN, f9300,                   MRC_FREQ_100 }, //       |  93
  { MRC_DDR_9333_TCK_MIN, f9333,    MRC_FREQ_133                }, //  70   |
  { MRC_DDR_9400_TCK_MIN, f9400,                   MRC_FREQ_100 }, //       |  94
  { MRC_DDR_9467_TCK_MIN, f9467,    MRC_FREQ_133                }, //  71   |
  { MRC_DDR_9500_TCK_MIN, f9500,                   MRC_FREQ_100 }, //       |  95
  { MRC_DDR_9600_TCK_MIN, f9600,    MRC_FREQ_133 | MRC_FREQ_100 }, //  72   |  96
  { MRC_DDR_9700_TCK_MIN, f9700,                   MRC_FREQ_100 }, //       |  97
  { MRC_DDR_9733_TCK_MIN, f9733,    MRC_FREQ_133                }, //  73   |
  { MRC_DDR_9800_TCK_MIN, f9800,                   MRC_FREQ_100 }, //       |  98
  { MRC_DDR_9867_TCK_MIN, f9867,    MRC_FREQ_133                }, //  74   |
  { MRC_DDR_9900_TCK_MIN, f9900,                   MRC_FREQ_100 }, //       |  99
  { MRC_DDR_10000_TCK_MIN, f10000,  MRC_FREQ_133 | MRC_FREQ_100 }, //  75   |  100
  { MRC_DDR_10100_TCK_MIN, f10100,                 MRC_FREQ_100 }, //       |  101
  { MRC_DDR_10133_TCK_MIN, f10133,  MRC_FREQ_133                }, //  76   |
  { MRC_DDR_10200_TCK_MIN, f10200,                 MRC_FREQ_100 }, //       |  102
  { MRC_DDR_10267_TCK_MIN, f10267,  MRC_FREQ_133                }, //  77   |
  { MRC_DDR_10300_TCK_MIN, f10300,                 MRC_FREQ_100 }, //       |  103
  { MRC_DDR_10400_TCK_MIN, f10400,  MRC_FREQ_133 | MRC_FREQ_100 }, //  78   |  104
  { MRC_DDR_10500_TCK_MIN, f10500,                 MRC_FREQ_100 }, //       |  105
  { MRC_DDR_10533_TCK_MIN, f10533,  MRC_FREQ_133                }, //  79   |
  { MRC_DDR_10600_TCK_MIN, f10600,                 MRC_FREQ_100 }, //       |  106
  { MRC_DDR_10667_TCK_MIN, f10667,  MRC_FREQ_133                }, //  80   |
  { MRC_DDR_10700_TCK_MIN, f10700,                 MRC_FREQ_100 }, //       |  107
  { MRC_DDR_10800_TCK_MIN, f10800,  MRC_FREQ_133 | MRC_FREQ_100 }, //  81   |  108
  { MRC_DDR_10900_TCK_MIN, f10900,                 MRC_FREQ_100 }, //       |  109
  { MRC_DDR_10933_TCK_MIN, f10933,  MRC_FREQ_133                }, //  82   |
  { MRC_DDR_11000_TCK_MIN, f11000,                 MRC_FREQ_100 }, //       |  110
  { MRC_DDR_11067_TCK_MIN, f11067,  MRC_FREQ_133                }, //  83   |
  { MRC_DDR_11100_TCK_MIN, f11100,                 MRC_FREQ_100 }, //       |  111
  { MRC_DDR_11200_TCK_MIN, f11200,  MRC_FREQ_133 | MRC_FREQ_100 }, //  84   |  112
  { MRC_DDR_11300_TCK_MIN, f11300,                 MRC_FREQ_100 }, //       |  113
  { MRC_DDR_11333_TCK_MIN, f11333,  MRC_FREQ_133                }, //  85   |
  { MRC_DDR_11400_TCK_MIN, f11400,                 MRC_FREQ_100 }, //       |  114
  { MRC_DDR_11467_TCK_MIN, f11467,  MRC_FREQ_133                }, //  86   |
  { MRC_DDR_11500_TCK_MIN, f11500,                 MRC_FREQ_100 }, //       |  115
  { MRC_DDR_11600_TCK_MIN, f11600,  MRC_FREQ_133 | MRC_FREQ_100 }, //  87   |  116
  { MRC_DDR_11700_TCK_MIN, f11700,                 MRC_FREQ_100 }, //       |  117
  { MRC_DDR_11733_TCK_MIN, f11733,  MRC_FREQ_133                }, //  88   |
  { MRC_DDR_11800_TCK_MIN, f11800,                 MRC_FREQ_100 }, //       |  118
  { MRC_DDR_11867_TCK_MIN, f11867,  MRC_FREQ_133                }, //  89   |
  { MRC_DDR_11900_TCK_MIN, f11900,                 MRC_FREQ_100 }, //       |  119
  { MRC_DDR_12000_TCK_MIN, f12000,  MRC_FREQ_133 | MRC_FREQ_100 }, //  90   |  120
  { MRC_DDR_12100_TCK_MIN, f12100,                 MRC_FREQ_100 }, //       |  121
  { MRC_DDR_12133_TCK_MIN, f12133,  MRC_FREQ_133                }, //  91   |
  { MRC_DDR_12200_TCK_MIN, f12200,                 MRC_FREQ_100 }, //       |  122
  { MRC_DDR_12267_TCK_MIN, f12267,  MRC_FREQ_133                }, //  92   |
  { MRC_DDR_12300_TCK_MIN, f12300,                 MRC_FREQ_100 }, //       |  123
  { MRC_DDR_12400_TCK_MIN, f12400,  MRC_FREQ_133 | MRC_FREQ_100 }, //  93   |  124
  { MRC_DDR_12500_TCK_MIN, f12500,                 MRC_FREQ_100 }, //       |  125
  { MRC_DDR_12533_TCK_MIN, f12533,  MRC_FREQ_133                }, //  94   |
  { MRC_DDR_12600_TCK_MIN, f12600,                 MRC_FREQ_100 }, //       |  126
  { MRC_DDR_12667_TCK_MIN, f12667,  MRC_FREQ_133                }, //  95   |
  { MRC_DDR_12700_TCK_MIN, f12700,                 MRC_FREQ_100 }, //       |  127
  { MRC_DDR_12800_TCK_MIN, f12800,  MRC_FREQ_133 | MRC_FREQ_100 }, //  96   |  128
  { 0,                     fNoInit, MRC_FREQ_INVALID           }
};
/**
    Calculate the memory clock value from the current memory frequency.

    @param[in, out] MrcData     - Pointer to MrcData data structure.
    @param[in]      Frequency   - Memory frequency to convert.

    @retval Returns the tCK value in [fs] for the given frequency.
**/
UINT32
ConvertFreq2Clock (
  IN OUT MrcParameters *const MrcData,
  IN     const MrcFrequency   Frequency
  )
{
  MrcOutput *Outputs;
  UINT32    tCKminActual;
  UINT32    Index;

  Outputs = &MrcData->Outputs;
  tCKminActual = MRC_DDR_533_TCK_MIN;

  for (Index = 0; Index < ARRAY_COUNT (FreqTable); Index++) {
    if (Frequency == FreqTable[Index].Frequency) {
      tCKminActual = FreqTable[Index].tCK;
      break;
    }
  }
#ifdef MRC_DEBUG_PRINT
  if (Index >= ARRAY_COUNT (FreqTable)) {
    MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_ERROR, "Could not find the matching frequency %u\n", Frequency);
  }
#endif

  if (Outputs->DdrType == MRC_DDR_TYPE_LPDDR5) {
    // tCK is 4:1 of the data rate.
    tCKminActual *= 4;
  }

  return tCKminActual;
}

/**
  Calculate the memory frequency from the memory clock value.

    @param[in, out] MrcData     - Pointer to MrcData data structure.
    @param[in]      RefClk      - The memory reference clock.
    @param[in]      tCKmin      - The tCKmin value in [fs] to convert.
    @param[out]     tCKminIndex - Pointer to the chosen table index.

    @retval Returns the frequency that matches the given tCK.
**/
static
UINT32
ConvertClock2Freq (
  IN OUT MrcParameters *const  MrcData,
  IN     const MrcRefClkSelect RefClk,
  IN     const UINT32          tCKmin,
  OUT    INT32         *const  tCKminIndex
  )
{
  MrcFrequency  Frequency;
  UINT32        Index;
  UINT32        tCKminPs;
  UINT32        FreqPs;
  UINT32        NextFreqPs;
  UINT8         FreqFlag;
  MrcOutput     *Outputs;

  Outputs = &MrcData->Outputs;

  //
  // Convert tCK value to the nearest frequency value.
  // Then find slowest valid frequency for the given reference clock.
  // Round to the [ps] resolution, because SPD FineTimeBase is 1ps.
  //
  Frequency = fNoInit;
  if (Outputs->DdrType == MRC_DDR_TYPE_LPDDR5) {
    tCKminPs = tCKmin / 4;
  } else {
    tCKminPs = tCKmin;
  }
  tCKminPs    = UDIVIDEROUND (tCKminPs, 1000);
  for (Index = 0; Index < ARRAY_COUNT (FreqTable) - 1; Index++) {
    FreqPs      = UDIVIDEROUND (FreqTable[Index].tCK, 1000);
    NextFreqPs  = UDIVIDEROUND (FreqTable[Index + 1].tCK, 1000);
    if ((tCKminPs <= FreqPs) && (tCKminPs > NextFreqPs)) {
      Frequency = FreqTable[Index].Frequency;
      break;
    }
  }

  while (Index) {
    FreqFlag = FreqTable[Index].FreqFlag;
    if ((FreqFlag & (1 << RefClk)) != 0) {
      if (Outputs->Gear2) {
        // In Gear2 make sure the frequency is a multiple of (2 * RefClk):
        //  133: multiples of 266, which has FreqTable index = 3, 7, 11, 15, 19 etc.
        //  100: multiples of 200, which has FreqTable index = 2, 5, 8, 11, 14, 17 etc.
        if ((RefClk == MRC_REF_CLOCK_133) && ((Index % 4) == 3)) {
          break;
        }
        if ((RefClk == MRC_REF_CLOCK_100) && ((Index % 3) == 2)) {
          break;
        }
      } else if (Outputs->Gear4) {
        // In Gear4 make sure the frequency is a multiple of (4 * RefClk):
        //  133: multiples of 533, which has FreqTable index = 3, 11, 19 etc.
        //  100: multiples of 400, which has FreqTable index = 5, 11, 17 etc.
        if ((RefClk == MRC_REF_CLOCK_133) && ((Index % 8) == 3)) {
          break;
        }
        if ((RefClk == MRC_REF_CLOCK_100) && ((Index % 6) == 5)) {
          break;
        }
      } else {
        break; // We can have this frequency at the given refclk
      }
    }

    Frequency = FreqTable[--Index].Frequency;
  }
  if (tCKminIndex != NULL) {
    *tCKminIndex = Index;
  }
  return Frequency;
}

/**
  Determine if the DIMM slot is filled.
  If a valid DRAM device type and valid module package are found then a DIMM is present.

    @param[in] MrcData - Pointer to MrcData data structure.
    @param[in] Spd     - Pointer to Spd data structure.

    @retval TRUE on valid value, otherwise FALSE and the value is set to zero.
**/
MrcDimmSts
DimmPresence (
  IN MrcDebug      *const Debug,
  IN const MrcSpd  *const Spd
  )
{
  UINT8         DramType;
  UINT8         ModuleType;

  //
  // The following code will more closely identify memory instead of just searching for non-zero data.
  //

  //
  // Check for valid DRAM device type and valid module package
  //
  DramType = Spd->Ddr4.Base.DramDeviceType.Bits.Type;
  ModuleType = Spd->Ddr4.Base.ModuleType.Bits.ModuleType;
  switch (DramType) {
    case MRC_SPD_DDR5_SDRAM_TYPE_NUMBER:
    case MRC_SPD_DDR4_SDRAM_TYPE_NUMBER:
      switch (ModuleType) {
        case UDimmMemoryPackage:
        case SoDimmMemoryPackage:
        case SoUDimmEccMemoryPackageDdr4:
        case DoubleSize1DpcMemoryPackage:
        case DoubleSize2DpcMemoryPackage:
          return DIMM_PRESENT;

        case RDimmMemoryPackage:
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "  ERROR: RDIMM is not supported !\n");

        default:
          break;
      }
      break;

    case MRC_SPD_LPDDR4_SDRAM_TYPE_NUMBER:
    case MRC_SPD_LPDDR4X_SDRAM_TYPE_NUMBER:
    case MRC_SPD_LPDDR5_SDRAM_TYPE_NUMBER:
    case MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER:
      switch (ModuleType) {
        case LpDimmMemoryPackage:
        case NonDimmMemoryPackage:
          return DIMM_PRESENT;

        default:
          break;
      }
      break;

    default:
      break;
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "  No DIMM detected in slot\n");
  return DIMM_NOT_PRESENT;
}

/**
   Determine if MemoryProfile is XMP

   @param[in] Profile

   @retval TRUE on XMP PROFILE 1-3 or USER_PROFILE4/5.
      Otherwise FALSE
**/
static
BOOLEAN
IsXmp (
  IN MrcProfile Profile
  )
{
  if ((Profile == XMP_PROFILE1) ||
      (Profile == XMP_PROFILE2) ||
      (Profile == XMP_PROFILE3)) {
    return TRUE;
  }
  /*
   * USER_PROFILE4/5 are similar to XMP_PROFILE
   */
  if ((Profile == USER_PROFILE4) ||
      (Profile == USER_PROFILE5)) {
    return TRUE;
  }

  return FALSE;
}

/**
   Determine if MemoryProfile is XMP and if DIMM doesn't support XMP

   @param[in] MrcData - Pointer to MrcData data structure.
   @param[in] Profile

   @retval TRUE on XMP PROFILE 1-3 and DIMM doesn't support XMP. Otherwise FALSE
**/
static
BOOLEAN
NeedIgnoreXmp (
  IN MrcParameters *const MrcData,
  IN MrcProfile           Profile
  )
{
  MrcOutput *Outputs;

  Outputs = &MrcData->Outputs;

  // The function works for both DDR5 and DDR4.
  // In DDR4, when Profile is XMP_PROFILE3, XmpProfileEnable is 0.
  if (((Profile == XMP_PROFILE1) && ((Outputs->XmpProfileEnable & XMP_PROFILE1_SUPPORT_MASK) == 0)) ||
      ((Profile == XMP_PROFILE2) && ((Outputs->XmpProfileEnable & XMP_PROFILE2_SUPPORT_MASK) == 0)) ||
      ((Profile == XMP_PROFILE3) && ((Outputs->XmpProfileEnable & XMP_PROFILE3_SUPPORT_MASK) == 0))) {
    return TRUE;
  }

  if (((Profile == USER_PROFILE4) && ((Outputs->XmpProfileEnable & XMP_USER_PROFILE4_SUPPORT_MASK) == 0)) ||
      ((Profile == USER_PROFILE5) && ((Outputs->XmpProfileEnable & XMP_USER_PROFILE5_SUPPORT_MASK) == 0))) {
    return TRUE;
  }

  return FALSE;
}

/**
   Determine if MemoryProfile is XMP and if DIMM supports the corresponding profile

   @param[in] DimmOut
   @param[in] Profile

   @retval TRUE on XMP PROFILE 1-3 and DIMM has the corresponding support. Otherwise FALSE
**/
static
BOOLEAN
XmpSupport (
  IN MrcDimmOut *const DimmOut,
  IN MrcProfile        Profile
  )
{
  UINT8 XmpSupport;

  XmpSupport = DimmOut->XmpSupport;
  if (((Profile == XMP_PROFILE1) && ((XmpSupport & XMP_PROFILE1_SUPPORT_MASK) != 0)) ||
      ((Profile == XMP_PROFILE2) && ((XmpSupport & XMP_PROFILE2_SUPPORT_MASK) != 0)) ||
      ((Profile == XMP_PROFILE3) && ((XmpSupport & XMP_PROFILE3_SUPPORT_MASK) != 0))) {
    return TRUE;
  }
  if (((Profile == USER_PROFILE4) && ((XmpSupport & XMP_USER_PROFILE4_SUPPORT_MASK) != 0)) ||
      ((Profile == USER_PROFILE5) && ((XmpSupport & XMP_USER_PROFILE5_SUPPORT_MASK) != 0))) {
    return TRUE;
  }

  return FALSE;
}

/**
   Determine if XMP Header and Profiles' CRC are correct

   @param[in, out] MrcData - Pointer to MrcData data structure.
   @param[in]      Spd
   @param[in]      DimmOut

   @retval TRUE if CRC is correct. Otherwise, FALSE
**/
static
BOOLEAN
Xmp3VerifyDimmCrc (
  IN OUT MrcParameters *const MrcData,
  IN     const MrcSpd  *const Spd,
  IN OUT MrcDimmOut    *const DimmOut
  )
{
  const SPD_EXTREME_MEMORY_PROFILE_HEADER_3_0  *Header5;
  MrcDebug            *Debug;
  UINT16              Crc;
  MrcProfile          Profile;
  UINT32              Index;

  Debug = &MrcData->Outputs.Debug;
  Header5 = &Spd->Ddr5.EndUser.Xmp.Header;

  GetDimmCrc ((UINT8 *) (&Header5->XmpId), SPD_XMP3_GLOBAL_SECTION_SIZE, &Crc);
  if (Crc != Header5->Crc) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "SPD XMP Header CRC error, Calculated CRC:0x%x, Header CRC:0x%x\n", Crc, Header5->Crc);
    // Don't return currently. Later on, we might change codes for strict checking
    // return FALSE;
  }

  // Verify XMP_PROFILE1/2/3
  for (Profile = 0; Profile < (XMP_PROFILE3 - XMP_PROFILE1 + 1); Profile++) {
    GetDimmCrc ((UINT8 *) (&Spd->Ddr5.EndUser.Xmp.Data[Profile]), SPD_XMP3_PROFILE_SIZE, &Crc);
    if (Crc != Spd->Ddr5.EndUser.Xmp.Data[Profile].Crc) {
      MRC_DEBUG_MSG (Debug,
          MSG_LEVEL_WARNING,
          "SPD XMP Profile%u CRC error, Calculated CRC:0x%x, Profile CRC:0x%x\n",
          Profile + 1,
          Crc,
          Spd->Ddr5.EndUser.Xmp.Data[Profile].Crc);
      // Don't return currently. Later on, we might change codes for strict checking
      // return FALSE;
    }
  }

  // Verify USER_PROFILE4/5
  for (Profile = USER_PROFILE4; Profile <= USER_PROFILE5; Profile++) {
    Index = Profile - XMP_PROFILE1;
    GetDimmCrc ((UINT8 *) (&Spd->Ddr5.EndUser.Xmp.Data[Index]), SPD_XMP3_PROFILE_SIZE, &Crc);
    if ((Crc != Spd->Ddr5.EndUser.Xmp.Data[Index].Crc) &&
        (Spd->Ddr5.EndUser.Xmp.Data[Index].tCKAVGmin.Data != 0))  {
      // When tCKAVGmin.Data != 0, assume this USER_PROFILE is enabled
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "SPD User Profile%d CRC error", Profile - XMP_PROFILE1 + 1);
    }
  }
  return TRUE;
}

/**
  Determine if the DIMM is valid and supported.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE on valid value, otherwise FALSE.
**/
static
BOOLEAN
ValidDimm (
  IN OUT MrcParameters *const MrcData,
  IN     const MrcSpd  *const Spd,
  IN OUT MrcDimmOut    *const DimmOut
  )
{
  MrcOutput   *Outputs;
  BOOLEAN     Ddr4;
  BOOLEAN     Ddr5;
  BOOLEAN     Lpddr;
  BOOLEAN     DimmValid;
  BOOLEAN     InvalidXmp;
  UINT8       DeviceType;
  MrcProfile  MemProfile;
  const SPD_EXTREME_MEMORY_PROFILE_HEADER_2_0  *Header4;
  const SPD_EXTREME_MEMORY_PROFILE_HEADER_3_0  *Header5;
  UINT16              XmpId;
  SPD_REVISION_STRUCT XmpRevision;
#ifdef MRC_DEBUG_PRINT
  static const UINT16 BytesUsedConst[] = {0, 128, 176, 256};
  MrcDebug            *Debug;
  const char          *DramTypeString;
  const char          *ModuleTypeString;
  const char          *ProfileStr;
  SPD_REVISION_STRUCT Revision;
  UINT16              BytesUsed;
  UINT16              BytesTotal;
  UINT16              CrcCoverage;
  UINT16              Byte;

  Debug = &MrcData->Outputs.Debug;
  ModuleTypeString = UnknownString;
#endif // MRC_DEBUG_PRINT

  Outputs    = &MrcData->Outputs;
  DimmValid  = TRUE;
  InvalidXmp = FALSE;
  Ddr4       = FALSE;
  Ddr5       = FALSE;
  Lpddr      = FALSE;
  MemProfile = MrcData->Inputs.MemoryProfile;
  DimmOut->XmpSupport = 0;

  //
  // The dram device type is at the same SPD offset for all types.
  //
  DeviceType = Spd->Ddr4.Base.DramDeviceType.Bits.Type;
  Header4 = NULL;
  Header5 = NULL;
  XmpId = 0;
  XmpRevision.Data = 0;
  switch (DeviceType) {
    case MRC_SPD_DDR5_SDRAM_TYPE_NUMBER:
      DimmOut->DdrType    = MRC_DDR_TYPE_DDR5;
      DimmOut->ModuleType = Spd->Ddr5.Base.ModuleType.Bits.ModuleType;
      Outputs->Vdd2Mv     = 1100;
      Ddr5                = TRUE;
      Header5    = &Spd->Ddr5.EndUser.Xmp.Header;
      XmpId = Header5->XmpId;
      if ((XMP_ID_STRING != Header5->XmpId) ||
          ((MemProfile == XMP_PROFILE1) && (Header5->XmpOrg.Bits.ProfileEnable1 == 0)) ||
          ((MemProfile == XMP_PROFILE2) && (Header5->XmpOrg.Bits.ProfileEnable2 == 0)) ||
          ((MemProfile == XMP_PROFILE3) && (Header5->XmpOrg.Bits.ProfileEnable3 == 0))) {
        if (IsXmp (MemProfile)) {
          DimmValid = FALSE;
          InvalidXmp = TRUE;
        }
      }

      if (XMP_ID_STRING == XmpId) {
        XmpRevision = Header5->XmpRevision;
        if (!Xmp3VerifyDimmCrc (MrcData, Spd, DimmOut)) {
          break;
        }
        if (0x30 == XmpRevision.Data) {
          DimmOut->XmpRevision = XmpRevision.Data;
        }

        DimmOut->IsPmicSupport10MVStep = (Header5->PmicCapabilities.Bits.OCCapable == 1);
        if (DimmOut->IsPmicSupport10MVStep && (Header5->PmicCapabilities.Bits.DefaultStepSize == PMIC_10MVSTEP)) {
          DimmOut->PmicDefaultStepSize = 10;
        } else{
          DimmOut->PmicDefaultStepSize = 5;
        }

        if (Header5->XmpOrg.Bits.ProfileEnable1 != 0) {
          DimmOut->XmpSupport |= XMP_PROFILE1_SUPPORT_MASK;
          Outputs->XmpProfileEnable |= XMP_PROFILE1_SUPPORT_MASK;
          DimmOut->XmpProfile1Config = Header5->XmpConf.Bits.ProfileConfig1;
          /*
           * Save AdvancedMemoryOCFeat for later detailed
           * checking when BIOS enables specific feature
           */
          DimmOut->XmpOverClockFeat[0] = Spd->Ddr5.EndUser.Xmp.Data[0].AdvancedMemoryOCFeat.Data;
        }
        if (Header5->XmpOrg.Bits.ProfileEnable2 != 0) {
          DimmOut->XmpSupport |= XMP_PROFILE2_SUPPORT_MASK;
          Outputs->XmpProfileEnable |= XMP_PROFILE2_SUPPORT_MASK;
          DimmOut->XmpProfile2Config = Header5->XmpConf.Bits.ProfileConfig2;
          DimmOut->XmpOverClockFeat[1] = Spd->Ddr5.EndUser.Xmp.Data[1].AdvancedMemoryOCFeat.Data;
        }
        if (Header5->XmpOrg.Bits.ProfileEnable3 != 0) {
          DimmOut->XmpSupport |= XMP_PROFILE3_SUPPORT_MASK;
          Outputs->XmpProfileEnable |= XMP_PROFILE3_SUPPORT_MASK;
          DimmOut->XmpProfile3Config = Header5->XmpConf.Bits.ProfileConfig3;
          DimmOut->XmpOverClockFeat[2] = Spd->Ddr5.EndUser.Xmp.Data[2].AdvancedMemoryOCFeat.Data;
        }

        if (DimmOut->XmpSupport & XMP_PROFILE1_SUPPORT_MASK) {
          /*
           * XMP3.0 has no Enable bit for USER_PROFILE4/5
           * We assume
           * 1) Only when XMP_PROFILE1 is enabled, USER_PROFILE4/5 can be enabled.
           * 2) Check (tCKAVGmin != 0) as a temporary solution
           */
          if (Spd->Ddr5.EndUser.Xmp.Data[USER_PROFILE4 - XMP_PROFILE1].tCKAVGmin.Data != 0) {
            DimmOut->XmpSupport |= XMP_USER_PROFILE4_SUPPORT_MASK;
            Outputs->XmpProfileEnable |= XMP_USER_PROFILE4_SUPPORT_MASK;
          }
          if (Spd->Ddr5.EndUser.Xmp.Data[USER_PROFILE5 - XMP_PROFILE1].tCKAVGmin.Data != 0) {
            DimmOut->XmpSupport |= XMP_USER_PROFILE5_SUPPORT_MASK;
            Outputs->XmpProfileEnable |= XMP_USER_PROFILE5_SUPPORT_MASK;
          }
        }
      }
      break;

    case MRC_SPD_DDR4_SDRAM_TYPE_NUMBER:
      DimmOut->DdrType    = MRC_DDR_TYPE_DDR4;
      DimmOut->ModuleType = Spd->Ddr4.Base.ModuleType.Bits.ModuleType;
      Ddr4                = TRUE;
      Outputs->Vdd2Mv     = 1200;
      Header4 = &Spd->Ddr4.EndUser.Xmp.Header;
      XmpId = Header4->XmpId;
      if ((XMP_ID_STRING != Header4->XmpId) ||
          ((MemProfile == XMP_PROFILE1) && (Header4->XmpOrgConf.Bits.ProfileEnable1 == 0)) ||
          ((MemProfile == XMP_PROFILE2) && (Header4->XmpOrgConf.Bits.ProfileEnable2 == 0))) {
        if (IsXmp (MemProfile)) {
          DimmValid = FALSE;
          InvalidXmp = TRUE;
        }
      }

      if (XMP_ID_STRING == XmpId) {
        XmpRevision = Header4->XmpRevision;
        if (0x20 == XmpRevision.Data) {
          DimmOut->XmpRevision = XmpRevision.Data;
        }
        if (Header4->XmpOrgConf.Bits.ProfileEnable1 != 0) {
          DimmOut->XmpSupport |= XMP_PROFILE1_SUPPORT_MASK;
          Outputs->XmpProfileEnable |= XMP_PROFILE1_SUPPORT_MASK;
          DimmOut->XmpProfile1Config = Header4->XmpOrgConf.Bits.ProfileConfig1;
        }
        if (Header4->XmpOrgConf.Bits.ProfileEnable2 != 0) {
          DimmOut->XmpSupport |= XMP_PROFILE2_SUPPORT_MASK;
          Outputs->XmpProfileEnable |= XMP_PROFILE2_SUPPORT_MASK;
          DimmOut->XmpProfile2Config = Header4->XmpOrgConf.Bits.ProfileConfig2;
        }
      }
      break;

    case MRC_SPD_LPDDR4_SDRAM_TYPE_NUMBER:
    case MRC_SPD_LPDDR4X_SDRAM_TYPE_NUMBER:
      DimmOut->DdrType    = MRC_DDR_TYPE_LPDDR4;
      DimmOut->ModuleType = Spd->Lpddr.Base.ModuleType.Bits.ModuleType;
      Lpddr               = TRUE;
      if (DeviceType == MRC_SPD_LPDDR4X_SDRAM_TYPE_NUMBER) {
        // Set Lp4x flag to handle small differences between LPDDR4 and LPDDR4X
        Outputs->Lp4x = TRUE;
      }
      Outputs->Vdd2Mv = 1100;
      break;

    case MRC_SPD_LPDDR5_SDRAM_TYPE_NUMBER:
    case MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER:
      DimmOut->DdrType    = MRC_DDR_TYPE_LPDDR5;
      DimmOut->ModuleType = Spd->Lpddr.Base.ModuleType.Bits.ModuleType;
      Lpddr               = TRUE;
      Outputs->Vdd2Mv     = 1050;
      if (DeviceType == MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER) {
        // Set LpX flag to handle small differences between LPDDR5 and LPDDR5X
        Outputs->LpX = TRUE;
      }
      break;

    default:
      DimmOut->DdrType    = MRC_DDR_TYPE_UNKNOWN;
      DimmOut->ModuleType = 0;
      DimmValid           = FALSE;
      break;
  }

  if (DimmValid) {
    switch (DimmOut->ModuleType) {
#if (SUPPORT_RDIMM == SUPPORT)
      case RDimmMemoryPackage:
#ifdef MRC_DEBUG_PRINT
        ModuleTypeString = RdimmString;
#endif // MRC_DEBUG_PRINT
        break;
#endif

#if (SUPPORT_UDIMM == SUPPORT)
      case UDimmMemoryPackage:
#ifdef MRC_DEBUG_PRINT
        ModuleTypeString = UdimmString;
#endif // MRC_DEBUG_PRINT
        break;
#endif

#if (SUPPORT_SODIMM == SUPPORT)
      case SoDimmMemoryPackage:
#ifdef MRC_DEBUG_PRINT
        ModuleTypeString = SodimmString;
#endif // MRC_DEBUG_PRINT
        Outputs->SodimmPresent = TRUE;
        break;

      case SoUDimmEccMemoryPackageDdr4:
        if (Ddr4) {
#ifdef MRC_DEBUG_PRINT
          ModuleTypeString = SodimmString;
#endif // MRC_DEBUG_PRINT
        } else {
          DimmValid = FALSE;
        }
        break;
#endif // SUPPORT_SODIMM

      case LpDimmMemoryPackage:
        if (Lpddr) {
#ifdef MRC_DEBUG_PRINT
          ModuleTypeString = LpDimmString;
#endif // MRC_DEBUG_PRINT
        } else {
          DimmValid = FALSE;
        }
        break;

      case NonDimmMemoryPackage:
      // case DoubleSize1DpcMemoryPackage:  // uses the same encoding
        if (Lpddr) {
#ifdef MRC_DEBUG_PRINT
          ModuleTypeString = MemoryDownString;
#endif // MRC_DEBUG_PRINT
        } else if (Ddr5) {
#ifdef MRC_DEBUG_PRINT
          ModuleTypeString = DoubleSize1DpcString;
#endif // MRC_DEBUG_PRINT
        } else {
          DimmValid = FALSE;
        }
        break;

      case DoubleSize2DpcMemoryPackage:
        if (Ddr5) {
#ifdef MRC_DEBUG_PRINT
          ModuleTypeString = DoubleSize2DpcString;
#endif // MRC_DEBUG_PRINT
        } else {
          DimmValid = FALSE;
        }
        break;

      default:
        DimmValid = FALSE;
        break;
    }
  }

#ifdef MRC_DEBUG_PRINT
  switch (MemProfile) {
      case STD_PROFILE:
      default:
        ProfileStr = StdString;
        break;
      case CUSTOM_PROFILE1:
        ProfileStr = CustomString;
        break;
      case USER_PROFILE4:
        ProfileStr = User4String;
        break;
      case USER_PROFILE5:
        ProfileStr = User5String;
        break;
      case XMP_PROFILE1:
        ProfileStr = Xmp1String;
        break;
      case XMP_PROFILE2:
        ProfileStr = Xmp2String;
        break;
      case XMP_PROFILE3:
        ProfileStr = Xmp3String;
        break;
  }

  switch (DeviceType) {
    case MRC_SPD_DDR5_SDRAM_TYPE_NUMBER:
      DramTypeString = Ddr5String;
      BytesTotal     = 256 * Spd->Ddr5.Base.Description.Bits.BytesTotal;
      BytesUsed      = 0; // DDR5 SPD Reserved Field
      CrcCoverage    = 125;
      Revision.Data  = Spd->Ddr5.Base.Revision.Data;
      break;

    case MRC_SPD_DDR4_SDRAM_TYPE_NUMBER:
      DramTypeString = Ddr4String;
      BytesTotal     = 256 * Spd->Ddr4.Base.Description.Bits.BytesTotal;
      BytesUsed      = BytesUsedConst[1] * Spd->Ddr4.Base.Description.Bits.BytesUsed;
      CrcCoverage    = 125;
      Revision.Data  = Spd->Ddr4.Base.Revision.Data;
      break;

    case MRC_SPD_LPDDR4_SDRAM_TYPE_NUMBER:
    case MRC_SPD_LPDDR4X_SDRAM_TYPE_NUMBER:
    case MRC_SPD_LPDDR5_SDRAM_TYPE_NUMBER:
    case MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER:
      DramTypeString = (DeviceType == MRC_SPD_LPDDR4_SDRAM_TYPE_NUMBER)  ? Lpddr4String :
                       ((DeviceType == MRC_SPD_LPDDR5_SDRAM_TYPE_NUMBER) ? Lpddr5String :
                        (DeviceType == MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER)? Lpddr5xString:
                                                                           Lpddr4xString
                       );
      BytesTotal     = 256 * Spd->Lpddr.Base.Description.Bits.BytesTotal;
      BytesUsed      = BytesUsedConst[1] * Spd->Lpddr.Base.Description.Bits.BytesUsed;
      CrcCoverage    = 125;
      Revision.Data  = Spd->Lpddr.Base.Revision.Data;
      break;

    default:
      DramTypeString = UnknownString;
      BytesTotal     = 0;
      BytesUsed      = 0;
      CrcCoverage    = 0;
      Revision.Data  = 0;
      break;
  }

  if (DimmValid) {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "  %s %s detected, Rev: %u.%u, Size: %u used/%u total, CRC coverage: 0 - %u\n",
      DramTypeString,
      ModuleTypeString,
      Revision.Bits.Major,
      Revision.Bits.Minor,
      BytesUsed,
      BytesTotal,
      CrcCoverage
      );
  } else {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_ERROR,
      "  %s %s detected, SPD Dram type %Xh, module type %Xh\n",
      DramTypeString,
      ModuleTypeString,
      DeviceType,
      DimmOut->ModuleType
      );
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  DIMM profile %s selected\n", ProfileStr);
  if (InvalidXmp) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "  %s, Memory Profile %d selected but DIMM does not support XMP!\n", ErrorString, MemProfile);
    return DimmValid;
  }
  MRC_DEBUG_MSG (
    Debug,
    MSG_LEVEL_NOTE,
    "  XMP String: %Xh\n",
    XmpId
  );
  if (DimmOut->XmpSupport) {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "  XMP Revision: %u.%u\n",
      XmpRevision.Bits.Major,
      XmpRevision.Bits.Minor
    );
    if (Ddr4) {
      MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_NOTE,
        XpString,
        1,
        Header4->XmpOrgConf.Bits.ProfileEnable1 ? "en" : "dis",
        Header4->XmpOrgConf.Bits.ProfileConfig1 + 1
      );
      MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_NOTE,
        XpString,
        2,
        Header4->XmpOrgConf.Bits.ProfileEnable2 ? "en" : "dis",
        Header4->XmpOrgConf.Bits.ProfileConfig2 + 1
      );
    } else if (Ddr5) {
      MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_NOTE,
        XpString,
        1,
        Header5->XmpOrg.Bits.ProfileEnable1 ? "en" : "dis",
        Header5->XmpConf.Bits.ProfileConfig1 + 1
      );
      MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_NOTE,
        XpString,
        2,
        Header5->XmpOrg.Bits.ProfileEnable2 ? "en" : "dis",
        Header5->XmpConf.Bits.ProfileConfig2 + 1
      );
      MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_NOTE,
        XpString,
        3,
        Header5->XmpOrg.Bits.ProfileEnable3 ? "en" : "dis",
        Header5->XmpConf.Bits.ProfileConfig3 + 1
      );

      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, XmpNameString, 1);
      for (Byte = 0; Byte < 16; Byte++) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %02X", Header5->Profile1Name[Byte]);
      }
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, XmpNameString, 2);
      for (Byte = 0; Byte < 16; Byte++) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %02X", Header5->Profile2Name[Byte]);
      }
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, XmpNameString, 3);
      for (Byte = 0; Byte < 16; Byte++) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %02X", Header5->Profile3Name[Byte]);
      }
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
    }
  }

#endif // MRC_DEBUG_PRINT

  return DimmValid;
}

/**
  Determine if the DIMM SDRAM device width is valid and return the value.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE on valid value, otherwise FALSE and the value is set to zero.
**/
static
BOOLEAN
ValidSdramDeviceWidth (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MrcDebug    *Debug;
  MrcOutput   *Outputs;
  MrcDdrType  DdrType;
  BOOLEAN     Lpddr;
  UINT8       ChPerSdramPkg;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  DdrType = DimmOut->DdrType;
  Lpddr   = Outputs->Lpddr;
  ChPerSdramPkg = MRC_SPD_CH_PER_SDRAM_PKG_RSVD;

  if (MRC_DDR_TYPE_DDR4 == DdrType) {
    DimmOut->SdramWidthIndex = Spd->Ddr4.Base.ModuleOrganization.Bits.SdramDeviceWidth;
  } else if (MRC_DDR_TYPE_DDR5 == DdrType) {
    DimmOut->SdramWidthIndex = Spd->Ddr5.Base.PrimarySdramIoWidth.Bits.SdramIoWidth;
  } else {
    DimmOut->SdramWidthIndex = Spd->Lpddr.Base.ModuleOrganization.Bits.SdramDeviceWidth;
    DimmOut->DiePerSdramPackage = Spd->Lpddr.Base.SdramPackageType.Bits.DiePerSdramPkg + 1;
    ChPerSdramPkg = Spd->Lpddr.Base.SdramPackageType.Bits.ChannelsPerPkg;
  }

  switch (DimmOut->SdramWidthIndex) {

#if (SUPPORT_DEVWIDTH_4 == SUPPORT)
    case MRC_SPD_SDRAM_DEVICE_WIDTH_4:
      DimmOut->SdramWidth = 4;
      break;
#endif
#if (SUPPORT_DEVWIDTH_8 == SUPPORT)
    case MRC_SPD_SDRAM_DEVICE_WIDTH_8:
      DimmOut->SdramWidth = 8;
      if (Lpddr) {
        Outputs->LpByteMode = TRUE;
      }
      break;
#endif
#if (SUPPORT_DEVWIDTH_16 == SUPPORT)
    case MRC_SPD_SDRAM_DEVICE_WIDTH_16:
      DimmOut->SdramWidth = 16;
      break;
#endif
#if (SUPPORT_DEVWIDTH_32 == SUPPORT)
    case MRC_SPD_SDRAM_DEVICE_WIDTH_32:
      DimmOut->SdramWidth = 32;
      break;
#endif

    default:
      DimmOut->SdramWidth = 0;
      MRC_DEBUG_MSG (Debug,
        MSG_LEVEL_ERROR,
        "%sSDRAM device width, %s%Xh\n",
        ErrorString,
        SpdValString,
        DimmOut->SdramWidthIndex
        );
      return FALSE;
  }

  if (Lpddr) {
    switch (ChPerSdramPkg) {
      case MRC_SPD_CH_PER_SDRAM_PKG_1:
        ChPerSdramPkg = 1;
        break;

      case MRC_SPD_CH_PER_SDRAM_PKG_2:
        ChPerSdramPkg = 2;
        break;

      case MRC_SPD_CH_PER_SDRAM_PKG_4:
        ChPerSdramPkg = 4;
        break;

      default:
      case MRC_SPD_CH_PER_SDRAM_PKG_RSVD:
        ChPerSdramPkg = 0;
        MRC_DEBUG_MSG (
          Debug,
          MSG_LEVEL_ERROR,
          "%s%s, %s%Xh\n",
          ErrorString,
          ChPerSdramPkgStr,
          SpdValString,
          ChPerSdramPkg
          );
        return FALSE;
    }
    DimmOut->ChannelsPerSdramPackage = ChPerSdramPkg;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Die per SDRAM Package: %u\n", DimmOut->DiePerSdramPackage);
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s: %u\n", ChPerSdramPkgStr, ChPerSdramPkg);
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  SDRAM device width: %u\n", DimmOut->SdramWidth);

  return TRUE;
}

/**
  Determine if the DIMM SDRAM row address size is valid and return the value.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE if the row address size is valid, otherwise FALSE and the value is set to zero.
**/
static
BOOLEAN
ValidRowSize (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  UINT8        RowBits;
  UINT8        RowAddress;
  MrcDebug        *Debug;

  Debug = &MrcData->Outputs.Debug;
  if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
    RowAddress = Spd->Ddr4.Base.SdramAddressing.Bits.RowAddress;
  } else if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    RowAddress = Spd->Ddr5.Base.PrimarySdramAddressing.Bits.RowAddress;
  } else {
    RowAddress = Spd->Lpddr.Base.SdramAddressing.Bits.RowAddress;
  }

  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    switch (RowAddress) {
#if (SUPPORT_ROW_16 == SUPPORT)
      case MRC_SPD5_SDRAM_ROW_16:
        DimmOut->RowSize = MRC_BIT16;
        RowBits          = 16;
        break;
#endif
#if (SUPPORT_ROW_17 == SUPPORT)
      case MRC_SPD5_SDRAM_ROW_17:
        DimmOut->RowSize = MRC_BIT17;
        RowBits          = 17;
        break;
#endif
#if (SUPPORT_ROW_18 == SUPPORT)
      case MRC_SPD5_SDRAM_ROW_18:
        DimmOut->RowSize = MRC_BIT18;
        RowBits          = 18;
        break;
#endif
      default:
        DimmOut->RowSize = 0;
        RowBits          = 0;
        break;
    }
  } else {
    switch (RowAddress) {
#if (SUPPORT_ROW_12 == SUPPORT)
      case MRC_SPD_SDRAM_ROW_12:
        DimmOut->RowSize = MRC_BIT12;
        RowBits          = 12;
        break;
#endif
#if (SUPPORT_ROW_13 == SUPPORT)
      case MRC_SPD_SDRAM_ROW_13:
        DimmOut->RowSize = MRC_BIT13;
        RowBits          = 13;
        break;
#endif
#if (SUPPORT_ROW_14 == SUPPORT)
      case MRC_SPD_SDRAM_ROW_14:
        DimmOut->RowSize = MRC_BIT14;
        RowBits          = 14;
        break;
#endif
#if (SUPPORT_ROW_15 == SUPPORT)
      case MRC_SPD_SDRAM_ROW_15:
        DimmOut->RowSize = MRC_BIT15;
        RowBits          = 15;
        break;
#endif
#if (SUPPORT_ROW_16 == SUPPORT)
      case MRC_SPD_SDRAM_ROW_16:
        DimmOut->RowSize = MRC_BIT16;
        RowBits          = 16;
        break;
#endif
#if (SUPPORT_ROW_17 == SUPPORT)
      case MRC_SPD_SDRAM_ROW_17:
        DimmOut->RowSize = MRC_BIT17;
        RowBits          = 17;
        break;
#endif
#if (SUPPORT_ROW_18 == SUPPORT)
      case MRC_SPD_SDRAM_ROW_18:
        DimmOut->RowSize = MRC_BIT18;
        RowBits          = 18;
        break;
#endif
      default:
        DimmOut->RowSize = 0;
        RowBits          = 0;
        break;
    }
  }

  if (DimmOut->RowSize == 0) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%sSDRAM row size, %s%Xh\n", ErrorString, SpdValString, RowAddress);
    return FALSE;
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Row bits: %u\n", RowBits);
  return TRUE;
}

/**
  Determine if the DIMM SDRAM column address size is valid and return the value.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE if the column address size is valid, otherwise FALSE and the value is set to zero.
**/
static
BOOLEAN
ValidColumnSize (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MrcDebug     *Debug;
  UINT8        ColumnBits;
  UINT8        ColumnAddress;

  Debug    = &MrcData->Outputs.Debug;
  if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
    ColumnAddress = Spd->Ddr4.Base.SdramAddressing.Bits.ColumnAddress;
  } else if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    ColumnAddress = Spd->Ddr5.Base.PrimarySdramAddressing.Bits.ColumnAddress;
  } else {
    ColumnAddress = Spd->Lpddr.Base.SdramAddressing.Bits.ColumnAddress;
  }

  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    switch (ColumnAddress) {
      case MRC_SPD5_SDRAM_COLUMN_10:
        ColumnBits = 10;
        break;
      case MRC_SPD5_SDRAM_COLUMN_11:
        ColumnBits = 11;
        break;
      default:
        ColumnBits = 0;
        break;
    }
  } else {
    switch (ColumnAddress) {
      case MRC_SPD_SDRAM_COLUMN_9:
        ColumnBits = 9;
        break;
      case MRC_SPD_SDRAM_COLUMN_10:
        ColumnBits = 10;
        break;
      case MRC_SPD_SDRAM_COLUMN_11:
        ColumnBits = 11;
        break;
      case MRC_SPD_SDRAM_COLUMN_12:
        ColumnBits = 12;
        break;
      default:
        ColumnBits = 0;
        break;
    }
  }

  if (ColumnBits == 0) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%sSDRAM column size, %s%Xh\n", ErrorString, SpdValString, ColumnAddress);
    DimmOut->ColumnSize = 0;
    return FALSE;
  }

  DimmOut->ColumnSize = 1 << ColumnBits;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Column bits: %u\n", ColumnBits);
  return TRUE;
}

/**
  Determine if the DIMM SDRAM primary bus width is valid and return the value.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE on valid value, otherwise FALSE and the value is set to zero.
**/
static
BOOLEAN
ValidPrimaryWidth (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MrcDebug  *Debug;
  MrcOutput *Outputs;
  UINT8     Width;
  UINT8     NumberOfChannels;
  BOOLEAN   Lpddr;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  Lpddr   = FALSE;
  NumberOfChannels = 0;

  if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
    Width = Spd->Ddr4.Base.ModuleMemoryBusWidth.Bits.PrimaryBusWidth;
  } else if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    Width = Spd->Ddr5.ModuleCommon.ModuleMemoryBusWidth.Bits.PrimaryBusWidth;
    NumberOfChannels = Spd->Ddr5.ModuleCommon.ModuleMemoryBusWidth.Bits.NumberOfChannels + 1;
  } else {  // LPDDR4 or LPDDR5
    Lpddr = TRUE;
    NumberOfChannels = Spd->Lpddr.Base.ModuleMemoryBusWidth.Bits.NumberOfChannels + 1;
    Width = Spd->Lpddr.Base.ModuleMemoryBusWidth.Bits.PrimaryBusWidth;
    NumberOfChannels = 1;//Hardcoded
    Width = MRC_SPD_PRIMARY_BUS_WIDTH_16;//Hardcoded
    // We will check for mixed width case later in MrcSpdProcessing()
    if (Width == MRC_SPD_PRIMARY_BUS_WIDTH_16) {
      Outputs->EnhancedChannelMode = TRUE;
    }
  }

  switch (Width) {
#if (SUPPORT_PRIWIDTH_8 == SUPPORT)
    case MRC_SPD_PRIMARY_BUS_WIDTH_8:
      DimmOut->PrimaryBusWidth = 8;
      break;
#endif
#if (SUPPORT_PRIWIDTH_16 == SUPPORT)
    case MRC_SPD_PRIMARY_BUS_WIDTH_16:
      DimmOut->PrimaryBusWidth = 16;
      break;
#endif
#if (SUPPORT_PRIWIDTH_32 == SUPPORT)
    case MRC_SPD_PRIMARY_BUS_WIDTH_32:
      DimmOut->PrimaryBusWidth = 32;
      break;
#endif
#if (SUPPORT_PRIWIDTH_64 == SUPPORT)
    case MRC_SPD_PRIMARY_BUS_WIDTH_64:
      DimmOut->PrimaryBusWidth = 64;
      break;
#endif
    default:
      DimmOut->PrimaryBusWidth = 0;
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%sSDRAM primary bus width, %s%Xh\n", ErrorString, SpdValString, Width);
      return FALSE;
      break;
  }
  if (Lpddr) {
    DimmOut->NumLpSysChannel = NumberOfChannels;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Number of channels: %u\n", NumberOfChannels);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Primary bus width: %u\n", DimmOut->PrimaryBusWidth);
  return TRUE;
}

/**
  Converts the input DDR5 SPD Byte 4 Bits 4:0 Density value to a DensityIndex
  value that can be used to index into the SdramCapacityTable array.

    @param[in] Dddr5Density - The DDR5 SDRAM Density Per Die per the DDR5 SPD spec

    @return The density as an index of SdramCapacityTable or 0xFF if the input is undefined.
**/
UINT8
Ddr5SpdToDensityIndex (
  IN UINT8 Ddr5Density
  )
{
  UINT8 DensityIndex;
  switch (Ddr5Density) {
    case MRC_SPD5_SDRAM_DENSITY_4Gb:
      DensityIndex = MrcDensity4Gb;
      break;
    case MRC_SPD5_SDRAM_DENSITY_8Gb:
      DensityIndex = MrcDensity8Gb;
      break;
    case MRC_SPD5_SDRAM_DENSITY_12Gb:
      DensityIndex = MrcDensity12Gb;
      break;
    case MRC_SPD5_SDRAM_DENSITY_16Gb:
      DensityIndex = MrcDensity16Gb;
      break;
    case MRC_SPD5_SDRAM_DENSITY_24Gb:
      DensityIndex = MrcDensity24Gb;
      break;
    case MRC_SPD5_SDRAM_DENSITY_32Gb:
      DensityIndex = MrcDensity32Gb;
      break;
    case MRC_SPD5_SDRAM_DENSITY_48Gb:
      DensityIndex = MrcDensity48Gb;
      break;
    case MRC_SPD5_SDRAM_DENSITY_64Gb:
      DensityIndex = MrcDensity64Gb;
      break;
    case MRC_SPD5_SDRAM_DENSITY_NONE:
    default:
      DensityIndex = 0xFF;
      break;
  }
  return DensityIndex;
}

/**
  Determines if the number of Bank are valid.
  Determines if the number of Bank Groups are valid.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE on valid value, otherwise FALSE.
**/
static
BOOLEAN
ValidBank (
  IN OUT MrcParameters  *const MrcData,
  IN const MrcSpd       *const Spd,
  IN OUT MrcDimmOut     *const DimmOut
  )
{
  MrcDebug     *Debug;
  UINT8        BankAddress;
  UINT8        BankGroup;
  UINT8        ValidCheck;
  UINT8        ValidCheckStep;

  Debug       = &MrcData->Outputs.Debug;
  ValidCheck  = TRUE;

  if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
    DimmOut->DensityIndex = Spd->Ddr4.Base.SdramDensityAndBanks.Bits.Density;
    BankAddress           = Spd->Ddr4.Base.SdramDensityAndBanks.Bits.BankAddress;
    BankGroup             = Spd->Ddr4.Base.SdramDensityAndBanks.Bits.BankGroup;
  } else if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    DimmOut->DensityIndex = Ddr5SpdToDensityIndex (Spd->Ddr5.Base.PrimarySdramDensityAndPackage.Bits.Density);
    BankAddress           = Spd->Ddr5.Base.PrimarySdramBankGroups.Bits.BanksPerBankGroup;
    BankGroup             = Spd->Ddr5.Base.PrimarySdramBankGroups.Bits.BankGroups;
  } else {
    DimmOut->DensityIndex = Spd->Lpddr.Base.SdramDensityAndBanks.Bits.Density;
    BankAddress           = Spd->Lpddr.Base.SdramDensityAndBanks.Bits.BankAddress;
    BankGroup             = Spd->Lpddr.Base.SdramDensityAndBanks.Bits.BankGroup;
  }

  ValidCheckStep = TRUE;
  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    switch (BankAddress) {
      case MRC_SPD_DDR5_SDRAM_BANK_1:
      case MRC_SPD_DDR5_SDRAM_BANK_2:
      case MRC_SPD_DDR5_SDRAM_BANK_4:
        DimmOut->Banks = MRC_BIT0 << BankAddress;
        break;
      default:
        ValidCheckStep = FALSE;
    }
  } else {
    switch (BankAddress) {
#if (SUPPORT_BANK_4 == SUPPORT)
      case MRC_SPD_DDR4_SDRAM_BANK_4:
#endif
#if (SUPPORT_BANK_8 == SUPPORT)
      case MRC_SPD_DDR4_SDRAM_BANK_8:
#endif
#if ((SUPPORT_BANK_4 == SUPPORT) || (SUPPORT_BANK_8 == SUPPORT))
        DimmOut->Banks = MRC_BIT2 << BankAddress;
        break;
#endif

      default:
        ValidCheckStep = FALSE;
    }
  }

  if (ValidCheckStep == FALSE) {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_ERROR,
      "%sSDRAM number of banks, %s%Xh\n",
      ErrorString,
      SpdValString,
      BankAddress
      );
      ValidCheck = FALSE;
  }

  ValidCheckStep = TRUE;
  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    switch (BankGroup) {
      case MRC_SPD_DDR5_SDRAM_BANK_GROUPS_1:
      case MRC_SPD_DDR5_SDRAM_BANK_GROUPS_2:
      case MRC_SPD_DDR5_SDRAM_BANK_GROUPS_4:
      case MRC_SPD_DDR5_SDRAM_BANK_GROUPS_8:
        DimmOut->BankGroups = MRC_BIT0 << BankGroup;
        break;
      default:
        ValidCheckStep = FALSE;
    }
  } else {
    if ((MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) && (BankGroup == MRC_SPD_DDR4_SDRAM_BANK_GROUPS_0)) {
      ValidCheckStep = FALSE;
    } else {
      DimmOut->BankGroups = MRC_BIT0 << BankGroup;
    }
  }

  if (ValidCheckStep == FALSE) {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_ERROR,
      "%sSDRAM number of bank groups, %s%Xh\n",
      ErrorString,
      SpdValString,
      BankGroup
      );
    ValidCheck = FALSE;
  }

  MRC_DEBUG_MSG (
    Debug,
    MSG_LEVEL_NOTE,
    (TRUE == ValidCheck) ? "  %u Banks in %u groups\n" : "",
    DimmOut->Banks,
    DimmOut->BankGroups
    );

  if (DimmOut->DensityIndex < ARRAY_COUNT (SdramCapacityTable)) {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "  SDRAM Capacity: %u Mb\n",
      SdramCapacityTable[DimmOut->DensityIndex] * 8
      );
  } else {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "  ERROR: SDRAM Capacity is reserved value: 0x%02X\n",
      DimmOut->DensityIndex
      );
  }

  return ValidCheck;
}

/**
  Determine if the number of ranks in the DIMM is valid and return the value.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE on valid value, otherwise FALSE and the value is set to zero.
**/
static
BOOLEAN
GetRankCount (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MrcDebug     *Debug;
  UINT8        RankCount;

  Debug = &MrcData->Outputs.Debug;
  if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
    RankCount = Spd->Ddr4.Base.ModuleOrganization.Bits.RankCount;
  } else if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    RankCount = Spd->Ddr5.ModuleCommon.ModuleOrganization.Bits.RankCount;
  } else {
    RankCount = Spd->Lpddr.Base.ModuleOrganization.Bits.RankCount;
  }

  DimmOut->RankInDimm = RankCount + 1;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Ranks: %u\n", DimmOut->RankInDimm);

  if (MrcData->Inputs.ForceSingleRank) {
    DimmOut->RankInDimm = 1;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Forcing to Single Rank because ForceSingleRank is 1\n" );
  }

  if (DimmOut->RankInDimm > MAX_RANK_IN_DIMM) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%snumber of ranks, %s%Xh\n", ErrorString, SpdValString, RankCount);
    DimmOut->RankInDimm = 0;
    return FALSE;
  }
  if (DimmOut->RankInDimm > 1) {
    MrcData->Outputs.Any2Ranks = TRUE;
  }
  return TRUE;
}

/**
  Calculate the size of the DIMM, in MBytes.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE on valid value, otherwise FALSE and the value is set to zero.
**/
static
BOOLEAN
GetDimmSize (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MrcDebug  *Debug;
  UINT32    DimmSize;
  UINT8     DensityIndex;
  BOOLEAN   Lpddr;

  Debug = &MrcData->Outputs.Debug;
  DensityIndex = DimmOut->DensityIndex;
  Lpddr = MrcData->Outputs.Lpddr;

  if ((DimmOut->SdramWidth > 0) && (DensityIndex < ARRAY_COUNT (SdramCapacityTable))) {
    if (Lpddr) {
      DimmSize = (SdramCapacityTable[DensityIndex] * DimmOut->DiePerSdramPackage * DimmOut->PrimaryBusWidth * DimmOut->NumLpSysChannel) / (DimmOut->SdramWidth * DimmOut->ChannelsPerSdramPackage);
    } else {
      DimmSize = (((SdramCapacityTable[DensityIndex] * DimmOut->PrimaryBusWidth) / DimmOut->SdramWidth) * DimmOut->RankInDimm);
    }
    if ((DimmSize >= DIMMSIZEMIN) && (DimmSize <= DIMMSIZEMAX)) {
      if (DimmSize != 0) {
        DimmOut->DimmCapacity = DimmSize;
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  DIMM size: %u MByte\n", DimmSize);
        return TRUE;
      }
    }
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_ERROR,
      "%sDIMM size: %u MB, valid range %u - %u MB ",
      ErrorString,
      DimmSize,
      DIMMSIZEMIN,
      DIMMSIZEMAX
      );
  }

  MRC_DEBUG_MSG (
    Debug,
    MSG_LEVEL_ERROR,
    "SDRAM capacity %s%Xh\n",
    SpdValString,
    DensityIndex
    );
  DimmOut->DimmCapacity = 0;
  return FALSE;
}

/**
  Obtain ECC support Status for this DIMM.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval Returns TRUE.
**/
static
BOOLEAN
ValidEccSupport (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
#if (SUPPORT_ECC == SUPPORT)
  UINT8        BusWidthExtension;
#endif // SUPPORT_ECC
  MrcDebug        *Debug;

  Debug = &MrcData->Outputs.Debug;

#if (SUPPORT_ECC == SUPPORT)
  if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
    BusWidthExtension = Spd->Ddr4.Base.ModuleMemoryBusWidth.Bits.BusWidthExtension;
  } else if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    BusWidthExtension = Spd->Ddr5.ModuleCommon.ModuleMemoryBusWidth.Bits.BusWidthExtension;
  } else {
    BusWidthExtension = Spd->Lpddr.Base.ModuleMemoryBusWidth.Bits.BusWidthExtension;
  }
  if ((MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) && (MRC_SPD5_BUS_WIDTH_EXTENSION_8 == BusWidthExtension)) {
    DimmOut->EccSupport = TRUE;
  } else if (MRC_SPD_BUS_WIDTH_EXTENSION_8 == BusWidthExtension) {
    DimmOut->EccSupport = TRUE;
  } else
#endif // SUPPORT_ECC
  {
    DimmOut->EccSupport = FALSE;
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  ECC is %ssupported\n", (DimmOut->EccSupport == FALSE) ? "not " : "");
  return TRUE;
}

/**
  Obtain address mirroring Status for this DIMM.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval Returns TRUE.
**/
static
BOOLEAN
GetAddressMirror (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MrcDebug  *Debug;
  UINT8  MappingRank1;

  Debug = &MrcData->Outputs.Debug;
  if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
    MappingRank1 = Spd->Ddr4.Module.Unbuffered.AddressMappingEdgeConn.Bits.MappingRank1;
  } else {
    MappingRank1 = 0;
  }

  DimmOut->AddressMirrored = (MappingRank1 != 0) ? TRUE : FALSE;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  DIMM has %saddress mirroring\n", (DimmOut->AddressMirrored == FALSE) ? "no " : "");
  return TRUE;
}

/**
  Obtain thermal and refresh support for this DIMM.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval Returns TRUE.
**/
static
BOOLEAN
GetThermalRefreshSupport (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MrcDebug        *Debug;
  const MrcInput  *Inputs;

  Inputs = &MrcData->Inputs;
  Debug  = &MrcData->Outputs.Debug;

  DimmOut->PartialSelfRefresh    = 0;
  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    DimmOut->OnDieThermalSensor = Spd->Ddr5.ModuleCommon.DeviceInfoThermalSensor.DeviceType.Bits.DevicesInstalledTS0 |
                                  Spd->Ddr5.ModuleCommon.DeviceInfoThermalSensor.DeviceType.Bits.DevicesInstalledTS1;
  } else {
    DimmOut->OnDieThermalSensor = (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) ? 0 : Spd->Lpddr.Base.ModuleThermalSensor.Bits.ThermalSensorPresence;
  }
  DimmOut->AutoSelfRefresh       = 0;
  DimmOut->ExtendedTemperRefresh = 0;
  DimmOut->ExtendedTemperRange   = 0;

  DimmOut->SelfRefreshTemp = ((!DimmOut->AutoSelfRefresh) && (DimmOut->ExtendedTemperRange) && (Inputs->ExtTemperatureSupport)) ? TRUE : FALSE;

  MRC_DEBUG_MSG (Debug,
    MSG_LEVEL_NOTE,
    "  Partial Array Self Refresh%s\n",
    DimmOut->PartialSelfRefresh ? IsSupString : NotSupString);
  MRC_DEBUG_MSG (Debug,
    MSG_LEVEL_NOTE,
    "  On-Die Thermal Sensor Readout%s\n",
    DimmOut->OnDieThermalSensor ? IsSupString : NotSupString);
  MRC_DEBUG_MSG (Debug,
    MSG_LEVEL_NOTE,
    "  Auto Self Refresh%s\n",
    DimmOut->AutoSelfRefresh ? IsSupString : NotSupString);
  MRC_DEBUG_MSG (Debug,
    MSG_LEVEL_NOTE,
    "  Extended Temperature Refresh Rate%s\n",
    DimmOut->ExtendedTemperRefresh ? IsSupString : NotSupString);
  MRC_DEBUG_MSG (Debug,
    MSG_LEVEL_NOTE,
    "  Extended Temperature Range%s\n",
    DimmOut->ExtendedTemperRange ? IsSupString : NotSupString);
  return TRUE;
}

/**
  Obtain Pseudo TRR support for this DIMM.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval Returns TRUE.
**/
static
BOOLEAN
GetpTRRsupport (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MrcDebug        *Debug;
  MrcOutput       *Outputs;
  UINT32          tMAC;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
    tMAC = Spd->Ddr4.Base.pTRRsupport.Bits.tMACencoding;
  } else if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    tMAC = MRC_TMAC_UNTESTED;
  } else {
    tMAC = Spd->Lpddr.Base.pTRRsupport.Bits.tMACencoding;
  }

  switch (tMAC) {
    case MRC_TMAC_UNTESTED:
      DimmOut->tMAC = MRC_TMAC_UNTESTED;
      break;
    case MRC_TMAC_200K:
      DimmOut->tMAC = 2;
      break;
    case MRC_TMAC_300K:
      DimmOut->tMAC = 3;
      break;
    case MRC_TMAC_400K:
      DimmOut->tMAC = 4;
      break;
    case MRC_TMAC_500K:
      DimmOut->tMAC = 5;
      break;
    case MRC_TMAC_600K:
      DimmOut->tMAC = 6;
      break;
    case MRC_TMAC_700K:
      DimmOut->tMAC = 7;
      break;
    case MRC_TMAC_UNLIMITED:
      DimmOut->tMAC = MRC_TMAC_UNLIMITED;
      break;
    default:
      DimmOut->tMAC = 0;
      MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_ERROR,
        "%stMAC value, %s%Xh\n",
        ErrorString,
        SpdValString,
        tMAC
        );
      return FALSE;
  }

  if (DimmOut->tMAC == MRC_TMAC_UNTESTED) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  tMAC is untested\n");
  } else if (DimmOut->tMAC == MRC_TMAC_UNLIMITED) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  tMAC is unlimited\n");
  } else {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  tMAC: %d00K\n", DimmOut->tMAC);
  }

  return TRUE;
}

/**
  Obtain which JEDEC reference design raw card was used as the basis for the DIMM assembly.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval Returns TRUE.
**/
static
BOOLEAN
GetReferenceRawCardSupport (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MrcDebug  *Debug;

  Debug = &MrcData->Outputs.Debug;
  if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
    DimmOut->ReferenceRawCard = (Spd->Ddr4.Module.Unbuffered.ReferenceRawCardUsed.Bits.Extension << MRC_SPD_REF_RAW_CARD_SIZE) |
    Spd->Ddr4.Module.Unbuffered.ReferenceRawCardUsed.Bits.Card;
  } else if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    DimmOut->ReferenceRawCard = Spd->Ddr5.ModuleCommon.ReferenceRawCardUsed.Bits.Card;
  } else {
    DimmOut->ReferenceRawCard = (Spd->Lpddr.Module.LpDimm.ReferenceRawCardUsed.Bits.Extension << MRC_SPD_REF_RAW_CARD_SIZE) |
    Spd->Lpddr.Module.LpDimm.ReferenceRawCardUsed.Bits.Card;
  }

  MRC_DEBUG_MSG (
    Debug,
    MSG_LEVEL_NOTE,
    "  Reference raw card: %u '%s'\n",
    DimmOut->ReferenceRawCard,
    (DimmOut->ReferenceRawCard < (sizeof (RrcString) / sizeof (RrcString[0]))) ?
    RrcString[DimmOut->ReferenceRawCard] : UnknownString
  );
  return TRUE;
}

/**
  Calculate the CRC16 of the provided SPD data. CRC16 formula is the same
    one that is used for calculating the CRC16 stored at SPD bytes 126-127.
    This can be used to detect DIMM change.

    @param[in]  Buffer - Pointer to the start of the data.
    @param[in]  Size   - Amount of data in the buffer, in bytes.
    @param[out] Crc    - Pointer to location to write the calculated CRC16 value.

    @retval Returns TRUE.
**/
BOOLEAN
GetDimmCrc (
  IN  const UINT8 *const Buffer,
  IN  const UINT32       Size,
  OUT UINT16 *const      Crc
  )
{
  const UINT8 *Data;
  UINT32      Value;
  UINT32      Byte;
  UINT8       Bit;

  Data  = Buffer;
  Value = CRC_SEED;
  for (Byte = 0; Byte < Size; Byte++) {
    Value ^= (UINT32) *Data++ << 8;
    for (Bit = 0; Bit < 8; Bit++) {
      Value = (Value & MRC_BIT15) ? (Value << 1) ^ CRC_XOR_MASK : Value << 1;
    }
  }

  *Crc = (UINT16) Value;
  return TRUE;
}

/**
  Calculate the medium and fine timebases, using integer math.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if medium timebase is valid, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmTimeBase (
  IN OUT MrcParameters  *const MrcData
  )
{
  const MrcInput            *Inputs;
  const MrcControllerIn     *ControllerIn;
  const MrcChannelIn        *ChannelIn;
  const MrcDimmIn           *DimmIn;
  const MrcSpd              *Spd;
  MrcDebug                  *Debug;
  MrcOutput                 *Outputs;
  MrcControllerOut          *ControllerOut;
  MrcChannelOut             *ChannelOut;
  MrcDimmOut                *DimmOut;
  MrcTimeBase               *TimeBase;
  MrcProfile                Profile;
  UINT8                     Controller;
  UINT8                     Channel;
  UINT8                     Dimm;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", TimeBaseString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }

          Spd      = &DimmIn->Spd.Data;
          TimeBase = &ChannelOut->TimeBase[Dimm][Profile];
          // DDR5 does not use timebase
          if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
            TimeBase->Mtb = 0;
            TimeBase->Ftb = 0;
          } else {
            switch (Profile) {
              case XMP_PROFILE1:
              case XMP_PROFILE2:
                if (XmpSupport (DimmOut, Profile)) {
                  switch (Spd->Ddr4.Base.Timebase.Bits.Medium) {
                    case 0:
                      TimeBase->Mtb = 125000;
                      break;
                    default:
                      TimeBase->Mtb = 0;
                      break;
                  }
                  switch (Spd->Ddr4.Base.Timebase.Bits.Fine) {
                    case 0:
                      TimeBase->Ftb = 1000;
                      break;
                    default:
                      TimeBase->Ftb = 0;
                      break;
                  }
                } else {
                  TimeBase->Ftb = 0;
                  TimeBase->Mtb = 0;
                }
                break;
              case CUSTOM_PROFILE1:
              case STD_PROFILE:
              default:
                if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
                  switch (Spd->Ddr4.Base.Timebase.Bits.Medium) {
                    case 0:
                      TimeBase->Mtb = 125000;
                      break;
                    default:
                      TimeBase->Mtb = 0;
                      break;
                  }
                  switch (Spd->Ddr4.Base.Timebase.Bits.Fine) {
                    case 0:
                      TimeBase->Ftb = 1000;
                      break;
                    default:
                      TimeBase->Ftb = 0;
                      break;
                  }
                } else {
                  switch (Spd->Lpddr.Base.Timebase.Bits.Medium) {
                    case 0:
                      TimeBase->Mtb = 125000;
                      break;
                    default:
                      TimeBase->Mtb = 0;
                      break;
                  }
                  switch (Spd->Lpddr.Base.Timebase.Bits.Fine) {
                    case 0:
                      TimeBase->Ftb = 1000;
                      break;
                    default:
                      TimeBase->Ftb = 0;
                      break;
                  }
                }
                break;
            } //switch
          }

          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u % 6u %u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            TimeBase->Mtb,
            TimeBase->Ftb
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  return TRUE;
}

/**
  Calculate the SDRAM minimum cycle time (tCKmin) that this DIMM supports.
    Then use the lookup table to obtain the frequency closest to the clock multiple.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if the DIMM frequency is supported, otherwise FALSE and the frequency is set to fInvalid.
**/
static
BOOLEAN
GetChannelDimmtCK (
  IN OUT MrcParameters  *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  INT32                 MediumTimebase;
  INT32                 FineTimebase;
  INT32                 tCKminMtb;
  INT32                 tCKminFine;
  INT32                 tCKminIndex;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                DimmCalculated;
  UINT32                MemoryClockMax;
  MrcIntOutput          *IntOutputs;
  INT32                 TemptCKminFine=0xFFFFFF00;               
  const SPD_EXTREME_MEMORY_PROFILE_DATA_2_0  *Data;
  const SPD_EXTREME_MEMORY_PROFILE_DATA_3_0  *Data3;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s (fs)\n", tCKString, HeaderString);
  IntOutputs = ((MrcIntOutput *) (MrcData->IntOutputs.Internal));
  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = fNoInit;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd            = &DimmIn->Spd.Data;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          FineTimebase   = TimeBase->Ftb;
          Calculated     = 0;
          DimmCalculated = 0;
          if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
            tCKminMtb  = 0;
            tCKminFine = 0;
          } else if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
            tCKminMtb   = Spd->Ddr4.Base.tCKmin.Bits.tCKmin;
            tCKminFine  = Spd->Ddr4.Base.tCKminFine.Bits.tCKminFine;
          } else {
            tCKminMtb   = Spd->Lpddr.Base.tCKmin.Bits.tCKmin;
            tCKminFine  = Spd->Lpddr.Base.tCKminFine.Bits.tCKminFine;
          }
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (XmpSupport (DimmOut, Profile)) {
                if (Inputs->MemoryProfile != Profile) {
                  MemoryClockMax = ConvertFreq2Clock(MrcData, f12800);
                  //yqr_tCK_debug-s
                  if(Profile==XMP_PROFILE1){
                    IoWrite8(0x72,0x87);
                    IoWrite8(0x73,(UINT8)MemoryClockMax);
                  }
                 
                  //yqr_tCK_debug-e
                } else {
                  MemoryClockMax = Outputs->MemoryClockMax;
                  //yqr_tCK_debug-s
                  if(Profile==XMP_PROFILE1){
                    IoWrite8(0x72,0x88);
                    IoWrite8(0x73,(UINT8)MemoryClockMax);
                  }
                  //yqr_tCK_debug-e
                }
                switch (DimmOut->DdrType) {
                  case MRC_DDR_TYPE_DDR4:
                    Data = &Spd->Ddr4.EndUser.Xmp.Data[Profile - XMP_PROFILE1];
                    tCKminMtb  = Data->tCKAVGmin.Bits.tCKmin;
                    tCKminFine = Data->tCKAVGminFine.Bits.tCKminFine;
                    //yqr_tCK_debug-s
                    TemptCKminFine=TemptCKminFine|tCKminFine;
                    //yqr_tCK_debug-e
                    DimmCalculated = (MediumTimebase * tCKminMtb) + (FineTimebase * tCKminFine);
                    Calculated = MAX (MemoryClockMax, DimmCalculated);
                    //yqr_tCK_debug-s
                    
                    if(Profile==XMP_PROFILE1){
                      IoWrite8(0x72,0xD0);
                      IoWrite8(0x73,MediumTimebase);  
                      IoWrite8(0x72,0xD1);
                      IoWrite8(0x73,MediumTimebase >> 8);  
                      IoWrite8(0x72,0xD2);
                      IoWrite8(0x73,MediumTimebase >> 16);  
                      IoWrite8(0x72,0xD3);
                      IoWrite8(0x73,MediumTimebase >> 24);  
                      IoWrite8(0x72,0xD4);
                      IoWrite8(0x73,FineTimebase);  
                      IoWrite8(0x72,0xD5);
                      IoWrite8(0x73,FineTimebase >> 8);  
                      IoWrite8(0x72,0xD6);
                      IoWrite8(0x73,FineTimebase >> 16);  
                      IoWrite8(0x72,0xD7);
                      IoWrite8(0x73,FineTimebase >> 24);  
                      IoWrite8(0x72,0xD8);
                      IoWrite8(0x73,tCKminMtb);  
                      IoWrite8(0x72,0xD9);
                      IoWrite8(0x73,tCKminMtb >> 8);  
                      IoWrite8(0x72,0xDA);
                      IoWrite8(0x73,tCKminMtb >> 16);  
                      IoWrite8(0x72,0xDB);
                      IoWrite8(0x73,tCKminMtb >> 24);  
                      IoWrite8(0x72,0xDC);
                      IoWrite8(0x73,tCKminFine);  
                      IoWrite8(0x72,0xDD);
                      IoWrite8(0x73,tCKminFine >> 8);  
                      IoWrite8(0x72,0xDE);
                      IoWrite8(0x73,tCKminFine >> 16);  
                      IoWrite8(0x72,0xDF);
                      IoWrite8(0x73,tCKminFine >> 24);  
                      IoWrite8(0x72,0xE0);
                      IoWrite8(0x73,DimmCalculated);
                      IoWrite8(0x72,0xE1);
                      IoWrite8(0x73,DimmCalculated >> 8);
                      IoWrite8(0x72,0xE2);
                      IoWrite8(0x73,DimmCalculated >> 16);
                      IoWrite8(0x72,0xE3);
                      IoWrite8(0x73,DimmCalculated >> 24);
                      IoWrite8(0x72,0xE4);
                      IoWrite8(0x73,Calculated);
                      IoWrite8(0x72,0xE5);
                      IoWrite8(0x73,Calculated >> 8);
                      IoWrite8(0x72,0xE6);
                      IoWrite8(0x73,Calculated >> 16);
                      IoWrite8(0x72,0xE7);
                      IoWrite8(0x73,Calculated >> 24);
                      IoWrite8(0x72,0xE8);
                      IoWrite8(0x73,TemptCKminFine);
                      IoWrite8(0x72,0xE9);
                      IoWrite8(0x73,TemptCKminFine >> 8);
                      IoWrite8(0x72,0xEA);
                      IoWrite8(0x73,TemptCKminFine >> 16);
                      IoWrite8(0x72, 0xEB);
                      IoWrite8(0x73, TemptCKminFine >> 24);

                      g_DDR4tCLKArr[0] = DimmCalculated;
                      IoWrite8(0x72, 0xEC);
                      IoWrite8(0x73, g_DDR4tCLKArr[0]);
                      IoWrite8(0x72, 0xED);
                      IoWrite8(0x73, g_DDR4tCLKArr[0] >> 8);
                      IoWrite8(0x72, 0xEE);
                      IoWrite8(0x73, g_DDR4tCLKArr[0] >> 16);
                      IoWrite8(0x72, 0xEF);
                      IoWrite8(0x73, g_DDR4tCLKArr[0] >> 24);
                    }
                    //yqr_tCK_debug-e
                    break;
                  case MRC_DDR_TYPE_DDR5:
                    Data3 = &Spd->Ddr5.EndUser.Xmp.Data[Profile - XMP_PROFILE1];
                    DimmCalculated = Data3->tCKAVGmin.Bits.tCKmin * 1000; // Convert Picoseconds to Femtoseconds
                    Calculated = MAX (MemoryClockMax, DimmCalculated);
                    break;
                  default:
                    break;
                }
              } else {
                Calculated = 0;
              }
              break;
            case CUSTOM_PROFILE1:
              if (Inputs->Ratio > 0) {
                DimmCalculated = MrcRatioToClock (MrcData, Inputs->Ratio, Outputs->RefClk, Inputs->BClkFrequency);
                Calculated = DimmCalculated;
                Calculated = MAX (Outputs->MemoryClockMax, Calculated);
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                DimmCalculated = Spd->Ddr5.Base.tCKmin.Bits.tCKmin * 1000; // Convert Picoseconds to Femtoseconds
              } else {
                DimmCalculated = (MediumTimebase * tCKminMtb) + (FineTimebase * tCKminFine);
              }
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Outputs->MemoryClockMax: %d DimmCalculated %d  Outputs->FreqMax = %d DimmCalculated's frequency = %d \n", Outputs->MemoryClockMax, DimmCalculated, Outputs->FreqMax, ConvertClock2Freq (MrcData, Outputs->RefClk, DimmCalculated, NULL));
              Outputs->MaxDimmFreq = (UINT16) ConvertClock2Freq (MrcData, Outputs->RefClk, DimmCalculated, NULL);
              Calculated = MAX (Outputs->MemoryClockMax, DimmCalculated);
              if (Inputs->CpuModel == cmRPL_DT_HALO || Inputs->CpuModel == cmRPL_ULX_ULT) {
                if (((Outputs->DdrType == MRC_DDR_TYPE_DDR5) &&
                     (Inputs->SaGv == MrcSaGvEnabled) &&
                     ((Inputs->SaGvFreq[IntOutputs->SaGvPoint]) == 0) &&
                     (!Inputs->DynamicMemoryBoost) &&
                     (!Inputs->RealtimeMemoryFrequency)) &&
                    (Inputs->MemoryProfile == STD_PROFILE)) {
                  if ((ConvertClock2Freq (MrcData, Outputs->RefClk, DimmCalculated, NULL) < f5200) &&
                      (ConvertClock2Freq (MrcData, Outputs->RefClk, Outputs->MemoryClockMax, NULL) >= f4800)) {
                    if (((Inputs->CpuModel == cmRPL_DT_HALO) || (Inputs->CpuModel == cmRPL_ULX_ULT)) && (IntOutputs->SaGvPoint == MrcSaGvPoint3)) {
                      // The max DIMM frequency is 4800, so the frequency in SAGV point 3 needs to be 4400.
                      // 2000 G2, 3600 G2, 4400 G2, 4800 G2
                      Outputs->MemoryClockMax = MRC_DDR_4400_TCK_MIN;
                      Calculated = MAX (Outputs->MemoryClockMax, DimmCalculated);
                      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "change frequency of the third SAGV point to 4400 \n");
                    } /*else if ((Inputs->CpuModel == cmRPL_ULX_ULT) && (IntOutputs->SaGvPoint == MrcSaGvPoint2)) {
                      // The max DIMM frequency is 4800, so the frequency in SAGV point 2 needs to be 4400.
                      // 2000G2, 4400G4, 4800G4, 4800G2
                      Outputs->MemoryClockMax = MRC_DDR_4400_TCK_MIN;
                      Calculated = MAX (Outputs->MemoryClockMax, DimmCalculated);
                      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "change frequency of the second SAGV point to 4400 \n");
                    }*/
                  }

                  //update the SAGV freq for doublesizedimm
                  // Update the SAGV frequency for doublesizedimm.
                  if ((Inputs->Ddr5DoubleSizeDimmPerChannel == 1) &&
                      (Inputs->CpuModel == cmRPL_DT_HALO)) {
                    if (Outputs->Any2Ranks) {
                      if (Outputs->MaxDimmFreq == f5600) {
                        //doublesizedimm 64G (1DPC 2R, SPD max Freq 5600), need to run max freq 5200
                        // 2000 3600 4800 5200
                        if (IntOutputs->SaGvPoint == MrcSaGvPoint3) {
                          Outputs->MemoryClockMax = MRC_DDR_4800_TCK_MIN;
                        } else if (IntOutputs->SaGvPoint == MrcSaGvPoint4) {
                          Outputs->MemoryClockMax = MRC_DDR_5200_TCK_MIN;
                        }
                      }
                    } else {
                      if (Outputs->MaxDimmFreq == f5600) {
                        //doublesizedimm 32G (1DPC 1R, SPD max Freq 5600), need to run max freq 5600
                        //2000 3600 5200 5600
                        if (IntOutputs->SaGvPoint == MrcSaGvPoint3) {
                          Outputs->MemoryClockMax = MRC_DDR_5200_TCK_MIN;
                        } else if (IntOutputs->SaGvPoint == MrcSaGvPoint4) {
                          Outputs->MemoryClockMax = MRC_DDR_5600_TCK_MIN;
                        }
                      } else if (Outputs->MaxDimmFreq == f6400) {
                        //doublesizedimm 32G (1DPC 1R, SPD max Freq 6400), need to run max freq 6400
                        //2000 G2  4800 G2  6000 G2 6400 G2
                        if (IntOutputs->SaGvPoint == MrcSaGvPoint2) {
                          Outputs->MemoryClockMax = MRC_DDR_4800_TCK_MIN;
                        } else if (IntOutputs->SaGvPoint == MrcSaGvPoint3) {
                          Outputs->MemoryClockMax = MRC_DDR_6000_TCK_MIN;
                        }else if (IntOutputs->SaGvPoint == MrcSaGvPoint4) {
                          Outputs->MemoryClockMax = MRC_DDR_6400_TCK_MIN;
                        }
                      }
                    }
                    Calculated = MAX (Outputs->MemoryClockMax, DimmCalculated);
                    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "update SAGV frequecies for doublesize \n");
                  }
                }
              }
              break;
          } //switch
          Actual[Profile] = MAX (Actual[Profile], Calculated);
          //yqr_tCK_debug-s
          if(Profile==XMP_PROFILE1){
              IoWrite8(0x72,0x8B);
              IoWrite8(0x73,(UINT8)Actual[Profile]);
          }
          //yqr_tCK_debug-e

          // tCK is at the 4:1 value in the SPD for LPDDR5 which is what is expected.
          // Find the closest tCK in the table
          ConvertClock2Freq (MrcData, Outputs->RefClk, Actual[Profile], &tCKminIndex);
          
//change_FSP start
        if ( (Profile == XMP_PROFILE1) || (Profile == XMP_PROFILE2) )
        {
            if ( (Calculated > MRC_DDR_3700_TCK_MIN) && (Calculated < MRC_DDR_3500_TCK_MIN) )   //3600
            {
                ConvertClock2Freq (MrcData, MRC_REF_CLOCK_100, Actual[Profile], &tCKminIndex);
            }
            if ( (Calculated > MRC_DDR_3067_TCK_MIN) && (Calculated < 675000) )
            {
                ConvertClock2Freq (MrcData, MRC_REF_CLOCK_100, Actual[Profile], &tCKminIndex);
            }
            if ( Calculated == MRC_DDR_5000_TCK_MIN || Calculated == MRC_DDR_4800_TCK_MIN || Calculated == MRC_DDR_4600_TCK_MIN || Calculated == MRC_DDR_4400_TCK_MIN || Calculated == MRC_DDR_5200_TCK_MIN || Calculated == MRC_DDR_5600_TCK_MIN   ) 
            {
                ConvertClock2Freq (MrcData, MRC_REF_CLOCK_100, Actual[Profile], &tCKminIndex);
            }
        }
//change_FSP end
 
          Actual[Profile] = FreqTable[tCKminIndex].tCK;
          //yqr_tCK_debug-s
          if(Profile==XMP_PROFILE1){
              IoWrite8(0x72,0x8C);
              IoWrite8(0x73,(UINT8)Actual[Profile]);
          }
          //yqr_tCK_debug-e
          if (Outputs->DdrType == MRC_DDR_TYPE_LPDDR5) {
            Actual[Profile] *= 4;
          }
          if (Profile == Inputs->MemoryProfile) {
            DimmOut->Speed = ConvertClock2Freq (MrcData, Outputs->RefClk, DimmCalculated, NULL);
          }

          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u % 6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tCK = Actual[Profile];
        //yqr_tCK_debug-s
        // if(Profile==XMP_PROFILE1){
        //     ChannelOut->Timing[Profile].tCK=0x66;
        // }
        IoWrite32(0x72,0xC0+Profile);
        IoWrite32(0x73,ChannelOut->Timing[Profile].tCK);
        //yqr_tCK_debug-e
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  Outputs->MemoryClock = Actual[Inputs->MemoryProfile];

  return TRUE;
}

/**
  Determine if the calculated CAS Latency is within the supported CAS Latency Mask

    @param[in] MrcData     - Pointer to MrcData data structure.
    @param[in] Profile     - the Profile for checking
    @param[in] tAA         - the calculated CAS Latency in tCKs.
    @param[in] DdrType     - the SDRAM type number.
    @param[in] tCLMask     - the supported CAS Latency mask.
    @param[in] tCLLimitMin - the minimum supported CAS Latency
    @param[in] SdramWidth  - SDRAM width (8 or 16)

    @retval TRUE if the CAS latency has been found in supported mask, otherwise FALSE.
**/
static
BOOLEAN
ValidtCL (
  IN MrcParameters    *const MrcData,
  IN MrcProfile       Profile,
  IN const UINT32     tAA,
  IN const MrcDdrType DdrType,
  IN const UINT64     tCLMask,
  IN const UINT32     tCLLimitMin,
  IN const UINT8      SdramWidth
  )
{
  const MRC_FUNCTION *MrcCall;
  BOOLEAN            Valid;

  Valid           = FALSE;
  MrcCall         = MrcData->Inputs.Call.Func;

  switch (DdrType) {
    case MRC_DDR_TYPE_DDR5:
      // DDR5 Only supports even values of tCL(tAA)
      if (((tAA % 2) != 0) || (tAA < tCLLimitMin)) {
        Valid = FALSE;
      } else {
        Valid = (MrcCall->MrcRightShift64 (tCLMask, (tAA - tCLLimitMin)/2) & 1);
      }
      break;

    case MRC_DDR_TYPE_DDR4:
    default:
      if (tAA >= tCLLimitMin) {
        Valid = (MrcCall->MrcRightShift64 (tCLMask, (tAA - tCLLimitMin)) & 1);
        if (!Valid && (Profile != STD_PROFILE)) {
          /*
           * Some XMP memory reports weird CAS Latency mask.
           * Lower tCL can pass, but bigger tCL can't.
           * Below codes work around it.
           */
          if ((MrcCall->MrcLeftShift64(1UL, tAA - tCLLimitMin) > tCLMask) && (tCLMask > 0)) {
            Valid = TRUE;
          }
        }
      }
      break;

    // Ignoring Valid tCL mask as it doesn't make sense in terms of LPDDR as tCL is binned by the frequency requested
    // and all parts must be of the same timing.
    case MRC_DDR_TYPE_LPDDR4:
    case MRC_DDR_TYPE_LPDDR5:
      Valid = TRUE;
      break;
  }
  return Valid;
}

/**
  Calculate the tCL value for LPDDR4.
  JEDEC Spec x8/x16 RL values:
    Lower Clk   Upper Clk      x16    x8
    Freq Limit  Freq Limit     RL     RL
    --------------------------------------
    10            266          6      6
    266           533          10     10
    533           800          14     16
    800           1066         20     22
    1066          1333         24     26
    1333          1600         28     32
    1600          1866         32     36
    1866          2133         36     40

    @param[in] tCK         - the memory tCK in femtoseconds.
    @param[in] SdramWidth  - SDRAM width (8 or 16)

    @retval LPDDR4 tCL in tCK units
**/
static
UINT32
GetLpddr4tCL (
  IN const UINT32     tCK,
  IN UINT8            SdramWidth
  )
{
  UINT32 tCL;

  tCL = 0;
  if (tCK > 0) {
    if (SdramWidth == 8) {
      if (tCK < MRC_DDR_3733_TCK_MIN) {
        tCL = 40;
      } else if (tCK < MRC_DDR_3200_TCK_MIN) {
        tCL = 36;
      } else if (tCK < MRC_DDR_2667_TCK_MIN) {
        tCL = 32;
      } else if (tCK < MRC_DDR_2133_TCK_MIN) {
        tCL = 26;
      } else if (tCK < MRC_DDR_1600_TCK_MIN) {
        tCL = 22;
      } else if (tCK < MRC_DDR_1067_TCK_MIN) {
        tCL = 16;
      } else if (tCK < MRC_DDR_533_TCK_MIN) {
        tCL = 10;
      } else {
        tCL = 6;
      }
    } else { // x16
      if (tCK < MRC_DDR_3733_TCK_MIN) {
        tCL = 36;
      } else if (tCK < MRC_DDR_3200_TCK_MIN) {
        tCL = 32;
      } else if (tCK < MRC_DDR_2667_TCK_MIN) {
        tCL = 28;
      } else if (tCK < MRC_DDR_2133_TCK_MIN) {
        tCL = 24;
      } else if (tCK < MRC_DDR_1600_TCK_MIN) {
        tCL = 20;
      } else if (tCK < MRC_DDR_1067_TCK_MIN) {
        tCL = 14;
      } else if (tCK < MRC_DDR_533_TCK_MIN) {
        tCL = 10;
      } else {
        tCL = 6;
      }
    }
  } // tCK > 0
  return tCL;
}

/**
  Calculate the tCL value for LPDDR5.
  JEDEC Spec x8/x16 RL values:
                                 No DBI
    Lower Clk   Upper Clk      x16    x8
    Freq Limit  Freq Limit     RL     RL
    --------------------------------------
    10             67           3      3
    67            133           4      4
    133           200           5      5
    200           267           6      7
    267           344           8      8
    344           400           9     10
    400           467          10     11
    467           533          12     13
    533           600          13     14
    600           750          16     17
    750           800          17     18

    @param[in] tCK       - The memory tCK in femtoseconds.
    @param[in] SdramWidth  - SDRAM width (8 or 16)

    @retval LPDDR4 tCL in tCK units
**/
static
UINT32
GetLpddr5tCL (
  IN const UINT32     tCK,
  IN UINT8            SdramWidth
  )
{
  UINT32 tCL;
  UINT32 tCKNorm;

  tCL = 0;
  // Scale tCK up to typical DDR ratio of 2:1 between tCK and Data Rate
  // We are always in 4:1 mode for WCK.
  tCKNorm = tCK / 4;

  if (SdramWidth == 8) {
    if (tCKNorm >= MRC_DDR_533_TCK_MIN) {
      tCL = 3;
    } else if (tCKNorm >= MRC_DDR_1067_TCK_MIN) {
      tCL = 4;
    } else if (tCKNorm >= MRC_DDR_1600_TCK_MIN) {
      tCL = 5;
    } else if (tCKNorm >= MRC_DDR_2133_TCK_MIN) {
      tCL = 7;
    } else if (tCKNorm >= MRC_DDR_2750_TCK_MIN) {
      tCL = 8;
    } else if (tCKNorm >= MRC_DDR_3200_TCK_MIN) {
      tCL = 10;
    } else if (tCKNorm >= MRC_DDR_3733_TCK_MIN) {
      tCL = 11;
    } else if (tCKNorm >= MRC_DDR_4267_TCK_MIN) {
      tCL = 13;
    } else if (tCKNorm >= MRC_DDR_4800_TCK_MIN) {
      tCL = 14;
    } else if (tCKNorm >= MRC_DDR_5500_TCK_MIN) {
      tCL = 16;
    } else if (tCKNorm >= MRC_DDR_6000_TCK_MIN) {
      tCL = 17;
    } else if (tCKNorm >= MRC_DDR_6400_TCK_MIN) {
      tCL = 18;
    } else if (tCKNorm >= MRC_DDR_7500_TCK_MIN) {
      tCL = 22;
    } else {
      tCL = 25;
    }
  } else { // x16
    if (tCKNorm >= MRC_DDR_533_TCK_MIN) {
      tCL = 3;
    } else if (tCKNorm >= MRC_DDR_1067_TCK_MIN) {
      tCL = 4;
    } else if (tCKNorm >= MRC_DDR_1600_TCK_MIN) {
      tCL = 5;
    } else if (tCKNorm >= MRC_DDR_2133_TCK_MIN) {
      tCL = 6;
    } else if (tCKNorm >= MRC_DDR_2750_TCK_MIN) {
      tCL = 8;
    } else if (tCKNorm >= MRC_DDR_3200_TCK_MIN) {
      tCL = 9;
    } else if (tCKNorm >= MRC_DDR_3733_TCK_MIN) {
      tCL = 10;
    } else if (tCKNorm >= MRC_DDR_4267_TCK_MIN) {
      tCL = 12;
    } else if (tCKNorm >= MRC_DDR_4800_TCK_MIN) {
      tCL = 13;
    } else if (tCKNorm >= MRC_DDR_5500_TCK_MIN) {
      tCL = 15;
    } else if (tCKNorm >= MRC_DDR_6000_TCK_MIN) {
      tCL = 16;
    } else if (tCKNorm >= MRC_DDR_6400_TCK_MIN) {
      tCL = 17;
    } else if (tCKNorm >= MRC_DDR_7500_TCK_MIN) {
      tCL = 20;
    } else {
      tCL = 23;
    }
  }

  return tCL;
}

/**
  Calculate the Minimum CAS Latency Time (tAAmin) for the given DIMMs.
      Step 1: Determine the common set of supported CAS Latency values for all modules
              on the memory channel using the CAS Latencies Supported in SPD.
      Step 2: Determine tAAmin(all) which is the largest tAAmin value for all modules on the memory channel.
      Step 3: Determine tCK(all) which is the largest tCKmin value for all
              the modules on the memory channel (Done in function GetChannelDimmtCK).
      Step 4: For a proposed tCK value between tCKmin and tCKmax, determine the desired CAS Latency.
              If tCKproposed is not a standard JEDEC value then tCKproposed must be adjusted to the
              next lower standard tCK value for calculating CLdesired.
      Step 5: Chose an actual CAS Latency that is greater than or equal to CLdesired and is
              supported by all modules on the memory channel as determined in step 1. If no such value exists,
              choose a higher tCKproposed value and repeat steps 4 and 5 until a solution is found.
      Step 6: Once the calculation of CLactual is completed, the BIOS must also verify that this CAS
              Latency value does not exceed tAAmax, which is 20 ns for all DDR3 speed grades.
              If not, choose a lower CL value and repeat steps 5 and 6 until a solution is found.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if the CAS latency has been calculated, otherwise FALSE and the returned value is set to zero.
**/
static
BOOLEAN
GetChannelDimmtAA (
  IN OUT MrcParameters  *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MRC_FUNCTION          *MrcCall;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  BOOLEAN               Found[MAX_PROFILE];
  BOOLEAN               CustomProfile;
  BOOLEAN               tCLOverride;
  BOOLEAN               Status;
  INT32                 MediumTimeBase;
  INT32                 FineTimeBase;
  INT32                 tCKminIndex;
  INT32                 tCKmin100;
  INT32                 tCKminIndexSave;
  INT32                 TimingFTB;
  UINT32                TimingMTB;
  UINT32                tCKmin;
  UINT64                CommonCasMask[MAX_PROFILE];
  UINT64                CasMask;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                tCLLimitMin;
  UINT32                tCLLimitMax;
  UINT32                tAAmax;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT8                 SdramWidth;
  UINT32                tempCalculated;
//   //yqrdebug
//   UINT32                tempTCL;
//   //yqrdebug

  const SPD_EXTREME_MEMORY_PROFILE_DATA_2_0  *Data;
  UINT32                                     Index;

  Inputs         = &MrcData->Inputs;
  Outputs        = &MrcData->Outputs;
  Debug          = &Outputs->Debug;
  MrcCall        = Inputs->Call.Func;
  tCKmin         = 0;
  Calculated     = 0;
  Status         = FALSE;
  tCLOverride    = FALSE;
  MediumTimeBase = 0;
  FineTimeBase   = 0;
  TimingMTB      = 0;
  TimingFTB      = 0;
  SdramWidth     = 0;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s tCL Mask\n", tAAString, HeaderString);

  if (Outputs->DdrType == MRC_DDR_TYPE_DDR4) {
    tAAmax = MRC_tAA_MAX_DDR4;
  } else {
    tAAmax = MRC_UINT32_MAX; // tAA_max is not defined for LPDDR4 / LPDDR5 / DDR5
  }

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_DDR4:
      tCLLimitMin = 7;
      tCLLimitMax = 24;
      break;
    case MRC_DDR_TYPE_DDR5:
      tCLLimitMin = 20;
      tCLLimitMax = 98;
      break;
    default:
      // For LPDDR4 and LPDDR5
      tCLLimitMin = 3;
      tCLLimitMax = 40;
      break;
  }

  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    CustomProfile            = ((Profile == CUSTOM_PROFILE1) && (Inputs->MemoryProfile == CUSTOM_PROFILE1));
    CommonCasMask[Profile] = MRC_UINT64_MAX;
    Actual[Profile]        = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd            = &DimmIn->Spd.Data;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          MediumTimeBase = ChannelOut->TimeBase[Dimm][Profile].Mtb;
          FineTimeBase   = ChannelOut->TimeBase[Dimm][Profile].Ftb;
          CasMask        = 0;
          SdramWidth     = DimmOut->SdramWidth;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }

              Index = Profile - XMP_PROFILE1;
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  Data        = &Spd->Ddr4.EndUser.Xmp.Data[Index];
                  TimingMTB   = Data->tAAmin.Bits.tAAmin;
                  TimingFTB   = Data->tAAminFine.Bits.tAAminFine;
                  CasMask     = Data->CasLatencies.Data & MRC_SPD_DDR4_CL_SUPPORTED_MASK;
                  Calculated  = (tCKmin == 0) ? 0 : ((MediumTimeBase * TimingMTB) + (FineTimeBase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                  //yqr-debug-s
                  if(Profile==XMP_PROFILE1){
                    tempCalculated  =((MediumTimeBase * TimingMTB) + (FineTimeBase * TimingFTB) - (g_DDR4tCLKArr[0] / 100) + (g_DDR4tCLKArr[0] - 1)) / g_DDR4tCLKArr[0];
                    IoWrite8(0x72,0xBC);
                    IoWrite8(0x73,tempCalculated);
                    IoWrite8(0x72,0xBD);
                    IoWrite8(0x73,tempCalculated >> 8);
                    IoWrite8(0x72,0xBE);
                    IoWrite8(0x73,tempCalculated >> 16);
                    IoWrite8(0x72,0xBF);
                    IoWrite8(0x73,tempCalculated >> 24);
                  }
                  //yqr-debug-e
                  if (Calculated <= Actual[STD_PROFILE]) {
                    // XMP profile's tCL has to be > STD profile's
            //change_FSP        Calculated = Actual[STD_PROFILE] + 1;
                  }
                  break;

                case MRC_DDR_TYPE_DDR5:
                  // Lower 32 bits
                  CasMask     = ((UINT32) Spd->Ddr5.EndUser.Xmp.Data[Index].CasLatencies.Data8[0]) |
                                ((UINT32) Spd->Ddr5.EndUser.Xmp.Data[Index].CasLatencies.Data8[1] << 8) |
                                ((UINT32) Spd->Ddr5.EndUser.Xmp.Data[Index].CasLatencies.Data8[2] << 16) |
                                ((UINT32) Spd->Ddr5.EndUser.Xmp.Data[Index].CasLatencies.Data8[3] << 24);
                  // Upper 32 bits
                  CasMask    |= MrcCall->MrcLeftShift64 ((UINT64) Spd->Ddr5.EndUser.Xmp.Data[Index].CasLatencies.Data8[4], 32);
                  Calculated  = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Index].tAAmin.Bits.tAAmin, tCKmin);
                  if ((Inputs->OCSafeMode) && ((Calculated + 2) < Actual[STD_PROFILE])) {
                    /*
                     * Sometimes, XMP profile might provide inappropriate tCL value.
                     * Add a checking to make sure tCL shouldn't be much smaller
                     * than STD_PROFILE's.
                     */
                    Calculated = Actual[STD_PROFILE] - 2;
                  }
                  break;

                default:
                  break;
              }
              break;

            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tCL > 0) {
                CasMask         = MRC_UINT64_MAX;
                Calculated      = DimmIn->Timing.tCL;
                tCLOverride     = TRUE;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                // Lower 32 bits
                CasMask     = ((UINT32) Spd->Ddr5.Base.CasLatencies.Data8[0]) |
                              ((UINT32) Spd->Ddr5.Base.CasLatencies.Data8[1] << 8) |
                              ((UINT32) Spd->Ddr5.Base.CasLatencies.Data8[2] << 16) |
                              ((UINT32) Spd->Ddr5.Base.CasLatencies.Data8[3] << 24);
                // Upper 32 bits
                CasMask    |= MrcCall->MrcLeftShift64 ((UINT64) Spd->Ddr5.Base.CasLatencies.Data8[4], 32);
                Calculated  = PicoSecondsToClocks (Spd->Ddr5.Base.tAAmin.Bits.tAAmin, tCKmin);
              } else {
                if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
                  TimingMTB   = Spd->Ddr4.Base.tAAmin.Bits.tAAmin;
                  TimingFTB   = Spd->Ddr4.Base.tAAminFine.Bits.tAAminFine;
                  CasMask     = Spd->Ddr4.Base.CasLatencies.Data & MRC_SPD_DDR4_CL_SUPPORTED_MASK;
                } else {
                  TimingMTB   = Spd->Lpddr.Base.tAAmin.Bits.tAAmin;
                  TimingFTB   = Spd->Lpddr.Base.tAAminFine.Bits.tAAminFine;
                  CasMask     = Spd->Lpddr.Base.CasLatencies.Data & MRC_SPD_LPDDR_CL_SUPPORTED_MASK;
                }
                if (DimmOut->DdrType == MRC_DDR_TYPE_LPDDR4) {
                  Calculated  = GetLpddr4tCL (tCKmin, SdramWidth);
                } else if (DimmOut->DdrType == MRC_DDR_TYPE_LPDDR5) {
                  Calculated  = GetLpddr5tCL (tCKmin, SdramWidth);
                } else { // DDR4
                  // Using 2.5% rounding here according to the latest JEDEC SPD spec
                  // @todo Update all other timings to use 2.5% instead of 1%
                  Calculated = (tCKmin == 0) ? 0 : ((MediumTimeBase * TimingMTB) + (FineTimeBase * TimingFTB) - (tCKmin * 25 / 1000) + (tCKmin - 1)) / tCKmin;
                }
              }
              break;
          } //end switch

          if (DimmOut->DdrType == MRC_DDR_TYPE_DDR4) {
            if (Calculated < 9) {
              Calculated = 9;                 // 9 is the smallest valid CL in DDR4
              CasMask |= MRC_BIT2;            // Make sure it's supported in CAS mask as well
            }
            if (Calculated == 23) {
              Calculated = 24;                // 23 is not a valid DDR4 CAS value, use 24
            }
            if (CustomProfile) {
              Calculated = MIN (Calculated, tCLLimitMax); // Enforce maximum allowed CAS value in User profile
            }
          }
          if (DimmOut->DdrType == MRC_DDR_TYPE_DDR5) {
            if (Calculated < 22) {
              Calculated = 22;                // 22 is the smallest valid CL in DDR5
            }
            if ((Profile == CUSTOM_PROFILE1) && ((Calculated % 2) != 0)) {
              Calculated++;                   // JEDEC spec only allows even numbers
            }
          }
          Actual[Profile] = MAX (Actual[Profile], Calculated);
          CommonCasMask[Profile] &= CasMask;
          MRC_DEBUG_MSG (
              Debug,
              MSG_LEVEL_NOTE,
              "  % 7u % 10u % 8u % 5u %6u % 8lXh\n",
              Profile,
              Controller,
              Channel,
              Dimm,
              Calculated,
              CasMask
              );
        } //for Dimm
      } //for Channel
    } //for Controller

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Profile %u common set of supported CAS Latency values = %lXh\n", Profile, CommonCasMask[Profile]);

    if ((Profile >= XMP_PROFILE1) && (tCKmin == 0)) {
      continue;
    }

    Found[Profile] = FALSE;
    ConvertClock2Freq (MrcData, Outputs->RefClk, tCKmin, &tCKminIndex);
    if ((Profile >= XMP_PROFILE1) && (Outputs->RefClk == MRC_REF_CLOCK_133) && (Outputs->Capable100)) {
      ConvertClock2Freq (MrcData, MRC_REF_CLOCK_100, tCKmin, &tCKmin100);
      if (tCKmin100 > tCKminIndex) {
        tCKminIndex = tCKmin100;
        if (Inputs->MemoryProfile == Profile) {
          Outputs->RefClk = MRC_REF_CLOCK_100;
        }
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Profile%u is RefClk 100 capable, switching to RefClk 100\n", Profile);
      }
    }
    do {
      for (; !Found[Profile] && (Actual[Profile] <= tCLLimitMax); Actual[Profile]++) {
        if (CustomProfile) {
          Found[Profile] = TRUE;
        } else if ((Actual[Profile] * tCKmin) <= tAAmax) {
          Found[Profile] = ValidtCL (
                            MrcData,
                            Profile,
                            Actual[Profile],
                            Outputs->DdrType,
                            CommonCasMask[Profile],
                            tCLLimitMin,
                            SdramWidth
                            );
        } // if
        if (Found[Profile]) {
          if (Profile == Inputs->MemoryProfile) {
            Outputs->MemoryClock = tCKmin;
            Status = TRUE;
          }
          for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
            ControllerOut = &Outputs->Controller[Controller];
            for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
              ChannelOut = &ControllerOut->Channel[Channel];
              for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
                DimmOut = &ChannelOut->Dimm[Dimm];
                if (DIMM_PRESENT == DimmOut->Status) {
                  ChannelOut->Timing[Profile].tCL = (UINT16) Actual[Profile];
                  ChannelOut->Timing[Profile].tCK = tCKmin;
                  //yqr_debug-s
                  if (Profile==XMP_PROFILE1)
                  {
                    IoWrite8(0x72, 0x84);
                    IoWrite8(0x73, (UINT8)tCKmin);
                  }
                  //yqr_debug-e
                } //if
              } //for Dimm
            } //for Channel
          } //for Controller
          break;
        } //if
      } //for Actual[Profile]
      if (!Found[Profile]) {
        if (CustomProfile && ((Inputs->Ratio > 0) || (tCLOverride == TRUE))) {
          break;
        } else {
          tCKminIndexSave = tCKminIndex;
          while (--tCKminIndex > 0) {
            if (((FreqTable[tCKminIndex].FreqFlag & (1 << Outputs->RefClk)) != 0)) {
              tCKmin = FreqTable[tCKminIndex].tCK;
              if (Outputs->DdrType == MRC_DDR_TYPE_LPDDR5) {
                tCKmin *= 4;
              }
              ConvertClock2Freq (MrcData, Outputs->RefClk, tCKmin, &tCKminIndex);
              Actual[Profile] = (tCKmin == 0) ? 0 : ((MediumTimeBase * TimingMTB) + (FineTimeBase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
              MRC_DEBUG_MSG (Debug,
                MSG_LEVEL_WARNING,
                "Warning: The memory frequency is being downgraded on profile %u, from %u to %u and the new tCL is %u\n",
                Profile,
                FreqTable[tCKminIndexSave].Frequency,
                FreqTable[tCKminIndex].Frequency,
                Actual[Profile]);
              break;
            }
          }
        }
      }
    } while (!Found[Profile] && (tCKminIndex > 0));
  } //for Profile

  Outputs->Frequency = ConvertClock2Freq (MrcData, Outputs->RefClk, Outputs->MemoryClock, NULL);
  Outputs->HighFrequency = Outputs->Frequency;

#ifdef MRC_DEBUG_PRINT
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Found[Profile] ? Actual[Profile] : 0);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n  Memory clock = %ufs\n  Memory Frequency = %u\n", Outputs->MemoryClock, Outputs->Frequency);
#endif

  return (Status);
}

/**
  Calculate the tCWL value for DDR5.

    @param[in] tCL - current tCL in DCLKs.

    @retval DDR5 tCWL in DCLK units
**/
static
UINT32
GetDdr5tCWL (
  IN const UINT32 tCL
  )
{
  UINT32 tCWL;

  tCWL = tCL - 2;
  return tCWL;
}

/**
  Calculate the tCWL value for DDR4.
  JEDEC Spec:
                    Set A   Set B
   --------------------------------
   1600 and below:  9       11
   1867:            10      12
   2133:            11      14
   2400:            12      16
   2667:            14      18
   2933:            16      20
   3200:            16      20

    @param[in] tCK - the memory DCLK in femtoseconds.
    @param[in] tCL - current tCL in DCLKs.

    @retval DDR4 tCWL in DCLK units
**/
static
UINT32
GetDdr4tCWL (
  IN const UINT32 tCK,
  IN const UINT32 tCL
  )
{
  UINT32 tCWL;

  //
  // We pick up tCWL from the JEDEC list per frequency,
  // so that it will be closest to tCL, for performance reasons.
  //
  if (tCK >= MRC_DDR_1333_TCK_MIN) {
    tCWL = 9;
  } else if (tCK >= MRC_DDR_1600_TCK_MIN) {
    tCWL = (tCL >= 11) ? 11 : 9;
  } else if (tCK >= MRC_DDR_1867_TCK_MIN) {
    tCWL = 12;
  } else if (tCK >= MRC_DDR_2133_TCK_MIN) {
    tCWL = 14;
  } else if (tCK >= MRC_DDR_2400_TCK_MIN) {
    tCWL = (tCL >= 16) ? 16 : 12;
  } else if (tCK >= MRC_DDR_2667_TCK_MIN) {
    tCWL = (tCL >= 18) ? 18 : 14;
  } else if (tCK >= MRC_DDR_2933_TCK_MIN) {
    tCWL = (tCL >= 20) ? 20 : 16;
  } else {
    tCWL = 20;
  }

  return tCWL;
}

/**
  Calculate the tCWL value for LPDDR4.

  JEDEC Spec x8/x16 WL values:
    Lower Clk   Upper Clk      SetA   SetB
    Freq Limit  Freq Limit     WL     WL
    --------------------------------------
    10            266          4      4
    266           533          6      8
    533           800          8      12
    800           1066         10     18
    1066          1333         12     22
    1333          1600         14     26
    1600          1866         16     30
    1866          2133         18     34

  @param[in] tCK             - the memory DCLK in femtoseconds.

@retval LpDDR4 tCWL Value
**/
static
UINT32
GetLpddr4tCWL (
  IN UINT32 tCKmin
  )
{
  UINT32 tCWL;

  tCWL = 0;
  //
  // Using WL Set B values from table 4.12 of LPDDR4 JEDEC Spec.
  // Adding 1 to take tDQSS into account.
  // We will subtract this 1 when programming TC_ODT.tCWL.
  //
  if (tCKmin < MRC_DDR_3733_TCK_MIN) {
    tCWL = 35;
  } else if (tCKmin < MRC_DDR_3200_TCK_MIN) {
    tCWL = 31;
  } else if (tCKmin < MRC_DDR_2667_TCK_MIN) {
    tCWL = 27;
  } else if (tCKmin < MRC_DDR_2133_TCK_MIN) {
    tCWL = 23;
  } else if (tCKmin < MRC_DDR_1600_TCK_MIN) {
    tCWL = 19;
  } else if (tCKmin < MRC_DDR_1067_TCK_MIN) {
    tCWL = 13;
  } else if (tCKmin < MRC_DDR_533_TCK_MIN) {
    tCWL = 9;
  } else {
    tCWL = 5;
  }

  return tCWL;
}

/**
  Calculate the tCWL value for LPDDR5.

  JEDEC Spec x8/x16 WL values:
    Lower Clk   Upper Clk      SetA   SetB
    Freq Limit  Freq Limit     WL     WL
    --------------------------------------
    10            67           2      2
    67            133          2      3
    133           200          3      4
    200           267          4      5
    267           344          4      7
    344           400          5      8
    400           467          6      9
    467           533          6      11
    533           600          7      12
    600           688          8      14
    688           750          9      15
    750           800          9      16

  @param[in] tCK   - The memory DCLK in femtoseconds.
  @param[in] WlSet - 0: Set A, 1: Set B

@retval LpDDR5 tCWL Value
**/
static
UINT32
GetLpddr5tCWL (
  IN UINT32 tCKmin,
  IN UINT8  WlSet
)
{
  UINT32 tCWL;
  UINT32 tCKNorm;

  tCWL = 0;
  tCKNorm = tCKmin / 4;
  //
  // Using WL Set B values from table 4.6.2 of LPDDR5 JEDEC Spec.
  // Adding 1 to take tDQSS into account.
  // We will subtract this 1 when programming TC_ODT.tCWL.
  //
  if (tCKNorm >= MRC_DDR_533_TCK_MIN) {
    tCWL = 2;
  } else if (tCKNorm >= MRC_DDR_1067_TCK_MIN) {
    tCWL = 3;
  } else if (tCKNorm >= MRC_DDR_1600_TCK_MIN) {
    tCWL = 4;
  } else if (tCKNorm >= MRC_DDR_2133_TCK_MIN) {
    tCWL = 5;
  } else if (tCKNorm >= MRC_DDR_2750_TCK_MIN) {
    tCWL = 7;
  } else if (tCKNorm >= MRC_DDR_3200_TCK_MIN) {
    tCWL = 8;
  } else if (tCKNorm >= MRC_DDR_3733_TCK_MIN) {
    tCWL = 9;
  } else if (tCKNorm >= MRC_DDR_4267_TCK_MIN) {
    tCWL = 11;
  } else if (tCKNorm >= MRC_DDR_4800_TCK_MIN) {
    tCWL = 12;
  } else if (tCKNorm >= MRC_DDR_5500_TCK_MIN) {
    tCWL = 14;
  } else if (tCKNorm >= MRC_DDR_6000_TCK_MIN) {
    tCWL = 15;
  } else if (tCKNorm >= MRC_DDR_6400_TCK_MIN) {
    tCWL = 16;
  } else if (tCKNorm >= MRC_DDR_7500_TCK_MIN) {
    tCWL = 19;
  } else {
    tCWL = 22;
  }
  return tCWL;
}

/**
  Calculate the minimum tCWL timing value for the given memory frequency.
    We calculate timings for all profiles so that this information can be passed out of MRC.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtCWL (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                tCL;

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  Debug       = &Outputs->Debug;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tCWLString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }

    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }

          tCKmin         = ChannelOut->Timing[Profile].tCK;
          tCL            = ChannelOut->Timing[Profile].tCL;
          Calculated     = 0;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (XmpSupport (DimmOut, Profile)) {
                switch (DimmOut->DdrType) {
                  case MRC_DDR_TYPE_DDR4:
                    // DDR4 XMP spec doesn't have tCWL, so use JEDEC formulas
                    Calculated = GetDdr4tCWL (tCKmin, tCL);
                    break;
                  case MRC_DDR_TYPE_DDR5:
                    // DDR5 XMP spec doesn't have tCWL, so use JEDEC formulas
                    Calculated = GetDdr5tCWL (tCL);
                    break;
                  default:
                    break;
                }
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tCWL > 0) {
                Calculated = DimmIn->Timing.tCWL;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (DimmOut->DdrType == MRC_DDR_TYPE_LPDDR4) {
                Calculated  = GetLpddr4tCWL (tCKmin);
              } else if (DimmOut->DdrType == MRC_DDR_TYPE_LPDDR5) {
                Calculated = GetLpddr5tCWL (tCKmin, 1); // We always use Set B (1)
              } else if (DimmOut->DdrType == MRC_DDR_TYPE_DDR4) {
                Calculated = GetDdr4tCWL (tCKmin, tCL);
              } else if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                Calculated = GetDdr5tCWL (tCL);
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tCWL = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the DRAM Page Size in units of Bytes

    @param[in, out] ColumnSize - The SDRAM Column Size in bits
    @param[in, out] SdramWidth - The SRAM IO width

    @return The DRAM page size in units of Bytes
**/
UINT16
CalculatePageSize (
  IN UINT16 ColumnSize,
  IN UINT8  SdramWidth
  )
{
  UINT16 ColumnSizeInBytes;
  // Calculate the total Column bit count and convert to Bytes
  ColumnSizeInBytes = (UINT16)(ColumnSize / 8);
  // Calculate the page size
  return ColumnSizeInBytes * SdramWidth;
}

/**
  Calculate the tFAW value for LPDDR4
  JEDEC Spec: 30ns for 4267, 40ns for all other data rates

  @param[in] tCK        - The memory tCK in femtoseconds.
  @param[in] Frequency  - Current frequency

  @retval tFAW in tCK units
**/
static
UINT32
GetLpddr4tFAW (
  IN const UINT32       tCK,
  IN const MrcFrequency Frequency
  )
{
  UINT32  tFAW;
  UINT32  ValueNs;

  tFAW = 0;
  ValueNs = (Frequency >= f4267) ? 30 : 40;

  if (tCK > 0) {
    tFAW = DIVIDECEIL ((ValueNs * 1000000 - (tCK / 100)), tCK);
  }

  return tFAW;
}

/**
  Calculate the tFAW value for LPDDR5
  JEDEC Spec:
    x16/x8 8B mode:   40ns
    x16/x8 16B mode:  20ns
    x16/x8 BG mode:   20ns

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in] tCK       - The memory tCK in femtoseconds.
    @param[in] Frequency - The memory frequency.

    @retval tFAW in tCK units
**/
static
UINT32
GetLpddr5tFAW (
  IN OUT MrcParameters *const MrcData,
  IN const UINT32      tCK,
  IN MrcFrequency      Frequency
  )
{
  UINT32  tFAW;
  UINT32  ValueNs;
  MRC_LP5_BANKORG  Lp5BGOrg;
  MrcOutput        *Outputs;

  tFAW = 0;
  ValueNs = 40;
  Outputs = &MrcData->Outputs;

  Lp5BGOrg = MrcGetBankBgOrg (MrcData, Frequency);

  if (tCK > 0) {
    if (Lp5BGOrg != MrcLp58Bank) {
      if (Outputs->LpX) {
        // LP5x
        tFAW = DIVIDECEIL ((15000000 - (tCK / 100)), tCK); // 15ns
      } else {
        tFAW = DIVIDECEIL ((ValueNs * 500000 - (tCK / 100)), tCK); // 20ns
      }
    } else {
      tFAW = DIVIDECEIL ((ValueNs * 1000000 - (tCK / 100)), tCK);
    }
  }


  return tFAW;
}

/**
  Calculate the minimum tFAW timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtFAW (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                TimingMTB;
  INT32                 MediumTimebase;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT16                PageSize;

  const SPD_EXTREME_MEMORY_PROFILE_DATA_2_0  *Data;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tFAWString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  Data = &Spd->Ddr4.EndUser.Xmp.Data[Profile - XMP_PROFILE1];
                  TimingMTB  = ((UINT32) (Data->tFAWMinUpper.Bits.tFAWminUpper) << 8) | (UINT32) (Data->tFAWmin.Bits.tFAWmin);
                  Calculated = (tCKmin == 0) ? 0 : ((MediumTimebase * TimingMTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                  break;
                case MRC_DDR_TYPE_DDR5:
                  PageSize = CalculatePageSize (DimmOut->ColumnSize, DimmOut->SdramWidth);
                  // DDR5 tFAW is defined in the spec
                  Calculated = (PageSize >= 2048) ? 40 : 32;
                  break;
                default:
                  break;
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tFAW > 0) {
                Calculated = DimmIn->Timing.tFAW;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                PageSize = CalculatePageSize (DimmOut->ColumnSize, DimmOut->SdramWidth);
                // DDR5 tFAW is defined in the spec
                Calculated = (PageSize >= 2048) ? 40 : 32;
              } else if (tCKmin > 0) {
                if (Outputs->Lpddr) {
                  if (DimmOut->DdrType == MRC_DDR_TYPE_LPDDR4) {
                    Calculated = GetLpddr4tFAW (tCKmin, Outputs->Frequency);
                  } else { // LPDDR5
                    Calculated = GetLpddr5tFAW (MrcData, tCKmin, Outputs->Frequency);
                  }
                } else { // DDR4
                  TimingMTB = ((UINT32) (Spd->Ddr4.Base.tFAWMinUpper.Bits.tFAWminUpper) << 8) | (UINT32) (Spd->Ddr4.Base.tFAWmin.Bits.tFAWmin);
                  Calculated = ((MediumTimebase * TimingMTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
              Debug,
              MSG_LEVEL_NOTE,
              "  % 7u % 10u % 8u % 5u %6u\n",
              Profile,
              Controller,
              Channel,
              Dimm,
              Calculated
            );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tFAW = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRAS timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtRAS (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                TimingMTB;
  INT32                 MediumTimebase;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;

  const SPD_EXTREME_MEMORY_PROFILE_DATA_2_0  *Data;
  UINT32                                     Index;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRASString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }

          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (!XmpSupport(DimmOut, Profile)) {
                break;
              }
              Index       = Profile - XMP_PROFILE1;
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  Data        = &Spd->Ddr4.EndUser.Xmp.Data[Index];
                  TimingMTB   = ((UINT32) (Data->tRASMintRCMinUpper.Bits.tRASminUpper) << 8) | (UINT32) (Data->tRASmin.Bits.tRASmin);
                  Calculated  = (tCKmin == 0) ? 0 : ((MediumTimebase * TimingMTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                  //yqr-tCAS debug-s
                //   if (Profile == XMP_PROFILE1)
                //   {
                //       IoWrite8(0x72, 0xE8);
                //       IoWrite8(0x73, (UINT8)tCKmin);
                //     //   IoWrite16(0x72, 0xC0);
                //     //   IoWrite16(0x73, (UINT16)Calculated);
                //   }
                  //yqr-tCAS debug-e
                  break;
                case MRC_DDR_TYPE_DDR5:
                  Calculated  = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Index].tRASmin.Bits.tRASmin, tCKmin);
                  break;
                default:
                  break;
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRAS > 0) {
                Calculated = DimmIn->Timing.tRAS;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                  Calculated  = PicoSecondsToClocks (Spd->Ddr5.Base.tRASmin.Bits.tRASmin, tCKmin);
                } else {
                  TimingMTB = ((UINT32) (Spd->Ddr4.Base.tRASMintRCMinUpper.Bits.tRASminUpper) << 8) | (UINT32) (Spd->Ddr4.Base.tRASmin.Bits.tRASmin);
                  if (Outputs->Lpddr) {
                    Calculated = DIVIDECEIL ((42000000 - (tCKmin / 100)), tCKmin); // 42ns
                  } else {
                    Calculated = ((MediumTimebase * TimingMTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                  }
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tRAS = (UINT16) Actual[Profile];
        // yqr-tCAS debug-s

        // if (Profile==XMP_PROFILE1)
        // {
        //     ChannelOut->Timing[Profile].tRAS=39;
        // }
        // yqr-tCAS debug-e
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRC timing value for the given memory frequency.
  SKL MC doesn't have a register for tRC, it uses (tRAS + tRP).
  Print the tRC values for each profile and issue a warning if tRC > (tRAS + tRP).

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtRC (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                TimingMTB;
  INT32                 TimingFTB;
  INT32                 MediumTimebase;
  INT32                 FineTimebase;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;

  const SPD_EXTREME_MEMORY_PROFILE_DATA_2_0  *Data;
  UINT32                                     Index;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRCString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }

    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          FineTimebase   = TimeBase->Ftb;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (!XmpSupport(DimmOut, Profile)) {
                break;
              }
              Index       = Profile - XMP_PROFILE1;
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  Data        = &Spd->Ddr4.EndUser.Xmp.Data[Index];
                  TimingMTB   = ((UINT32) (Data->tRASMintRCMinUpper.Bits.tRCminUpper) << 8) | (UINT32) (Data->tRCmin.Bits.tRCmin);
                  TimingFTB   = Data->tRCminFine.Bits.tRCminFine;
                  Calculated  = (tCKmin == 0) ? 0 : ((MediumTimebase * TimingMTB) + (FineTimebase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                  break;
                case MRC_DDR_TYPE_DDR5:
                  Calculated  = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Index].tRCmin.Bits.tRCmin, tCKmin);
                  break;
                default:
                  break;
              }
              break;
            case CUSTOM_PROFILE1:
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                  Calculated  = PicoSecondsToClocks (Spd->Ddr5.Base.tRCmin.Bits.tRCmin, tCKmin);
                } else {
                  TimingMTB = ((UINT32) (Spd->Ddr4.Base.tRASMintRCMinUpper.Bits.tRCminUpper) << 8) | (UINT32) (Spd->Ddr4.Base.tRCmin.Bits.tRCmin);
                  TimingFTB = Spd->Ddr4.Base.tRCminFine.Bits.tRCminFine;
                  if (Outputs->Lpddr) {
                    TimingMTB = (UINT32) (Spd->Lpddr.Base.tRPpb.Bits.tRPpb);
                    TimingFTB = Spd->Lpddr.Base.tRPpbFine.Bits.tRPpbFine;
                    Calculated = DIVIDECEIL (((MediumTimebase * TimingMTB) + (FineTimebase * TimingFTB) + 42000000 - (tCKmin / 100)), tCKmin); // tRC = tRAS + tRPpb (tRAS is 42ns)
                  } else {
                    Calculated = ((MediumTimebase * TimingMTB) + (FineTimebase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                  }
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        if (ChannelOut->Timing[Profile].tRAS + ChannelOut->Timing[Profile].tRCDtRP < (UINT16) Actual[Profile]) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "\nWARNING: tRC is bigger than tRAS+tRP !\n");
        }
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRCD timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtRCD (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                TimingMTB;
  INT32                 TimingFTB;
  INT32                 MediumTimebase;
  INT32                 FineTimebase;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;

  const SPD_EXTREME_MEMORY_PROFILE_DATA_2_0  *Data;
  UINT32                                      Index;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRCDString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }

          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          FineTimebase   = TimeBase->Ftb;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (!XmpSupport(DimmOut, Profile)) {
                break;
              }
              Index       = Profile - XMP_PROFILE1;
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  Data        = &Spd->Ddr4.EndUser.Xmp.Data[Index];
                  TimingMTB   = Data->tRCDmin.Bits.tRCDmin;
                  TimingFTB   = Data->tRCDminFine.Bits.tRCDminFine;
                  Calculated  = (tCKmin == 0) ? 0 : ((MediumTimebase * TimingMTB) + (FineTimebase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                  break;
                case MRC_DDR_TYPE_DDR5:
                  Calculated  = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Index].tRCDmin.Bits.tRCDmin, tCKmin);
                  break;
                default:
                  break;
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRCDtRP > 0) {
                Calculated = DimmIn->Timing.tRCDtRP;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                  Calculated  = PicoSecondsToClocks (Spd->Ddr5.Base.tRCDmin.Bits.tRCDmin, tCKmin);
                } else {
                  if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
                    TimingMTB = Spd->Ddr4.Base.tRCDmin.Bits.tRCDmin;
                    TimingFTB = Spd->Ddr4.Base.tRCDminFine.Bits.tRCDminFine;
                  } else {
                    TimingMTB = Spd->Lpddr.Base.tRCDmin.Bits.tRCDmin;
                    TimingFTB = Spd->Lpddr.Base.tRCDminFine.Bits.tRCDminFine;
                  }
                  Calculated = ((MediumTimebase * TimingMTB) + (FineTimebase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          //yqr_debug
          if(Profile==XMP_PROFILE1){
            IoWrite32(0x72, 0x84);
            IoWrite32(0x73, (UINT16)tCKmin);
          }

          //yqr_debug
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tRCDtRP = (UINT16) Actual[Profile];
        // yqr-debug-s
        IoWrite16(0x72, 0x92+Profile*2);
        IoWrite16(0x73, (UINT16)Actual[Profile]);
        // yqr-debug-e
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tREFI timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtREFI (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                TimingMTB;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tREFIString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT == DimmOut->Status) {
            Calculated     = 0;
            tCKmin         = ChannelOut->Timing[Profile].tCK;
            switch (Profile) {
              case XMP_PROFILE1:
              case XMP_PROFILE2:
              case XMP_PROFILE3:
              case USER_PROFILE4:
              case USER_PROFILE5:
                Calculated = 0;
                if (!XmpSupport(DimmOut, Profile)) {
                  break;
                }
                TimingMTB = 0;
                switch (DimmOut->DdrType) {
                  case MRC_DDR_TYPE_DDR4:
                    TimingMTB = TREFIMIN_DDR4;
                    break;
                  case MRC_DDR_TYPE_DDR5:
                    TimingMTB = (Inputs->FineGranularityRefresh) ? TREFIMIN_DDR5 : TREFIMIN_DDR5 * 2; // Fine Granularity Refresh mode uses tREFI2
                    break;
                  default:
                    break;
                }
                Calculated = (tCKmin == 0) ? 0 : ((TimingMTB + ((tCKmin / 1000) - 1)) / (tCKmin / 1000));
                break;
              case CUSTOM_PROFILE1:
                if (DimmIn->Timing.tREFI > 0) {
                  Calculated = DimmIn->Timing.tREFI;
                  break;
                } else {
                  // In AUTO mode, so no break.
                }
                /*FALLTHROUGH*/
              case STD_PROFILE:
              default:
                if (tCKmin > 0) {
                  switch (DimmOut->DdrType)
                  {
                    default:
                    case MRC_DDR_TYPE_DDR5:
                      if (Inputs->FineGranularityRefresh) {
                        // Fine Granularity Refresh mode uses tREFI2
                        TimingMTB = TREFIMIN_DDR5;
                      } else {
                        TimingMTB = TREFIMIN_DDR5 * 2;
                      }
                      break;
                    case MRC_DDR_TYPE_DDR4:
                      TimingMTB = TREFIMIN_DDR4;
                      break;
                    case MRC_DDR_TYPE_LPDDR4:
                    case MRC_DDR_TYPE_LPDDR5:
                      TimingMTB = TREFIMIN_LPDDR;
                      break;
                  }
                  if (DimmOut->DdrType == MRC_DDR_TYPE_DDR4) {
                    // make sure not overflow
                    Calculated = (((TimingMTB * 100) + ((tCKmin / 10) - 100)) / (tCKmin / 10));
                  } else {
                    // Scaled formula factor to 1000 to better accuracy values
                    Calculated = (((TimingMTB * 1000) + (tCKmin - 1000)) / tCKmin);
                  }
                }
                break;
            } //switch

            Actual[Profile] = MAX (Actual[Profile], Calculated);
            MRC_DEBUG_MSG (
              Debug,
              MSG_LEVEL_NOTE,
              "  % 7u % 10u % 8u % 5u %6u\n",
              Profile,
              Controller,
              Channel,
              Dimm,
              Calculated
            );
          } //DimmOut->Status
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tREFI = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRFC timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtRFC (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                TimingMTB;
  INT32                 MediumTimebase;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                Index;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRFCString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }

          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (!XmpSupport(DimmOut, Profile)) {
                break;
              }
              Index = Profile - XMP_PROFILE1;
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  TimingMTB = Spd->Ddr4.EndUser.Xmp.Data[Index].tRFC1min.Bits.tRFCmin;
                  Calculated = (tCKmin == 0) ? 0 : ((MediumTimebase * TimingMTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                  break;
                case MRC_DDR_TYPE_DDR5:
                  Calculated = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Index].tRFC1min.Bits.tRFC1min * 1000, tCKmin);
                  break;
                default:
                  break;
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRFC > 0) {
                Calculated = DimmIn->Timing.tRFC;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                  // DDR5 SPD tRFC1min is in units of nanoseconds
                  Calculated = PicoSecondsToClocks (Spd->Ddr5.Base.tRFC1min.Bits.tRFC1min * 1000, tCKmin);
                } else {
                  if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
                    TimingMTB = Spd->Ddr4.Base.tRFC1min.Bits.tRFCmin;
                  } else {
                    TimingMTB = Spd->Lpddr.Base.tRFCab.Bits.tRFCab;
                  }
                  Calculated = ((MediumTimebase * TimingMTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tRFC = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRFCpb timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtRFCpb (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                TimingMTB;
  INT32                 MediumTimebase;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                Index;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRFCpbString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }

          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          Index = Profile - XMP_PROFILE1;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                Calculated = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Index].tRFCsbmin.Bits.tRFCsbmin * 1000, tCKmin);
              } else {
                Calculated = 0;
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRFCpb > 0) {
                Calculated = DimmIn->Timing.tRFCpb;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (Outputs->Lpddr) {
                  TimingMTB = Spd->Lpddr.Base.tRFCpb.Bits.tRFCpb;
                  Calculated = ((MediumTimebase * TimingMTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                } else if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                  // DDR5 SPD tRFCsbmin is in units of nanoseconds
                  Calculated = PicoSecondsToClocks (Spd->Ddr5.Base.tRFCsbmin.Bits.tRFCsbmin * 1000, tCKmin);
                } else {
                  // This timing does not exist in DDR4.
                  Calculated = 0;
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
              Debug,
              MSG_LEVEL_NOTE,
              "  % 7u % 10u % 8u % 5u %6u\n",
              Profile,
              Controller,
              Channel,
              Dimm,
              Calculated
            );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tRFCpb = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRFC2 timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtRFC2 (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                TimingMTB;
  INT32                 MediumTimebase;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                Index;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRFC2String, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }

          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport(DimmOut, Profile)) {
                break;
              }
              Index = Profile - XMP_PROFILE1;
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  TimingMTB = Spd->Ddr4.EndUser.Xmp.Data[Index].tRFC2min.Bits.tRFCmin;
                  Calculated = (tCKmin == 0) ? 0 : ((MediumTimebase * TimingMTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                  break;
                case MRC_DDR_TYPE_DDR5:
                  // DDR5 SPD tRFC2min is in units of nanoseconds
                  Calculated  = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Index].tRFC2min.Bits.tRFC2min * 1000, tCKmin);
                  break;
                default:
                  break;
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRFC2 > 0) {
                Calculated = DimmIn->Timing.tRFC2;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
                TimingMTB  = Spd->Ddr4.Base.tRFC2min.Bits.tRFCmin;
                Calculated = (tCKmin == 0) ? 0 : ((MediumTimebase * TimingMTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
              } else if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                // DDR5 SPD tRFC2min is in units of nanoseconds
                Calculated  = PicoSecondsToClocks (Spd->Ddr5.Base.tRFC2min.Bits.tRFC2min * 1000, tCKmin);
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tRFC2 = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRFC4 timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtRFC4 (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                TimingMTB;
  INT32                 MediumTimebase;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRFC4String, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport(DimmOut, Profile)) {
                break;
              }
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  TimingMTB = Spd->Ddr4.EndUser.Xmp.Data[Profile - XMP_PROFILE1].tRFC4min.Bits.tRFCmin;
                  Calculated = (tCKmin == 0) ? 0 : ((MediumTimebase * TimingMTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                  break;
                case MRC_DDR_TYPE_DDR5:
                  // No tRFC4 in DDR5
                default:
                  break;
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRFC4 > 0) {
                Calculated = DimmIn->Timing.tRFC4;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
                TimingMTB  = Spd->Ddr4.Base.tRFC4min.Bits.tRFCmin;
                Calculated = (tCKmin == 0) ? 0 : ((MediumTimebase * TimingMTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tRFC4 = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRP timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtRP (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  INT32                 MediumTimebase;
  INT32                 FineTimebase;
  UINT32                TimingMTB;
  INT32                 TimingFTB;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;

  const SPD_EXTREME_MEMORY_PROFILE_DATA_2_0  *Data;
  UINT32                                      Index;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRPString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }

          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          FineTimebase   = TimeBase->Ftb;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (!XmpSupport(DimmOut, Profile)) {
                continue;
              }
              Index       = Profile - XMP_PROFILE1;
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  Data        = &Spd->Ddr4.EndUser.Xmp.Data[Index];
                  TimingMTB   = Data->tRPmin.Bits.tRPmin;
                  TimingFTB   = Data->tRPminFine.Bits.tRPminFine;
                  Calculated  = (tCKmin == 0) ? 0 : ((MediumTimebase * TimingMTB) + (FineTimebase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                  break;
                case MRC_DDR_TYPE_DDR5:
                  Calculated  = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Index].tRPmin.Bits.tRPmin, tCKmin);
                  break;
                default:
                  break;
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRCDtRP > 0) {
                Calculated = DimmIn->Timing.tRCDtRP;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                  Calculated  = PicoSecondsToClocks (Spd->Ddr5.Base.tRPmin.Bits.tRPmin, tCKmin);
                } else {
                  if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
                    TimingMTB = Spd->Ddr4.Base.tRPmin.Bits.tRPmin;
                    TimingFTB = Spd->Ddr4.Base.tRPminFine.Bits.tRPminFine;
                  } else {
                    TimingMTB = Spd->Lpddr.Base.tRPpb.Bits.tRPpb;
                    TimingFTB = Spd->Lpddr.Base.tRPpbFine.Bits.tRPpbFine;
                  }
                  Calculated = ((MediumTimebase * TimingMTB) + (FineTimebase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          //
          // Take MAX of tRCD and tRP if they are different in SPD.
          // This assumes that GetChannelDimmtRCD() was already called.
          //
          Actual[Profile] = MAX (Actual[Profile], ChannelOut->Timing[Profile].tRCDtRP);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tRCDtRP = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRPab timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtRPab (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  BOOLEAN               Flag;
  BOOLEAN               Lpddr;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  INT32                 MediumTimebase;
  INT32                 FineTimebase;
  UINT32                TimingMTB;
  INT32                 TimingFTB;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  Flag    = FALSE;

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          Lpddr   = Outputs->Lpddr;
          if ((DIMM_PRESENT == DimmOut->Status) && Lpddr) {
            Spd            = &DimmIn->Spd.Data;
            Calculated     = 0;
            tCKmin         = ChannelOut->Timing[Profile].tCK;
            TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
            MediumTimebase = TimeBase->Mtb;
            FineTimebase   = TimeBase->Ftb;
            if (tCKmin > 0) {
              TimingMTB  = Spd->Lpddr.Base.tRPab.Bits.tRPab;
              TimingFTB  = Spd->Lpddr.Base.tRPabFine.Bits.tRPabFine;
              Calculated = ((MediumTimebase * TimingMTB) + (FineTimebase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
            }

            if (Calculated >= TRPABMINPOSSIBLE) {
              Actual[Profile] = MAX (Actual[Profile], Calculated);
            }
            if (!Flag) {
              Flag = TRUE;
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRPabString, HeaderString);
              MRC_DEBUG_MSG (
                Debug,
                MSG_LEVEL_NOTE,
                "  % 7u % 10u % 8u % 5u %6u\n",
                Profile,
                Controller,
                Channel,
                Dimm,
                Calculated
                );
            } //Flag
          } //DimmOut->Status
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  if (Flag) {
    //
    // Set the best case timing for all controllers/channels/dimms, for each profile.
    //
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
    for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
      if (NeedIgnoreXmp(MrcData, Profile)) {
        continue;
      }
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        ControllerOut = &Outputs->Controller[Controller];
        for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
          ChannelOut = &ControllerOut->Channel[Channel];
          ChannelOut->Timing[Profile].tRPab = (UINT16) Actual[Profile];
        }
      }
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
  }

  return TRUE;

}

/**
  Calculate the minimum tRRD timing value for the given memory frequency.
    MRC should not set tRRD below 4nCK for all frequencies.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE
**/
static
BOOLEAN
GetChannelDimmtRRD (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                MinCk;
  BOOLEAN               Lpddr;
  MRC_LP5_BANKORG       Lp5BGOrg;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  Lpddr   = Outputs->Lpddr;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRRDString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport(DimmOut, Profile)) {
                break;
              }
              Calculated = 0; // tRRD isn't used for DDR4 or DDR5
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRRD > 0) {
                Calculated = DimmIn->Timing.tRRD;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (!Lpddr) {
                  Calculated = 0; // tRRD isn't used for DDR4 or DDR5
                } else {
                  if ((Outputs->DdrType == MRC_DDR_TYPE_LPDDR4) && (Outputs->Frequency >= f4267)) {
                    // LPDDR4 tRRD spec max(TimeBase, 4nCK)
                    // TimeBase is (f4267) ? 7.5ns : 10ns
                    Calculated = 7500000;  // 7.5ns
                  } else { // LP4 below 4267 or LP5
                    Calculated = 10000000; // 10ns
                    Lp5BGOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);
                    if (Outputs->DdrType == MRC_DDR_TYPE_LPDDR5) {
                      if (Lp5BGOrg != MrcLp58Bank) {
                        if (Outputs->LpX) {
                          Calculated = 3750000; // 3.75ns
                        } else {
                          Calculated = 5000000; // 5ns
                        }
                      }
                    }
                  }
                  Calculated = DIVIDECEIL ((Calculated - (tCKmin / 100)), tCKmin); // Convert to tCK
                }
              }
              if (Lpddr) {
                if (Outputs->DdrType == MRC_DDR_TYPE_LPDDR4) {
                  MinCk = 4;
                } else {
                  // LPDDR5
                  MinCk = 2;
                }
                Calculated = MAX (Calculated, MinCk);
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tRRD = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRRD same bank group timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtRRD_L (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                TimingMTB;
  INT32                 TimingFTB;
  INT32                 MediumTimebase;
  INT32                 FineTimebase;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                Index;

  const SPD_EXTREME_MEMORY_PROFILE_DATA_2_0  *Data;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRRDLString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }

          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          FineTimebase   = TimeBase->Ftb;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport(DimmOut, Profile)) {
                break;
              }
              Index = Profile - XMP_PROFILE1;
              Calculated = 0;
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  Data        = &Spd->Ddr4.EndUser.Xmp.Data[Index];
                  TimingMTB   = Data->tRRD_Lmin.Bits.tRRDmin;
                  TimingFTB   = Data->tRRD_LminFine.Bits.tRRDminFine;
                  Calculated  = (tCKmin == 0) ? 0 : ((MediumTimebase * TimingMTB) + (FineTimebase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                  break;
                case MRC_DDR_TYPE_DDR5:
                  // DDR5 tRRD_L = MAX (8nCK, 5ns)
                  Calculated = PicoSecondsToClocks (MRC_TRRD_L_MIN_DDR5, tCKmin);
                  Calculated = MAX (8, Calculated);
                  break;
                default:
                  break;
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRRD_L > 0) {
                Calculated = DimmIn->Timing.tRRD_L;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                // DDR5 tRRD_L = MAX (8nCK, 5ns)
                Calculated = PicoSecondsToClocks (MRC_TRRD_L_MIN_DDR5, tCKmin);
                Calculated = MAX (8, Calculated);
              } else if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
                TimingMTB = Spd->Ddr4.Base.tRRD_Lmin.Bits.tRRDmin;
                TimingFTB = Spd->Ddr4.Base.tRRD_LminFine.Bits.tRRDminFine;
                Calculated = (tCKmin == 0) ? 0 : ((MediumTimebase * TimingMTB) + (FineTimebase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tRRD_L = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRRD different bank group timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtRRD_S (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                TimingMTB;
  INT32                 TimingFTB;
  INT32                 MediumTimebase;
  INT32                 FineTimebase;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                Index;

  const SPD_EXTREME_MEMORY_PROFILE_DATA_2_0  *Data;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRRDSString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }

          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          FineTimebase   = TimeBase->Ftb;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport(DimmOut, Profile)) {
                break;
              }

              Calculated = 0;
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  Index = Profile - XMP_PROFILE1;
                  Data        = &Spd->Ddr4.EndUser.Xmp.Data[Index];
                  TimingMTB   = Data->tRRD_Smin.Bits.tRRDmin;
                  TimingFTB   = Data->tRRD_SminFine.Bits.tRRDminFine;
                  Calculated  = (tCKmin == 0) ? 0 : ((MediumTimebase * TimingMTB) + (FineTimebase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                  break;
                case MRC_DDR_TYPE_DDR5:
                  // DDR5 tRRD_S = 8nCK
                  Calculated  = 8;
                  break;
                default:
                  break;
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRRD_S > 0) {
                Calculated = DimmIn->Timing.tRRD_S;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                // DDR5 tRRD_S = 8nCK
                Calculated  = 8;
              } else if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
                TimingMTB = (UINT32) Spd->Ddr4.Base.tRRD_Smin.Bits.tRRDmin;
                TimingFTB = (INT32) Spd->Ddr4.Base.tRRD_SminFine.Bits.tRRDminFine;
                Calculated = (tCKmin == 0) ? 0 : ((MediumTimebase * TimingMTB) + (FineTimebase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tRRD_S = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the tRTP value for DDR5.
  JEDEC Spec: tRTP = 7.5ns

    @param[in] tCK - the memory DCLK in femtoseconds.

    @retval DDR5 tRTP in DCLK units
**/
static
UINT32
GetDdr5tRTP (
  IN const UINT32 tCK
  )
{
  UINT32 tRTP;

  tRTP = (tCK == 0) ? 0 : PicoSecondsToClocks (7500, tCK); // 7.5ns
  if ((tRTP == 13) || (tRTP == 16) || (tRTP == 19) || (tRTP == 22)) {
    // 13, 16, 19, 22 are not valid values, use the next one
    tRTP++;
  }

  // Must be in the range JEDEC allows
  tRTP = RANGE(tRTP, 12, 24);

  return tRTP;
}

/**
  Calculate the tRTP value for DDR4.
  JEDEC Spec: tRTP = max (4nCK, 7.5ns)

    @param[in] tCK - the memory DCLK in femtoseconds.

    @retval DDR4 tRTP in DCLK units
**/
static
UINT32
GetDdr4tRTP (
  IN const UINT32 tCK
  )
{
  UINT32 tRTP;

  tRTP = (tCK == 0) ? 0 : DIVIDECEIL ((7500000 - (tCK / 100)), tCK); // 7.5ns
  tRTP = RANGE (tRTP, DDR4_TRTPMINPOSSIBLE, DDR4_TRTPMAXPOSSIBLE);   // JEDEC limits
  if (tRTP == 11) {
    tRTP++;         // 11 is not a valid value, use 12
  }
  return tRTP;
}

/**
  Calculate the tWR value for DDR4.
  JEDEC Spec: tWR = 15ns

    @param[in] tCK - the memory DCLK in femtoseconds.

    @retval DDR4 tWR in DCLK units
**/
static
UINT32
GetDdr4tWR (
  IN const UINT32 tCK
  )
{
  UINT32 tWR;

  tWR = (tCK == 0) ? 0 : DIVIDECEIL ((15000000 - (tCK / 100)), tCK); // 15ns
  if (tWR % 2) {
    tWR++;          // Calculated tWR is odd number, round up to next even number
  }
  tWR = RANGE (tWR, DDR4_TWRMINPOSSIBLE, DDR4_TWRMAXPOSSIBLE);  // JEDEC limits
  if (tWR == 22) {
    tWR = 24;       // tWR of 22 is not a valid value, use the next one
  }
  return tWR;
}

/**
  Calculate the tWR value for DDR5.

    @param[in] tCK   - the memory DCLK in femtoseconds.
          [in] WRmin - SPD tWRmin value

    @retval DDR4 tWR in DCLK units
**/
static
UINT32
GetDdr5tWR (
  IN const UINT32 tCK,
  IN const UINT32 tWRmin
  )
{
  UINT32 tWR;

  tWR = PicoSecondsToClocks (tWRmin, tCK);
  tWR = RANGE (tWR, DDR5_TWRMINPOSSIBLE, DDR5_TWRMAXPOSSIBLE);  // JEDEC limits
  tWR = DIVIDECEIL(tWR - DDR5_TWRMINPOSSIBLE, 6) * 6 + DDR5_TWRMINPOSSIBLE; // Step size is 6 CK
  return tWR;
}

/**
  Calculate the tCCD_L value for DDR4.
  JEDEC Spec:
   1333 and below:  4 nCK
   1600:            max(5 nCK, 6.250 ns)
   1867:            max(5 nCK, 5.355 ns)
   2133:            max(5 nCK, 5.355 ns)
   2400 and above:  max(5 nCK, 5 ns)

    @param[in] tCK - the memory DCLK in femtoseconds.

    @retval DDR4 tCCD_L in DCLK units
**/
static
UINT32
GetDdr4tCCDL (
  IN const UINT32 tCK
  )
{
  UINT32 tCCD_L;

  if (tCK >= MRC_DDR_1333_TCK_MIN) {
    tCCD_L = tCCD_L_1333_AND_LOWER;
  } else if (tCK >= MRC_DDR_1600_TCK_MIN) {
    tCCD_L = DIVIDECEIL (tCCD_L_1600_FS - (tCK / 100), tCK);      // 6.250 ns
  } else if (tCK >= MRC_DDR_2133_TCK_MIN) {
    tCCD_L = DIVIDECEIL (tCCD_L_1867_2133_FS - (tCK / 100), tCK); // 5.355 ns
  } else {
    tCCD_L = DIVIDECEIL (tCCD_L_2400_FS - (tCK / 100), tCK);      // 5 ns
  }

  if (tCK < MRC_DDR_1333_TCK_MIN) {
    tCCD_L = RANGE (tCCD_L, 5, DDR4_TCCDLMAXPOSSIBLE);
  }

  return tCCD_L;
}

/**
  Calculate the tWR value for LPDDR4
  JEDEC Spec: 18ns for x16 and 20ns for ByteMode parts

    @param[in] tCK              - The memory DCLK in femtoseconds.
    @param[in] SdramWidthIndex  - 1 for X8 and 2 for X16 SDRAM width

    @retval tWR in DCLK units
**/
static
UINT32
GetLpddr4tWR (
  IN const UINT32     tCK,
  IN UINT8            SdramWidthIndex
  )
{
  UINT32  tWR;
  UINT32  LpCoreTiming;
  BOOLEAN ByteMode;

  tWR = 0;
  ByteMode = (SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8);
  LpCoreTiming = (ByteMode) ? 20000000 : 18000000;

  if (tCK > 0) {
    tWR = DIVIDECEIL ((LpCoreTiming - (tCK / 100)), tCK);
    if (ByteMode) {
      if (tWR <= 6) {
        tWR = 6;
      } else if (tWR <= 12) {
        tWR = 12;
      } else if (tWR <= 16) {
        tWR = 16;
      } else if (tWR <= 22) {
        tWR = 22;
      } else if (tWR <= 28) {
        tWR = 28;
      } else if (tWR <= 32) {
        tWR = 32;
      } else if (tWR <= 38) {
        tWR = 38;
      } else {
        tWR = 44;
      }
    } else {
      if (tWR <= 6) {
        tWR = 6;
      } else if (tWR <= 10) {
        tWR = 10;
      } else if (tWR <= 16) {
        tWR = 16;
      } else if (tWR <= 20) {
        tWR = 20;
      } else if (tWR <= 24) {
        tWR = 24;
      } else if (tWR <= 30) {
        tWR = 30;
      } else if (tWR <= 34) {
        tWR = 34;
      } else {
        tWR = 40;
      }
    }
  } // tCK > 0

  return tWR;
}

/**
  Calculate the tWR value for LPDDR5 per JEDEC spec (WRITE Recovery Time)
  x16: tWRMin = max(34ns, 3nCK)
  x8 : tWRMin = max(36ns, 3nCK)

  @param[in] tCK              - the memory DCLK in femtoseconds.
  @param[in] SdramWidthIndex  - 1 for X8 and 2 for X16 SDRAM width

  @retval tWR in tCK units
**/
static
UINT32
GetLpddr5tWR (
  IN const UINT32     tCK,
  IN UINT8            SdramWidthIndex
  )
{
  UINT32  tWR;

  tWR = 0;
  if (tCK > 0) {
    if (SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8) {
      tWR = DIVIDECEIL ((36000000 - (tCK / 100)), tCK); // 36ns
      if (tWR <= 3) {
        tWR = 3;
      } else if (tWR <= 5) {
        tWR = 5;
      } else if (tWR <= 8) {
        tWR = 8;
      } else if (tWR <= 10) {
        tWR = 10;
      } else if (tWR <= 13) {
        tWR = 13;
      } else if (tWR <= 15) {
        tWR = 15;
      } else if (tWR <= 17) {
        tWR = 17;
      } else if (tWR <= 20) {
        tWR = 20;
      } else if (tWR <= 22) {
        tWR = 22;
      } else if (tWR <= 25) {
        tWR = 25;
      } else if (tWR <= 28) {
        tWR = 28;
      } else if (tWR <= 29) {
        tWR = 29;
      } else if (tWR <= 34) {
        tWR = 34;
      } else {
        tWR = 39;
      }
    } else { // x16
      tWR = DIVIDECEIL ((34000000 - (tCK / 100)), tCK); // 34ns
      if (tWR <= 3) {
        tWR = 3;
      } else if (tWR <= 5) {
        tWR = 5;
      } else if (tWR <= 7) {
        tWR = 7;
      } else if (tWR <= 10) {
        tWR = 10;
      } else if (tWR <= 12) {
        tWR = 12;
      } else if (tWR <= 14) {
        tWR = 14;
      } else if (tWR <= 16) {
        tWR = 16;
      } else if (tWR <= 19) {
        tWR = 19;
      } else if (tWR <= 21) {
        tWR = 21;
      } else if (tWR <= 24) {
        tWR = 24;
      } else if (tWR <= 26) {
        tWR = 26;
      } else if (tWR <= 28) {
        tWR = 28;
      } else if (tWR <= 32) {
        tWR = 32;
      } else {
        tWR = 37;
      }
    }
  } // tCK > 0
  return tWR;
}

/**
  Calculate the minimum tRTP timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtRTP (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  Debug       = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRTPString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (!XmpSupport(DimmOut, Profile)) {
                break;
              }
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  Calculated = GetDdr4tRTP (tCKmin);
                  break;
                case MRC_DDR_TYPE_DDR5:
                  Calculated = GetDdr5tRTP (tCKmin);
                  break;
                default:
                  break;
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRTP > 0) {
                Calculated = DimmIn->Timing.tRTP;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (DimmOut->DdrType == MRC_DDR_TYPE_DDR5) {
                  Calculated = GetDdr5tRTP (tCKmin);
                } else if (DimmOut->DdrType == MRC_DDR_TYPE_DDR4) {
                  Calculated = GetDdr4tRTP (tCKmin);
                } else {
                  Calculated = DIVIDECEIL ((7500000 - (tCKmin / 100)), tCKmin); // 7.5ns
                  if (DimmOut->DdrType == MRC_DDR_TYPE_LPDDR4) {
                    if (Calculated < LPDDR4_TRTPMINPOSSIBLE) {
                      Calculated = LPDDR4_TRTPMINPOSSIBLE;
                    }
                  } else if (DimmOut->DdrType == MRC_DDR_TYPE_LPDDR5) {
                    if (Calculated > 2) {
                      Calculated -= 2;
                    } else {
                      Calculated = 0;
                    }
                  }
                }
              }
              break;
          } //switch
          if (DimmOut->DdrType == MRC_DDR_TYPE_DDR5) {
            Calculated = MAX (Calculated, 12);          // Minimum tRTP is 12 clocks
          }

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tRTP = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tWR timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtWR (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT8                 SdramWidthIndex;

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  Debug       = &Outputs->Debug;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tWRString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }

          Spd = &DimmIn->Spd.Data;
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport(DimmOut, Profile)) {
                break;
              }
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  Calculated = GetDdr4tWR (tCKmin);
                  break;
                case MRC_DDR_TYPE_DDR5:
                  Calculated = GetDdr5tWR (tCKmin, Spd->Ddr5.EndUser.Xmp.Data[Profile - XMP_PROFILE1].tWRmin.Bits.tWRmin);
                  break;
                default:
                  break;
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tWR > 0) {
                Calculated = DimmIn->Timing.tWR;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (DimmOut->DdrType == MRC_DDR_TYPE_DDR5) {
                  Calculated = GetDdr5tWR (tCKmin, Spd->Ddr5.Base.tWRmin.Bits.tWRmin);
                } else if (DimmOut->DdrType == MRC_DDR_TYPE_DDR4) {
                  Calculated = GetDdr4tWR (tCKmin);
                } else { // LPDDR
                  SdramWidthIndex = Spd->Lpddr.Base.ModuleOrganization.Bits.SdramDeviceWidth;
                  if (DimmOut->DdrType == MRC_DDR_TYPE_LPDDR4) {
                    Calculated = GetLpddr4tWR (tCKmin, SdramWidthIndex);
                  } else {
                    // LPDDR5
                    Calculated = GetLpddr5tWR (tCKmin, SdramWidthIndex);
                  }
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tWR = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tWTR timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtWTR (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *SpdIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                LpCoreAcTiming;
  UINT8                 SdramWidthIndex;
  BOOLEAN               Lpddr;
  BOOLEAN               Lpddr5;
  MRC_LP5_BANKORG       Lp5BGOrg;
  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tWTRString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }

          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tWTR > 0) {
                Calculated = DimmIn->Timing.tWTR;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                Lpddr5 = (DimmOut->DdrType == MRC_DDR_TYPE_LPDDR5);
                Lpddr  = Outputs->Lpddr;
                if (Lpddr) {
                  SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data;
                  SdramWidthIndex = SpdIn->Lpddr.Base.ModuleOrganization.Bits.SdramDeviceWidth;
                  // 12ns for LP4 ByteMode and LP5, 10ns for LP4 x16
                  LpCoreAcTiming = (Lpddr5 || (SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8)) ? 12000000 : 10000000;

                  if (Lpddr5) {
                    Lp5BGOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);
                    if (Lp5BGOrg != MrcLp5BgMode) {
                      // 14ns for LP5 ByteMode, 12ns for LP5 x16
                      LpCoreAcTiming = (SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8) ? 14000000 : 12000000;
                    } else {
                      // 12ns for LP5 ByteMode, 10ns for LP5 x16
                      LpCoreAcTiming = (SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8) ? 12000000 : 10000000;
                    }
                  }

                  Calculated = DIVIDECEIL ((LpCoreAcTiming - (tCKmin / 100)), tCKmin);
                  //Check max 8nCK for LP4, 4nCK for LP5
                  LpCoreAcTiming = (Lpddr5) ? 4 : 8;
                  Calculated = MAX (Calculated, LpCoreAcTiming);
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tWTR = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the tWTR_L timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtWTR_L (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcSpd          *SpdIn;
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT8                 SdramWidthIndex;
  UINT32                LpCoreAcTiming;
  MRC_LP5_BANKORG       Lp5BGOrg;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tWTRLString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport(DimmOut, Profile)) {
                break;
              }
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  Calculated = (tCKmin == 0) ? 0 : (7500000 - (tCKmin / 100) + tCKmin - 1) / tCKmin; // 7.5ns
                  break;
                case MRC_DDR_TYPE_DDR5:
                  // max(16nCK, 10ns)
                  Calculated = (tCKmin == 0) ? 0 : (10000000 - (tCKmin / 100) + tCKmin - 1) / tCKmin; // 10ns
                  Calculated = MAX(16, Calculated);
                  break;
                default:
                  break;
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tWTR_L > 0) {
                Calculated = DimmIn->Timing.tWTR_L;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
                Calculated = (tCKmin == 0) ? 0 : (7500000 - (tCKmin / 100) + tCKmin - 1) / tCKmin; // 7.5ns
              } else if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                // max(16nCK, 10ns)
                Calculated = (tCKmin == 0) ? 0 : (10000000 - (tCKmin / 100) + tCKmin - 1) / tCKmin; // 10ns
                Calculated = MAX(16, Calculated);
              } else if (Outputs->DdrType == MRC_DDR_TYPE_LPDDR5) {
                Lp5BGOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);
                if (Lp5BGOrg != MrcLp58Bank) {
                  SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data;
                  SdramWidthIndex = SpdIn->Lpddr.Base.ModuleOrganization.Bits.SdramDeviceWidth;
                  // 14ns for x8 LP5, 12ns for LP5 x16
                  LpCoreAcTiming = (SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8) ? 14000000 : 12000000;
                  //RPL_todo. Add update for LP5x.
                  Calculated = DIVIDECEIL ((LpCoreAcTiming - (tCKmin / 100)), tCKmin);
                  //Check max 8nCK for LP4, 4nCK for LP5
                  LpCoreAcTiming = 4;
                  Calculated = MAX (Calculated, LpCoreAcTiming);
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tWTR_L = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the tWTR_S timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtWTR_S (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcSpd          *SpdIn;
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT8                 SdramWidthIndex;
  UINT32                LpCoreAcTiming;
  MRC_LP5_BANKORG       Lp5BGOrg;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tWTRSString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          switch (Profile) {
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tWTR_S > 0) {
                Calculated = DimmIn->Timing.tWTR_S;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
            case STD_PROFILE:
            default:
              Calculated = (tCKmin == 0) ? 0 : (2500000 - (tCKmin / 100) + tCKmin - 1) / tCKmin; // DDR4: 2.5ns
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                // Max(4nCK, 2.5ns)
                Calculated = MAX (4, Calculated);
              } else if (DimmOut->DdrType == MRC_DDR_TYPE_LPDDR5) {
                Lp5BGOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);
                if (Lp5BGOrg == MrcLp5BgMode) {
                  SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data;
                  SdramWidthIndex = SpdIn->Lpddr.Base.ModuleOrganization.Bits.SdramDeviceWidth;
                  // 8.25ns for LP5 ByteMode and LP5, 6.25ns for LP5 x16
                  LpCoreAcTiming = (SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8) ? 8250000 : 6250000;
                  Calculated = DIVIDECEIL ((LpCoreAcTiming - (tCKmin / 100)), tCKmin);
                  //Check max 4nCK for LP5
                  LpCoreAcTiming = 4;
                  Calculated = MAX (Calculated, LpCoreAcTiming);
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tWTR_S = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the tCCD_L timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmtCCD_L (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                TimingMTB;
  INT32                 TimingFTB;
  INT32                 MediumTimebase;
  INT32                 FineTimebase;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                JedecValue;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tCCDLSString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          FineTimebase   = TimeBase->Ftb;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport(DimmOut, Profile)) {
                break;
              }
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  // DDR4 XMP spec doesn't have tCCD_L, so use JEDEC formulas
                  Calculated = GetDdr4tCCDL (tCKmin);
                  break;
                case MRC_DDR_TYPE_DDR5:
                  // DDR5 tCCD_L = MAX (8nCK, 5ns)
                  Calculated = PicoSecondsToClocks (5000, tCKmin);
                  Calculated = MAX (8, Calculated);
                  break;
                default:
                  break;
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tCCD_L > 0) {
                Calculated = DimmIn->Timing.tCCD_L;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                // DDR5 tCCD_L = MAX (8nCK, 5ns)
                Calculated = PicoSecondsToClocks (5000, tCKmin);
                Calculated = MAX (8, Calculated);
              } else if (MRC_DDR_TYPE_DDR4 == DimmOut->DdrType) {
                TimingMTB = (UINT32) Spd->Ddr4.Base.tCCD_Lmin.Bits.tCCDLmin;
                TimingFTB =  (INT32) Spd->Ddr4.Base.tCCD_LminFine.Bits.tCCDLminFine;
                Calculated = (tCKmin == 0) ? 0 : ((MediumTimebase * TimingMTB) + (FineTimebase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;

                // Avoid JEDEC violation when downclocking (i.e. running a 2400 DIMM at 1600)
                // Allow SPD value to be more relaxed than JEDEC spec, but not more tight
                JedecValue = GetDdr4tCCDL (tCKmin);
                Calculated = MIN(MAX (Calculated, JedecValue), DDR4_TCCDLMAXPOSSIBLE);
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tCCD_L  = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum command rate mode value for the given channel.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmNmode (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", NmodeString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = (Profile < XMP_PROFILE1) ? NMODEMINPOSSIBLE : 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 2;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (XmpSupport(DimmOut, Profile)) {
                Calculated = 2; //change_FSP NMODEMINPOSSIBLE;
                /*
                 * NMode 1 at Gear2 becomes NMODE 2 actually.
                 * Although XMP3 defines SystemCmdRate, we want to
                 * still use NMODEMINPOSSIBLE for it and keep
                 * below codes commented out
                 */
#if 0
                if (DimmOut->DdrType == MRC_DDR_TYPE_DDR5) {
                  Calculated = DimmIn->Spd.Data.Ddr5.EndUser.Xmp.Data[Profile - XMP_PROFILE1].SystemCmdRate.NMode;
                  Calculated = RANGE(Calculated, NMODEMINPOSSIBLE, NMODEMAXPOSSIBLE);
                }
#endif
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.NMode > 0) {
                Calculated = DimmIn->Timing.NMode;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              Calculated = NMODEMINPOSSIBLE;
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].NMode = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the VDD voltage value for the given channel.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmVdd (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput                        *Inputs;
  const MrcControllerIn                 *ControllerIn;
  const MrcChannelIn                    *ChannelIn;
  const MrcDimmIn                       *DimmIn;
  const MrcSpd                          *Spd;
  MrcDebug                              *Debug;
  MrcOutput                             *Outputs;
  MrcControllerOut                      *ControllerOut;
  MrcChannelOut                         *ChannelOut;
  MrcDimmOut                            *DimmOut;
  MrcProfile                            Profile;
  MrcDdrType                            DdrType;
  UINT32                                Actual[MAX_PROFILE];
  UINT32                                Calculated;
  UINT8                                 Controller;
  UINT8                                 Channel;
  UINT8                                 Dimm;

  const SPD_EXTREME_MEMORY_PROFILE_DATA_2_0  *Data;
  const SPD_EXTREME_MEMORY_PROFILE_DATA_3_0  *Data3;
  UINT32                                      Index;

  Inputs   = &MrcData->Inputs;
  Outputs  = &MrcData->Outputs;
  Debug    = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s %s:\n  %s\n", VoltageString, VddString, HeaderString);

  //
  // Find the best case voltage value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = (Profile < XMP_PROFILE1) ? VDD_0_50 : 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          DdrType = DimmOut->DdrType;
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd        = &DimmIn->Spd.Data;
          Calculated = VDD_1_50;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (!XmpSupport(DimmOut, Profile)) {
                break;
              }

              Index = Profile - XMP_PROFILE1;
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  Data        = &Spd->Ddr4.EndUser.Xmp.Data[Index];
                  Calculated  = XMP_VDD_INCREMENT_2 * Data->Vdd.Bits.Decimal;
                  Calculated  = MIN (Calculated, XMP_VDD_INTEGER - 1);
                  Calculated += (XMP_VDD_INTEGER * Data->Vdd.Bits.Integer);
                  Calculated  = MAX (Calculated, XMP_VDD_MIN_POSSIBLE);
                  Calculated  = MIN (Calculated, XMP_VDD_MAX_POSSIBLE);
                  break;
                case MRC_DDR_TYPE_DDR5:
                  Data3       = &Spd->Ddr5.EndUser.Xmp.Data[Index];
                  Calculated  = XMP_VDD_INCREMENT * Data3->Vdd.Bits.Decimal;
                  Calculated  = MIN (Calculated, XMP_VDD_INTEGER - 1);
                  if (Data3->Vdd.Bits.Integer >= 3) {
                    // 3 is reserved value
                    Calculated = VDD_1_10;
                  } else {
                    Calculated += (XMP_VDD_INTEGER * Data3->Vdd.Bits.Integer);
                    Calculated  = MAX (Calculated, XMP3_VDD_MIN_POSSIBLE);
                    Calculated  = MIN (Calculated, XMP3_VDD_MAX_POSSIBLE);
                  }
                  break;
                default:
                  break;
              }
              break;
            case CUSTOM_PROFILE1:
              if (Inputs->VddVoltage > 0) {
                Calculated = Inputs->VddVoltage;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_LPDDR4 == DdrType) {
                if (Outputs->Lp4x) {
                  Calculated = VDD_0_60;
                } else {
                  Calculated = VDD_1_10;
                }
              } else if (MRC_DDR_TYPE_LPDDR5 == DdrType) {
                Calculated = VDD_0_50;
              } else if (MRC_DDR_TYPE_DDR5 == DdrType) {
                if (Inputs->VddVoltage > 0) {
                  Calculated = Inputs->VddVoltage;
                } else {
                  Calculated = VDD_1_10;
                }
              } else {  // DDR4
                Calculated = VDD_1_20;
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %4u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case voltage for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmOut = &ChannelOut->Dimm[Dimm];
          Outputs->VddVoltage[Profile] = (MrcVddSelect) MAX((UINT32)(Outputs->VddVoltage[Profile]), Actual[Profile]);
          DimmOut->VddVoltage[Profile] = (MrcVddSelect) Actual[Profile];
        }
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the VDDQ voltage value for the given channel.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmVddq (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput                        *Inputs;
  const MrcControllerIn                 *ControllerIn;
  const MrcChannelIn                    *ChannelIn;
  const MrcDimmIn                       *DimmIn;
  const MrcSpd                          *Spd;
  MrcDebug                              *Debug;
  MrcOutput                             *Outputs;
  MrcControllerOut                      *ControllerOut;
  MrcChannelOut                         *ChannelOut;
  MrcDimmOut                            *DimmOut;
  MrcProfile                            Profile;
  UINT32                                Actual[MAX_PROFILE];
  UINT32                                Calculated;
  UINT8                                 Controller;
  UINT8                                 Channel;
  UINT8                                 Dimm;

  const SPD_EXTREME_MEMORY_PROFILE_DATA_3_0  *Data3;
  UINT32                                      Index;

  Inputs   = &MrcData->Inputs;
  Outputs  = &MrcData->Outputs;
  Debug    = &Outputs->Debug;

  if (Outputs->DdrType != MRC_DDR_TYPE_DDR5) {
    return TRUE;
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s %s:\n  %s\n", VoltageString, VddqString, HeaderString);

  //
  // Find the best case voltage value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = (Profile < XMP_PROFILE1) ? VDD_0_50 : 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd        = &DimmIn->Spd.Data;
          Calculated = VDD_1_50;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (!XmpSupport(DimmOut, Profile)) {
                break;
              }

              Index = Profile - XMP_PROFILE1;
              Data3       = &Spd->Ddr5.EndUser.Xmp.Data[Index];
              Calculated  = XMP_VDD_INCREMENT * Data3->Vddq.Bits.Decimal;
              Calculated  = MIN (Calculated, XMP_VDD_INTEGER - 1);
              if (Data3->Vddq.Bits.Integer >= 3) {
                // 3 is reserved value
                Calculated = VDD_1_10;
              } else {
                Calculated += (XMP_VDD_INTEGER * Data3->Vddq.Bits.Integer);
                Calculated  = MAX (Calculated, XMP3_VDD_MIN_POSSIBLE);
                Calculated  = MIN (Calculated, XMP3_VDD_MAX_POSSIBLE);
              }
              break;
            case CUSTOM_PROFILE1:
              if (Inputs->VddqVoltage > 0) {
                Calculated = Inputs->VddqVoltage;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (Inputs->VddqVoltage > 0) {
                Calculated = Inputs->VddqVoltage;
              } else {
                Calculated = VDD_1_10;
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %4u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case voltage for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmOut = &ChannelOut->Dimm[Dimm];
          Outputs->VddqVoltage[Profile] = (MrcVddSelect) MAX((UINT32)(Outputs->VddqVoltage[Profile]), Actual[Profile]);
          DimmOut->VddqVoltage[Profile] = (MrcVddSelect) Actual[Profile];
        }
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the VPP voltage value for the given channel.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmVpp (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput                        *Inputs;
  const MrcControllerIn                 *ControllerIn;
  const MrcChannelIn                    *ChannelIn;
  const MrcDimmIn                       *DimmIn;
  const MrcSpd                          *Spd;
  MrcDebug                              *Debug;
  MrcOutput                             *Outputs;
  MrcControllerOut                      *ControllerOut;
  MrcChannelOut                         *ChannelOut;
  MrcDimmOut                            *DimmOut;
  MrcProfile                            Profile;
  UINT32                                Actual[MAX_PROFILE];
  UINT32                                Calculated;
  UINT8                                 Controller;
  UINT8                                 Channel;
  UINT8                                 Dimm;

  const SPD_EXTREME_MEMORY_PROFILE_DATA_3_0  *Data3;
  UINT32                                      Index;

  Inputs   = &MrcData->Inputs;
  Outputs  = &MrcData->Outputs;
  Debug    = &Outputs->Debug;

  if (Outputs->DdrType != MRC_DDR_TYPE_DDR5) {
    return TRUE;
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s %s:\n  %s\n", VoltageString, VppString, HeaderString);

  //
  // Find the best case voltage value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = (Profile < XMP_PROFILE1) ? VDD_0_50 : 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd        = &DimmIn->Spd.Data;
          Calculated = VDD_1_50;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (!XmpSupport(DimmOut, Profile)) {
                break;
              }

              Index = Profile - XMP_PROFILE1;
              Data3       = &Spd->Ddr5.EndUser.Xmp.Data[Index];
              Calculated  = XMP_VDD_INCREMENT * Data3->Vpp.Bits.Decimal;
              Calculated  = MIN (Calculated, XMP_VDD_INTEGER - 1);
              if (Data3->Vpp.Bits.Integer >= 3) {
                // 3 is reserved value
                Calculated = VDD_1_80;
              } else {
                Calculated += (XMP_VDD_INTEGER * Data3->Vpp.Bits.Integer);
                Calculated  = MAX (Calculated, XMP3_VDD_MIN_POSSIBLE);
                Calculated  = MIN (Calculated, XMP3_VDD_MAX_POSSIBLE);
              }
              break;
            case CUSTOM_PROFILE1:
              if (Inputs->VppVoltage > 0) {
                Calculated = Inputs->VppVoltage;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (Inputs->VppVoltage > 0) {
                Calculated = Inputs->VppVoltage;
              } else {
                Calculated = VDD_1_80;
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  % 7u % 10u % 8u % 5u %4u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case voltage for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmOut = &ChannelOut->Dimm[Dimm];
          Outputs->VppVoltage[Profile] = (MrcVddSelect) MAX((UINT32)(Outputs->VppVoltage[Profile]), Actual[Profile]);
          DimmOut->VppVoltage[Profile] = (MrcVddSelect) Actual[Profile];
        }
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Analyze the given DIMM SPD data to determine DIMM presence and configuration.

    @param[in, out] MrcData - Pointer to MRC global data structure.
    @param[in] Controller   - Current controller number.
    @param[in] Channel      - Current channel number.
    @param[in] Dimm         - Current DIMM number.

    @retval mrcSuccess if DIMM is present otherwise mrcDimmNotExist.
**/
static
MrcStatus
SpdDimmRecognition (
  IN OUT MrcParameters *const MrcData,
  IN  const UINT8          Controller,
  IN  UINT8                Channel,
  IN  UINT8                Dimm
  )
{
  static const SpdRecogCallTable CallTable[] = {
    {ValidDimm},
    {ValidSdramDeviceWidth},
    {ValidPrimaryWidth},
    {GetRankCount},
    {ValidBank},
    {GetDimmSize},
    {ValidRowSize},
    {ValidColumnSize},
    {ValidEccSupport},
    {GetAddressMirror},
    {GetThermalRefreshSupport},
    {GetpTRRsupport},
    {GetReferenceRawCardSupport}
  };
  const MrcSpd *Spd;
  const UINT8  *CrcStart;
  MrcDimmOut   *DimmOut;
  MrcDimmIn    *DimmIn;
  BOOLEAN      Status;
  UINT32       CrcSize;
  UINT8        Index;

  DimmIn  = &MrcData->Inputs.Controller[Controller].Channel[Channel].Dimm[Dimm];
  Spd     = &DimmIn->Spd.Data;
  DimmOut = &MrcData->Outputs.Controller[Controller].Channel[Channel].Dimm[Dimm];
  DimmOut->Status = DIMM_NOT_PRESENT;

  if (DIMM_PRESENT == DimmPresence (&MrcData->Outputs.Debug, Spd)) {
    Status = TRUE;
    for (Index = 0; (Status == TRUE) && (Index < ARRAY_COUNT (CallTable)); Index++) {
      Status &= CallTable[Index].mrc_task (MrcData, Spd, DimmOut);
    }
    if (Status == FALSE) {
      DimmOut->Status = DIMM_DISABLED;
      return mrcDimmNotExist;
    }
    DimmOut->Status = DIMM_PRESENT;
    CrcStart = MrcSpdCrcArea (MrcData, Controller, Channel, Dimm, &CrcSize);
    GetDimmCrc ((const UINT8*const) CrcStart, CrcSize, &DimmOut->Crc);
  } else {
    return mrcDimmNotExist;
  }

  if (DIMM_DISABLED == DimmIn->Status) {
    DimmOut->Status = DIMM_DISABLED;
    DimmOut->Crc = 0;
  }

  return mrcSuccess;
}

/**
  Calculate the timing of all DIMMs on all channels.

    @param[in, out] MrcData - The MRC "global data".

    @retval mrcSuccess on success, mrcDimmNotExist if no DIMMs found.
**/
static
MrcStatus
SpdTimingCalculation (
  IN OUT MrcParameters *const MrcData
  )
{
  static const SpdTimeCallTable CallTable[] = {
    {GetChannelDimmTimeBase}, // Note: This must be done first as all other calculations are based on this.
    {GetChannelDimmtCK},      // Note: This must be done second as all other calculations are based on this.
    {GetChannelDimmtAA},
    {GetChannelDimmtCWL},
    {GetChannelDimmtRAS},
    {GetChannelDimmtRCD},
    {GetChannelDimmtRC},      // Note: This must be done after GetChannelDimmtRAS and GetChannelDimmtRCD
    {GetChannelDimmtREFI},
    {GetChannelDimmtRFC},
    {GetChannelDimmtRFCpb},
    {GetChannelDimmtRP},      // Note: This must be done after GetChannelDimmtRCD
    {GetChannelDimmtRPab},    // Note: This must be done after GetChannelDimmtRP
    {GetChannelDimmtFAW},
    {GetChannelDimmtRRD},
    {GetChannelDimmtRTP},
    {GetChannelDimmtWR},
    {GetChannelDimmtWTR},
    {GetChannelDimmtRFC2},
    {GetChannelDimmtRFC4},
    {GetChannelDimmtRRD_L},
    {GetChannelDimmtRRD_S},
    {GetChannelDimmtWTR_L},
    {GetChannelDimmtWTR_S},
    {GetChannelDimmtCCD_L},
    {GetChannelDimmNmode},
    {GetChannelDimmVdd},
    {GetChannelDimmVddq},
    {GetChannelDimmVpp}
  };
  BOOLEAN    Status;
  UINT8      Index;

  //
  // Find the "least common denominator" timing across the DIMMs.
  // tAA must be done first before any other timings are calculated.
  //
  Status = TRUE;
  for (Index = 0; (Status == TRUE) && (Index < ARRAY_COUNT (CallTable)); Index++) {
    Status &= CallTable[Index].mrc_task (MrcData);
    if (Status != TRUE) {
      MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s failed step %d", __FUNCTION__, Index);
    }
  }
  
  return (Status == FALSE) ? mrcDimmNotExist : mrcSuccess;
}

/**
  Determine the starting address and size of the SPD area to generate a CRC.

    @param[in, out] MrcData    - The MRC "global data".
    @param[in]      Controller - Controller index.
    @param[in]      Channel    - Channel index.
    @param[in]      Dimm       - Dimm index.
    @param[out]     CrcSize    - Location to write CRC block size.

    @retval The starting address of the CRC block.
**/
const UINT8 *
MrcSpdCrcArea (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8                Controller,
  IN     UINT8                Channel,
  IN     UINT8                Dimm,
  OUT    UINT32        *const CrcSize
  )
{
  const MrcDimmIn *DimmIn;
  const UINT8     *CrcStart;
  UINT8           DdrType;

  DimmIn   = &MrcData->Inputs.Controller[Controller].Channel[Channel].Dimm[Dimm];

  DdrType = DimmIn->Spd.Data.Ddr3.General.DramDeviceType.Bits.Type;
  if (MRC_SPD_DDR4_SDRAM_TYPE_NUMBER == DdrType) {
    CrcStart = (void *) &DimmIn->Spd.Data.Ddr4.ManufactureInfo;
    *CrcSize = SPD4_MANUF_SIZE;
  } else if (MRC_SPD_DDR5_SDRAM_TYPE_NUMBER == DdrType) {
    CrcStart = (void *) &DimmIn->Spd.Data.Ddr5.ManufactureInfo;
    *CrcSize = SPD5_MANUF_SIZE;
  } else if ((MRC_SPD_LPDDR4_SDRAM_TYPE_NUMBER == DdrType) || (MRC_SPD_LPDDR4X_SDRAM_TYPE_NUMBER == DdrType) || (MRC_SPD_LPDDR5_SDRAM_TYPE_NUMBER == DdrType) || (MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER == DdrType)) {
    CrcStart = (void *) &DimmIn->Spd.Data.Lpddr.ManufactureInfo;
    *CrcSize = SPDLP_MANUF_SIZE;
  } else {
    CrcStart = NULL;
    *CrcSize = 0;
  }
  return (CrcStart);
}

/**
  Check that DqsMapCpu2Dram has valid values.

  @param[in, out] MrcData - The MRC "global data".

  @retval mrcSuccess on success, mrcWrongInputParameter if these tables have invalid values.
**/
MrcStatus
MrcCheckLpddrMapping (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcDebug        *Debug;
  MrcInput        *Inputs;
  MrcChannelIn    *ChannelIn;
  MrcStatus       Status;
  MrcOutput       *Outputs;
  UINT32          Controller;
  UINT8           CpuByte;
  UINT8           DramByte;
  UINT8           Channel;
  UINT8           *DqsMapCpu2Dram;
  UINT8           DqsMap;
  BOOLEAN         DqsMapCpu2DramGood;
  BOOLEAN         Lpddr;

  Status  = mrcSuccess;
  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  Lpddr   = Outputs->Lpddr;
  DqsMapCpu2DramGood = TRUE;

  if (!Lpddr) {
    return mrcSuccess;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      ChannelIn = &Inputs->Controller[Controller].Channel[Channel];
      DqsMapCpu2Dram = ChannelIn->DqsMapCpu2Dram[dDIMM0]; // Lpddr only uses DIMM0
      DqsMap = 0;

      for (CpuByte = 0; CpuByte < MAX_BYTE_IN_LP_CHANNEL; CpuByte++) {
        DramByte = DqsMapCpu2Dram[CpuByte];
        if (DramByte < MAX_BYTE_IN_LP_CHANNEL) {
          DqsMap |= (1 << DramByte);
        }
      } // for CpuByte
      if (DqsMap != ((1 << MAX_BYTE_IN_LP_CHANNEL) - 1)) {
        DqsMapCpu2DramGood = FALSE;
      }
    } // Channel
  } // Controller

  if (!DqsMapCpu2DramGood) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR,"%s input array %s has invalid values !\n", gErrString, "DqsMapCpu2Dram");
    Status = mrcWrongInputParameter;
  }

  return Status;
}

/**
  Check that some MRC input parameters make sense for the current DDR type.

  @param[in] MrcData - The MRC "global data".

  @retval mrcSuccess on success, mrcWrongInputParameter if parameters have invalid values.
**/
MrcStatus
MrcCheckInputParams (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcStatus             Status;

  Status = MrcCheckLpddrMapping (MrcData);

  return Status;
}

/**
  Process the SPD information for all DIMMs on all channels.

    @param[in, out] MrcData - The MRC "global data".

    @retval mrcSuccess on success, mrcDimmNotExist if no DIMMs found.
**/
MrcStatus
MrcSpdProcessing (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcDebug                      *Debug;
  const MrcInput                *Inputs;
  const MrcControllerIn         *ControllerIn;
  const MrcChannelIn            *ChannelIn;
  const MrcDimmIn               *DimmIn;
  const MrcSpd                  *SpdIn;
  const SPD4_MANUFACTURING_DATA *ManufactureData;
  const SPD5_MANUFACTURING_DATA *Ddr5ManufactureData;
  const SPD5_MANUFACTURING_DATA *CurDimmManufactureData;
  MrcOutput                     *Outputs;
  MrcSaveData                   *SaveOutputs;
  MrcControllerOut              *ControllerOut;
  MrcChannelOut                 *ChannelOut;
  MrcDimmOut                    *DimmOut;
  MrcStatus                     Status;
  UINT32                        DimmCount;
  UINT16                        DateCode;
  UINT8                         Controller;
  UINT8                         Channel;
  UINT8                         MaxChannel;
  UINT8                         Dimm;
  UINT8                         MaxDimm;
  UINT8                         ValidRankBitMask;
  UINT8                         ExpectedWidth;
  UINT8                         DensityIndex;
  UINT8                         SdramWidthIndex;
  UINT8                         DimmPartNumber;
  UINT8                         DeviceType;
  BOOLEAN                       Ddr4;
  BOOLEAN                       Ddr5;
  BOOLEAN                       Lpddr4;
  BOOLEAN                       Lpddr;
  BOOLEAN                       Xmp1DPC;
  BOOLEAN                       IsFirstDimm;
  UINT16                        FirstDimmCrc = 0;
  UINT16                        CurDimmCrc = 0;
  UINT8                         FirstDimmRankInDimm = 0;
  UINT8                         CurDimmRankInDimm = 0;
  UINT8                         FirstDimmSdramWidth = 0;
  UINT8                         CurDimmSdramWidth = 0;
  UINT8                         SnLength;
  UINT8                         Ddr4Pn641A2[sizeof (SPD4_MODULE_PART_NUMBER)] =
                                  {0x48, 0x4D, 0x41, 0x38, 0x31, 0x47, 0x53, 0x36, 0x43, 0x4A, 0x52, 0x38, 0x4E, 0x2D, 0x58, 0x4E ,0x20 ,0x20 ,0x20 ,0x20};
  UINT8                         IsDdr4Pn641A2 = 0;
  UINT8                         Ddr4Pn638A2[sizeof (SPD4_MODULE_PART_NUMBER)]  =
                                  {0x38, 0x41, 0x54, 0x46, 0x32, 0x47, 0x36, 0x34, 0x48, 0x5A, 0x2D, 0x33, 0x47, 0x32, 0x42, 0x59, 0x20, 0x20, 0x20, 0x20};
  UINT8                         IsDdr4Pn638A2 = 0;
  UINT8                         Ddr5Pn48BA1[sizeof (SPD5_MODULE_PART_NUMBER)] =
                                  {0x4D, 0x54, 0x43, 0x38, 0x43, 0x31, 0x30, 0x38, 0x34, 0x53, 0x31, 0x53, 0x43, 0x34, 0x38, 0x42, 0x41, 0x31, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20};

//Y240118_yqr-Add DDR4 DRAM MODULE_PART_NUMBER buffers 
   //Ddr4 Micron MODULE_PART_NUMBER : CM4X8GD4000Z18K2
   UINT8                        Ddr4Micron[sizeof (SPD4_MODULE_PART_NUMBER)]  =
                                  {0x43, 0x4d, 0x34, 0x58, 0x38, 0x47, 0x44, 0x34, 0x30, 0x30, 0x30, 0x5a, 0x31, 0x38, 0x4b, 0x32};

//K230720-S
    UINT8                         Ddr5Samsung[sizeof (SPD5_MODULE_PART_NUMBER)]  =
                                    {0x43, 0x4D, 0x4B, 0x35, 0x58, 0x31, 0x36, 0x47, 0x31, 0x4C, 0x36, 0x30, 0x43, 0x33, 0x36, 0x41, 0x32};
     UINT8                         Ddr5SamsungB[sizeof (SPD5_MODULE_PART_NUMBER)]  =
                                    {0x43, 0x4D, 0x4B, 0x35, 0x58, 0x31, 0x36, 0x47, 0x31, 0x42, 0x36, 0x30, 0x43, 0x33, 0x36, 0x41, 0x32};
     UINT8                         Ddr5SSMemory = 0;
     UINT8                         Ddr5SGlowayA[sizeof (SPD5_MODULE_PART_NUMBER)]  =
                                    {0x2F,0x47,0x4D,0x35,0x55,0x48,0x36, 0x38,0x43,0x33,0x38,0x41,0x47,0x2D,0x44,0x41,0x42,0x59,0x53,0x4E};
     UINT8                         Ddr5SKMemory = 0;
     UINT8                         OemTempSpd = 0;
//K230720-E
//Eric +s
     UINT8                         Ddr5KingstoneA[sizeof (SPD5_MODULE_PART_NUMBER)]  =
                                    {0x4B,0x46,0x35,0x36,0x34,0x43,0x33, 0x32,0x2D,0x31,0x36};
     UINT8                         Ddr5GskillSSA[sizeof (SPD5_MODULE_PART_NUMBER)]  =
                                    {0x46,0x35,0x2D,0x36,0x30,0x30,0x30, 0x4A,0x34,0x30,0x34,0x30,0x46,0x31,0x36,0x47};
//Eric +e
  const SPD_UNBUF_REFERENCE_RAW_CARD  *Ddr4FirstDimmRawCard = NULL;
  const SPD_UNBUF_REFERENCE_RAW_CARD  *Ddr4CurDimmRawCard = NULL;
  const SPD5_REFERENCE_RAW_CARD       *Ddr5FirstDimmRawCard = NULL;
  const SPD5_REFERENCE_RAW_CARD       *Ddr5CurDimmRawCard = NULL;
  const SPD5_MANUFACTURING_DATA       *FirstDimmManufactureData = NULL;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  SaveOutputs = &MrcData->Save.Data;
  Debug   = &Outputs->Debug;
  Status  = mrcDimmNotExist;

  Outputs->DdrType  = MRC_DDR_TYPE_UNKNOWN;
  Outputs->tMAC     = MRC_TMAC_UNLIMITED;
  SaveOutputs->MaxDqBits = 0;
  MaxDimm           = MAX_DIMMS_IN_CHANNEL;
  MaxChannel        = MAX_CHANNEL;
  Ddr4              = FALSE;
  Ddr5              = FALSE;
  Lpddr4            = FALSE;
  Outputs->IsDdr4Pn641A2 = FALSE;
  Outputs->IsDdr4Pn638A2 = FALSE;
  Outputs->IsDdr5Pn48BA1 = FALSE;
  Outputs->MADieMemory1RBy168GB = FALSE;
  Outputs->MADieMemory1RBy168GBRPL2 = FALSE;
//K230720-S 
              IoWrite8(0x70,0x7A);              
              IoWrite8(0x71,0x00);  
              IoWrite8(0x70,0x60);              
              IoWrite8(0x71,0x00);
              IoWrite8(0x70,0x61);              
              IoWrite8(0x71,0x00);
              IoWrite8(0x70,0x62);              
              IoWrite8(0x71,0x00);
//K230720-E
  //
  // Scan Dimms for first valid DdrType
  //
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerIn = &Inputs->Controller[Controller];
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      ChannelIn = &ControllerIn->Channel[Channel];
      for (Dimm = 0; Dimm < MaxDimm; Dimm++) {
        DimmIn = &ChannelIn->Dimm[Dimm];
        SpdIn = &DimmIn->Spd.Data;
        DeviceType = SpdIn->Ddr4.Base.DramDeviceType.Bits.Type; // Device Type for all DdrTypes is in the same location
        if (DimmPresence (Debug, SpdIn) == DIMM_PRESENT) {
          switch (DeviceType) {
           case MRC_SPD_DDR4_SDRAM_TYPE_NUMBER:
             Outputs->DdrType   = MRC_DDR_TYPE_DDR4;
             Outputs->Lpddr     = FALSE;
             SaveOutputs->MaxDqBits = 64 + NUM_ECC_BITS_DDR4;
             MaxChannel = 1;
             Ddr4 = TRUE;
             break;

           case MRC_SPD_LPDDR4_SDRAM_TYPE_NUMBER:
           case MRC_SPD_LPDDR4X_SDRAM_TYPE_NUMBER:
             Outputs->DdrType   = MRC_DDR_TYPE_LPDDR4;
             Outputs->Lpddr     = TRUE;
             SaveOutputs->MaxDqBits = 16;
             MaxDimm = 1;
             Lpddr4 = TRUE;
             break;

           case MRC_SPD_LPDDR5_SDRAM_TYPE_NUMBER:
           case MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER:
             Outputs->DdrType   = MRC_DDR_TYPE_LPDDR5;
             Outputs->Lpddr     = TRUE;
             SaveOutputs->MaxDqBits = 16;
             MaxDimm = 1;
             break;

           case MRC_SPD_DDR5_SDRAM_TYPE_NUMBER:
             Outputs->DdrType   = MRC_DDR_TYPE_DDR5;
             Outputs->Lpddr     = FALSE;
             SaveOutputs->MaxDqBits = 32 + NUM_ECC_BITS_DDR5;
             MaxDimm = 2;
             MaxChannel = 2;
             Ddr5 = TRUE;
             break;
          }
        }
        if (Outputs->DdrType != MRC_DDR_TYPE_UNKNOWN) {
          break;
        }
      }
      if (Outputs->DdrType != MRC_DDR_TYPE_UNKNOWN) {
        break;
      }
    }
    if (Outputs->DdrType != MRC_DDR_TYPE_UNKNOWN) {
      break;
    }
  }
  // Now we know the memory types across the system set MaxChannel based on DDR Type
  Outputs->MaxChannels = MaxChannel;

  // Set Lpddr boolean variable
  Lpddr = Outputs->Lpddr;

  //
  // Scan thru each DIMM to see if it is a valid DIMM and to get its configuration.
  //
  DimmCount  = 0;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerIn  = &Inputs->Controller[Controller];
    ControllerOut = &Outputs->Controller[Controller];
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      ChannelIn   = &ControllerIn->Channel[Channel];
      ChannelOut  = &ControllerOut->Channel[Channel];
      ChannelOut->DimmCount = 0;
      for (Dimm = 0; Dimm < MaxDimm; Dimm++) {
        DimmIn  = &ChannelIn->Dimm[Dimm];
        DimmOut = &ChannelOut->Dimm[Dimm];
        if (DimmIn->Status == DIMM_ENABLED || DimmIn->Status == DIMM_DISABLED) {
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "SPD Dimm recognition, %s %u/%u/%u\n",
            CcdString,
            Controller,
            Channel,
            Dimm
            );

          Status = SpdDimmRecognition (MrcData, Controller, Channel, Dimm);
          if (Status == mrcSuccess) {
            DimmCount++;
            ChannelOut->DimmCount++;
            if (ChannelOut->DimmCount == 2) {
              Outputs->Any2Dpc = TRUE;
            }
            if (Outputs->DdrType == MRC_DDR_TYPE_UNKNOWN) {
              Outputs->DdrType = DimmOut->DdrType;
            } else if (Outputs->DdrType != DimmOut->DdrType) {
              Status = mrcMixedDimmSystem;
            }
            if (Status == mrcMixedDimmSystem) {
              MRC_DEBUG_MSG (
                Debug,
                MSG_LEVEL_ERROR,
                "%s configuration, system contains a mix of memory types\n",
                ErrorString
                );
              return (Status);
            }
          }
        }
      }
    }
  }
  // Check for mixed PrimaryBusWidth - for LP4 and LP5 only
  if (Lpddr) {
    ExpectedWidth = MRC_SPD_PRIMARY_BUS_WIDTH_16;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MaxChannel; Channel++) {
        for (Dimm = 0; Dimm < MaxDimm; Dimm++) {
          if (Outputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Status == DIMM_PRESENT) {
            SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data;
            if (SpdIn->Lpddr.Base.ModuleMemoryBusWidth.Bits.PrimaryBusWidth != ExpectedWidth) {
              Status = mrcMixedDimmSystem;
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Mixed PrimaryBusWidth across DRAMs!\n", gErrString);
              return (Status);
            }
          }
        }
      }
    }
  }

  //check for mixed memory module - for DDR4 and DDR5 only
  //We should not regard it as mixed memory, when different memory modules are in different memory controllers
  //only treat as mixed memory when different memory modules on the same memory controller
  Outputs->IsMixedMemory = FALSE;
  if (Ddr4 || Ddr5) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MaxChannel; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        IsFirstDimm = TRUE;
        for (Dimm = 0; Dimm < MaxDimm; Dimm++) {
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (Outputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Status == DIMM_PRESENT) {
            MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "SPD Mixed Dimm Checking, MC%u CH%u Dimm%u\n", Controller, Channel, Dimm);
            if (IsFirstDimm) {
              if (Ddr4) {
                Ddr4FirstDimmRawCard = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data.Ddr4.Module.Unbuffered.ReferenceRawCardUsed;
                FirstDimmCrc = Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data.Ddr4.Base.Crc.Crc[0];
                MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR4 Ddr4FirstDimmRawCard->Data = 0x%x !\n", Ddr4FirstDimmRawCard->Data);
                MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR4 FirstDimmCrc = 0x%x !\n", FirstDimmCrc);
              }
              if (Ddr5) {
                Ddr5FirstDimmRawCard = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data.Ddr5.ModuleCommon.ReferenceRawCardUsed;
                FirstDimmCrc = Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data.Ddr5.Base.Crc.Crc[0];
                FirstDimmManufactureData = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data.Ddr5.ManufactureInfo;
                FirstDimmRankInDimm = DimmOut->RankInDimm;
                FirstDimmSdramWidth = DimmOut->SdramWidth;
                MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR5 Ddr5FirstDimmRawCard->Data = 0x%x !\n", Ddr5FirstDimmRawCard->Data);
                MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR5 FirstDimmCrc = 0x%x !\n", FirstDimmCrc);
                MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR5 FirstDimmManufactureData->DramIdCode.Data = 0x%x !\n", FirstDimmManufactureData->DramIdCode.Data);
                MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR5 FirstDimmManufactureData->ModuleId.IdCode.Data = 0x%x !\n", FirstDimmManufactureData->ModuleId.IdCode.Data);
                MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR5 FirstDimmSdramWidth = 0x%x !\n", FirstDimmSdramWidth);
                MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR5 FirstDimmRankInDimm = 0x%x !\n", FirstDimmRankInDimm);
              }
              IsFirstDimm = FALSE;
            } else {
              if (Ddr4) {
                Ddr4CurDimmRawCard = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data.Ddr4.Module.Unbuffered.ReferenceRawCardUsed;
                CurDimmCrc = Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data.Ddr4.Base.Crc.Crc[0];
                MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR4 Ddr4CurDimmRawCard->Data = 0x%x !\n", Ddr4CurDimmRawCard->Data);
                MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR4 CurDimmCrc = 0x%x !\n", CurDimmCrc);
                if (Ddr4FirstDimmRawCard->Data != Ddr4CurDimmRawCard->Data || FirstDimmCrc != CurDimmCrc) {
                  MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR4 Spd crc(byte 126-127) or raw card(byte 130) are not the same, set IsMixedMemory to TRUE!\n");
                  Outputs->IsMixedMemory = TRUE;
                  break;
                }
              }
              if (Ddr5) {
                Ddr5CurDimmRawCard = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data.Ddr5.ModuleCommon.ReferenceRawCardUsed;
                CurDimmCrc = Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data.Ddr5.Base.Crc.Crc[0];
                CurDimmManufactureData = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data.Ddr5.ManufactureInfo;
                CurDimmRankInDimm = DimmOut->RankInDimm;
                CurDimmSdramWidth = DimmOut->SdramWidth;

                MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR5 Ddr5CurDimmRawCard->Data = 0x%x !\n", Ddr5CurDimmRawCard->Data);
                MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR5 CurDimmCrc = 0x%x !\n", CurDimmCrc);
                MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR5 CurDimmManufactureData->DramIdCode.Data = 0x%x !\n", CurDimmManufactureData->DramIdCode.Data);
                MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR5 CurDimmManufactureData->ModuleId.IdCode.Data = 0x%x !\n", CurDimmManufactureData->ModuleId.IdCode.Data);
                MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR5 CurDimmSdramWidth = 0x%x !\n", CurDimmSdramWidth);
                MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR5 CurDimmRankInDimm = 0x%x !\n", CurDimmRankInDimm);
                if ( (Ddr5FirstDimmRawCard->Data != Ddr5CurDimmRawCard->Data) ||
                     (FirstDimmCrc != CurDimmCrc) ||
                     (FirstDimmManufactureData->DramIdCode.Data != CurDimmManufactureData->DramIdCode.Data) ||
                     (FirstDimmManufactureData->ModuleId.IdCode.Data != CurDimmManufactureData->ModuleId.IdCode.Data) ||
                     (FirstDimmSdramWidth != CurDimmSdramWidth) ||
                     (FirstDimmRankInDimm != CurDimmRankInDimm) ) {
                  MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDR5 Spd crc(byte 126-127) or raw card(byte 232) are not the same, set IsMixedMemory to TRUE!\n");
                  Outputs->IsMixedMemory = TRUE;
                  break;
                }
              }
            }
          }
        }
      }
    }
  }

  Outputs->Ddr5HXMemory               = FALSE;
  Outputs->Ddr5MOMemory               = FALSE;
  Outputs->Ddr5SGMemory               = FALSE;
  Outputs->Ddr5SGMemory1RBy8          = FALSE;
  Outputs->Ddr5SGMemory1RBy16         = FALSE;
  Outputs->Ddr5MOMemory1RBy8          = FALSE;
  Outputs->Ddr5HXMemoryMDie2RBy8      = FALSE;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerOut = &Outputs->Controller[Controller];
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      Xmp1DPC = FALSE;
      ChannelOut = &ControllerOut->Channel[Channel];
      for (Dimm = 0; Dimm < MaxDimm; Dimm++) {
        DimmOut = &ChannelOut->Dimm[Dimm];
        if ((DIMM_PRESENT == DimmOut->Status) || (DIMM_DISABLED == DimmOut->Status)) {
          if (Ddr5) {
            SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data;
            Ddr5ManufactureData = &SpdIn->Ddr5.ManufactureInfo;
            if ((Ddr5ManufactureData->DramIdCode.Data == 0xAD80) ||
                (Ddr5ManufactureData->ModuleId.IdCode.Data == 0xAD80)) {
              Outputs->Ddr5HXMemory = TRUE;
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "memory ID 0xAD80 found and Outputs->Ddr5HXMemory = %d\n",
                             Outputs->Ddr5HXMemory);
               if ((DimmOut->RankInDimm == 2) &&
                   (DimmOut->SdramWidth == 8) &&
                   (Ddr5ManufactureData->ModulePartNumber.ModulePartNumber[6] == 0x4D)) {
                Outputs->Ddr5HXMemoryMDie2RBy8 = TRUE;
                MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Ddr5HXMemoryMDie2RBy8 is true for DIMM %d \n", Dimm);
              }
            }

            if ((Ddr5ManufactureData->DramIdCode.Data == 0x2C80) ||
                (Ddr5ManufactureData->ModuleId.IdCode.Data == 0x2C80)) {
              Outputs->Ddr5MOMemory = TRUE;
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "memory ID 0x2C80 found and Outputs->Ddr5MOMemory = %d \n",
                             Outputs->Ddr5MOMemory);
              if ((DimmOut->RankInDimm == 1) && (DimmOut->SdramWidth == 8)) {
                Outputs->Ddr5MOMemory1RBy8 = TRUE;
                MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Ddr5MOMemory1RBy8 is true for DIMM %d \n", Dimm);
              }
            }
            if ((Ddr5ManufactureData->DramIdCode.Data == 0xCE80) ||
                (Ddr5ManufactureData->ModuleId.IdCode.Data == 0xCE80)) {
              Outputs->Ddr5SGMemory = TRUE;
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "memory ID 0xCE80 found and Outputs->Ddr5SGMemory = %d \n",
                             Outputs->Ddr5SGMemory);
              if ((DimmOut->RankInDimm == 1) && (DimmOut->SdramWidth == 8)) {
                Outputs->Ddr5SGMemory1RBy8 = TRUE;
                MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Ddr5SGMemory1RBy8 is true for DIMM %d \n", Dimm);
              } else if ((DimmOut->RankInDimm == 1) && (DimmOut->SdramWidth == 16)) {
                Outputs->Ddr5SGMemory1RBy16 = TRUE;
                MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Ddr5SGMemory1RBy16 is true for DIMM %d \n", Dimm);
              }
            }
          }
        }
      } // Dimm
    } // Channel
  } //Controller
  //
  // Get the maximum allowed frequency / refclk
  //
  MrcMcCapabilityPreSpd (MrcData);

  if (DimmCount > 0) {
    if (Ddr4) {
      Outputs->BurstLength = 4;   // BL8  - 8 UI's,  4 tCK
    } else if (Ddr5) {
      Outputs->BurstLength = 8;   // BL16 - 16 UI's, 8 tCK
    } else if (Lpddr4) {
      Outputs->BurstLength = 16;  // BL32 - 32 UI's, 16 tCK
    } else { // LPDDR5
      Outputs->BurstLength = 4;   // BL32 - tCK in 4:1 is 8 UI's per clock, 4tCK
    }

    Outputs->TCRSensitiveHynixDDR4  = FALSE;
    Outputs->TCRSensitiveMicronDDR4 = FALSE;
    Outputs->Ddr5PopulatedH         = FALSE;
    Outputs->Ddr5PopulatedH56002RBy8 = FALSE;
    //
    // Scan thru each channel to see if it is a valid channel and to get its configuration.
    //
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "SPD Dimm timing calculation\n");
    if (mrcSuccess == SpdTimingCalculation (MrcData)) {
      Outputs->EccSupport = TRUE;

      //
      // Count up the number of valid DIMMs.
      //
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        ControllerOut = &Outputs->Controller[Controller];
        for (Channel = 0; Channel < MaxChannel; Channel++) {
          Xmp1DPC = FALSE;
          ChannelOut = &ControllerOut->Channel[Channel];
          for (Dimm = 0; Dimm < MaxDimm; Dimm++) {
            DimmOut = &ChannelOut->Dimm[Dimm];
            if ((DIMM_PRESENT == DimmOut->Status) || (DIMM_DISABLED == DimmOut->Status)) {
              if (Ddr4 && (Outputs->TCRSensitiveHynixDDR4 == FALSE) && (Outputs->TCRSensitiveMicronDDR4 == FALSE)) {
                SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data;
                ManufactureData = &SpdIn->Ddr4.ManufactureInfo;
                if ((ManufactureData->DramIdCode.Data == 0xAD80) ||
                    (ManufactureData->ModuleId.IdCode.Data == 0xAD80)) { // Hynix
                  DateCode = (ManufactureData->ModuleId.Date.Year << 8) | ManufactureData->ModuleId.Date.Week;
                  if (DateCode < 0x1512) {
                    Outputs->TCRSensitiveHynixDDR4 = TRUE;
                  }
                } else if ((ManufactureData->DramIdCode.Data == 0x2C80) ||
                           (ManufactureData->ModuleId.IdCode.Data == 0x2C80)) { // Micron
                  DensityIndex = SpdIn->Ddr4.Base.SdramDensityAndBanks.Bits.Density;
                  SdramWidthIndex = SpdIn->Ddr4.Base.ModuleOrganization.Bits.SdramDeviceWidth;
                  if ((DensityIndex == MRC_SPD_SDRAM_DENSITY_4Gb) &&
                      (SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8)) { // DRAM Density = 4Gb and DRAM Width = x8
                    DimmPartNumber = ManufactureData->ModulePartNumber.ModulePartNumber[15];
                    if (DimmPartNumber == 0x41) { // DIMM Part# Byte[15] = A-Die
                      Outputs->TCRSensitiveMicronDDR4 = TRUE;
                    }
                  }
                }
              }

              if (Ddr4) {
                SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data;
                ManufactureData = &SpdIn->Ddr4.ManufactureInfo;
                //Recognize special handling case - just one dimm is enough to special handle to all dimms
                for (SnLength = 0; SnLength < sizeof (SPD4_MODULE_PART_NUMBER); SnLength++) {
                  if (ManufactureData->ModulePartNumber.ModulePartNumber[SnLength] != Ddr4Pn641A2[SnLength]) {
                    break;
                  } else if ((SnLength == sizeof (SPD4_MODULE_PART_NUMBER) - 1)) {
                     IsDdr4Pn641A2 |= 1;
                  }
                }

                if (!IsDdr4Pn641A2) { // Check for another part number if not found
                  for (SnLength = 0; SnLength < sizeof (SPD4_MODULE_PART_NUMBER); SnLength++) {
                    if (ManufactureData->ModulePartNumber.ModulePartNumber[SnLength] != Ddr4Pn638A2[SnLength]) {
                      break;
                    } else if ((SnLength == sizeof (SPD4_MODULE_PART_NUMBER) - 1)) {
                      IsDdr4Pn638A2 |= 1;
                    }
                  }
                }

                //Y240118_yqr- find DDR4 all timing after training -Start


                


                IoWrite16(0x72, 0xB0);
                IoWrite16(0x73, ChannelOut->Timing[XMP_PROFILE1].tCL);
                IoWrite16(0x72, 0xB2);
                IoWrite16(0x73, ChannelOut->Timing[XMP_PROFILE1].tRCDtRP);
                IoWrite16(0x72, 0xB4);
                IoWrite16(0x73, ChannelOut->Timing[XMP_PROFILE1].tRAS);
                IoWrite16(0x72, 0xB6);
                IoWrite16(0x73, ChannelOut->Timing[XMP_PROFILE1].tREFI);

                
                //Y240118_yqr- find DDR4 all timing after training -End
                // Y240118_yqr_Recognize DDR4 DRAM to modify tCl timing_Start
                if (ManufactureData->DramIdCode.Data == 0xCE80)
                { // Samsung IC
                    IoWrite8(0x70, 0x60);
                    IoWrite8(0x71, 0x11);
                }
                if (ManufactureData->DramIdCode.Data == 0xAD80)
                { // Hynix IC
                    IoWrite8(0x70, 0x60);
                    IoWrite8(0x71, 0x22);
                }
                if (ManufactureData->DramIdCode.Data == 0x2C80)
                { // Micron IC
                    IoWrite8(0x70, 0x60);
                    IoWrite8(0x71, 0x33);
                }

                if (ManufactureData->DramIdCode.Data == 0x918A)
                { // CXMT IC
                    IoWrite8(0x70, 0x60);
                    IoWrite8(0x71, 0x44);
                }
                

                if (ManufactureData->ModuleId.IdCode.Data == 0x9E02)
                { // Corasir
                    IoWrite8(0x70, 0x61);
                    IoWrite8(0x71, 0x11);
                }
                if (ManufactureData->ModuleId.IdCode.Data == 0x920B)
                { // Kingbank
                    IoWrite8(0x70, 0x61);
                    IoWrite8(0x71, 0x22);
                }
                if (ManufactureData->ModuleId.IdCode.Data == 0x9801)
                { // Kingstone
                    IoWrite8(0x70, 0x61);
                    IoWrite8(0x71, 0x33);
                }
                if (ManufactureData->ModuleId.IdCode.Data == 0xAD80)
                { // Hynix
                    IoWrite8(0x70, 0x61);
                    IoWrite8(0x71, 0x44);
                }
                if (ManufactureData->ModuleId.IdCode.Data == 0xCE80)
                { // Samsung
                    IoWrite8(0x70, 0x61);
                    IoWrite8(0x71, 0x55);
                }
                if (ManufactureData->ModuleId.IdCode.Data == 0xBC88)
                { // CUSO
                    IoWrite8(0x70, 0x61);
                    IoWrite8(0x71, 0x66);
                }
                //0x43 0x4D 0x34 0x58 0x38 0x47 0x44 0x34 0x30 0x30 0x30 0x5A 0x31 0x38 0x4B 0x32
                //67    77   52   88   56   71   68   52   48   ~    ~    90   49   56   75   50
                //C     M    4    X    8    G    D    M    0    0    0    Z    1    8    K    2
                // for (SnLength = 1; SnLength < 20; SnLength++)
                // {
                //     if (ManufactureData->ModulePartNumber.ModulePartNumber[SnLength] != Ddr5SGlowayA[SnLength])
                //     {
                //         break;
                //     }
                //     else if ((SnLength == 19))
                //     {
                //         Ddr5SKMemory |= 1;
                //         IoWrite8(0x70, 0x62);
                //         IoWrite8(0x71, 0x11);
                //     }
                // }

                // for (SnLength = 0; SnLength < 11; SnLength++)
                // {
                //     if (ManufactureData->ModulePartNumber.ModulePartNumber[SnLength] != Ddr5KingstoneA[SnLength])
                //     {
                //         break;
                //     }
                //     else if ((SnLength == 10))
                //     {
                //         Ddr5SKMemory |= 1;
                //         IoWrite8(0x70, 0x62);
                //         IoWrite8(0x71, 0x22);
                //     }
                // }

                // for (SnLength = 0; SnLength < 16; SnLength++)
                // {
                //     if (ManufactureData->ModulePartNumber.ModulePartNumber[SnLength] != Ddr5GskillSSA[SnLength])
                //     {
                //         break;
                //     }
                //     else if ((SnLength == 15))
                //     {
                //         Ddr5SKMemory |= 1;
                //         IoWrite8(0x70, 0x62);
                //         IoWrite8(0x71, 0x33);
                //     }
                // }

                // for (SnLength = 0; SnLength < 17; SnLength++)
                // {
                //     if (ManufactureData->ModulePartNumber.ModulePartNumber[SnLength] != Ddr5SamsungB[SnLength])
                //     {
                //         break;
                //     }
                //     else if ((SnLength == 16))
                //     {
                //         Ddr5SKMemory |= 1;
                //         IoWrite8(0x70, 0x62);
                //         IoWrite8(0x71, 0x44);
                //     }
                // }

        

                // Y240118_yqr_Recognize DDR4 DRAM to modify tCl timing_End
              }

              if (Ddr5) {
                SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data;
                Ddr5ManufactureData = &SpdIn->Ddr5.ManufactureInfo;
                if ((Ddr5ManufactureData->DramIdCode.Data == 0xAD80) ||
                    (Ddr5ManufactureData->ModuleId.IdCode.Data == 0xAD80)) {
                  Outputs->Ddr5PopulatedH = TRUE;
                  if ((Ddr5ManufactureData->ModuleId.IdCode.Data == 0xAD80) ||
                     (Ddr5ManufactureData->DramIdCode.Data == 0xAD80)) {
                    if (((DimmOut->RankInDimm == 2) && (DimmOut->SdramWidth == 8)) &&
                        (Outputs->MaxDimmFreq == f5600) &&
                       ((Inputs->CpuModel == cmRPL_DT_HALO) || (Inputs->CpuModel == cmRPL_ULX_ULT)) && (Outputs->FreqMax > f2000)) {
                      Outputs->Ddr5PopulatedH56002RBy8 = TRUE;
                    }
                  }
                  if ((Ddr5ManufactureData->ModulePartNumber.ModulePartNumber[6] == 0x41) &&
                      (DimmOut->DimmCapacity == 16384) && (DimmOut->RankInDimm == 2)) { //16GB sub channel: 32GB DIMM full channel and 2R
                    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "A die and 32GB DIMM found \n", Dimm);
                    Outputs->WRWRDRAddOneMemory = TRUE;
                  }
                }

                if ((Ddr5ManufactureData->DramIdCode.Data == 0x2C80) ||
                    (Ddr5ManufactureData->ModuleId.IdCode.Data == 0x2C80)) {
                  if (Ddr5ManufactureData->DramStepping == 0x41) {
                    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MA die found DIMM %d \n", Dimm);
                    Outputs->MADieMemory = TRUE;
                    if (((DimmOut->RankInDimm == 1) && (DimmOut->SdramWidth == 16) && ((DimmOut->DimmCapacity) == 4096)) &&
                       (Outputs->FreqMax == f4000)) { // 4GB sub channel.
                      Outputs->MADieMemory1RBy168GB = TRUE;
                      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MADieMemory1RBy168GB found DIMM %d \n", Dimm);
                    }
                    if (((DimmOut->RankInDimm == 1) && (DimmOut->SdramWidth == 16) && ((DimmOut->DimmCapacity) == 4096)) &&
                       (Inputs->CpuModel  == cmRPL_2_DT_HALO)) { // 4GB sub channel.
                      Outputs->MADieMemory1RBy168GBRPL2 = TRUE;
                      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MADieMemory1RBy168GBRPL2 found DIMM %d \n", Dimm);
                    }
                    for (SnLength = 0; SnLength < sizeof (SPD5_MODULE_PART_NUMBER); SnLength++) {
                      if (Ddr5ManufactureData->ModulePartNumber.ModulePartNumber[SnLength] != Ddr5Pn48BA1[SnLength]) {
                        break;
                      } else if ((SnLength == sizeof (SPD5_MODULE_PART_NUMBER) - 1)) {
                        Outputs->IsDdr5Pn48BA1 = TRUE;
                      }
                    }
                  }
                   else if ((Ddr5ManufactureData->DramStepping == 0x47) || (Ddr5ManufactureData->DramStepping == 0x44)) {
                    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MG or MD die found DIMM %d \n", Dimm);
                    Outputs->MGDieMemory = TRUE;
                  }
                }
//K230720-S
                //Recognize special handling case - just one dimm is enough to special handle to all dimms
                OemTempSpd = Ddr5ManufactureData->ModuleId.IdCode.Data8[0];     //512,513
                IoWrite8(0x70,0x64);              
                IoWrite8(0x71,OemTempSpd); 
                OemTempSpd = Ddr5ManufactureData->ModuleId.IdCode.Data8[1];
                IoWrite8(0x70,0x65);              
                IoWrite8(0x71,OemTempSpd);  
//Ericy +s
                if (!Ddr5SSMemory) { // Check for another part number if not found
                    if (Ddr5ManufactureData->DramIdCode.Data == 0xCE80) {   //Samsung IC
                                       IoWrite8(0x70,0x60);              
                                       IoWrite8(0x71,0x11); 
                                   }
                    if (Ddr5ManufactureData->DramIdCode.Data == 0xAD80) {   //Hynix IC
                                       IoWrite8(0x70,0x60);              
                                       IoWrite8(0x71,0x22); 
                                   }
                    if (Ddr5ManufactureData->DramIdCode.Data == 0x2C80) {   //Micron IC
                                       IoWrite8(0x70,0x60);              
                                       IoWrite8(0x71,0x33); 
                                   }
                    if (Ddr5ManufactureData->ModuleId.IdCode.Data == 0x9E02) {  //Corasir
                                       IoWrite8(0x70,0x61);              
                                       IoWrite8(0x71,0x11); 
                                   }
                    if (Ddr5ManufactureData->ModuleId.IdCode.Data == 0x920B) {  //Kingbank
                                       IoWrite8(0x70,0x61);              
                                       IoWrite8(0x71,0x22); 
                                   }
                    if (Ddr5ManufactureData->ModuleId.IdCode.Data == 0x9801) {  //Kingstone
                                       IoWrite8(0x70,0x61);              
                                       IoWrite8(0x71,0x22); 
                                   }
                    if (Ddr5ManufactureData->ModuleId.IdCode.Data == 0xAD80) {  //Hynix
                                       IoWrite8(0x70,0x61);              
                                       IoWrite8(0x71,0x44); 
                                   }
                    if (Ddr5ManufactureData->ModuleId.IdCode.Data == 0xCE80) {  //Samsung
                                       IoWrite8(0x70,0x61);              
                                       IoWrite8(0x71,0x55); 
                                   }
                               }

//Ericy +e
                if (!Ddr5SKMemory) { // Check for another part number if not found
                                 for (SnLength = 1; SnLength < 20; SnLength++) {
                                   if (Ddr5ManufactureData->ModulePartNumber.ModulePartNumber[SnLength] != Ddr5SGlowayA[SnLength]) {
                                     break;
                                   } else if ((SnLength == 19)) {
                                       Ddr5SKMemory |= 1;
                                       IoWrite8(0x70,0x62);              
                                       IoWrite8(0x71,0x11);    
                                   }
                                 }
                                 
                                 for (SnLength = 0; SnLength < 11; SnLength++) {
                                   if (Ddr5ManufactureData->ModulePartNumber.ModulePartNumber[SnLength] != Ddr5KingstoneA[SnLength]) {
                                     break;
                                   } else if ((SnLength == 10)) {
                                       Ddr5SKMemory |= 1;
                                       IoWrite8(0x70,0x62);              
                                       IoWrite8(0x71,0x22);    
                                   }
                                 }
                                 
                                 for (SnLength = 0; SnLength < 16; SnLength++) {
                                   if (Ddr5ManufactureData->ModulePartNumber.ModulePartNumber[SnLength] != Ddr5GskillSSA[SnLength]) {
                                     break;
                                   } else if ((SnLength == 15)) {
                                       Ddr5SKMemory |= 1;
                                       IoWrite8(0x70,0x62);              
                                       IoWrite8(0x71,0x33);    
                                   }
                                 }
                                 
                                 for (SnLength = 0; SnLength < 17; SnLength++) {
                                   if (Ddr5ManufactureData->ModulePartNumber.ModulePartNumber[SnLength] != Ddr5SamsungB[SnLength]) {
                                     break;
                                   } else if ((SnLength == 16)) {
                                       Ddr5SKMemory |= 1;
                                       IoWrite8(0x70,0x62);              
                                       IoWrite8(0x71,0x44);    
                                   }
                                 }
                                 
                               }
//K230720-E
              }

            }
            if (DIMM_PRESENT == DimmOut->Status) {
#if (MAX_RANK_IN_CHANNEL > 8)
#error The next switch statement and ValidRankBitMask needs updated to support additional ranks.
#endif
              switch (DimmOut->RankInDimm) {
                case 1:
                  ValidRankBitMask = 1;
                  break;
#if (MAX_RANK_IN_DIMM > 1)

                case 2:
                  ValidRankBitMask = 3;
                  break;
#endif
#if (MAX_RANK_IN_DIMM > 2)

                case 3:
                  ValidRankBitMask = 7;
                  break;
#endif
#if (MAX_RANK_IN_DIMM > 3)

                case 4:
                  ValidRankBitMask = 15;
                  break;
#endif

                default:
                  ValidRankBitMask = 0;
                  break;
              }

              ChannelOut->ValidRankBitMask |= ValidRankBitMask << (Dimm * MAX_RANK_IN_DIMM);
              Outputs->EccSupport  &= DimmOut->EccSupport;
              Outputs->tMAC         = MIN (Outputs->tMAC, DimmOut->tMAC);

              if ((XmpSupport(DimmOut, XMP_PROFILE1) && (DimmOut->XmpProfile1Config == 0)) ||
                  (XmpSupport(DimmOut, XMP_PROFILE2) && (DimmOut->XmpProfile2Config == 0)) ||
                  (XmpSupport(DimmOut, XMP_PROFILE3) && (DimmOut->XmpProfile3Config == 0))) {
                Xmp1DPC = TRUE;
              }
            } // DIMM_PRESENT
          } // Dimm

          if ((ChannelOut->DimmCount > 0) && (ChannelOut->ValidRankBitMask > 0)) {
            ControllerOut->ChannelCount++;
            ControllerOut->Channel[Channel].Status = CHANNEL_PRESENT;

            if ((Outputs->XmpProfileEnable != 0) && Xmp1DPC && ((ChannelOut->DimmCount > 1))) {
              //Check XmpConfig Warning
              MRC_DEBUG_MSG (Debug,
                  MSG_LEVEL_WARNING,
                  "Warning: MC%u C%u: XMP memory config is 1DPC, but is populated with 2DPC\n",
                  Controller,
                  Channel);
              Outputs->XmpConfigWarning = TRUE;
            }
          }
        } // Channel

        if (ControllerOut->ChannelCount > 0) {
          ControllerOut->Status = CONTROLLER_PRESENT;
          Status                = mrcSuccess;
        }
      }

      //One dimm is enough to special handle to all dimms
      if (Ddr4 && IsDdr4Pn641A2) {
        Outputs->IsDdr4Pn641A2 = TRUE;
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\nOutputs->IsDdr4Pn641A2 = TRUE is set to all DIMMs\n\n");
      } else if (Ddr4 && IsDdr4Pn638A2) {
        Outputs->IsDdr4Pn638A2 = TRUE;
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\nOutputs->IsDdr4Pn638A2 = TRUE is set to all DIMMs\n\n");
      }
    }
  }

  if (Status != mrcSuccess) {
    return Status;
  }

  Status = MrcCheckInputParams (MrcData);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "SodimmPresent: %u\n", Outputs->SodimmPresent);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Outputs->Ddr5PopulatedH56002RBy8: %u\n",Outputs->Ddr5PopulatedH56002RBy8);
  return Status;
}

```