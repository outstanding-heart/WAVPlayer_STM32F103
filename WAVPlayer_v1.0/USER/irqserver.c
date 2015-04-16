
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private variables ---------------------------------------------------------*/

uint32_t VolumeSub;
uint32_t VolumeAdd;
uint32_t NextMusic;
uint32_t LastMusic;
uint32_t PlayWait;
uint32_t StopKey;

extern uint32_t volume;
extern int Chose;
extern int Number;
extern int DACdone;
extern int PlayIRQBuffer;
extern int PlayOrWait;					

/* 音量加 ------------------------------------------------------------------- */
void Volume_Sub(void)
{
	int i;

	if (volume < 8)
	{
		volume++;
		SerialPutString("音量 -\n");	
	}		
	for(i=0; i<200 ;i++);	
	
	VolumeSub = Noset;
		
}

/* 音量减 ------------------------------------------------------------------- */
void Volume_Add(void)
{
	int i;
	
	if (volume > 1)
	{
		volume--;
		SerialPutString("音量 +\n");
		
	}	
	for(i=0; i<100 ;i++);
	
	VolumeAdd = Noset;
	
}

/* 下一首 ------------------------------------------------------------------- */
void Next_Music(void)
{
	int i;
	
	/* 停止当前播放 */	
	DACdone = 2;
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
		
	if(Chose < (Number-1))
	{
		Chose++;
	}
	else if(Chose == (Number-1))					//读到播放列表中最后一首歌，返回第一首歌
	{
		SerialPutString("已到最后一首，将跳转至第一首歌\n");
		Chose = 0;
	}
	else
	{
		SerialPutString("Chose Error!\n");
	}
//	sprintf(string, "%d\n" ,Chose);
//	SerialPutString(string);
	SerialPutString("下一曲\n");
	/* 播放下一曲 */
	PlayIRQBuffer = Set; 
	for(i=0; i<100 ;i++);
	
	NextMusic = Noset;
	
}

/* 上一首 ---------------------------------------------------------- */
void Last_Music(void)
{
	int i;
		
	/* 停止当前播放 */
	DACdone = 2;
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
	
	if(Chose > 0)
	{
		Chose--;
	}
	else if(Chose == 0)
	{
		SerialPutString("已到第一首，将跳转至最后一首歌\n");
		Chose = Number - 1;
	}
//	sprintf(string, "%d\n" ,Chose);
//	SerialPutString(string);
	SerialPutString("上一曲\n");
	/* 播放上一曲 */
	PlayIRQBuffer = Set; 
	for(i=0; i<100 ;i++);
	
	LastMusic = Noset;
}

/* 暂停/继续播放 -------------------------------------------------------- */
void Play_Wait(void)
{
	int i;
	if((PlayOrWait == Set) && (DACdone != 2))
	{
		TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
		PlayOrWait = Noset;
		SerialPutString("暂停\n");
	}
	else if((PlayOrWait == Noset) && (DACdone != 2))
	{
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
		PlayOrWait = Set;
		SerialPutString("继续播放\n");
	}
	for(i=0; i<100 ;i++);
	
	PlayWait = Noset;
}

/* 停止播放 --------------------------------------------------------------- */
void Stop_Key(void)
{
	int i;
	if(DACdone != 2)
	{
		SerialPutString("停止\n");
		DACdone = 2;
		PlayIRQBuffer = Noset;
		TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
		for(i=0; i<100 ;i++);
	}
	
	StopKey = Noset;
}
