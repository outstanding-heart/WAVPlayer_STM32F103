
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WAVSD_H
#define __WAVSD_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "wav.h"
#include "ff.h"

/* Private define ------------------------------------------------------------*/
#define Num_1 1
#define Num_4 4
#define Bit_1 1
#define Bit_2 2
#define Bit_4 4
#define BuffMax 1024*7

#define Setted 3							//刚打完
#define Setting 2							//正在打
#define Set 1								//上好膛
#define Noset 0								//未上膛

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	BYTE Data[BuffMax]; 			//Data of Buffer.
	uint32_t Read;					//The state of Read, Noset/Set/Setting/Setted.
	uint32_t Write;					//The state of Write. Noset/Set.
}Buffer;

#endif  /* __WAVSD_H */

/*****************************END OF FILE**************************************/
