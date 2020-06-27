
#include "usart6.h"

/*
**************************************************************************************************
*file：uart4.c
*author：xavier 
*date：2020/6/22
*version：V1.0.0
*brief：串口4模块底层驱动文件  提供与机旁RS485通信应用接口
*
***************************************************************************************************
*description:
*history：
*/


Usart6_Data_Struct  gl_Usart6Data;   //串口1全局数据



/*
**************************************************************************************************
*function：uart4_Init
*author：xavier 
*date：2020/6/22
*para: 无
*returnval:无
*brief：串口4硬件初始化配置
***************************************************************************************************
*description: baudrate=9600 datalen=8bit stopbits=1bit parity=no hardwareflow=no
*history：
*/
void usart6_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	u32 timeout=160000;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE); 

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_UART4); 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_UART4); 


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOC,&GPIO_InitStructure); 

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	USART_Init(USART6, &USART_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;	   
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   
	NVIC_Init(&NVIC_InitStructure);
	
	USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);          //使用DMA模式接收串口数据
	USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);          //使用DMA模式发送串口数据
	USART_Cmd(USART6, ENABLE); 
	
	USART_ClearFlag(USART6, USART_FLAG_TC |USART_FLAG_RXNE);
	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);  //使用串口空闲中断
	USART_ITConfig(USART6, USART_IT_TC, ENABLE);  //使用串口发送完成中断

	while((DMA_GetCmdStatus(DMA2_Stream2) != DISABLE)&&((timeout--)>0));  //为0时可以配置DMA相关寄存器
	
	DMA_DeInit(DMA2_Stream2);	
	// 从内存传输到外设， 外设地址不变， 内存地址递增， 数据长度可变    
	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART6->DR; // 外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)gl_Usart6Data.rx_DataBuff; // 存储器0地址 
	DMA_InitStructure.DMA_BufferSize = 0;   // 数据传输量
	DMA_InitStructure.DMA_DIR 	= DMA_DIR_PeripheralToMemory; // 外设到存储器 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc	   = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryDataSize	= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode	  = DMA_FIFOMode_Disable;			
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;  
	DMA_InitStructure.DMA_MemoryBurst 	= DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream2, ENABLE);
	
	while((DMA_GetCmdStatus(DMA2_Stream2) != DISABLE)&&((timeout--)>0));  //为0时可以配置DMA相关寄存器

	DMA_DeInit(DMA2_Stream6);
	// 从内存传输到外设， 外设地址不变， 内存地址递增， 数据长度可变    
	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART6->DR; // 外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)gl_Usart6Data.tx_DataBuff; // 存储器0地址 
	DMA_InitStructure.DMA_BufferSize = 0;   // 数据传输量
	DMA_InitStructure.DMA_DIR 	= DMA_DIR_MemoryToPeripheral; // 存储器到外设
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc	   = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryDataSize	= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode	  = DMA_FIFOMode_Disable;			
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;  
	DMA_InitStructure.DMA_MemoryBurst 	= DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream6, &DMA_InitStructure);

	// 开启中断
	#if 0
	DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);  
	// 与USART1同样的优先级 3/0
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;    
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	NVIC_Init(&NVIC_InitStructure);
	#endif
}

/*
**************************************************************************************************
*function：uart4_SendData
*author：xavier 
*date：2020/6/23
*para: pData-待发送的数据 len-待发送数据长度
*returnval:无
*brief：串口4数据发送
***************************************************************************************************
*description: 
*history：
*/
s8 uart4_SendData(const u8 *pData,u8 len)
{
	u8 index=0;
	s16 timeout=0;

	if((len<=0)||(len>=USART6_TXBUFF_SIZE))
		return -1;	
	
	DMA_Cmd(DMA2_Stream6, DISABLE);
	for(index=0;index<len;index++)
		gl_Usart6Data.tx_DataBuff[index]=pData[index];		
	for (timeout = 1600; (timeout>=0) && (DMA_GetCmdStatus(DMA2_Stream6) != DISABLE); timeout--) ;
	if (timeout<0)    {return -2;} 

	DMA_SetCurrDataCounter(DMA2_Stream6, len);
	DMA_Cmd(DMA2_Stream6, ENABLE);

	return 0;
}

/*
**************************************************************************************************
*function：UART4_IRQHandler
*author：xavier 
*date：2020/6/23
*para: 无
*returnval:无
*brief：串口4中断处理函数
***************************************************************************************************
*description: 
*history：
*/
void UART4_IRQHandler(void)
{
	u8 temp=0;
	
	if(USART_GetITStatus(USART6, USART_IT_IDLE)!=RESET)    //空闲中断产生 一帧已经传输完成
	{ 
		temp=USART6->SR;
		temp=USART6->DR;  //由软件序列清除IDLE标记 详见中文参考手册
		DMA_Cmd(DMA2_Stream2, DISABLE);   //关闭DMA通道
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);//清除传输完成标记
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TEIF2);//清除传输错误标记
		gl_Uart4Data.rx_Num=USART6_RXBUFF_SIZE-DMA_GetCurrDataCounter(DMA2_Stream2);
		gl_Uart4Data.rx_ParseFlag=1;  //解析标记 一帧已经结束
		gl_Uart4Data.rx_TimeOutCnt=0;  //清除超时计数
		
		DMA_SetCurrDataCounter(DMA2_Stream2,USART6_RXBUFF_SIZE);//设置传输长度
		DMA_Cmd(DMA2_Stream2, ENABLE);   //开启DMA通道
	}

	if(USART_GetITStatus(USART6, USART_IT_TC)!=RESET)
	{
		USART_ClearITPendingBit(USART6, USART_IT_TC);
	    DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6); // 传输完成标志	    
	    DMA_Cmd(DMA2_Stream6, DISABLE);
	}
}

/*
**************************************************************************************************
*function：DMA1_Stream4_IRQHandler
*author：xavier 
*date：2020/6/23
*para: 无
*returnval:无
*brief：DMA1_Stream4中断处理函数
***************************************************************************************************
*description: 
*history：
*/
void DMA2_Stream6_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA2_Stream6, DMA_FLAG_TCIF6) != RESET)
	{
	    DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6 | DMA_FLAG_FEIF6); // 传输完成标志
	    while(!USART_GetFlagStatus(USART6, USART_FLAG_TC));
	    USART_ClearFlag(USART6, USART_FLAG_TC);
	    DMA_Cmd(DMA2_Stream6, DISABLE);
	 }
}






















