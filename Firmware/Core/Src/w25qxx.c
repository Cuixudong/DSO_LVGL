#include "w25qxx.h"
#include "spi.h"

uint16_t W25QXX_TYPE = W25Q128;//Ĭ�Ͼ���25Q128

//SPI �ٶ����ú���
void SPI1_SetSpeed(uint8_t SpeedSet)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SpeedSet));//�ж���Ч��
    __HAL_SPI_DISABLE(&hspi1);  //�ر�SPI
    hspi1.Instance->CR1&=0XFFC7;    //λ3-5���㣬�������ò�����
    hspi1.Instance->CR1|=SpeedSet;//����SPI�ٶ�
    __HAL_SPI_ENABLE(&hspi1);   //ʹ��SPI
}

//SPIx ��дһ���ֽ�
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{
    uint8_t  Rxdata;
    HAL_SPI_TransmitReceive(&hspi1,&TxData,&Rxdata,1, 1000);
    return Rxdata;  //�����յ�������
}

void FLASH_Delay(uint16_t i)
{
    uint32_t t;
    t=i*6;
    for(; t>0; t--)
    {
        ;
    }
}

//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25Q64
//����Ϊ8M�ֽ�,����128��Block,2048��Sector

//��ʼ��SPI FLASH��IO��
void W25QXX_Init(void)
{
    uint8_t t = 0;
    SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_4); //����Ϊ18Mʱ��,����ģʽ
    W25QXX_TYPE=W25QXX_ReadID();//��ȡFLASH ID.
    do
    {
        HAL_Delay(100);
        W25QXX_TYPE=W25QXX_ReadID();//��ȡFLASH ID.
        t ++;
    }while((W25QXX_TYPE != W25Q128) && (t < 20));
}

//��ȡW25QXX��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
uint8_t W25QXX_ReadSR(void)
{
    uint8_t byte=0;
    W25QXX_CS_LOW;  //ʹ������
    SPI1_ReadWriteByte(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������
    byte=SPI1_ReadWriteByte(0Xff);             //��ȡһ���ֽ�
    W25QXX_CS_HIGH; //ȡ��Ƭѡ
    return byte;
}
//дW25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void W25QXX_Write_SR(uint8_t sr)
{
    W25QXX_CS_LOW;  //ʹ������
    SPI1_ReadWriteByte(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������
    SPI1_ReadWriteByte(sr);               //д��һ���ֽ�
    W25QXX_CS_HIGH; //ȡ��Ƭѡ
}
//W25QXXдʹ��
//��WEL��λ
void W25QXX_Write_Enable(void)
{
    W25QXX_CS_LOW;  //ʹ������
    SPI1_ReadWriteByte(W25X_WriteEnable);   //����дʹ��
    W25QXX_CS_HIGH; //ȡ��Ƭѡ
}
//W25QXXд��ֹ
//��WEL����
void W25QXX_Write_Disable(void)
{
    W25QXX_CS_LOW;  //ʹ������
    SPI1_ReadWriteByte(W25X_WriteDisable);     //����д��ָֹ��
    W25QXX_CS_HIGH; //ȡ��Ƭѡ
}
//��ȡоƬID W25X16��ID:0XEF14
uint16_t W25QXX_ReadID(void)
{
    uint16_t Temp = 0;
    W25QXX_CS_LOW;
    SPI1_ReadWriteByte(0x90);//���Ͷ�ȡID����
    SPI1_ReadWriteByte(0x00);
    SPI1_ReadWriteByte(0x00);
    SPI1_ReadWriteByte(0x00);
    Temp|=SPI1_ReadWriteByte(0xFF)<<8;
    Temp|=SPI1_ReadWriteByte(0xFF);
    W25QXX_CS_HIGH;
    return Temp;
}
//��ȡSPI FLASH
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
    uint16_t i;
    W25QXX_CS_LOW;  //ʹ������
    SPI1_ReadWriteByte(W25X_ReadData);         //���Ͷ�ȡ����
    SPI1_ReadWriteByte((uint8_t)((ReadAddr)>>16));  //����24bit��ַ
    SPI1_ReadWriteByte((uint8_t)((ReadAddr)>>8));
    SPI1_ReadWriteByte((uint8_t)ReadAddr);
    for(i=0; i<NumByteToRead; i++)
    {
        pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //ѭ������
    }
    W25QXX_CS_HIGH; //ȡ��Ƭѡ
}
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    uint16_t i;
    W25QXX_Write_Enable();  //SET WEL
    W25QXX_CS_LOW;  //ʹ������
    SPI1_ReadWriteByte(W25X_PageProgram);   //����дҳ����
    SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>16)); //����24bit��ַ
    SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>8));
    SPI1_ReadWriteByte((uint8_t)WriteAddr);
    for(i=0; i<NumByteToWrite; i++)SPI1_ReadWriteByte(pBuffer[i]); //ѭ��д��
    W25QXX_CS_HIGH; //ȡ��Ƭѡ
    W25QXX_Wait_Busy(); //�ȴ�д�����
}
//�޼���дSPI FLASH
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ����
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    uint16_t pageremain;
    pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���
    if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
    while(1)
    {
        W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
        if(NumByteToWrite==pageremain)break;//д�������
        else //NumByteToWrite>pageremain
        {
            pBuffer+=pageremain;
            WriteAddr+=pageremain;

            NumByteToWrite-=pageremain; //��ȥ�Ѿ�д���˵��ֽ���
            if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
            else pageremain=NumByteToWrite; //����256���ֽ���
        }
    };
}
//дSPI FLASH
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
#ifndef MEM_ALLOC_TABLE_SIZE
uint8_t W25QXX_BUFFER[4096];
#endif
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t * W25QXX_BUF;
#ifdef MEM_ALLOC_TABLE_SIZE
    W25QXX_BUF=mymalloc(4096);  //ʹ���ڴ����
    if(W25QXX_BUF==NULL)return; //����ʧ��
