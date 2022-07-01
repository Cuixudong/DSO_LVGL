#include "fontupd.h"
#include "w25qxx.h"
#if USE_OLED
#include "oled.h"
#else
#include "lcd.h"
#endif
#include "text.h"
#include "string.h"

//code �ַ�ָ�뿪ʼ
//���ֿ��в��ҳ���ģ
//code �ַ����Ŀ�ʼ��ַ,GBK��
//mat  ���ݴ�ŵ�ַ (size/8+((size%8)?1:0))*(size) bytes��С
//size:�����С
void Get_HzMat(char *code,unsigned char *mat,uint8_t size)
{
    char qh,ql;
    unsigned char i;
    unsigned long foffset;
    uint8_t csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���
    qh=*code;
    ql=*(++code);
    if((uint8_t)qh<0x81||(uint8_t)ql<0x40||(uint8_t)ql==0xff||(uint8_t)qh==0xff)//�� ���ú���
    {
        for(i=0; i<csize; i++)*mat++=0x00; //�������
        return; //��������
    }
    if(ql<0x7f)ql-=0x40;//ע��!
    else ql-=0x41;
    qh-=0x81;
    foffset=((unsigned long)190*qh+ql)*csize;    //�õ��ֿ��е��ֽ�ƫ����
    switch(size)
    {
    case 12:
        W25QXX_Read(mat,foffset+ftinfo.f12addr,24);
        break;
    case 16:
        W25QXX_Read(mat,foffset+ftinfo.f16addr,32);
        break;
    }
}
//��ʾһ��ָ����С�ĺ���
//x,y :���ֵ�����
//font:����GBK��
//size:�����С
//mode:0,������ʾ,1,������ʾ
void Show_Font(uint16_t x,uint16_t y,char *font,uint8_t size,uint8_t mode)
{
    uint8_t temp,t,t1;
    uint16_t y0=y;
    uint8_t dzk[72];
    uint8_t csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���
    if(size!=12&&size!=16)return;    //��֧�ֵ�size
    Get_HzMat(font,dzk,size);    //�õ���Ӧ��С�ĵ�������
    for(t=0; t<csize; t++)
    {
        temp=dzk[t];            //�õ���������
        for(t1=0; t1<8; t1++)
        {
            if(mode)
            {
                if(temp&0x80)
                {
                    #if USE_OLED
                    OLED_DrawPoint(x,y,1);
                    #else
                    lcd_draw_point(x,y,RED);
                    #endif
                }
                else
                {
                    #if USE_OLED
                    OLED_DrawPoint(x,y,0);
                    #else
                    lcd_draw_point(x,y,BLACK);
                    #endif
                }
            }
            else
            {
                if(temp&0x80)
                {
                    #if USE_OLED
                    OLED_DrawPoint(x,y,0);
                    #else
                    lcd_draw_point(x,y,BLACK);
                    #endif
                }
                else
                {
                    #if USE_OLED
                    OLED_DrawPoint(x,y,1);
                    #else
                    lcd_draw_point(x,y,RED);
                    #endif
                }
            }
            temp<<=1;
            y++;
            if((y-y0)==size)
            {
                y=y0;
                x++;
                break;
            }
        }
    }
}
//��ָ��λ�ÿ�ʼ��ʾһ���ַ���
//֧���Զ�����
//(x,y):��ʼ����
//width,height:����
//str  :�ַ���
//size :�����С
//mode:0,�ǵ��ӷ�ʽ;1,���ӷ�ʽ
void Show_String(uint16_t x,uint16_t y,uint16_t width,uint16_t height,char *str,uint8_t size,uint8_t mode)
{
    uint16_t x0=x;
    uint16_t y0=y;
    uint8_t bHz=0;     //�ַ���������
    while(*str!=0)//����δ����
    {
        if(!bHz)
        {
            if((uint8_t)(*str)>0x80)bHz=1;//����
            else              //�ַ�
            {
                if(x>(x0+width-size/2))//����
                {
                    y+=size;
                    x=x0;
                }
                if(y>(y0+height-size))break;//Խ�緵��
                if(*str==13)//���з���
                {
                    y+=size;
                    x=x0;
                    str++;
                }
                else
                {
                    #if USE_OLED
                    OLED_ShowChar(x,y,*str,size,mode);//��Ч����д��
                    #else
                    lcd_show_char(x,y,*str,size,mode,RED);
                    #endif
                }
                str++;
                x+=size/2; //�ַ�,Ϊȫ�ֵ�һ��
            }
        } else//����
        {
            bHz=0;//�к��ֿ�
            if(x>(x0+width-size))//����
            {
                y+=size;
                x=x0;
            }
            if(y>(y0+height-size))break;//Խ�緵��
            Show_Font(x,y,str,size,mode); //��ʾ�������,������ʾ
            str+=2;
            x+=size;//��һ������ƫ��
        }
    }
}
//��ָ����ȵ��м���ʾ�ַ���
//����ַ����ȳ�����len,����Show_Str��ʾ
//len:ָ��Ҫ��ʾ�Ŀ��
void Show_Str_Mid(uint16_t x,uint16_t y,char *str,uint8_t size,uint8_t len)
{
    uint16_t strlenth=0;
    strlenth=strlen((const char*)str);
    strlenth*=size/2;
    if(strlenth>len)Show_String(x,y,128,64,str,size,1);
    else
    {
        strlenth=(len-strlenth)/2;
        Show_String(strlenth+x,y,128,64,str,size,1);
    }
}
