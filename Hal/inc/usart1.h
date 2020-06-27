#ifndef    __USART1_H__
#define    __USART1_H__

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



#define		  USART1_RXBUFF_SIZE            100
#define		  USART1_TXBUFF_SIZE            64

/** 
* @brief:uart4 data structure defines
*/
typedef struct
{
	u8 rx_Num;        //��������
	u8 rx_ParseFlag;   //һ֡�������
	u8 rx_DataBuff[USART1_RXBUFF_SIZE];	 //��������
	u8 tx_DataBuff[USART1_TXBUFF_SIZE];
	u16 rx_TimeOutCnt;  //���ճ�ʱ����
}Usart1_Data_Struct;

/** 
* @brief :uart4  exported data
*/
extern Usart1_Data_Struct  gl_Usart1Data;


/** 
* @brief uart4  exported function 
*/
extern void usart1_Init(void);
s8 usart1_SendData(const u8 *pData,u8 len);
















#endif

