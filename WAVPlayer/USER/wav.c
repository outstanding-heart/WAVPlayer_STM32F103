
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "wav.h"

/* Private define ------------------------------------------------------------*/

#define Num_1 1
#define Num_4 4
#define Bit_1 1
#define Bit_2 2
#define Bit_4 4

#define volume 1																				//越小音量越大

/* Exported types ------------------------------------------------------------*/

/* Global variable */
uint32_t DataAddress;				//数据地址位
WavHeader Nowwavheader;

/* Init Wav */
// StartAddr: 相对开始地址
// 返回值为数据起始地址  
uint32_t Wav_Init(WavHeader *wavheader, uint32_t StartAddr)
{
	uint32_t Address;

	Address = StartAddr;

	//RIFF
	SerialPutString("\n");
	SerialPutString("\n");
	Flash_Read_Choose_Wav_Info((*wavheader).riffheader.szRiffID, Address, Num_4, Bit_1);		//'RIFF'
	(*wavheader).riffheader.szRiffID[4] = '\0';													//最后一位赋值为'\0'
	Address = Address + Num_4 * Bit_1;															//下一地址

	(*wavheader).riffheader.dwRiffSize = Flash_Read_Choose_Wav_Num(Address, Num_1, Bit_4);		//文件长度
	Address = Address + Num_1 * Bit_4;															//下一地址

	Flash_Read_Choose_Wav_Info((*wavheader).riffheader.szRiffFormat, Address, Num_4, Bit_1);	//'WAVE'
	(*wavheader).riffheader.szRiffFormat[4] = '\0';												//最后一位赋值为'\0'
	Address = Address + Num_4 * Bit_1;															//下一地址

	//FMT
	Flash_Read_Choose_Wav_Info((*wavheader).pcmwaveformat.szFmtID, Address, Num_4, Bit_1);		//'fmt' 
	(*wavheader).pcmwaveformat.szFmtID[4] = '\0';													//最后一位赋值为'\0'
	Address = Address + Num_4 * Bit_1;															//下一地址

	(*wavheader).pcmwaveformat.wBitsPerSample = Flash_Read_Choose_Wav_Num(Address, Num_1, Bit_4);//fmt大小
	Address = Address + Num_1 * Bit_4;															//下一地址

	(*wavheader).pcmwaveformat.wf.wFormatag = Flash_Read_Choose_Wav_Num(Address, Num_1, Bit_2);	//格式种类
	Address = Address + Num_1 * Bit_2;															//下一地址

	(*wavheader).pcmwaveformat.wf.wChannls = Flash_Read_Choose_Wav_Num(Address, Num_1, Bit_2);	//通道数
	Address = Address + Num_1 * Bit_2;															//下一地址

	(*wavheader).pcmwaveformat.wf.nSamplesPerSec = Flash_Read_Choose_Wav_Num(Address, Num_1, Bit_4);//采样频率
	Address = Address + Num_1 * Bit_4;															//下一地址

	(*wavheader).pcmwaveformat.wf.dwAvgBytesperSec = Flash_Read_Choose_Wav_Num(Address, Num_1, Bit_4);//传输速率
	Address = Address + Num_1 * Bit_4;															//下一地址

	(*wavheader).pcmwaveformat.wf.wBlockAlign = Flash_Read_Choose_Wav_Num(Address, Num_1, Bit_2);//DATA数据块长
	Address = Address + Num_1 * Bit_2;															//下一地址

	(*wavheader).pcmwaveformat.wf.wBitsPerSample = Flash_Read_Choose_Wav_Num(Address, Num_1, Bit_2);//PCM位宽
	Address = Address + Num_1 * Bit_2;															//下一地址
	
	//DATA
	Flash_Read_Choose_Wav_Info((*wavheader).datablack.szDataID, Address, Num_4, Bit_1);			//'data'
	(*wavheader).datablack.szDataID[4] = '\0';													//最后一位赋值为'\0'
	Address = Address + Num_4 * Bit_1;															//下一地址
	
	(*wavheader).datablack.dwDataSize = Flash_Read_Choose_Wav_Num(Address, Num_1, Bit_4);		//语音数据长度
	Address = Address + Num_1 * Bit_4;															//下一地址
	

	//PrintWavHeader(wavheader);
	return Address;
}

