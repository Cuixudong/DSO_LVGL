#include "fontupd.h"
#include "w25qxx.h"
#if USE_OLED
#include "oled.h"
#else
#include "lcd.h"
#endif
#include "text.h"
#include "string.h"

//code 字符指针开始
//从字库中查找出字模
//code 字符串的开始地址,GBK码
//mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小
//size:字体大小
void Get_HzMat(char *code,unsigned char *mat,uint8_t size)
{
    char qh,ql;
    unsigned char i;
    unsigned long foffset;
    uint8_t csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数
    qh=*code;
    ql=*(++code);
    if((uint8_t)qh<0x81||(uint8_t)ql<0x40||(uint8_t)ql==0xff||(uint8_t)qh==0xff)//非 常用汉字
    {
        for(i=0; i<csize; i++)*mat++=0x00; //填充满格
        return; //结束访问
    }
    if(ql<0x7f)ql-=0x40;//注意!
    else ql-=0x41;
    qh-=0x81;
    foffset=((unsigned long)190*qh+ql)*csize;    //得到字库中的字节偏移量
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
//显示一个指定大小的汉字
//x,y :汉字的坐标
//font:汉字GBK码
//size:字体大小
//mode:0,正常显示,1,叠加显示
void Show_Font(uint16_t x,uint16_t y,char *font,uint8_t size,uint8_t mode)
{
    uint8_t temp,t,t1;
    uint16_t y0=y;
    uint8_t dzk[72];
    uint8_t csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数
    if(size!=12&&size!=16)return;    //不支持的size
    Get_HzMat(font,dzk,size);    //得到相应大小的点阵数据
    for(t=0; t<csize; t++)
    {
        temp=dzk[t];            //得到点阵数据
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
//在指定位置开始显示一个字符串
//支持自动换行
//(x,y):起始坐标
//width,height:区域
//str  :字符串
//size :字体大小
//mode:0,非叠加方式;1,叠加方式
void Show_String(uint16_t x,uint16_t y,uint16_t width,uint16_t height,char *str,uint8_t size,uint8_t mode)
{
    uint16_t x0=x;
    uint16_t y0=y;
    uint8_t bHz=0;     //字符或者中文
    while(*str!=0)//数据未结束
    {
        if(!bHz)
        {
            if((uint8_t)(*str)>0x80)bHz=1;//中文
            else              //字符
            {
                if(x>(x0+width-size/2))//换行
                {
                    y+=size;
                    x=x0;
                }
                if(y>(y0+height-size))break;//越界返回
                if(*str==13)//换行符号
                {
                    y+=size;
                    x=x0;
                    str++;
                }
                else
                {
                    #if USE_OLED
                    OLED_ShowChar(x,y,*str,size,mode);//有效部分写入
                    #else
                    lcd_show_char(x,y,*str,size,mode,RED);
                    #endif
                }
                str++;
                x+=size/2; //字符,为全字的一半
            }
        } else//中文
        {
            bHz=0;//有汉字库
            if(x>(x0+width-size))//换行
            {
                y+=size;
                x=x0;
            }
            if(y>(y0+height-size))break;//越界返回
            Show_Font(x,y,str,size,mode); //显示这个汉字,空心显示
            str+=2;
            x+=size;//下一个汉字偏移
        }
    }
}
//在指定宽度的中间显示字符串
//如果字符长度超过了len,则用Show_Str显示
//len:指定要显示的宽度
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
