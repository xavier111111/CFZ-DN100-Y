#include "timer.h"


Timer_Data_Struct  gl_SysTimeData;



/*
*SYSCLK=100MHZ APB1=25MHZ 定时器频率=APB1*2=50mhz
* 定时周期=(period+1)*(pre+1)/(50000000)
*=(100)*(500)/(50000000)=0.001s=1ms 
*/
void timer3_Init(void)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  

	TIM_TimeBaseInitStructure.TIM_Period = 100-1;	
	TIM_TimeBaseInitStructure.TIM_Prescaler=500-1;  
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 
	TIM_Cmd(TIM3,ENABLE); 

	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



















