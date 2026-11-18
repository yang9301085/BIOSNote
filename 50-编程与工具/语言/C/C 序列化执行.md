
通过结构体函数来实现
如：
```C
#include<stdio.h>

typedef struct
{
    int (*test_task) (void);
}Fun_Test;

int test1(){
    return 1;
}

int test2(){
    return 1;
}

int test3(){
    return 1;
}

int test4(){
    return 1;
}

int main(){

    int a=0;

    static const Fun_Test testTable[]={
        {test1},
        {test2},
        {test3},
        {test4}
    };

    for (int i = 0; i < (sizeof(testTable)/sizeof(testTable[0])); i++)
    {
        a+=testTable[i].test_task();
    }

    printf("a=%d\n",a);
    return 0;

}
```

实例：Intel MRC中对memory SPD timing的计算

```C


typedef struct {
  BOOLEAN (*mrc_task) (MrcParameters * const MrcData);
} SpdTimeCallTable;


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
    {GetChannelDimmtCK},      // Note: This must be done second as all other calculations are based on this.
    {GetChannelDimmtAA},
    {GetChannelDimmtCWL},
    {GetChannelDimmtRAS},
    {GetChannelDimmtRCD},
    {GetChannelDimmtRC},      // Note: This must be done after GetChannelDimmtRAS and GetChannelDimmtRCD
    {GetChannelDimmtREFI},
    {GetChannelDimmtRFC},
    {GetChannelDimmtRFCpb},
    {GetChannelDimmtRP},      // Note: This must be done after GetChannelDimmtRCD
    {GetChannelDimmtRPab},    // Note: This must be done after GetChannelDimmtRP
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
  
  BOOLEAN    Status;
  UINT8      Index;

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
```