#else
    W25QXX_BUF=W25QXX_BUFFER;   //��ʹ���ڴ����
#endif
    secpos=WriteAddr/4096;//������ַ 0~511 for w25x16
    secoff=WriteAddr%4096;//�������ڵ�ƫ��
    secremain=4096-secoff;//����ʣ��ռ��С
    if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
    while(1)
    {
        W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//������������������
        for(i=0; i<secremain; i++) //У������
        {
            if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����
        }
        if(i<secremain)//��Ҫ����
        {
            W25QXX_Erase_Sector(secpos);//�����������
            for(i=0; i<secremain; i++)  //����
            {
                W25QXX_BUF[i+secoff]=pBuffer[i];
            }
            W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������

        } else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.
        if(NumByteToWrite==secremain)break;//д�������
        else//д��δ����
        {
            secpos++;//������ַ��1
            secoff=0;//ƫ��λ��Ϊ0

            pBuffer+=secremain;  //ָ��ƫ��
            WriteAddr+=secremain;//д��ַƫ��
            NumByteToWrite-=secremain;  //�ֽ����ݼ�
            if(NumByteToWrite>4096)secremain=4096;  //��һ����������д����
            else secremain=NumByteToWrite;  //��һ����������д����
        }
    };
#ifdef MEM_ALLOC_TABLE_SIZE
    myfree(W25QXX_BUF); //�ͷ��ڴ�
#endif
}
//��������оƬ
//��Ƭ����ʱ��:
//W25X16:25s
//W25X32:40s
//W25X64:40s
//�ȴ�ʱ�䳬��...
void W25QXX_Erase_Chip(void)
{
    W25QXX_Write_Enable();  //SET WEL
    W25QXX_Wait_Busy();
    W25QXX_CS_LOW;  //ʹ������
    SPI1_ReadWriteByte(W25X_ChipErase); //����Ƭ��������
    W25QXX_CS_HIGH; //ȡ��Ƭѡ
    W25QXX_Wait_Busy(); //�ȴ�оƬ��������
}
//����һ������
//Dst_Addr:������ַ 0~2047 for W25Q64
//����һ��ɽ��������ʱ��:150ms
void W25QXX_Erase_Sector(uint32_t Dst_Addr)
{
    Dst_Addr*=4096;
    W25QXX_Write_Enable();                  //SET WEL
    W25QXX_Wait_Busy();
    W25QXX_CS_LOW;                          //ʹ������
    SPI1_ReadWriteByte(W25X_SectorErase);   //������������ָ��
    SPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>16));//����24bit��ַ
    SPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>8));
    SPI1_ReadWriteByte((uint8_t)Dst_Addr);
    W25QXX_CS_HIGH; //ȡ��Ƭѡ
    W25QXX_Wait_Busy(); //�ȴ��������
}
//�ȴ�����
void W25QXX_Wait_Busy(void)
{
    while ((W25QXX_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}
//�������ģʽ
void W25QXX_PowerDown(void)
{
    W25QXX_CS_LOW;  //ʹ������
    SPI1_ReadWriteByte(W25X_PowerDown); //���͵�������
    W25QXX_CS_HIGH; //ȡ��Ƭѡ
    FLASH_Delay(3); //�ȴ�TPD
}
//����
void W25QXX_WAKEUP(void)
{
    W25QXX_CS_LOW;  // ʹ������
    SPI1_ReadWriteByte(W25X_ReleasePowerDown); // send W25X_PowerDown command 0xAB
    W25QXX_CS_HIGH; //ȡ��Ƭѡ
    FLASH_Delay(3); //�ȴ�TRES1
}
