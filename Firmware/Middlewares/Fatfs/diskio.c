/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h" /* FatFs lower layer API */
#include "w25qxx.h"
#include "malloc.h"
#include "sdio.h"

#define     EX_FLASH                1    //�ⲿflash,���Ϊ0
#define     SD_CARD                 0    //SDcard
#define     FLASH_SECTOR_SIZE       512
uint32_t    FLASH_SECTOR_COUNT =    12*1024*2;    //12M�ֽ�,Ĭ��ΪW25Q128
#define     FLASH_BLOCK_SIZE        8         //ÿ��BLOCK��8������

//��ʼ������
DSTATUS disk_initialize (
    BYTE pdrv                /* Physical drive nmuber (0..) */
)
{
    uint8_t res=0;
    switch(pdrv)
    {
    case EX_FLASH://�ⲿflash
        W25QXX_Init();
        if(W25QXX_TYPE==W25Q128)
        {
            FLASH_SECTOR_COUNT=12*1024*2;    //W25Q128
        }
        break;
    case SD_CARD:       //SD��
        res=Init_SD();  //SD����ʼ��
        break;
    default:
        res=1;
    }
    if(res)
        return  STA_NOINIT;
    else 
        return RES_OK; //��ʼ���ɹ�
}

//��ô���״̬
DSTATUS disk_status (
    BYTE pdrv        /* Physical drive nmuber (0..) */
)
{
    return RES_OK;
}

//������
//drv:���̱��0~9
//*buff:���ݽ��ջ����׵�ַ
//sector:������ַ
//count:��Ҫ��ȡ��������
DRESULT disk_read (
    BYTE pdrv,        /* Physical drive nmuber (0..) */
    BYTE *buff,        /* Data buffer to store read data */
    DWORD sector,    /* Sector address (LBA) */
    UINT count        /* Number of sectors to read (1..128) */
)
{
    uint8_t res=0;
    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������
    switch(pdrv)
    {
    case EX_FLASH://�ⲿflash
        for(; count>0; count--)
        {
            W25QXX_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
            sector++;
            buff+=FLASH_SECTOR_SIZE;
        }
        res=RES_OK;
        break;
    case SD_CARD://SD��
        res=SD_ReadDisk(buff,sector,count);
        while(res)//������
        {
            Init_SD();  //���³�ʼ��SD��
            res=SD_ReadDisk(buff,sector,count);
            //printf("sd rd error:%d\r\n",res);
        }
        break;
    default:
        res=RES_ERROR;
    }
    //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    return res;
}

//д����
//drv:���̱��0~9
//*buff:���������׵�ַ
//sector:������ַ
//count:��Ҫд���������
#if _USE_WRITE
DRESULT disk_write (
    BYTE pdrv,            /* Physical drive nmuber (0..) */
    const BYTE *buff,    /* Data to be written */
    DWORD sector,        /* Sector address (LBA) */
    UINT count            /* Number of sectors to write (1..128) */
)
{
    uint8_t res=0;
    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������
    switch(pdrv)
    {
    case EX_FLASH://�ⲿflash
        for(; count>0; count--)
        {
            W25QXX_Write((uint8_t*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
            sector++;
            buff+=FLASH_SECTOR_SIZE;
        }
        res=RES_OK;
        break;
    case SD_CARD://SD��
        res=SD_WriteDisk((uint8_t *)buff,sector,count);
        while(res)//д����
        {
            Init_SD();  //���³�ʼ��SD��
            res=SD_WriteDisk((uint8_t *)buff,sector,count);
        }
        break;
    default:
        res=RES_ERROR;
    }
    //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    return res;
}
#endif


//����������Ļ��
//drv:���̱��0~9
//ctrl:���ƴ���
//*buff:����/���ջ�����ָ��
#if _USE_IOCTL
DRESULT disk_ioctl (
    BYTE pdrv,        /* Physical drive nmuber (0..) */
    BYTE cmd,        /* Control code */
    void *buff        /* Buffer to send/receive control data */
)
{
    DRESULT res;
    if(pdrv==SD_CARD)//SD��
    {
        switch(cmd)
        {
        case CTRL_SYNC:
            res = RES_OK;
            break;
        case GET_SECTOR_SIZE:
            *(DWORD*)buff = 512;
            res = RES_OK;
            break;
        case GET_BLOCK_SIZE:
            *(WORD*)buff = SDCardInfo.LogBlockSize;
            res = RES_OK;
            break;
        case GET_SECTOR_COUNT:
            *(DWORD*)buff = SDCardInfo.LogBlockNbr;
            res = RES_OK;
            break;
        default:
            res = RES_PARERR;
            break;
        }
    } 
    else if(pdrv==EX_FLASH)  //�ⲿFLASH
    {
        switch(cmd)
        {
        case CTRL_SYNC:
            res = RES_OK;
            break;
        case GET_SECTOR_SIZE:
            *(WORD*)buff = FLASH_SECTOR_SIZE;
            res = RES_OK;
            break;
        case GET_BLOCK_SIZE:
            *(WORD*)buff = FLASH_BLOCK_SIZE;
            res = RES_OK;
            break;
        case GET_SECTOR_COUNT:
            *(DWORD*)buff = FLASH_SECTOR_COUNT;
            res = RES_OK;
            break;
        default:
            res = RES_PARERR;
            break;
        }
    } else res=RES_ERROR;//�����Ĳ�֧��
    return res;
}
#endif
//���ʱ��
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */
DWORD get_fattime (void)
{
    return 0;
}
//��̬�����ڴ�
void *ff_memalloc (UINT size)
{
    return (void*)mymalloc(size,SRAMIN);
}
//�ͷ��ڴ�
void ff_memfree (void* mf)
{
    myfree(SRAMIN,mf);
}
