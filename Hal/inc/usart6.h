#ifndef       __USART6_H__
#define	      __USART6_H__

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



#define		  USART6_RXBUFF_SIZE            100
#define		  USART6_TXBUFF_SIZE            64

/** 
* @brief:uart4 data structure defines
*/
typedef struct
{
	u8 rx_Num;        //接收数量
	u8 rx_ParseFlag;   //一帧解析标记
	u8 rx_DataBuff[USART6_RXBUFF_SIZE];	 //接收数据
	u8 tx_DataBuff[USART6_TXBUFF_SIZE];
	u16 rx_TimeOutCnt;  //接收超时计数
}Usart6_Data_Struct;

/** 
* @brief :uart4  exported data
*/
extern Usart6_Data_Struct  gl_Usart6Data;


/** 
* @brief uart4  exported function 
*/
extern void usart6_Init(void);
s8 usart6_SendData(const u8 *pData,u8 len);









#endif

