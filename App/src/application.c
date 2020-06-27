#include "application.h"

void application_Init(void)
{
	hal_Init();
	timer3_Init();
	uart4_Init();
	uart5_Init();
	usart2_Init();
}

void test_KeyLed(void)
{
	if(gl_SysTimeData.msCnt<=500)
	{
		led_DisplayDriver(1, ENABLE);
		led_DisplayDriver(2, ENABLE);
		led_DisplayDriver(3, ENABLE);
	}
	else
	{
		led_DisplayDriver(1,DISABLE);
		led_DisplayDriver(2,DISABLE);
		led_DisplayDriver(3, DISABLE);
	}
}

u8 testarry[11]={1,2,3,4,5,6,5,4,3,2,1};
void test_Rs485(void)
{
	#if  1
		if(gl_SysTimeData.request_HostCnt>=1000)
		{
			uart4_SendData(testarry, 11);
			gl_SysTimeData.request_HostCnt=0;
		}
	#endif

	#if 0
	if(gl_Uart4Data.rx_ParseFlag)
	{
		gl_Uart4Data.rx_ParseFlag=0;
		uart4_SendData(&gl_Uart4Data.rx_DataBuff[2], gl_Uart4Data.rx_Num-4);
	}
	#endif
	#if  1
	if(gl_SysTimeData.request_HostCnt1>=1000)
	{
		uart5_SendData(testarry, 11);
		gl_SysTimeData.request_HostCnt1=0;
	}
	#endif
	#if 1
	if(gl_Uart5Data.rx_ParseFlag)
	{
		gl_Uart5Data.rx_ParseFlag=0;
		uart5_SendData(&gl_Uart5Data.rx_DataBuff[2], gl_Uart5Data.rx_Num-4);
	}
	#endif
	#if  1
	if(gl_SysTimeData.request_HostCnt2>=1000)
	{
		usart2_SendData(testarry, 11);
		gl_SysTimeData.request_HostCnt2=0;
	}
	#endif
	#if 0
	if(gl_Usart2Data.rx_ParseFlag)
	{
		gl_Usart2Data.rx_ParseFlag=0;
		usart2_SendData(&gl_Usart2Data.rx_DataBuff[2], gl_Usart2Data.rx_Num-4);
	}
	#endif
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET) 
	{
		gl_SysTimeData.msCnt++;
		if(gl_SysTimeData.msCnt++>=1000)
		{
			gl_SysTimeData.msCnt=0;
			gl_SysTimeData.sCnt++;
		}
		gl_SysTimeData.request_HostCnt++;
		gl_SysTimeData.request_HostCnt1++;
		gl_SysTimeData.request_HostCnt2++;
		gl_Uart4Data.rx_TimeOutCnt++;
		if(gl_Uart4Data.rx_TimeOutCnt>=8000)
			gl_Uart4Data.rx_TimeOutCnt=8000;
		gl_Uart5Data.rx_TimeOutCnt++;
		if(gl_Uart5Data.rx_TimeOutCnt>=8000)
			gl_Uart5Data.rx_TimeOutCnt=8000;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);	
}




