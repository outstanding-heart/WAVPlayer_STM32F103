
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ff.h"

/* Private variables ---------------------------------------------------------*/
//FATFS fs;									//记录文件系统盘符信息的结构体
FILINFO finfo;
DIR dirs;
char path[32] = {"0:/蔡琴"};				//指定目录

/*----------------------------------------------------------------------------*/
void PrintList(PlayList List)
{
	char string[NumStrMax];
	int i;
	
	sprintf(string ,"\n%s path:\n", path);
	SerialPutString(string);
	sprintf(string, "There are %d files:\n", List.Number);
	for(i=0 ;i<List.Number; i++)
	{
		sprintf(string ,"ID: %d, Name: %s, Size: %ld\n" ,List.File[i].ID, List.File[i].Name, List.File[i].Size);
		SerialPutString(string);
	}
}

/* 读取播放目录并存入结构体中 ------------------------------------------------*/
void ShowAllFile(PlayList *PlayList)
{	
//	char string[NumStrMax];
	int res;								//读写文件的返回值
	int i;
	char *Name;
	
#if _USE_LFN
	char lfn[128];
	finfo.lfname = lfn;
	finfo.lfsize = sizeof(lfn);
#endif

	//支持中文和长文件名！！！！！初始化很重要
	(*PlayList).Number=0;								//初始化播放列表中的数目为0
	
	//打印指定文件目录
	res = f_opendir(&dirs, path);							//打开一个目录
	if(res == FR_OK)
	{		
		
		i = 0;

		while(f_readdir(&dirs, &finfo) == FR_OK)
		{
			if(!finfo.fname[0]) 
			{
				//SerialPutString("\n~~~!~~~\n");
				break;					//如果读到头，则跳出
			}
			if((finfo.fattrib != AM_DIR) && (finfo.fname[0] != '.') && (finfo.lfname[0] != '.'))
			{
#if _USE_LFN
				Name = *finfo.lfname ? finfo.lfname : finfo.fname;
#else
				Name = finfo.fname;
#endif				
//				SerialPutString(Name);
//				SerialPutString("\n");
				
				(*PlayList).File[i].ID = i;
				sprintf((*PlayList).File[i].Name, "%s", Name);
				sprintf((*PlayList).File[i].Apath, "%s/%s", path, Name);
				(*PlayList).File[i].Size = finfo.fsize;
				(*PlayList).Number++;
				i++;
				
			}
		}
	}

	
}
