#ifndef    _UART4_H__
#define    _UART4_H__

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

#define       RS485_HOST_DIR_EN          PAout(4)

#define		  UART4_RXBUFF_SIZE            100
#define		  UART4_TXBUFF_SIZE            64

/** 
* @brief:uart4 data structure defines
*/
typedef struct
{
	u8 rx_Num;        //��������
	u8 rx_ParseFlag;   //һ֡�������
	u8 rx_DataBuff[UART4_RXBUFF_SIZE];	 //��������
	u8 tx_DataBuff[UART4_TXBUFF_SIZE];
	u16 rx_TimeOutCnt;  //���ճ�ʱ����
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

