#include "fontupd.h"
#include "Fatfs/ff.h"
#include "w25qxx.h"
#if USE_OLED
#include "oled.h"
#else
#include "lcd.h"
#endif
#include "malloc.h"

//字库存放起始地址
#define FONTINFOADDR     12*1024*1024                 //MiniSTM32是从12M地址开始的

//字库信息结构体.
//用来保存字库基本信息，地址，大小等
_font_info ftinfo;

//字库存放的路径
const char *GBK12_PATH="1:/SYSTEM/FONT/GBK12.FON";        //GBK12的存放位置
const char *GBK16_PATH="1:/SYSTEM/FONT/GBK16.FON";        //GBK16的存放位置
const char *UNIGBK_PATH="1:/SYSTEM/FONT/UNIGBK.BIN";        //UNIGBK.BIN的存放位置

//显示当前字体更新进度
//x,y:坐标
//size:字体大小
//fsize:整个文件大小
//pos:当前文件指针位置
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
        OLED_ShowNum(x,y,t,3,size,0x80);//显示数值
        #else
        lcd_show_num(x,y,t,3,size,RED);
        #endif
    }
    return 0;
}
//更新某一个
//x,y:坐标
//size:字体大小
//fxpath:路径
//fx:更新的内容 0,ungbk;1,gbk12;2,gbk16;3,gbk24;
//返回值:0,成功;其他,失败.
uint8_t updata_fontx(uint16_t x,uint16_t y,uint8_t size,char *fxpath,uint8_t fx)
{
    uint32_t flashaddr=0;
    FIL * fftemp;
    uint8_t *tempbuf;
    uint8_t res;
    uint16_t bread;
    uint32_t offx=0;
    uint8_t rval=0;
    fftemp=(FIL*)mymalloc(SRAMIN,sizeof(FIL));    //分配内存
    if(fftemp==NULL)rval=1;
    tempbuf=mymalloc(SRAMIN,4096);    //分配4096个字节空间
    if(tempbuf==NULL)rval=1;
    res=f_open(fftemp,(const TCHAR*)fxpath,FA_READ);
    if(res)rval=2;//打开文件失败
    if(rval==0)
    {
        switch(fx)
        {
        case 0:                                                //更新UNIGBK.BIN
            ftinfo.ugbkaddr=FONTINFOADDR+sizeof(ftinfo);    //信息头之后，紧跟UNIGBK转换码表
            ftinfo.ugbksize=fftemp->fsize;                    //UNIGBK大小
            flashaddr=ftinfo.ugbkaddr;
            break;
        case 1:
            ftinfo.f12addr=ftinfo.ugbkaddr+ftinfo.ugbksize;    //UNIGBK之后，紧跟GBK12字库
            ftinfo.gbk12size=fftemp->fsize;                    //GBK12字库大小
            flashaddr=ftinfo.f12addr;                        //GBK12的起始地址
            break;
        case 2:
            ftinfo.f16addr=ftinfo.f12addr+ftinfo.gbk12size;    //GBK12之后，紧跟GBK16字库
            ftinfo.gbk16size=fftemp->fsize;                    //GBK16字库大小
            flashaddr=ftinfo.f16addr;                        //GBK16的起始地址
            break;
        }
        while(res==FR_OK)//循环执行
        {
            res=f_read(fftemp,tempbuf,4096,(UINT *)&bread);        //读取数据
            if(res!=FR_OK)break;                                //执行错误
            W25QXX_Write(tempbuf,offx+flashaddr,4096);        //从0开始写入4096个数据
            offx+=bread;
            //fupd_prog(x,y,size,fftemp->fsize,offx);             //进度显示
            if(bread!=4096)break;                                //读完了.
        }
        f_close(fftemp);
    }
    myfree(SRAMIN,fftemp);    //释放内存
    myfree(SRAMIN,tempbuf);    //释放内存
    return res;
}

//更新字体文件,UNIGBK,GBK12,GBK16,GBK24一起更新
//x,y:提示信息的显示地址
//size:字体大小
//提示信息字体大小
//返回值:0,更新成功;
//         其他,错误代码.
uint8_t update_font(uint16_t x,uint16_t y,uint8_t size)
{
    char *gbk16_path=(char *)GBK16_PATH;
    char *gbk12_path=(char *)GBK12_PATH;
    char *unigbk_path=(char *)UNIGBK_PATH;
    uint8_t res;
    res=0XFF;
    ftinfo.fontok=0XFF;
    W25QXX_Write((uint8_t*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));    //清除之前字库成功的标志.防止更新到一半重启,导致的字库部分数据丢失.
    W25QXX_Read((uint8_t*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));    //重新读出ftinfo结构体数据
    #if USE_OLED
    OLED_ShowString(x,y,"Update UNICD.BIN",size);
    OLED_Refresh_Gram();
    #else
    lcd_show_string(x,y,240,size,size,"Update UNICD.BIN",RED);
    #endif
    
    res=updata_fontx(x+20*size/2,y,size,unigbk_path,0);            //更新UNIGBK.BIN
    if(res)return 1;
    
    #if USE_OLED
    OLED_ShowString(x,y,"Update GBK12.BIN",size);
    OLED_Refresh_Gram();
    #else
    lcd_show_string(x,y,240,size,size,"Update GBK12.BIN",RED);
    #endif
    res=updata_fontx(x+20*size/2,y,size,gbk12_path,1);            //更新GBK12.FON
    if(res)return 2;
    #if USE_OLED
    OLED_ShowString(x,y,"Update GBK16.BIN",size);
    OLED_Refresh_Gram();
    #else
    lcd_show_string(x,y,240,size,size,"Update GBK16.BIN",RED);
    #endif
    res=updata_fontx(x+20*size/2,y,size,gbk16_path,2);            //更新GBK16.FON
    if(res)return 3;
    //全部更新好了
    ftinfo.fontok=0XAA;
    W25QXX_Write((uint8_t*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));    //保存字库信息
    #if USE_OLED
    OLED_ShowString(x,y,"Update Font OK  ",size);
    OLED_Refresh_Gram();
    #else
    lcd_show_string(x,y,240,size,size,"Update Font OK  ",RED);
    #endif
    return 0;//无错误.
}
//初始化字体
//返回值:0,字库完好.
//         其他,字库丢失
uint8_t font_init(void)
{
    W25QXX_Read((uint8_t*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//读出ftinfo结构体数据
    if(ftinfo.fontok!=0XAA)return 1;            //字库错误.
    return 0;
}
