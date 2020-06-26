#ifndef    _UART5_H__
#define    _UART5_H__

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

#define       RS485_DISP_DIR_EN          PAout(7)

#define		  UART5_RXBUFF_SIZE            100
#define		  UART5_TXBUFF_SIZE            64

/** 
* @brief:uart4 data structure defines
*/
typedef struct
{
	u8 rx_Num;        //接收数量
	u8 rx_ParseFlag;   //一帧解析标记	
	u8 rx_DataBuff[UART5_RXBUFF_SIZE];	 //接收数据
	u8 tx_DataBuff[UART5_TXBUFF_SIZE];
	u16 rx_TimeOutCnt;  //接收超时计数
}Uart5_Data_Struct;

/** 
* @brief :uart4  exported data
*/
extern Uart5_Data_Struct  gl_Uart5Data;


/** 
* @brief uart4  exported function 
*/
extern void uart5_Init(void);
extern s8 uart5_SendData(const u8 *pData,u8 len);






#endif


