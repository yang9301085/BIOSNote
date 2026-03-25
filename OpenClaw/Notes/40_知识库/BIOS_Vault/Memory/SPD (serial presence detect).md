## SPD : serial presence detect 内存存在检测
* 是一种访问内存模块有关信息的标准化方式
* SPD数据提供关于存储器通道上的所有模块的关键信息，并且旨在由系统的BIOS使用，以便正确地初始化和优化系统存储器通道。这个芯片中记录了内存太多重要信息，从频率、时序和电压，到制造厂商、制造日期等，每一个参数都会在它正式出厂前，由内存厂商依据其实际性能参数输入到SPD中
* 一个通过串行接口传输并且掉电后不会丢失数据的带电可擦可编程只读存储器（EEPROM）
* 在Intel 平台上 SPD相关信息初始化在`Intel\ClientOneSiliconPkg\IpBlock\MemoryInit\Adl\LibraryPrivate\PeiMemoryInitLib\Source\SpdProcessing\MrcSpdProcessing.c`中
* [[DDR4 SPD SPEC.pdf | DDR4 SPD SPEC]]
* SPD Address Map
 ![[Pasted image 20240126151500.png]]


---
以Kingston DDR4 内存，Micron DRAM 为例，下面是这跟内存条 SPD 中 数据：

000  23 11 0C 02 86 29 00 08 00 60 00 03 01 03 00 00
010  00 00 07 0D F8 0F 00 00 6E 6E 6E 11 00 6E F0 0A
020  20 08 00 05 00 A8 1B 28 28 00 78 00 14 3C 00 00
030  00 00 00 00 00 00 00 00 00 00 00 00 16 36 16 36
040  16 36 16 36 00 00 16 36 16 36 16 36 16 36 00 00
050  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
060  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
070  00 00 00 00 00 00 9C B5 00 00 00 00 E7 D6 E7 D8
080  11 01 60 00 00 00 00 00 00 00 00 00 00 00 00 00
090  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0A0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0B0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0C0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0D0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0E0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0F0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 72 AB
100  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
110  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
120  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
130  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
140  01 98 04 22 32 5D 5A C7 98 4B 46 33 36 30 30 43
150  31 38 44 34 2F 31 36 47 58 20 20 20 20 00 80 2C
160  45 00 00 00 00 00 00 00 00 00 00 00 45 00 00 88
170  07 39 35 35 33 37 30 33 00 00 00 01 00 00 00 00
180  0C 4A 17 20 00 00 00 00 00 A3 00 00 05 F8 0F 00
190  00 50 62 62 10 AD 78 F0 0A 20 08 00 05 00 9F 20
1A0  28 00 00 00 00 00 00 00 00 FB 8D 00 D8 D8 F6 BA
1B0  00 00 00 00 00 00 00 00 A3 00 00 06 F4 07 00 00
1C0  56 61 61 10 C0 56 F0 0A 20 08 00 05 00 C0 26 26
1D0  00 00 00 00 00 00 00 00 AF AF C2 89 89 B2 AD 00
1E0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
1F0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

---
通过软件解析得到一下数值：
![[_H__spd_Kingston%20KF3600C18D4-16GX.html.png]]

关于SPD中 memory timing 的计算：


| Manufacture | Module ID |
| ---- | ---- |
| KingBank | 0x920B |
| CUSO | 0xBC88 |
| Corsair | 0x9E02 |
| KingSton | 0x9801 |

| DRAM | DRAM ID |
| ---- | ---- |
| CXMT | 0x918A |
| Micron | 0x2C80 |
| Hynix | 0xAD80 |
| Samsung | 0xCE80 |
|  |  |
```C
static BOOLEAN GetChannelDimmtRAS (IN OUT MrcParameters *const MrcData)
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin
  UINT32                TimingMTB;
  INT32                 MediumTimebase;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  
  const SPD_EXTREME_MEMORY_PROFILE_DATA_2_0  *Data;
  UINT32                                     Index;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRASString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          tCKmin         = ChannelOut->Timing[Profile].tCK;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
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
              Index       = Profile - XMP_PROFILE1;
              switch (DimmOut->DdrType) {
                case MRC_DDR_TYPE_DDR4:
                  Data        = &Spd->Ddr4.EndUser.Xmp.Data[Index];
                  TimingMTB   = ((UINT32) (Data->tRASMintRCMinUpper.Bits.tRASminUpper) << 8) | (UINT32) (Data->tRASmin.Bits.tRASmin);
                  Calculated  = (tCKmin == 0) ? 0 : ((MediumTimebase * TimingMTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                  //yqr-tCAS debug-s
                  IoWrite32(0x72,0xC0);
                  IoWrite32(0x73,Calculated);
                  //yqr-tCAS debug-e
                  break;
                case MRC_DDR_TYPE_DDR5:
                  Calculated  = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Index].tRASmin.Bits.tRASmin, tCKmin);
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
                  Calculated  = PicoSecondsToClocks (Spd->Ddr5.Base.tRASmin.Bits.tRASmin, tCKmin);
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
            "  % 7u % 10u % 8u % 5u %6u\n",
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
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        ChannelOut->Timing[Profile].tRAS = (UINT16) Actual[Profile];
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }

  //yqr-tCAS debug-s
  IoWrite32(0x72,0xC4);
  IoWrite32(0x73,Actual[XMP_PROFILE1]);
  //yqr-tCAS debug-e
  
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
  
  return TRUE;

}
```

---
