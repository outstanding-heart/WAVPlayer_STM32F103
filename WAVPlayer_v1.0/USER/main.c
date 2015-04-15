
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "wavsd.h"
#include "wav.h"

/* Private variables ---------------------------------------------------------*/
uint32_t PlayIRQBuffer;			//串口1缓存值
uint32_t Tim3IRQBuffer;

FATFS fs;									//记录文件系统盘符信息的结构体
int Chose;									//所选的ID
int Number;									//歌曲数量
char *Apath;								//音乐文件的绝对路径
char *Name;									//音乐文件名

int main(void)
{ 
	
	char string[NumStrMax];

//	int i;
	WavHeader wavheader;
	PlayList Playlist;

	PlayIRQBuffer = Noset;
	Chose = 0;										//默认选择播放第一首歌
	
	USART1_Init();
	SerialPutString("\nUsart init!\n");

	EXTI_Play_Config();
	EXTI_LastNext_Config();
	SerialPutString("\nEXTI init!\n");
	
	f_mount(0, &fs);								//挂载文件系统
	
	ShowAllFile(&Playlist);							//从SD卡中读取指定目录下的文件列表
	PrintList(Playlist);
	Number = Playlist.Number;

	Apath = Playlist.File[Chose].Apath;
	Name = Playlist.File[Chose].Name;
	sprintf(string ,"This music is: %s\n" ,Name);
	SerialPutString(string);
	
	while(1)										//播放主循环
	{
		if (PlayIRQBuffer == Set)
		{	
			ShowAllFile(&Playlist);	
			sprintf(string, "%d\n", Set);
			SerialPutString(string);
			EXTI_Volume_Config();
			EXTI_PlayOrWait_Config();
			EXTI_Stop_Config();
			
			Apath = Playlist.File[Chose].Apath;
			Name = Playlist.File[Chose].Name;
			sprintf(string ,"This music is: %s" ,Name);
			SerialPutString(string);
			
			//SerialPutString("Recive!\n");

			//PrintList(PlayList);
			WavPlay(&wavheader, Apath);				//播放文件
			
			
			
		}
	}
}

/* 串口中断函数 */
void USART1_IRQHandler(void)
{
	uint8_t Key;
	char string[NumStrMax];

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{

		Key = USART_ReceiveData(USART1);

		SerialPutChar(Key);
		
		if ((Key >= '0')&&(Key <= (Number+'0')))
		{
			Chose = Key - '0';
			sprintf(string, "Chose = %d\n", Chose);
			SerialPutString(string);
			PlayIRQBuffer = Set; 
		}
		else 
		{
			SerialPutString("Bad value!\n");
		}
		
	}
}

/* Wake up 播放键中断服务函数 */
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		SerialPutString("播放\n");
		PlayIRQBuffer = Set; 
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

