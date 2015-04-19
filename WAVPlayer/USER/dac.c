
#include "main.h"

/* DAC≥ı ºªØ */
void DAC1_Init(void)
{

	DAC_InitTypeDef    DAC_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	
	/* TIM2 Base Configuration */
	/* TIM2 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	/* Time base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
	TIM_TimeBaseStructure.TIM_Period = 0xFF;          
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;       
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	/* TIM2 TRGO selection */
	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
	/* TIM2 enable counter */
	TIM_Cmd(TIM6, ENABLE);


	/* Enable GPIOA clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/* Configure PA.04 (DAC_OUT1) in analog mode */ 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable DAC clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	/* DAC channel1 Configuration */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
	//DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;/*Disable;*/
	/* DAC Channel1 Init */
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	/* DAC Channel2 Init */
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);
	/* Enable DAC Channel1 */
	DAC_Cmd(DAC_Channel_1, ENABLE);
	/* Enable DAC Channel2 */
	DAC_Cmd(DAC_Channel_2, ENABLE);
	
//	DAC_SetChannel1Data(DAC_Align_12b_L, 0x7FF0);
//	while (1)
//	{
//    /* Start DAC Channel1 conversion by software */
//		DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
//	}
}

void DAC_Play(void)
{
//	uint32_t i;
	while(1)
	{
//		DAC_SetChannel1Data(DAC_Align_8b_R, 255);
//		for(i=0; i<10000; i++)
//		{}

		DAC_SetChannel1Data(DAC_Align_12b_R, 4095);
//		for(i=0; i<10000; i++)
//		{}
	}

}
