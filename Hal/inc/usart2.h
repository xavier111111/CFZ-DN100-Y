#ifndef    __USART2_H__
#define    __USART2_H__

#include "system_define.h"




/*
**************************************************************************************************
*file��uart4.h
*author��xavier 
*data��2020/6/23
*version��V1.0.0
*brief��uart4������ݽṹ������
***************************************************************************************************
*description:
*history��
*/


/** 
* @brief:uart4 data macro definition
*/

#define       RS485_REMOTE_DIR_EN          PAout(5)

#define		  USART2_RXBUFF_SIZE            100
#define		  USART2_TXBUFF_SIZE            64

/** 
* @brief:uart4 data structure defines
*/
typedef struct
{
	u8 rx_Num;        //��������
	u8 rx_ParseFlag;   //һ֡�������	
	u8 rx_DataBuff[USART2_RXBUFF_SIZE];	 //��������
	u8 tx_DataBuff[USART2_TXBUFF_SIZE];
	u16 rx_TimeOutCnt;  //���ճ�ʱ����
}Usart2_Data_Struct;

/** 
* @brief :uart4  exported data
*/
extern Usart2_Data_Struct  gl_Usart2Data;


/** 
* @brief uart4  exported function 
*/
extern void usart2_Init(void);
extern s8 usart2_SendData(const u8 *pData,u8 len);
















#endif

