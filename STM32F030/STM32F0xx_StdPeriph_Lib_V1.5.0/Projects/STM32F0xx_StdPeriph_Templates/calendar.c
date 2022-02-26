#include "calendar.h"

RTC_TimeTypeDef VFD_Time;

void Calendar_Init(void)
{
  RTC_InitTypeDef RTC_Type;
  
  RTC_Type.RTC_AsynchPrediv = 10;
  RTC_Type.RTC_HourFormat = RTC_HourFormat_12;
  RTC_Type.RTC_SynchPrediv = 10;
  
  RTC_WriteProtectionCmd(DISABLE);
  RTC_EnterInitMode();
  RTC_DeInit();
  RTC_Init(&RTC_Type);
  RTC_ExitInitMode();
  RTC_WriteProtectionCmd(ENABLE);
  
  //RTC_GetTime();
}