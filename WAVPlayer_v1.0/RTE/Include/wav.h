
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WAV_H
#define __WAV_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

typedef struct
{
	uint8_t szRiffID[5]; 			//'R','I','F','F' 
	uint32_t dwRiffSize;			//去头文件大小，总数据-8
	uint8_t szRiffFormat[5]; 		//'W','A','V','E'
}RiffHeader;

typedef struct
{
	uint16_t wFormatag; 			//编码格式，为1时表示数据为线性PCM编码
	uint16_t wChannls;				//声道数，单为1，双为2
	uint32_t nSamplesPerSec;		//采样频率，11,025/22,050
	uint32_t dwAvgBytesperSec;		//每秒的数据量
	uint16_t wBlockAlign;			//每个采样需要的字节数，1/2
	uint16_t wBitsPerSample;		//每个采样所需要的bit数，8/16/32
}WaveFormat;

typedef struct
{
	uint8_t szFmtID[5]; 			//'f','m','t',' '
	uint32_t wBitsPerSample;		//WAVE文件的采样大小
	WaveFormat wf; 					//波形格式
}PcmWaveFormat;

typedef struct
{
	uint8_t szDataID[5];			//'d','a','t','a'
	uint32_t dwDataSize;			//数据段的大小
}DataBlack;

typedef struct
{
	uint8_t szFactID[5];			//'f','a','c','t'
	uint32_t dwFactSize;			//？？
}FactBlock;

typedef struct
{
	RiffHeader riffheader;
	PcmWaveFormat pcmwaveformat;
	FactBlock factblock;
	DataBlack datablack;
}WavHeader;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t Flash_Read_Choose_Wav_Num(uint32_t StartAddr, uint32_t Num, uint32_t BitNum);
void Flash_Read_Choose_Wav_Info(uint8_t *Info, uint32_t StartAddr, uint32_t Num, uint32_t BitNum);
void PrintWavHeader(WavHeader wavheader);
//uint32_t Wav_Init(WavHeader *wavheader, uint32_t StartAddr);
//void Play_Wav(WavHeader wavheader, uint32_t address);
void Time_Init(WavHeader wavheader);
//void Wav_Config(WavHeader *wavheader);

void WavPlay(WavHeader *wavheader, char *Apath);
void WavHeaderInit(WavHeader *wavheader);
void WavPlayConfig(WavHeader *wavheader, char *Apath);
void WAV_Info(uint8_t *Info, uint32_t Addr, uint32_t Num);
uint32_t WAV_Num(uint32_t Addr, uint32_t Byte);

#endif /* __WAV_H */

/*****************************END OF FILE**************************************/
