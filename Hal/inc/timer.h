#ifndef    __TIMER_H__
#define    __TIMER_H__


#include "system_define.h"






typedef struct
{
	u16 msCnt;
	u16 sCnt;
	u16 request_HostCnt;  //�������������
	u16 request_HostCnt1;  //�������������
	u16 request_HostCnt2;  //�������������
}Timer_Data_Struct;












extern Timer_Data_Struct  gl_SysTimeData;



extern void timer3_Init(void);



#endif





