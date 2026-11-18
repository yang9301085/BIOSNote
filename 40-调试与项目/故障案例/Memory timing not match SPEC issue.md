DDR4 内存实际 timing与XMP 不符
通过CPUZ 查看实际DDR4 tRAS等时序与XMP中不符
大部分时序都是根据tCK计算得到的

---
如 DDR4 SPD SPEC：

#### 8.1.18 Byte 17 (0x011): Timebases  
This byte, shown in Table 35, defines a value in picoseconds that represents the fundamental timebase for fine grain and medium  
grain timing calculations. These values are used as a multiplier for formulating subsequent timing parameters.  
8.1.18.1 Relating the MTB and FTB  
When a timing value tXX cannot be expressed by an integer number of MTB units, the SPD must be encoded using both the MTB  
and FTB. The Fine Offsets are encoded using a two’s complement value which, when multiplied by the FTB yields a positive or  
negative correction factor. Typically, for safety and for legacy compatibility, the MTB portion is rounded UP and the FTB correction  
is a negative value. The general algorithm for programming SPD values is:  

|  |  |
| ---- | ---- |
| Temp_val = tXX / MTB  <br>Remainder = Temp_val modulo 1  <br>Fine_Correction = 1 - Remainder  <br>if (Remainder == 0) then  <br>tXX(MTB) = Temp_val  <br>tXX(FTB) = 0  <br>else  <br>tXX(MTB) = ceiling (Temp_val)  <br>tXX(FTB) = Fine_Correction * MTB / FTB  <br>endif | // Calculate as real number  <br>// Determine if integer # MTBs  <br>// If needed, what correction  <br>// Integer # MTBs?  <br>// Convert to integer  <br>// No correction needed  <br>// Needs correction  <br>// Round up for safety in legacy systems  <br>// Correction is negative offset |
|  |  |

To recalculate the value of tXX from the SPD values, a general formula BIOSes may use is:  
tXX = tXX(MTB)  \*  MTB + tXX(FTB) \* FTB

---

如：计算tCL
```C
Calculated  = (tCKmin == 0) ? 0 : ((MediumTimeBase * TimingMTB) + (FineTimeBase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
```
