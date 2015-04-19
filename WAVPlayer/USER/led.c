
#include "main.h"

/* LED≥ı ºªØ */
void LED_Init(void)
{

	GPIO_InitTypeDef   GPIO_InitStructure;

	/* Enable GPIOF clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	/* Configure PF.09 (LED4) in analog mode */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOF, GPIO_Pin_9);

}

void LED_Play(void)
{
	int i;
	while(1)
	{
		GPIO_ResetBits(GPIOF, GPIO_Pin_9);
		for(i=0; i<100000; i++)
		{}
		GPIO_SetBits(GPIOF, GPIO_Pin_9);
		for(i=0; i<100000; i++)
		{}
			
	}
}

