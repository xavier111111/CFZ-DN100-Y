#ifndef    _UART5_H__
#define    _UART5_H__

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

#define       RS485_DISP_DIR_EN          PAout(7)

#define		  UART5_RXBUFF_SIZE            100
#define		  UART5_TXBUFF_SIZE            64

/** 
* @brief:uart4 data structure defines
*/
typedef struct
{
	u8 rx_Num;        //��������
	u8 rx_ParseFlag;   //һ֡�������	
	u8 rx_DataBuff[UART5_RXBUFF_SIZE];	 //��������
	u8 tx_DataBuff[UART5_TXBUFF_SIZE];
	u16 rx_TimeOutCnt;  //���ճ�ʱ����
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