void PrintWavHeader(WavHeader Wavheader)
{
	char string[NumStrMax];
	SerialPutString("~~Read Start~~ ");
	
	SerialPutString("\n szRiffID: ");//1
	SerialPutString(Wavheader.riffheader.szRiffID);
	
	SerialPutString("\n dwRiffSize: ");//2
	sprintf(string, "%d", Wavheader.riffheader.dwRiffSize);
	SerialPutString(string);
	
	SerialPutString("\n szRiffFormat: ");//3
	SerialPutString(Wavheader.riffheader.szRiffFormat);
	
	SerialPutString("\n szFmtID: ");//4
	SerialPutString(Wavheader.pcmwaveformat.szFmtID);
	
	SerialPutString("\n wBitsPerSample: ");//5
	sprintf(string, "%d", Wavheader.pcmwaveformat.wBitsPerSample);
	SerialPutString(string);
	
	SerialPutString("\n wFormatag: ");//6
	sprintf(string, "%d", Wavheader.pcmwaveformat.wf.wFormatag);
	SerialPutString(string);
	
	SerialPutString("\n wChannls: ");//7
	sprintf(string, "%d", Wavheader.pcmwaveformat.wf.wChannls);
	SerialPutString(string);
	
	SerialPutString("\n nSamplesPerSec: ");//8
	sprintf(string, "%d", Wavheader.pcmwaveformat.wf.nSamplesPerSec);
	SerialPutString(string);
	
	SerialPutString("\n dwAvgBytesperSec: ");//9
	sprintf(string, "%d", Wavheader.pcmwaveformat.wf.dwAvgBytesperSec);
	SerialPutString(string);
	
	SerialPutString("\n wBlockAlign: ");//10
	sprintf(string, "%d", Wavheader.pcmwaveformat.wf.wBlockAlign);
	SerialPutString(string);
	
	SerialPutString("\n wBitsPerSample: ");//11
	sprintf(string, "%d", Wavheader.pcmwaveformat.wf.wBitsPerSample);
	SerialPutString(string);
	
	SerialPutString("\n szDataID: ");//12
	SerialPutString(Wavheader.datablack.szDataID);
	
	SerialPutString("\n dwDataSize: ");//13
	sprintf(string, "%d", Wavheader.datablack.dwDataSize);
	SerialPutString(string);
	
}


/* 定时器三中断初始化 */
void Time_Init(WavHeader wavheader)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	uint32_t nSamplesPerSec = wavheader.pcmwaveformat.wf.nSamplesPerSec;
	
	//PrintWavHeader(wavheader);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);									//APB1单频8MHz
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
	TIM_TimeBaseStructure.TIM_Period = 1000000/nSamplesPerSec ;								//定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = 71;       										//预分频值=(72M/1M)-1
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    										//时钟分频因子
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  							//计数模式
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
	//定时器初始化完成
	
	/* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Enable the usart1 receive interrupt */
	
}

