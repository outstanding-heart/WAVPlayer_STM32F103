
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm3210e_eval_sdio_sd.h"


uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];
uint8_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
SD_Error Status = SD_OK;

__IO uint32_t SDCardOperation = SD_OPERATION_ERASE;

/* Private function prototypes -----------------------------------------------*/

void SD_EraseTest(void);
void SD_SingleBlockTest(void);
void SD_MultiBlockTest(void);
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);
TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength);

/**
  * @brief  Control SD.
  * @param  None
  * @retval None
  */
void SD_Config(void)
{
	//char string[NumStrMax];
	
	NVIC_Configuration();
	Status = SD_Init();
	
	if (Status == SD_OK)
	{
		SerialPutString("SD_Init OK!\n");
	}
	else
	{
		SerialPutString("SD_Init Fail");
	}
	
	//PrintCardType();
	
	while((Status == SD_OK) && (SDCardOperation != SD_OPERATION_END) && (SD_Detect()== SD_PRESENT))
	{
		switch(SDCardOperation)
		{
			/*-------------------------- SD Erase Test ---------------------------- */
			case (SD_OPERATION_ERASE):
			{
				SerialPutString("Into Erase!\n");
				SD_EraseTest();
				SDCardOperation = SD_OPERATION_BLOCK;
				SerialPutString("Erase OK!\n");
				break;
			}
			/*-------------------------- SD Single Block Test --------------------- */
			case (SD_OPERATION_BLOCK):
			{
				SerialPutString("Into Single\n");
				SD_SingleBlockTest();
				SDCardOperation = SD_OPERATION_MULTI_BLOCK;
				SerialPutString("Single OK!\n");
				break;
			}       
			/*-------------------------- SD Multi Blocks Test --------------------- */
			case (SD_OPERATION_MULTI_BLOCK):
			{
				SerialPutString("Into Multi!\n");
				SD_MultiBlockTest();
				SDCardOperation = SD_OPERATION_END;
				SerialPutString("Multi OK!\n");
				break;
			}              
		}
	}
}

/**
  * @brief  Configures SDIO IRQ channel.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  Tests the SD card erase operation.
  * @param  None
  * @retval None
  */
void SD_EraseTest(void)
{
	/*------------------- Block Erase ------------------------------------------*/
	if (Status == SD_OK)
	{
		/* Erase NumberOfBlocks Blocks of WRITE_BL_LEN(512 Bytes) */
		Status = SD_Erase(0x00, (BLOCK_SIZE * NUMBER_OF_BLOCKS));
	}
	//

	if (Status == SD_OK)
	{
		Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);

		/* Check if the Transfer is finished */
		Status = SD_WaitReadOperation();

		/* Wait until end of DMA transfer */
		while(SD_GetStatus() != SD_TRANSFER_OK);
	}

	/* Check the correctness of erased blocks */
	if (Status == SD_OK)
	{
		EraseStatus = eBuffercmp(Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
	}

	if(EraseStatus == PASSED)
	{
		SerialPutString("Erase OK!\n");
	}
	else
	{
		SerialPutString("Erase Fail!\n");   
	}
}


/**
  * @brief  Tests the SD card Single Blocks operations.
  * @param  None
  * @retval None
  */
void SD_SingleBlockTest(void)
{
	/*------------------- Block Read/Write --------------------------*/
	/* Fill the buffer to send */	
	Fill_Buffer(Buffer_Block_Tx, BLOCK_SIZE, 0x320F);
	
	if (Status == SD_OK)
	{
		/* Write block of 512 bytes on address 0 */
		Status = SD_WriteBlock(Buffer_Block_Tx, 0x00, BLOCK_SIZE);
		/* Check if the Transfer is finished */
		Status = SD_WaitWriteOperation();
		while(SD_GetStatus() != SD_TRANSFER_OK);
	}

	if (Status == SD_OK)
	{
		/* Read block of 512 bytes from address 0 */
		Status = SD_ReadBlock(Buffer_Block_Rx, 0x00, BLOCK_SIZE);
		/* Check if the Transfer is finished */
		Status = SD_WaitReadOperation();
		while(SD_GetStatus() != SD_TRANSFER_OK);
	}

	/* Check the correctness of written data */
	if (Status == SD_OK)
	{
		TransferStatus1 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE);
	}

	if(TransferStatus1 == PASSED)
	{
		SerialPutString("SingleBlock Read-Write OK!\n");
	}
	else
	{
		SerialPutString("SingleBlock Read-Write Fail!\n");  
	}
}


/**
  * @brief  Tests the SD card Multiple Blocks operations.
  * @param  None
  * @retval None
  */
void SD_MultiBlockTest(void)
{
	/*--------------- Multiple Block Read/Write ---------------------*/
	/* Fill the buffer to send */
	Fill_Buffer(Buffer_MultiBlock_Tx, MULTI_BUFFER_SIZE, 0x0);

	if (Status == SD_OK)
	{
		/* Write multiple block of many bytes on address 0 */
		Status = SD_WriteMultiBlocks(Buffer_MultiBlock_Tx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
		/* Check if the Transfer is finished */
		Status = SD_WaitWriteOperation();
		while(SD_GetStatus() != SD_TRANSFER_OK);
	}

	if (Status == SD_OK)
	{
		/* Read block of many bytes from address 0 */
		Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
		/* Check if the Transfer is finished */
		Status = SD_WaitReadOperation();
		while(SD_GetStatus() != SD_TRANSFER_OK);
	}

	/* Check the correctness of written data */
	if (Status == SD_OK)
	{
		TransferStatus2 = Buffercmp(Buffer_MultiBlock_Tx, Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
	}

	if(TransferStatus2 == PASSED)
	{
		SerialPutString("MultiBlock Read-Write OK!\n");
	}
	else
	{
		SerialPutString("MultiBlock Read-Write Fail!\n");
	}
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
	while (BufferLength--)
	{
		if (*pBuffer1 != *pBuffer2)
		{
			return FAILED;
		}

		pBuffer1++;
		pBuffer2++;
	}

	return PASSED;
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferLength: size of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  * @retval None
  */
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
	uint16_t index = 0;

	/* Put in global buffer same values */
	for (index = 0; index < BufferLength; index++)
	{
		pBuffer[index] = index + Offset;
	}
}

/**
  * @brief  Checks if a buffer has all its values are equal to zero.
  * @param  pBuffer: buffer to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer values are zero
  *         FAILED: At least one value from pBuffer buffer is different from zero.
  */
TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength)
{
	while (BufferLength--)
	{
		/* In some SD Cards the erased state is 0xFF, in others it's 0x00 */
		if ((*pBuffer != 0xFF) && (*pBuffer != 0x00))
		{
			return FAILED;
		}

		pBuffer++;
	}

	return PASSED;
}

void SDIO_IRQHandler(void)
{
	SD_ProcessIRQSrc();
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{}
}
#endif