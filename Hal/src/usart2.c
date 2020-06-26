#include "usart2.h"

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


Usart2_Data_Struct  gl_Usart2Data;   //����4ȫ������



/*
**************************************************************************************************
*function��uart5_Init
*author��xavier 
*date��2020/6/22
*para: ��
*returnval:��
*brief������5Ӳ����ʼ������
***************************************************************************************************
*description: baudrate=9600 datalen=8bit stopbits=1bit parity=no hardwareflow=no
*history��
*/
void usart2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	u32 timeout=160000;

	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE); 

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	//RS485����GPIO����	   PA5
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
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
	USART_Init(USART2, &USART_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;	   
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   
	NVIC_Init(&NVIC_InitStructure);
	
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);          //ʹ��DMAģʽ���մ�������
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);          //ʹ��DMAģʽ���ʹ�������
	USART_Cmd(USART2, ENABLE); 
	
	USART_ClearFlag(USART2, USART_FLAG_TC |USART_FLAG_RXNE);
	
	USART_ITConfig(USART2, USART_IT_IDLE , ENABLE);  //ʹ�ô��ڿ����жϺͷ�������ж�
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);	//ʹ�ô��ڿ����жϺͷ�������ж�

	 while((DMA_GetCmdStatus(DMA1_Stream5) != DISABLE)&&((timeout--)>0));  //Ϊ0ʱ��������DMA��ؼĴ���

	DMA_DeInit(DMA1_Stream5);
	// ���ڴ洫�䵽���裬 �����ַ���䣬 �ڴ��ַ������ ���ݳ��ȿɱ�    
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR; // �����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)gl_Usart2Data.rx_DataBuff; // �洢��0��ַ 
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
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);

	DMA_Cmd(DMA1_Stream5, ENABLE);
	
	timeout=160000;
	while((DMA_GetCmdStatus(DMA1_Stream6) != DISABLE)&&((timeout--)>0));  //Ϊ0ʱ��������DMA��ؼĴ���

	DMA_DeInit(DMA1_Stream6);
	// ���ڴ洫�䵽���裬 �����ַ���䣬 �ڴ��ַ������ ���ݳ��ȿɱ�    
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR; // �����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)gl_Usart2Data.tx_DataBuff; // �洢��0��ַ 
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
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);

	// ����DMA����ж�
	#if    0
	DMA_ITConfig(DMA1_Stream7,DMA_IT_TC,ENABLE);  
	// ��USART1ͬ�������ȼ� 3/0
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream7_IRQn;    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;    
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	NVIC_Init(&NVIC_InitStructure);
	#endif

	RS485_REMOTE_DIR_EN=1;          //Ĭ��Ϊ����ģʽ
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
s8 usart2_SendData(const u8 *pData,u8 len)
{
	u8 index=0;
	s16 timeout=0;

	if((len<=0)||(len>=USART2_TXBUFF_SIZE))
		return -1;	
	
	RS485_REMOTE_DIR_EN=0;   //����Ϊ����ģʽ
	
	DMA_Cmd(DMA1_Stream6, DISABLE);
	for(index=0;index<len;index++)
		gl_Usart2Data.tx_DataBuff[index]=pData[index];		
	for (timeout = 1600; (timeout>=0) && (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE); timeout--) ;
	if (timeout<0)    {RS485_REMOTE_DIR_EN=1;return -2;} 

	DMA_SetCurrDataCounter(DMA1_Stream6, len);
	DMA_Cmd(DMA1_Stream6, ENABLE);

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
void USART2_IRQHandler(void)
{
	u8 temp=0;
	
	if(USART_GetITStatus(USART2, USART_IT_IDLE)!=RESET)    //�����жϲ��� һ֡�Ѿ��������
	{ 
		temp=USART2->SR;
		temp=USART2->DR;  //������������IDLE��� ������Ĳο��ֲ�
		DMA_Cmd(DMA1_Stream5, DISABLE);   //�ر�DMAͨ��
		DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);//���������ɱ��
		DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TEIF5);//������������
		gl_Usart2Data.rx_Num=USART2_RXBUFF_SIZE-DMA_GetCurrDataCounter(DMA1_Stream5);
		gl_Usart2Data.rx_ParseFlag=1;  //������� һ֡�Ѿ�����
		gl_Usart2Data.rx_TimeOutCnt=0;  //�����ʱ����
		
		DMA_SetCurrDataCounter(DMA1_Stream5, USART2_RXBUFF_SIZE);//���ô��䳤��
		DMA_Cmd(DMA1_Stream5, ENABLE);   //����DMAͨ��
	}

	if(USART_GetITStatus(USART2, USART_IT_TC)!=RESET)
	{

		USART_ClearITPendingBit(USART2, USART_IT_TC);
	    DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6); // ������ɱ�־	    
	    DMA_Cmd(DMA1_Stream6, DISABLE);
	    RS485_REMOTE_DIR_EN=1;   //����Ϊ����ģʽ
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
void DMA1_Stream6_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) != RESET)
	{
	    DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6 | DMA_FLAG_FEIF6); // ������ɱ�־
	    while(!USART_GetFlagStatus(USART2, USART_FLAG_TC));
	    USART_ClearFlag(USART2, USART_FLAG_TC);
	    DMA_Cmd(DMA1_Stream6, DISABLE);
	    RS485_REMOTE_DIR_EN=1;   //����Ϊ����ģʽ
	 }
}





