void Wav_Config(WavHeader *wavheader)
{
	
	uint32_t  address;//, count;
	char string[NumStrMax], 
	*RIFF = "RIFF\0", *WAVE = "WAVE\0", *fmt = "fmt \0", *data = "data\0";
	
	address = Wav_Init(&(*wavheader), 0);
	
	if(strcmp((*wavheader).riffheader.szRiffID, RIFF) != 0)		//检测RIFF标志
	{
		SerialPutString("RIFF error!");
		while(1)
		{}
	}
	
	if(strcmp((*wavheader).riffheader.szRiffFormat, WAVE) != 0)	//检测WAVE标志
	{
		SerialPutString("WAVE error!");
		while(1)
		{}
	}
	
	if(strcmp((*wavheader).pcmwaveformat.szFmtID, fmt) != 0)		//检测fmt标志
	{
		SerialPutString("fmt error!");
		while(1)
		{}
	}
	
	if(strcmp((*wavheader).datablack.szDataID, data) != 0)			//检测data标志
	{
		SerialPutString("data error!");
		while(1)
		{}
	}
	
	sprintf(string, "%d ", address);
	SerialPutString("\n");
	SerialPutString(string);
	PrintWavHeader((*wavheader));
	Nowwavheader = (*wavheader);
	
	DAC1_Init();
	Time_Init(*wavheader);

	DataAddress = address;
	
}


/* 定时器3中断处理函数，负责DAC音频输出 */
// WavHeader wavheader;
void TIM3_IRQHandler()
{
	uint16_t Data16, data16, Data, data;
	uint8_t Data8, data8;
//	char string[NumStrMax];

	while(DataAddress < Nowwavheader.datablack.dwDataSize)					//只寻址数据
	{
		if(TIM_GetITStatus(TIM3 , TIM_IT_Update) == SET)
		{
			TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
			//SerialPutString("Tim3_IRQ ok\n");
			if(Nowwavheader.pcmwaveformat.wf.wChannls == 1)
			{
				if(Nowwavheader.pcmwaveformat.wf.wBitsPerSample == 8)		//单声道八位
				{
					Data8 = Flash_Read_Wav_Data(DataAddress, Bit_1) ;

					//data8 = Data8<<4;
					
					DAC_SetChannel1Data(DAC_Align_12b_R, Data8);
					DAC_SetChannel2Data(DAC_Align_12b_L, Data8);
					
					DataAddress = DataAddress + Bit_1;
				}
				else if(Nowwavheader.pcmwaveformat.wf.wBitsPerSample == 16)	//单声道十六位
				{
					Data = Flash_Read_Wav_Data(DataAddress, Bit_2);
					
					data = Data+(1<<15) ;
					data = data>>4;
					
					DAC_SetChannel1Data(DAC_Align_12b_L, data);
					DAC_SetChannel2Data(DAC_Align_12b_L, data);

					DataAddress = DataAddress + Bit_2;
				}
				else
				{
					SerialPutString("wBitsPerSample error!");
					while(1)
					{}
				}
			}
			else if(Nowwavheader.pcmwaveformat.wf.wChannls == 2)
			{
				if(Nowwavheader.pcmwaveformat.wf.wBitsPerSample == 8)		//立体声八位
				{
					Data = Flash_Read_Wav_Data(DataAddress, Bit_1) ;
					DAC_SetChannel1Data(DAC_Align_12b_R, Data);
					DataAddress = DataAddress + Bit_1;
					DAC_SetChannel2Data(DAC_Align_12b_R, Data);
					DataAddress = DataAddress + Bit_1;
				}
				else if(Nowwavheader.pcmwaveformat.wf.wBitsPerSample == 16)	//立体声十六位
				{
					Data = Flash_Read_Wav_Data(DataAddress, Bit_2);
					
					data = Data+(1<<15) ;
					DAC_SetChannel1Data(DAC_Align_12b_R, data/volume);
					DataAddress = DataAddress + Bit_2;
					DAC_SetChannel2Data(DAC_Align_12b_R, Data);
					DataAddress = DataAddress + Bit_2;
				}
				else
				{
					SerialPutString("wBitsPerSample error!");
					while(1)
					{}
				}
			}
			else
			{
				SerialPutString("wChannls error!");
				while(1)
				{}
			}
		}
	
		//sprintf(string, "%d ", Data);
		//SerialPutString(string);	
	}
	if (DataAddress == Nowwavheader.datablack.dwDataSize)
	{
		TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
	}
}


