#include "uart5.h"


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


Uart5_Data_Struct  gl_Uart5Data;   //����4ȫ������



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
void uart5_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	u32 timeout=160000;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE); 

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); 


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOC,&GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOD,&GPIO_InitStructure); 

	//RS485����GPIO����	   PA7
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
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
	USART_Init(UART5, &USART_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;	   
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   
	NVIC_Init(&NVIC_InitStructure);
	
	USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE);          //ʹ��DMAģʽ���մ�������
	USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE);          //ʹ��DMAģʽ���ʹ�������
	USART_Cmd(UART5, ENABLE); 
	
	USART_ClearFlag(UART5, USART_FLAG_TC |USART_FLAG_RXNE);
	
	USART_ITConfig(UART5, USART_IT_IDLE , ENABLE);  //ʹ�ô��ڿ����жϺͷ�������ж�
	USART_ITConfig(UART5, USART_IT_TC, ENABLE);	//ʹ�ô��ڿ����жϺͷ�������ж�

	 while((DMA_GetCmdStatus(DMA1_Stream0) != DISABLE)&&((timeout--)>0));  //Ϊ0ʱ��������DMA��ؼĴ���

	DMA_DeInit(DMA1_Stream0);
	// ���ڴ洫�䵽���裬 �����ַ���䣬 �ڴ��ַ������ ���ݳ��ȿɱ�    
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&UART5->DR; // �����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)gl_Uart5Data.rx_DataBuff; // �洢��0��ַ 
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
	DMA_Init(DMA1_Stream0, &DMA_InitStructure);

	DMA_Cmd(DMA1_Stream0, ENABLE);
	
	timeout=160000;
	while((DMA_GetCmdStatus(DMA1_Stream7) != DISABLE)&&((timeout--)>0));  //Ϊ0ʱ��������DMA��ؼĴ���

	DMA_DeInit(DMA1_Stream7);
	// ���ڴ洫�䵽���裬 �����ַ���䣬 �ڴ��ַ������ ���ݳ��ȿɱ�    
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&UART5->DR; // �����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)gl_Uart5Data.tx_DataBuff; // �洢��0��ַ 
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
	DMA_Init(DMA1_Stream7, &DMA_InitStructure);

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

	RS485_DISP_DIR_EN=0;          //Ĭ��Ϊ����ģʽ
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
s8 uart5_SendData(const u8 *pData,u8 len)
{
	u8 index=0;
	s16 timeout=0;

	if((len<=0)||(len>=UART5_TXBUFF_SIZE))
		return -1;	
	
	RS485_DISP_DIR_EN=1;   //����Ϊ����ģʽ
	
	DMA_Cmd(DMA1_Stream7, DISABLE);
	for(index=0;index<len;index++)
		gl_Uart5Data.tx_DataBuff[index]=pData[index];		
	for (timeout = 1600; (timeout>=0) && (DMA_GetCmdStatus(DMA1_Stream7) != DISABLE); timeout--) ;
	if (timeout<0)    {RS485_DISP_DIR_EN=0;return -2;} 

	DMA_SetCurrDataCounter(DMA1_Stream7, len);
	DMA_Cmd(DMA1_Stream7, ENABLE);

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
void UART5_IRQHandler(void)
{
	u8 temp=0;
	
	if(USART_GetITStatus(UART5, USART_IT_IDLE)!=RESET)    //�����жϲ��� һ֡�Ѿ��������
	{ 
		temp=UART5->SR;
		temp=UART5->DR;  //������������IDLE��� ������Ĳο��ֲ�
		DMA_Cmd(DMA1_Stream0, DISABLE);   //�ر�DMAͨ��
		DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);//���������ɱ��
		DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TEIF0);//������������
		gl_Uart5Data.rx_Num=UART5_RXBUFF_SIZE-DMA_GetCurrDataCounter(DMA1_Stream0);
		gl_Uart5Data.rx_ParseFlag=1;  //������� һ֡�Ѿ�����
		gl_Uart5Data.rx_TimeOutCnt=0;  //�����ʱ����
		
		DMA_SetCurrDataCounter(DMA1_Stream0, UART5_RXBUFF_SIZE);//���ô��䳤��
		DMA_Cmd(DMA1_Stream0, ENABLE);   //����DMAͨ��
	}

	if(USART_GetITStatus(UART5, USART_IT_TC)!=RESET)
	{

		USART_ClearITPendingBit(UART5, USART_IT_TC);
	    DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7); // ������ɱ�־	    
	    DMA_Cmd(DMA1_Stream7, DISABLE);
	    RS485_DISP_DIR_EN=0;   //����Ϊ����ģʽ
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
void DMA1_Stream7_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_Stream7, DMA_FLAG_TCIF7) != RESET)
	{
	    DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_FEIF7); // ������ɱ�־
	    while(!USART_GetFlagStatus(UART5, USART_FLAG_TC));
	    USART_ClearFlag(UART5, USART_FLAG_TC);
	    DMA_Cmd(DMA1_Stream7, DISABLE);
	    RS485_DISP_DIR_EN=0;   //����Ϊ����ģʽ
	 }
}






















