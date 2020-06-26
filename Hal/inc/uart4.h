#ifndef    _UART4_H__
#define    _UART4_H__

#include "system_define.h"


/*
**************************************************************************************************
*file：uart4.h
*author：xavier 
*data：2020/6/23
*version：V1.0.0
*brief：uart4相关数据结构及声明
***************************************************************************************************
*description:
*history：
*/


/** 
* @brief:uart4 data macro definition
*/

#define       RS485_HOST_DIR_EN          PAout(4)

#define		  UART4_RXBUFF_SIZE            100
#define		  UART4_TXBUFF_SIZE            64

/** 
* @brief:uart4 data structure defines
*/
typedef struct
{
	u8 rx_Num;        //接收数量
	u8 rx_ParseFlag;   //一帧解析标记
	u8 rx_DataBuff[UART4_RXBUFF_SIZE];	 //接收数据
	u8 tx_DataBuff[UART4_TXBUFF_SIZE];
	u16 rx_TimeOutCnt;  //接收超时计数
}Uart4_Data_Struct;

/** 
* @brief :uart4  exported data
*/
extern Uart4_Data_Struct  gl_Uart4Data;


/** 
* @brief uart4  exported function 
*/
extern void uart4_Init(void);
s8 uart4_SendData(const u8 *pData,u8 len);



#endif

