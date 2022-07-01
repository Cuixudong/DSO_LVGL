/**
  ******************************************************************************
  * @file    sdio.c
  * @brief   This file provides code for the configuration
  *          of the SDIO instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sdio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SD_HandleTypeDef hsd;

/* SDIO init function */

void MX_SDIO_SD_Init(void)
{

  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 0;
  if (HAL_SD_Init(&hsd) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_SD_MspInit(SD_HandleTypeDef* sdHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(sdHandle->Instance==SDIO)
  {
  /* USER CODE BEGIN SDIO_MspInit 0 */

  /* USER CODE END SDIO_MspInit 0 */
    /* SDIO clock enable */
    __HAL_RCC_SDIO_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**SDIO GPIO Configuration
    PC8     ------> SDIO_D0
    PC9     ------> SDIO_D1
    PC10     ------> SDIO_D2
    PC11     ------> SDIO_D3
    PC12     ------> SDIO_CK
    PD2     ------> SDIO_CMD
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN SDIO_MspInit 1 */

  /* USER CODE END SDIO_MspInit 1 */
  }
}

void HAL_SD_MspDeInit(SD_HandleTypeDef* sdHandle)
{

  if(sdHandle->Instance==SDIO)
  {
  /* USER CODE BEGIN SDIO_MspDeInit 0 */

  /* USER CODE END SDIO_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SDIO_CLK_DISABLE();

    /**SDIO GPIO Configuration
    PC8     ------> SDIO_D0
    PC9     ------> SDIO_D1
    PC10     ------> SDIO_D2
    PC11     ------> SDIO_D3
    PC12     ------> SDIO_CK
    PD2     ------> SDIO_CMD
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);

  /* USER CODE BEGIN SDIO_MspDeInit 1 */

  /* USER CODE END SDIO_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
//SD_ReadDisk/SD_WriteDisk����ר��buf,�����������������ݻ�������ַ����4�ֽڶ����ʱ��,
//��Ҫ�õ�������,ȷ�����ݻ�������ַ��4�ֽڶ����.
__align(4) uint8_t SDIO_DATA_BUFFER[512];

HAL_SD_CardInfoTypeDef SDCardInfo;
DMA_HandleTypeDef SDTxDMAHandler,SDRxDMAHandler;    //SD��DMA���ͺͽ��վ��

#define SD_DMA_MODE         0
#define SD_TIMEOUT          ((uint32_t)100000000)   //��ʱʱ��
#define SD_TRANSFER_OK      ((uint8_t)0x00)
#define SD_TRANSFER_BUSY    ((uint8_t)0x01)

uint8_t Init_SD(void)
{
    hsd.Instance = SDIO;
    hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
    hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    hsd.Init.ClockDiv = 0;
    if (HAL_SD_Init(&hsd) != HAL_OK)
    {
        return 1;
    }
    if (HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK)
    {
        return 2;
    }
  
    HAL_SD_GetCardInfo(&hsd,&SDCardInfo);
#if (SD_DMA_MODE==1)                        //ʹ��DMAģʽ
    HAL_NVIC_SetPriority(SDMMC1_IRQn,2,0);  //����SDMMC1�жϣ���ռ���ȼ�2�������ȼ�0
    HAL_NVIC_EnableIRQ(SDMMC1_IRQn);        //ʹ��SDMMC1�ж�

    //���÷���DMA
    SDRxDMAHandler.Instance=DMA2_Stream3;
    SDRxDMAHandler.Init.Channel=DMA_CHANNEL_4;
    SDRxDMAHandler.Init.Direction=DMA_PERIPH_TO_MEMORY;
    SDRxDMAHandler.Init.PeriphInc=DMA_PINC_DISABLE;
    SDRxDMAHandler.Init.MemInc=DMA_MINC_ENABLE;
    SDRxDMAHandler.Init.PeriphDataAlignment=DMA_PDATAALIGN_WORD;
    SDRxDMAHandler.Init.MemDataAlignment=DMA_MDATAALIGN_WORD;
    SDRxDMAHandler.Init.Mode=DMA_PFCTRL;
    SDRxDMAHandler.Init.Priority=DMA_PRIORITY_VERY_HIGH;
    SDRxDMAHandler.Init.FIFOMode=DMA_FIFOMODE_ENABLE;
    SDRxDMAHandler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;
    SDRxDMAHandler.Init.MemBurst=DMA_MBURST_INC4;
    SDRxDMAHandler.Init.PeriphBurst=DMA_PBURST_INC4;

    __HAL_LINKDMA(hsd, hdmarx, SDRxDMAHandler); //������DMA��SD���ķ���DMA��������
    HAL_DMA_DeInit(&SDRxDMAHandler);
    HAL_DMA_Init(&SDRxDMAHandler);              //��ʼ������DMA

    //���ý���DMA
    SDTxDMAHandler.Instance=DMA2_Stream6;
    SDTxDMAHandler.Init.Channel=DMA_CHANNEL_4;
    SDTxDMAHandler.Init.Direction=DMA_MEMORY_TO_PERIPH;
    SDTxDMAHandler.Init.PeriphInc=DMA_PINC_DISABLE;
    SDTxDMAHandler.Init.MemInc=DMA_MINC_ENABLE;
    SDTxDMAHandler.Init.PeriphDataAlignment=DMA_PDATAALIGN_WORD;
    SDTxDMAHandler.Init.MemDataAlignment=DMA_MDATAALIGN_WORD;
    SDTxDMAHandler.Init.Mode=DMA_PFCTRL;
    SDTxDMAHandler.Init.Priority=DMA_PRIORITY_VERY_HIGH;
    SDTxDMAHandler.Init.FIFOMode=DMA_FIFOMODE_ENABLE;
    SDTxDMAHandler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;
    SDTxDMAHandler.Init.MemBurst=DMA_MBURST_INC4;
    SDTxDMAHandler.Init.PeriphBurst=DMA_PBURST_INC4;

    __HAL_LINKDMA(hsd, hdmatx, SDTxDMAHandler);//������DMA��SD���ķ���DMA��������
    HAL_DMA_DeInit(&SDTxDMAHandler);
    HAL_DMA_Init(&SDTxDMAHandler);              //��ʼ������DMA


    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 3, 0);  //����DMA�ж����ȼ�
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 3, 0);  //����DMA�ж����ȼ�
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
#endif
    return 0;
}

//�õ�����Ϣ
//cardinfo:����Ϣ�洢��
//����ֵ:����״̬
uint8_t SD_GetCardInfo(HAL_SD_CardInfoTypeDef *cardinfo)
{
    uint8_t sta;
    sta=HAL_SD_GetCardInfo(&hsd,cardinfo);
    return sta;
}

//�ж�SD���Ƿ���Դ���(��д)����
//����ֵ:SD_TRANSFER_OK ������ɣ����Լ�����һ�δ���
//       SD_TRANSFER_BUSY SD����æ�������Խ�����һ�δ���
uint8_t SD_GetCardState(void)
{
    return((HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER) ?\
        SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}

#if (SD_DMA_MODE==1)        //DMAģʽ

//ͨ��DMA��ȡSD��һ������
//buf:�����ݻ�����
//sector:������ַ
//blocksize:������С(һ�㶼��512�ֽ�)
//cnt:��������
//����ֵ:����״̬;0,����;����,�������;
uint8_t SD_ReadBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt)
{
    uint8_t err=0;
    uint32_t timeout=SD_TIMEOUT;
    err=HAL_SD_ReadBlocks_DMA(&hsd,(uint8_t*)buf,sector,cnt);//ͨ��DMA��ȡSD��n������

    if(err==0)
    {
        //�ȴ�SD������
        while(SD_GetCardState()!=SD_TRANSFER_OK)
        {
            if(timeout-- == 0)
            {
                err=SD_TRANSFER_BUSY;
            }
        }
    }
    return err;
}

//дSD��
//buf:д���ݻ�����
//sector:������ַ
//blocksize:������С(һ�㶼��512�ֽ�)
//cnt:��������
//����ֵ:����״̬;0,����;����,�������;
uint8_t SD_WriteBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt)
{
    uint8_t err=0;
    uint32_t timeout=SD_TIMEOUT;
    err=HAL_SD_WriteBlocks_DMA(&hsd,(uint8_t*)buf,sector,cnt);//ͨ��DMAдSD��n������

    if(err==0)
    {
        //�ȴ�SD��д��
        while(SD_GetCardState()!=SD_TRANSFER_OK)
        {
            if(timeout-- == 0)
            {
                err=SD_TRANSFER_BUSY;
            }
        }
    }
    return err;
}

//��SD��
//buf:�����ݻ�����
//sector:������ַ
//cnt:��������
//����ֵ:����״̬;0,����;����,�������;
uint8_t SD_ReadDisk(uint8_t* buf,uint32_t sector,uint32_t cnt)
{
    uint8_t sta=HAL_OK;
    long long lsector=sector;
    uint32_t timeout=SD_TIMEOUT;
    uint8_t n;

    sta=SD_ReadBlocks_DMA((uint32_t*)buf,lsector, 512,cnt);

    return sta;
}

//дSD��
//buf:д���ݻ�����
//sector:������ַ
//cnt:��������
//����ֵ:����״̬;0,����;����,�������;
uint8_t SD_WriteDisk(uint8_t *buf,uint32_t sector,uint32_t cnt)
{
    uint8_t sta=HAL_OK;
    long long lsector=sector;
    uint32_t timeout=SD_TIMEOUT;
    uint8_t n;

    sta=SD_WriteBlocks_DMA((uint32_t*)buf,lsector,512,cnt);//���sector��д����

    return sta;
}

//SDMMC1�жϷ�����
void SDMMC1_IRQHandler(void)
{
    HAL_SD_IRQHandler(&hsd);
}

//DMA2������6�жϷ�����
void DMA2_Stream6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(hsd.hdmatx);
}

//DMA2������3�жϷ�����
void DMA2_Stream3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(hsd.hdmarx);
}

#else                                   //��ѵģʽ

//��SD��
//buf:�����ݻ�����
//sector:������ַ
//cnt:��������
//����ֵ:����״̬;0,����;����,�������;
uint8_t SD_ReadDisk(uint8_t* buf,uint32_t sector,uint32_t cnt)
{
    uint8_t sta=HAL_OK;
    uint32_t timeout=SD_TIMEOUT;
    long long lsector=sector;
    DISABLE_INT();//�ر����ж�(POLLINGģʽ,�Ͻ��жϴ��SDIO��д����!!!)
    sta=HAL_SD_ReadBlocks(&hsd, (uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//���sector�Ķ�����

    //�ȴ�SD������
    while(SD_GetCardState()!=SD_TRANSFER_OK)
    {
        if(timeout-- == 0)
        {
            sta=SD_TRANSFER_BUSY;
        }
    }
    ENABLE_INT();//�������ж�
    return sta;
}

//дSD��
//buf:д���ݻ�����
//sector:������ַ
//cnt:��������
//����ֵ:����״̬;0,����;����,�������;
uint8_t SD_WriteDisk(uint8_t *buf,uint32_t sector,uint32_t cnt)
{
    uint8_t sta=HAL_OK;
    uint32_t timeout=SD_TIMEOUT;
    long long lsector=sector;
    DISABLE_INT();//�ر����ж�(POLLINGģʽ,�Ͻ��жϴ��SDIO��д����!!!)
    sta=HAL_SD_WriteBlocks(&hsd,(uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//���sector��д����

    //�ȴ�SD��д��
    while(SD_GetCardState()!=SD_TRANSFER_OK)
    {
        if(timeout-- == 0)
        {
            sta=SD_TRANSFER_BUSY;
        }
    }
    ENABLE_INT();//�������ж�
    return sta;
}

#endif

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
