#include "uart4.h"

/*
**************************************************************************************************
*file��uart4.c
*author��xavier 
*date��2020/6/22
*version��V1.0.0
*brief������4ģ��ײ������ļ�  �ṩ�����RS485ͨ��Ӧ�ýӿ�
*
***************************************************************************************************
*description:
*history��
*/


Uart4_Data_Struct  gl_Uart4Data;   //����4ȫ������



/*
**************************************************************************************************
*function��uart4_Init
*author��xavier 
*date��2020/6/22
*para: ��
*returnval:��
*brief������4Ӳ����ʼ������
***************************************************************************************************
*description: baudrate=9600 datalen=8bit stopbits=1bit parity=no hardwareflow=no
*history��
*/
void uart4_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	u32 timeout=160000;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE); 

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4); 


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

	//RS485����GPIO����	   PA4
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	USART_Init(UART4, &USART_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;	   
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   
	NVIC_Init(&NVIC_InitStructure);
	
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);          //ʹ��DMAģʽ���մ�������
	USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);          //ʹ��DMAģʽ���ʹ�������
	USART_Cmd(UART4, ENABLE); 
	
	USART_ClearFlag(UART4, USART_FLAG_TC |USART_FLAG_RXNE);
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);  //ʹ�ô��ڿ����ж�
	USART_ITConfig(UART4, USART_IT_TC, ENABLE);  //ʹ�ô��ڿ����ж�

	while((DMA_GetCmdStatus(DMA1_Stream2) != DISABLE)&&((timeout--)>0));  //Ϊ0ʱ��������DMA��ؼĴ���
	
	DMA_DeInit(DMA1_Stream2);	
	// ���ڴ洫�䵽���裬 �����ַ���䣬 �ڴ��ַ������ ���ݳ��ȿɱ�    
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&UART4->DR; // �����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)gl_Uart4Data.rx_DataBuff; // �洢��0��ַ 
	DMA_InitStructure.DMA_BufferSize = 0;   // ���ݴ�����
	DMA_InitStructure.DMA_DIR 	= DMA_DIR_PeripheralToMemory; // ���赽�洢�� 
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
	DMA_Init(DMA1_Stream2, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream2, ENABLE);
	
	while((DMA_GetCmdStatus(DMA1_Stream4) != DISABLE)&&((timeout--)>0));  //Ϊ0ʱ��������DMA��ؼĴ���

	DMA_DeInit(DMA1_Stream4);
	// ���ڴ洫�䵽���裬 �����ַ���䣬 �ڴ��ַ������ ���ݳ��ȿɱ�    
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&UART4->DR; // �����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)gl_Uart4Data.tx_DataBuff; // �洢��0��ַ 
	DMA_InitStructure.DMA_BufferSize = 0;   // ���ݴ�����
	DMA_InitStructure.DMA_DIR 	= DMA_DIR_MemoryToPeripheral; // �洢��������
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
	DMA_Init(DMA1_Stream4, &DMA_InitStructure);

	// �����ж�
	#if 0
	DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);  
	// ��USART1ͬ�������ȼ� 3/0
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;    
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	NVIC_Init(&NVIC_InitStructure);
	#endif

	RS485_HOST_DIR_EN=1;          //Ĭ��Ϊ����ģʽ
}

/*
**************************************************************************************************
*function��uart4_SendData
*author��xavier 
*date��2020/6/23
*para: pData-�����͵����� len-���������ݳ���
*returnval:��
*brief������4���ݷ���
***************************************************************************************************
*description: 
*history��
*/
s8 uart4_SendData(const u8 *pData,u8 len)
{
	u8 index=0;
	s16 timeout=0;

	if((len<=0)||(len>=UART4_TXBUFF_SIZE))
		return -1;	
	
	RS485_HOST_DIR_EN=0;   //����Ϊ����ģʽ
	
	DMA_Cmd(DMA1_Stream4, DISABLE);
	for(index=0;index<len;index++)
		gl_Uart4Data.tx_DataBuff[index]=pData[index];		
	for (timeout = 1600; (timeout>=0) && (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE); timeout--) ;
	if (timeout<0)    {RS485_HOST_DIR_EN=1;return -2;} 

	DMA_SetCurrDataCounter(DMA1_Stream4, len);
	DMA_Cmd(DMA1_Stream4, ENABLE);

	return 0;
}

/*
**************************************************************************************************
*function��UART4_IRQHandler
*author��xavier 
*date��2020/6/23
*para: ��
*returnval:��
*brief������4�жϴ�����
***************************************************************************************************
*description: 
*history��
*/
void UART4_IRQHandler(void)
{
	u8 temp=0;
	
	if(USART_GetITStatus(UART4, USART_IT_IDLE)!=RESET)    //�����жϲ��� һ֡�Ѿ��������
	{ 
		temp=UART4->SR;
		temp=UART4->DR;  //������������IDLE��� ������Ĳο��ֲ�
		DMA_Cmd(DMA1_Stream2, DISABLE);   //�ر�DMAͨ��
		DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF2);//���������ɱ��
		DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TEIF2);//������������
		gl_Uart4Data.rx_Num=UART4_RXBUFF_SIZE-DMA_GetCurrDataCounter(DMA1_Stream2);
		gl_Uart4Data.rx_ParseFlag=1;  //������� һ֡�Ѿ�����
		gl_Uart4Data.rx_TimeOutCnt=0;  //�����ʱ����
		
		DMA_SetCurrDataCounter(DMA1_Stream2, UART4_RXBUFF_SIZE);//���ô��䳤��
		DMA_Cmd(DMA1_Stream2, ENABLE);   //����DMAͨ��
	}

	if(USART_GetITStatus(UART4, USART_IT_TC)!=RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_TC);
	    DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4); // ������ɱ�־	    
	    DMA_Cmd(DMA1_Stream4, DISABLE);
	    RS485_HOST_DIR_EN=1;   //����Ϊ����ģʽ
	}
}

/*
**************************************************************************************************
*function��DMA1_Stream4_IRQHandler
*author��xavier 
*date��2020/6/23
*para: ��
*returnval:��
*brief��DMA1_Stream4�жϴ�����
***************************************************************************************************
*description: 
*history��
*/
void DMA1_Stream4_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_Stream4, DMA_FLAG_TCIF4) != RESET)
	{
	    DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4 | DMA_FLAG_FEIF4); // ������ɱ�־
	    while(!USART_GetFlagStatus(UART4, USART_FLAG_TC));
	    USART_ClearFlag(UART4, USART_FLAG_TC);
	    DMA_Cmd(DMA1_Stream4, DISABLE);
	    RS485_HOST_DIR_EN=1;   //����Ϊ����ģʽ
	 }
}














