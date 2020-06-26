#ifndef    __TIMER_H__
#define    __TIMER_H__


#include "system_define.h"






typedef struct
{
	u16 msCnt;
	u16 sCnt;
	u16 request_HostCnt;  //向主机请求计数
	u16 request_HostCnt1;  //向主机请求计数
	u16 request_HostCnt2;  //向主机请求计数
}Timer_Data_Struct;












extern Timer_Data_Struct  gl_SysTimeData;



extern void timer3_Init(void);



#endif





