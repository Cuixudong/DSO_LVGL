#include "fontupd.h"
#include "Fatfs/ff.h"
#include "w25qxx.h"
#if USE_OLED
#include "oled.h"
#else
#include "lcd.h"
#endif
#include "malloc.h"

//�ֿ�����ʼ��ַ
#define FONTINFOADDR     12*1024*1024                 //MiniSTM32�Ǵ�12M��ַ��ʼ��

//�ֿ���Ϣ�ṹ��.
//���������ֿ������Ϣ����ַ����С��
_font_info ftinfo;

//�ֿ��ŵ�·��
const char *GBK12_PATH="1:/SYSTEM/FONT/GBK12.FON";        //GBK12�Ĵ��λ��
const char *GBK16_PATH="1:/SYSTEM/FONT/GBK16.FON";        //GBK16�Ĵ��λ��
const char *UNIGBK_PATH="1:/SYSTEM/FONT/UNIGBK.BIN";        //UNIGBK.BIN�Ĵ��λ��

//��ʾ��ǰ������½���
//x,y:����
//size:�����С
//fsize:�����ļ���С
//pos:��ǰ�ļ�ָ��λ��
uint32_t fupd_prog(uint16_t x,uint16_t y,uint8_t size,uint32_t fsize,uint32_t pos)
{
    float prog;
    uint8_t t=0XFF;
    prog=(float)pos/fsize;
    prog*=100;
    if(t!=prog)
    {
        #if USE_OLED
        OLED_ShowString(x+3*size/2,y,"%",size);
        #else
        lcd_show_char(x+3*size/2,y,'%',size,0x00,RED);
        #endif
        t=prog;
        if(t>100)t=100;
        #if USE_OLED
        OLED_ShowNum(x,y,t,3,size,0x80);//��ʾ��ֵ
        #else
        lcd_show_num(x,y,t,3,size,RED);
        #endif
    }
    return 0;
}
//����ĳһ��
//x,y:����
//size:�����С
//fxpath:·��
//fx:���µ����� 0,ungbk;1,gbk12;2,gbk16;3,gbk24;
//����ֵ:0,�ɹ�;����,ʧ��.
uint8_t updata_fontx(uint16_t x,uint16_t y,uint8_t size,char *fxpath,uint8_t fx)
{
    uint32_t flashaddr=0;
    FIL * fftemp;
    uint8_t *tempbuf;
    uint8_t res;
    uint16_t bread;
    uint32_t offx=0;
    uint8_t rval=0;
    fftemp=(FIL*)mymalloc(SRAMIN,sizeof(FIL));    //�����ڴ�
    if(fftemp==NULL)rval=1;
    tempbuf=mymalloc(SRAMIN,4096);    //����4096���ֽڿռ�
    if(tempbuf==NULL)rval=1;
    res=f_open(fftemp,(const TCHAR*)fxpath,FA_READ);
    if(res)rval=2;//���ļ�ʧ��
    if(rval==0)
    {
        switch(fx)
        {
        case 0:                                                //����UNIGBK.BIN
            ftinfo.ugbkaddr=FONTINFOADDR+sizeof(ftinfo);    //��Ϣͷ֮�󣬽���UNIGBKת�����
            ftinfo.ugbksize=fftemp->fsize;                    //UNIGBK��С
            flashaddr=ftinfo.ugbkaddr;
            break;
        case 1:
            ftinfo.f12addr=ftinfo.ugbkaddr+ftinfo.ugbksize;    //UNIGBK֮�󣬽���GBK12�ֿ�
            ftinfo.gbk12size=fftemp->fsize;                    //GBK12�ֿ��С
            flashaddr=ftinfo.f12addr;                        //GBK12����ʼ��ַ
            break;
        case 2:
            ftinfo.f16addr=ftinfo.f12addr+ftinfo.gbk12size;    //GBK12֮�󣬽���GBK16�ֿ�
            ftinfo.gbk16size=fftemp->fsize;                    //GBK16�ֿ��С
            flashaddr=ftinfo.f16addr;                        //GBK16����ʼ��ַ
            break;
        }
        while(res==FR_OK)//ѭ��ִ��
        {
            res=f_read(fftemp,tempbuf,4096,(UINT *)&bread);        //��ȡ����
            if(res!=FR_OK)break;                                //ִ�д���
            W25QXX_Write(tempbuf,offx+flashaddr,4096);        //��0��ʼд��4096������
            offx+=bread;
            //fupd_prog(x,y,size,fftemp->fsize,offx);             //������ʾ
            if(bread!=4096)break;                                //������.
        }
        f_close(fftemp);
    }
    myfree(SRAMIN,fftemp);    //�ͷ��ڴ�
    myfree(SRAMIN,tempbuf);    //�ͷ��ڴ�
    return res;
}

//���������ļ�,UNIGBK,GBK12,GBK16,GBK24һ�����
//x,y:��ʾ��Ϣ����ʾ��ַ
//size:�����С
//��ʾ��Ϣ�����С
//����ֵ:0,���³ɹ�;
//         ����,�������.
uint8_t update_font(uint16_t x,uint16_t y,uint8_t size)
{
    char *gbk16_path=(char *)GBK16_PATH;
    char *gbk12_path=(char *)GBK12_PATH;
    char *unigbk_path=(char *)UNIGBK_PATH;
    uint8_t res;
    res=0XFF;
    ftinfo.fontok=0XFF;
    W25QXX_Write((uint8_t*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));    //���֮ǰ�ֿ�ɹ��ı�־.��ֹ���µ�һ������,���µ��ֿⲿ�����ݶ�ʧ.
    W25QXX_Read((uint8_t*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));    //���¶���ftinfo�ṹ������
    #if USE_OLED
    OLED_ShowString(x,y,"Update UNICD.BIN",size);
    OLED_Refresh_Gram();
    #else
    lcd_show_string(x,y,240,size,size,"Update UNICD.BIN",RED);
    #endif
    
    res=updata_fontx(x+20*size/2,y,size,unigbk_path,0);            //����UNIGBK.BIN
    if(res)return 1;
    
    #if USE_OLED
    OLED_ShowString(x,y,"Update GBK12.BIN",size);
    OLED_Refresh_Gram();
    #else
    lcd_show_string(x,y,240,size,size,"Update GBK12.BIN",RED);
    #endif
    res=updata_fontx(x+20*size/2,y,size,gbk12_path,1);            //����GBK12.FON
    if(res)return 2;
    #if USE_OLED
    OLED_ShowString(x,y,"Update GBK16.BIN",size);
    OLED_Refresh_Gram();
    #else
    lcd_show_string(x,y,240,size,size,"Update GBK16.BIN",RED);
    #endif
    res=updata_fontx(x+20*size/2,y,size,gbk16_path,2);            //����GBK16.FON
    if(res)return 3;
    //ȫ�����º���
    ftinfo.fontok=0XAA;
    W25QXX_Write((uint8_t*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));    //�����ֿ���Ϣ
    #if USE_OLED
    OLED_ShowString(x,y,"Update Font OK  ",size);
    OLED_Refresh_Gram();
    #else
    lcd_show_string(x,y,240,size,size,"Update Font OK  ",RED);
    #endif
    return 0;//�޴���.
}
//��ʼ������
//����ֵ:0,�ֿ����.
//         ����,�ֿⶪʧ
uint8_t font_init(void)
{
    W25QXX_Read((uint8_t*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//����ftinfo�ṹ������
    if(ftinfo.fontok!=0XAA)return 1;            //�ֿ����.
    return 0;
}
