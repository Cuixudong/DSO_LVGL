#ifndef _SDMMC_SDCARD_H
#define _SDMMC_SDCARD_H

#include "main.h"


#define SD_TIMEOUT          ((uint32_t)100000000)   //��ʱʱ��
#define SD_TRANSFER_OK      ((uint8_t)0x00)
#define SD_TRANSFER_BUSY    ((uint8_t)0x01)

#define SD_DMA_MODE         0                       //1��DMAģʽ��0����ѯģʽ   

extern SD_HandleTypeDef         SDCARD_Handler;     //SD�����
extern HAL_SD_CardInfoTypeDef   SDCardInfo;         //SD����Ϣ�ṹ��

uint8_t SD_Init(void);
uint8_t SD_GetCardInfo(HAL_SD_CardInfoTypeDef *cardinfo);
uint8_t SD_GetCardState(void);
uint8_t SD_ReadDisk(uint8_t* buf,uint32_t sector,uint32_t cnt);
uint8_t SD_WriteDisk(uint8_t *buf,uint32_t sector,uint32_t cnt);
uint8_t SD_ReadBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt);
uint8_t SD_WriteBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt);
#endif
