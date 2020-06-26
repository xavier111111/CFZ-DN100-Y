#ifndef    __HAL_H__
#define    __HAL_H__

#include "system_define.h"


/*
**************************************************************************************************
*file：hal.h
*author：xavier 
*data：2020/6/23
*version：V1.0.0
*brief：hal抽象层相关数据结构及声明
***************************************************************************************************
*description:
*history：
*/


/** 
* @brief:hal data macro definition
*/

#define			LED1_GPIO_PIN		GPIO_Pin_15
#define			LED2_GPIO_PIN		GPIO_Pin_10
#define			LED3_GPIO_PIN		GPIO_Pin_11

#define			ALARM_GPIO_PIN		GPIO_Pin_1
#define			ALARM_GPORT			GPIOC

#define			KEY1_GPIO_PIN		GPIO_Pin_0
#define			KEY2_GPIO_PIN		GPIO_Pin_6
#define			KEY3_GPIO_PIN		GPIO_Pin_5
#define			KEY4_GPIO_PIN		GPIO_Pin_4

#define			WDWAKE_GPIO_PIN		GPIO_Pin_0
#define			WDDONE_GPIO_PIN		GPIO_Pin_1

#define			PLOW_GPIO_PIN		GPIO_Pin_1  //电源电压低检测









extern u16 CRC16(u8 *pdata, u8 len) ;
extern void hal_Init(void);
extern void led_DisplayDriver(u8 chan,FunctionalState en);





#endif

