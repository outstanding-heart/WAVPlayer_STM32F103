
/* Include ------------------------------------------------------------------- */
#include "diskio.h"
#include "ff.h"
#include "main.h"
#include "stm3210e_eval_sdio_sd.h"

/* Inidialize a Drive */
DSTATUS disk_initialize( BYTE drv)
{
	SD_Error Status;
	NVIC_Configuration();
	/* Supports only single drive */
	if (drv)
	{
		return STA_NOINIT;
	}
	
	/* SD Init */
	Status = SD_Init();
	if (Status != SD_OK)
	{
		SerialPutString("SD init Error\n");
		return STA_NOINIT;
	}
	else
	{
		SerialPutString("SD init OK\n");
		return RES_OK;
	}
}
/* Disk status */
DSTATUS disk_status( BYTE drv)
{
	SD_Error Status;
	/* Supports only single drive */
	if (drv)
	{
		return STA_NOINIT;
	}
	
	/* SD Init */
	Status = SD_Init();
	if (Status != SD_OK)
	{
		return STA_NOINIT;
	}
	else
	{
		return RES_OK;
	}
}


/* Read Sector(s) */

DRESULT disk_read( BYTE drv, BYTE *buff, DWORD sector, BYTE count )
{
	
	if (count > 1)
	{
		SD_ReadMultiBlocks(buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);
		SD_WaitReadOperation();
		
		while(SD_GetStatus() != SD_TRANSFER_OK);
	}
	
	else
	{
		
		SD_ReadBlock(buff, sector*BLOCK_SIZE, BLOCK_SIZE);
		SD_WaitReadOperation();
		
		while(SD_GetStatus() != SD_TRANSFER_OK);
	}
	return RES_OK;
	
}

/* Write Sector(s) */
#if __READONLY == 0
DRESULT disk_write( BYTE drv, const BYTE *buff, DWORD sector, BYTE count )
{
	if (count > 1)
	{
		SD_WriteMultiBlocks((uint8_t *)buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);
		SD_WaitWriteOperation();
		while(SD_GetStatus() != SD_TRANSFER_OK);
	}
	else
	{
		SD_WriteBlock((uint8_t *)buff, sector*BLOCK_SIZE, BLOCK_SIZE);
		
		SD_WaitWriteOperation();
		while(SD_GetStatus() != SD_TRANSFER_OK);
	}
	return RES_OK;
}

#endif

/* Miscellaneous Functions */
DRESULT disk_ioctl(BYTE drv, BYTE ctrl, void *buff )
{
	if (drv==0)
	{   
		switch (ctrl)
		{
			case CTRL_SYNC :
				return RES_OK;
			case GET_SECTOR_COUNT :
				return RES_OK;
			case GET_BLOCK_SIZE :
				return RES_OK;        
			case CTRL_POWER :
				break;
			case CTRL_LOCK :
				break;
			case CTRL_EJECT :
				break;
			/* MMC/SDC command */
			case MMC_GET_TYPE :
				break;
			case MMC_GET_CSD :
				break;
			case MMC_GET_CID :
				break;
			case MMC_GET_OCR :
				break;
			case MMC_GET_SDSTAT :
				break;        
		}
	}
	else if(drv==1)
	{
		switch (ctrl)
		{
			case CTRL_SYNC :
				return RES_OK;
			case GET_SECTOR_COUNT :
				return RES_OK;
			case GET_SECTOR_SIZE :
				return RES_OK;
			case GET_BLOCK_SIZE :
				return RES_OK;        
			case CTRL_POWER :
				break;
			case CTRL_LOCK :
				break;
			case CTRL_EJECT :
				break;
	/* MMC/SDC command */
			case MMC_GET_TYPE :
				break;
			case MMC_GET_CSD :
				break;
			case MMC_GET_CID :
				break;
			case MMC_GET_OCR :
				break;
			case MMC_GET_SDSTAT :
				break;        
		}         
	}
	else
	{                                 
		return RES_PARERR;  
	}
	return RES_PARERR;

}


/* Get current time */
DWORD get_fattime(void)
{
	return 0;
}
