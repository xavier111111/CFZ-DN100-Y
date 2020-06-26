#include "application.h"


/*
**************************************************************************************************
*file：main.c
*author：xavier 
*date：2020/6/19
*version：V1.0.0
*brief：主函数文件 
*chip:stm32f407vet6 
*flash:address 0x0800_0000-0x080F_FFFF  total:1Mbyte
*ccm sram: address:0x1000_0000-0x1000_FFFF    ccm sram total:64Kbyte 
*sram:    address:0x2000_0000-0x2001_FFFF     sram total:128Kbyte 
* HSE_VALUE:8MHZ  PLL_M:8  PLL_N 200  PLL_P 2
* SYSCLK=(HSE_VALUE/PLL_M)*PLL_N/PLL_P=100MHZ
***************************************************************************************************
*description:
*history：
*/









int main(void)
{
	application_Init();

	while(1)
	{
		test_KeyLed();
		test_Rs485();
	}
	
	return 0;
}


















 

