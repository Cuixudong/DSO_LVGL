#include "lcd.h"
#include "lcdfont.h"

extern SRAM_HandleTypeDef hsram1;   /* SRAM句柄(用于控制LCD) */

/* LCD的画笔颜色和背景色 */
uint32_t g_point_color = WHITE;    /* 画笔颜色 */
uint32_t g_back_color  = BLACK;    /* 背景色 */

/* 管理LCD重要参数 */
_lcd_dev lcddev;

/**
 * @brief       ST7789 寄存器初始化代码
 * @param       无
 * @retval      无
 */
void lcd_ex_st7789_reginit(void)
{
    lcd_wr_regno(0x11);

    HAL_Delay(120); 

    lcd_wr_regno(0x21);//对P100屏幕反色显示问题解决
    
    lcd_wr_regno(0x36);
    lcd_wr_data(0x00);

    lcd_wr_regno(0x3A);
    lcd_wr_data(0X05);

    lcd_wr_regno(0xB2);
    lcd_wr_data(0x0C);
    lcd_wr_data(0x0C);
    lcd_wr_data(0x00);
    lcd_wr_data(0x33);
    lcd_wr_data(0x33);

    lcd_wr_regno(0xB7);
    lcd_wr_data(0x35);

    lcd_wr_regno(0xBB); /* vcom */
    lcd_wr_data(0x32);  /* 30 */

    lcd_wr_regno(0xC0);
    lcd_wr_data(0x0C);

    lcd_wr_regno(0xC2);
    lcd_wr_data(0x01);

    lcd_wr_regno(0xC3); /* vrh */
    lcd_wr_data(0x10);  /* 17 0D */

    lcd_wr_regno(0xC4); /* vdv */
    lcd_wr_data(0x20);  /* 20 */

    lcd_wr_regno(0xC6);
    lcd_wr_data(0x0f);

    lcd_wr_regno(0xD0);
    lcd_wr_data(0xA4); 
    lcd_wr_data(0xA1); 

    lcd_wr_regno(0xE0); /* Set Gamma  */
    lcd_wr_data(0xd0);
    lcd_wr_data(0x00);
    lcd_wr_data(0x02);
    lcd_wr_data(0x07);
    lcd_wr_data(0x0a);
    lcd_wr_data(0x28);
    lcd_wr_data(0x32);
    lcd_wr_data(0X44);
    lcd_wr_data(0x42);
    lcd_wr_data(0x06);
    lcd_wr_data(0x0e);
    lcd_wr_data(0x12);
    lcd_wr_data(0x14);
    lcd_wr_data(0x17);


    lcd_wr_regno(0XE1);  /* Set Gamma */
    lcd_wr_data(0xd0);
    lcd_wr_data(0x00);
    lcd_wr_data(0x02);
    lcd_wr_data(0x07);
    lcd_wr_data(0x0a);
    lcd_wr_data(0x28);
    lcd_wr_data(0x31);
    lcd_wr_data(0x54);
    lcd_wr_data(0x47);
    lcd_wr_data(0x0e);
    lcd_wr_data(0x1c);
    lcd_wr_data(0x17);
    lcd_wr_data(0x1b); 
    lcd_wr_data(0x1e);


    lcd_wr_regno(0x2A);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0xef);

    lcd_wr_regno(0x2B);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x01);
    lcd_wr_data(0x3f);

    lcd_wr_regno(0x29); /* display on */
}
/**
 * @brief       ST7796 寄存器初始化代码
 * @param       无
 * @retval      无
 */
void lcd_ex_st7796_reginit(void)
{
    lcd_wr_regno(0x11); /* Sleep Out */

    HAL_Delay(120);      /* Delay 120ms */

    lcd_wr_regno(0x21);

    lcd_wr_regno(0x3A);
    lcd_wr_data(0X05);


    lcd_wr_regno(0xf0) ;
    lcd_wr_data(0xc3) ;

    lcd_wr_regno(0xf0) ;
    lcd_wr_data(0x96) ;

    lcd_wr_regno(0x36);
    lcd_wr_data(0x48);

    lcd_wr_regno(0xB4);
    lcd_wr_data(0x01);

    lcd_wr_regno(0xB7) ;
    lcd_wr_data(0xC6) ;

    lcd_wr_regno(0xe8);
    lcd_wr_data(0x40);
    lcd_wr_data(0x8a);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x29);
    lcd_wr_data(0x19);
    lcd_wr_data(0xa5);
    lcd_wr_data(0x33);

    lcd_wr_regno(0xc2);
    lcd_wr_data(0xa7);

    lcd_wr_regno(0xc5);
    lcd_wr_data(0x19);

    lcd_wr_regno(0xe0); /* Positive Voltage Gamma Control */
    lcd_wr_data(0xf0);
    lcd_wr_data(0x00);
    lcd_wr_data(0x08);
    lcd_wr_data(0x0e);
    lcd_wr_data(0x0d);
    lcd_wr_data(0x1a);
    lcd_wr_data(0x37);
    lcd_wr_data(0x54);
    lcd_wr_data(0x47);
    lcd_wr_data(0x2b);
    lcd_wr_data(0x16);
    lcd_wr_data(0x15);
    lcd_wr_data(0x1a);
    lcd_wr_data(0x1d);

    lcd_wr_regno(0xe1); /* Negative Voltage Gamma Control */
    lcd_wr_data(0xf0);
    lcd_wr_data(0x02);
    lcd_wr_data(0x06);
    lcd_wr_data(0x0c);
    lcd_wr_data(0x0e);
    lcd_wr_data(0x29);
    lcd_wr_data(0x34);
    lcd_wr_data(0x44);
    lcd_wr_data(0x47);
    lcd_wr_data(0x2b);
    lcd_wr_data(0x17);
    lcd_wr_data(0x16);
    lcd_wr_data(0x19);
    lcd_wr_data(0x1d);

    lcd_wr_regno(0xf0);
    lcd_wr_data(0x3c);

    lcd_wr_regno(0xf0);
    lcd_wr_data(0x69);

    HAL_Delay(120);      /* Delay 120ms */

    lcd_wr_regno(0x29); /* Display ON */
}
/**
 * @brief       ILI9341寄存器初始化代码
 * @param       无
 * @retval      无
 */
void lcd_ex_ili9341_reginit(void)
{
    lcd_wr_regno(0xCF);
    lcd_wr_data(0x00);
    lcd_wr_data(0xC1);
    lcd_wr_data(0X30);
    lcd_wr_regno(0xED);
    lcd_wr_data(0x64);
    lcd_wr_data(0x03);
    lcd_wr_data(0X12);
    lcd_wr_data(0X81);
    lcd_wr_regno(0xE8);
    lcd_wr_data(0x85);
    lcd_wr_data(0x10);
    lcd_wr_data(0x7A);
    lcd_wr_regno(0xCB);
    lcd_wr_data(0x39);
    lcd_wr_data(0x2C);
    lcd_wr_data(0x00);
    lcd_wr_data(0x34);
    lcd_wr_data(0x02);
    lcd_wr_regno(0xF7);
    lcd_wr_data(0x20);
    lcd_wr_regno(0xEA);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_regno(0xC0); /* Power control */
    lcd_wr_data(0x1B);  /* VRH[5:0] */
    lcd_wr_regno(0xC1); /* Power control */
    lcd_wr_data(0x01);  /* SAP[2:0];BT[3:0] */
    lcd_wr_regno(0xC5); /* VCM control */
    lcd_wr_data(0x30);  /* 3F */
    lcd_wr_data(0x30);  /* 3C */
    lcd_wr_regno(0xC7); /* VCM control2 */
    lcd_wr_data(0XB7);
    lcd_wr_regno(0x36); /*  Memory Access Control */
    lcd_wr_data(0x48);
    lcd_wr_regno(0x3A);
    lcd_wr_data(0x55);
    lcd_wr_regno(0xB1);
    lcd_wr_data(0x00);
    lcd_wr_data(0x1A);
    lcd_wr_regno(0xB6); /*  Display Function Control */
    lcd_wr_data(0x0A);
    lcd_wr_data(0xA2);
    lcd_wr_regno(0xF2); /*  3Gamma Function Disable */
    lcd_wr_data(0x00);
    lcd_wr_regno(0x26); /* Gamma curve selected */
    lcd_wr_data(0x01);
    lcd_wr_regno(0xE0); /* Set Gamma */
    lcd_wr_data(0x0F);
    lcd_wr_data(0x2A);
    lcd_wr_data(0x28);
    lcd_wr_data(0x08);
    lcd_wr_data(0x0E);
    lcd_wr_data(0x08);
    lcd_wr_data(0x54);
    lcd_wr_data(0XA9);
    lcd_wr_data(0x43);
    lcd_wr_data(0x0A);
    lcd_wr_data(0x0F);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_regno(0XE1);    /* Set Gamma */
    lcd_wr_data(0x00);
    lcd_wr_data(0x15);
    lcd_wr_data(0x17);
    lcd_wr_data(0x07);
    lcd_wr_data(0x11);
    lcd_wr_data(0x06);
    lcd_wr_data(0x2B);
    lcd_wr_data(0x56);
    lcd_wr_data(0x3C);
    lcd_wr_data(0x05);
    lcd_wr_data(0x10);
    lcd_wr_data(0x0F);
    lcd_wr_data(0x3F);
    lcd_wr_data(0x3F);
    lcd_wr_data(0x0F);
    lcd_wr_regno(0x2B);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x01);
    lcd_wr_data(0x3f);
    lcd_wr_regno(0x2A);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0xef);
    lcd_wr_regno(0x11); /* Exit Sleep */
    HAL_Delay(120);
    lcd_wr_regno(0x29); /* display on */
 }
 

/**
 * @brief       NT35310寄存器初始化代码 
 * @param       无
 * @retval      无
 */
void lcd_ex_nt35310_reginit(void)
{
    lcd_wr_regno(0xED);
    lcd_wr_data(0x01);
    lcd_wr_data(0xFE);

    lcd_wr_regno(0xEE);
    lcd_wr_data(0xDE);
    lcd_wr_data(0x21);

    lcd_wr_regno(0xF1);
    lcd_wr_data(0x01);
    lcd_wr_regno(0xDF);
    lcd_wr_data(0x10);

    /* VCOMvoltage */
    lcd_wr_regno(0xC4);
    lcd_wr_data(0x8F);  /* 5f */

    lcd_wr_regno(0xC6);
    lcd_wr_data(0x00);
    lcd_wr_data(0xE2);
    lcd_wr_data(0xE2);
    lcd_wr_data(0xE2);
    lcd_wr_regno(0xBF);
    lcd_wr_data(0xAA);

    lcd_wr_regno(0xB0);
    lcd_wr_data(0x0D);
    lcd_wr_data(0x00);
    lcd_wr_data(0x0D);
    lcd_wr_data(0x00);
    lcd_wr_data(0x11);
    lcd_wr_data(0x00);
    lcd_wr_data(0x19);
    lcd_wr_data(0x00);
    lcd_wr_data(0x21);
    lcd_wr_data(0x00);
    lcd_wr_data(0x2D);
    lcd_wr_data(0x00);
    lcd_wr_data(0x3D);
    lcd_wr_data(0x00);
    lcd_wr_data(0x5D);
    lcd_wr_data(0x00);
    lcd_wr_data(0x5D);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xB1);
    lcd_wr_data(0x80);
    lcd_wr_data(0x00);
    lcd_wr_data(0x8B);
    lcd_wr_data(0x00);
    lcd_wr_data(0x96);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xB2);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x02);
    lcd_wr_data(0x00);
    lcd_wr_data(0x03);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xB3);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xB4);
    lcd_wr_data(0x8B);
    lcd_wr_data(0x00);
    lcd_wr_data(0x96);
    lcd_wr_data(0x00);
    lcd_wr_data(0xA1);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xB5);
    lcd_wr_data(0x02);
    lcd_wr_data(0x00);
    lcd_wr_data(0x03);
    lcd_wr_data(0x00);
    lcd_wr_data(0x04);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xB6);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xB7);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x3F);
    lcd_wr_data(0x00);
    lcd_wr_data(0x5E);
    lcd_wr_data(0x00);
    lcd_wr_data(0x64);
    lcd_wr_data(0x00);
    lcd_wr_data(0x8C);
    lcd_wr_data(0x00);
    lcd_wr_data(0xAC);
    lcd_wr_data(0x00);
    lcd_wr_data(0xDC);
    lcd_wr_data(0x00);
    lcd_wr_data(0x70);
    lcd_wr_data(0x00);
    lcd_wr_data(0x90);
    lcd_wr_data(0x00);
    lcd_wr_data(0xEB);
    lcd_wr_data(0x00);
    lcd_wr_data(0xDC);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xB8);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xBA);
    lcd_wr_data(0x24);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xC1);
    lcd_wr_data(0x20);
    lcd_wr_data(0x00);
    lcd_wr_data(0x54);
    lcd_wr_data(0x00);
    lcd_wr_data(0xFF);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xC2);
    lcd_wr_data(0x0A);
    lcd_wr_data(0x00);
    lcd_wr_data(0x04);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xC3);
    lcd_wr_data(0x3C);
    lcd_wr_data(0x00);
    lcd_wr_data(0x3A);
    lcd_wr_data(0x00);
    lcd_wr_data(0x39);
    lcd_wr_data(0x00);
    lcd_wr_data(0x37);
    lcd_wr_data(0x00);
    lcd_wr_data(0x3C);
    lcd_wr_data(0x00);
    lcd_wr_data(0x36);
    lcd_wr_data(0x00);
    lcd_wr_data(0x32);
    lcd_wr_data(0x00);
    lcd_wr_data(0x2F);
    lcd_wr_data(0x00);
    lcd_wr_data(0x2C);
    lcd_wr_data(0x00);
    lcd_wr_data(0x29);
    lcd_wr_data(0x00);
    lcd_wr_data(0x26);
    lcd_wr_data(0x00);
    lcd_wr_data(0x24);
    lcd_wr_data(0x00);
    lcd_wr_data(0x24);
    lcd_wr_data(0x00);
    lcd_wr_data(0x23);
    lcd_wr_data(0x00);
    lcd_wr_data(0x3C);
    lcd_wr_data(0x00);
    lcd_wr_data(0x36);
    lcd_wr_data(0x00);
    lcd_wr_data(0x32);
    lcd_wr_data(0x00);
    lcd_wr_data(0x2F);
    lcd_wr_data(0x00);
    lcd_wr_data(0x2C);
    lcd_wr_data(0x00);
    lcd_wr_data(0x29);
    lcd_wr_data(0x00);
    lcd_wr_data(0x26);
    lcd_wr_data(0x00);
    lcd_wr_data(0x24);
    lcd_wr_data(0x00);
    lcd_wr_data(0x24);
    lcd_wr_data(0x00);
    lcd_wr_data(0x23);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xC4);
    lcd_wr_data(0x62);
    lcd_wr_data(0x00);
    lcd_wr_data(0x05);
    lcd_wr_data(0x00);
    lcd_wr_data(0x84);
    lcd_wr_data(0x00);
    lcd_wr_data(0xF0);
    lcd_wr_data(0x00);
    lcd_wr_data(0x18);
    lcd_wr_data(0x00);
    lcd_wr_data(0xA4);
    lcd_wr_data(0x00);
    lcd_wr_data(0x18);
    lcd_wr_data(0x00);
    lcd_wr_data(0x50);
    lcd_wr_data(0x00);
    lcd_wr_data(0x0C);
    lcd_wr_data(0x00);
    lcd_wr_data(0x17);
    lcd_wr_data(0x00);
    lcd_wr_data(0x95);
    lcd_wr_data(0x00);
    lcd_wr_data(0xF3);
    lcd_wr_data(0x00);
    lcd_wr_data(0xE6);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xC5);
    lcd_wr_data(0x32);
    lcd_wr_data(0x00);
    lcd_wr_data(0x44);
    lcd_wr_data(0x00);
    lcd_wr_data(0x65);
    lcd_wr_data(0x00);
    lcd_wr_data(0x76);
    lcd_wr_data(0x00);
    lcd_wr_data(0x88);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xC6);
    lcd_wr_data(0x20);
    lcd_wr_data(0x00);
    lcd_wr_data(0x17);
    lcd_wr_data(0x00);
    lcd_wr_data(0x01);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xC7);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xC8);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xC9);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xE0);
    lcd_wr_data(0x16);
    lcd_wr_data(0x00);
    lcd_wr_data(0x1C);
    lcd_wr_data(0x00);
    lcd_wr_data(0x21);
    lcd_wr_data(0x00);
    lcd_wr_data(0x36);
    lcd_wr_data(0x00);
    lcd_wr_data(0x46);
    lcd_wr_data(0x00);
    lcd_wr_data(0x52);
    lcd_wr_data(0x00);
    lcd_wr_data(0x64);
    lcd_wr_data(0x00);
    lcd_wr_data(0x7A);
    lcd_wr_data(0x00);
    lcd_wr_data(0x8B);
    lcd_wr_data(0x00);
    lcd_wr_data(0x99);
    lcd_wr_data(0x00);
    lcd_wr_data(0xA8);
    lcd_wr_data(0x00);
    lcd_wr_data(0xB9);
    lcd_wr_data(0x00);
    lcd_wr_data(0xC4);
    lcd_wr_data(0x00);
    lcd_wr_data(0xCA);
    lcd_wr_data(0x00);
    lcd_wr_data(0xD2);
    lcd_wr_data(0x00);
    lcd_wr_data(0xD9);
    lcd_wr_data(0x00);
    lcd_wr_data(0xE0);
    lcd_wr_data(0x00);
    lcd_wr_data(0xF3);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xE1);
    lcd_wr_data(0x16);
    lcd_wr_data(0x00);
    lcd_wr_data(0x1C);
    lcd_wr_data(0x00);
    lcd_wr_data(0x22);
    lcd_wr_data(0x00);
    lcd_wr_data(0x36);
    lcd_wr_data(0x00);
    lcd_wr_data(0x45);
    lcd_wr_data(0x00);
    lcd_wr_data(0x52);
    lcd_wr_data(0x00);
    lcd_wr_data(0x64);
    lcd_wr_data(0x00);
    lcd_wr_data(0x7A);
    lcd_wr_data(0x00);
    lcd_wr_data(0x8B);
    lcd_wr_data(0x00);
    lcd_wr_data(0x99);
    lcd_wr_data(0x00);
    lcd_wr_data(0xA8);
    lcd_wr_data(0x00);
    lcd_wr_data(0xB9);
    lcd_wr_data(0x00);
    lcd_wr_data(0xC4);
    lcd_wr_data(0x00);
    lcd_wr_data(0xCA);
    lcd_wr_data(0x00);
    lcd_wr_data(0xD2);
    lcd_wr_data(0x00);
    lcd_wr_data(0xD8);
    lcd_wr_data(0x00);
    lcd_wr_data(0xE0);
    lcd_wr_data(0x00);
    lcd_wr_data(0xF3);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xE2);
    lcd_wr_data(0x05);
    lcd_wr_data(0x00);
    lcd_wr_data(0x0B);
    lcd_wr_data(0x00);
    lcd_wr_data(0x1B);
    lcd_wr_data(0x00);
    lcd_wr_data(0x34);
    lcd_wr_data(0x00);
    lcd_wr_data(0x44);
    lcd_wr_data(0x00);
    lcd_wr_data(0x4F);
    lcd_wr_data(0x00);
    lcd_wr_data(0x61);
    lcd_wr_data(0x00);
    lcd_wr_data(0x79);
    lcd_wr_data(0x00);
    lcd_wr_data(0x88);
    lcd_wr_data(0x00);
    lcd_wr_data(0x97);
    lcd_wr_data(0x00);
    lcd_wr_data(0xA6);
    lcd_wr_data(0x00);
    lcd_wr_data(0xB7);
    lcd_wr_data(0x00);
    lcd_wr_data(0xC2);
    lcd_wr_data(0x00);
    lcd_wr_data(0xC7);
    lcd_wr_data(0x00);
    lcd_wr_data(0xD1);
    lcd_wr_data(0x00);
    lcd_wr_data(0xD6);
    lcd_wr_data(0x00);
    lcd_wr_data(0xDD);
    lcd_wr_data(0x00);
    lcd_wr_data(0xF3);
    lcd_wr_data(0x00);
    lcd_wr_regno(0xE3);
    lcd_wr_data(0x05);
    lcd_wr_data(0x00);
    lcd_wr_data(0xA);
    lcd_wr_data(0x00);
    lcd_wr_data(0x1C);
    lcd_wr_data(0x00);
    lcd_wr_data(0x33);
    lcd_wr_data(0x00);
    lcd_wr_data(0x44);
    lcd_wr_data(0x00);
    lcd_wr_data(0x50);
    lcd_wr_data(0x00);
    lcd_wr_data(0x62);
    lcd_wr_data(0x00);
    lcd_wr_data(0x78);
    lcd_wr_data(0x00);
    lcd_wr_data(0x88);
    lcd_wr_data(0x00);
    lcd_wr_data(0x97);
    lcd_wr_data(0x00);
    lcd_wr_data(0xA6);
    lcd_wr_data(0x00);
    lcd_wr_data(0xB7);
    lcd_wr_data(0x00);
    lcd_wr_data(0xC2);
    lcd_wr_data(0x00);
    lcd_wr_data(0xC7);
    lcd_wr_data(0x00);
    lcd_wr_data(0xD1);
    lcd_wr_data(0x00);
    lcd_wr_data(0xD5);
    lcd_wr_data(0x00);
    lcd_wr_data(0xDD);
    lcd_wr_data(0x00);
    lcd_wr_data(0xF3);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xE4);
    lcd_wr_data(0x01);
    lcd_wr_data(0x00);
    lcd_wr_data(0x01);
    lcd_wr_data(0x00);
    lcd_wr_data(0x02);
    lcd_wr_data(0x00);
    lcd_wr_data(0x2A);
    lcd_wr_data(0x00);
    lcd_wr_data(0x3C);
    lcd_wr_data(0x00);
    lcd_wr_data(0x4B);
    lcd_wr_data(0x00);
    lcd_wr_data(0x5D);
    lcd_wr_data(0x00);
    lcd_wr_data(0x74);
    lcd_wr_data(0x00);
    lcd_wr_data(0x84);
    lcd_wr_data(0x00);
    lcd_wr_data(0x93);
    lcd_wr_data(0x00);
    lcd_wr_data(0xA2);
    lcd_wr_data(0x00);
    lcd_wr_data(0xB3);
    lcd_wr_data(0x00);
    lcd_wr_data(0xBE);
    lcd_wr_data(0x00);
    lcd_wr_data(0xC4);
    lcd_wr_data(0x00);
    lcd_wr_data(0xCD);
    lcd_wr_data(0x00);
    lcd_wr_data(0xD3);
    lcd_wr_data(0x00);
    lcd_wr_data(0xDD);
    lcd_wr_data(0x00);
    lcd_wr_data(0xF3);
    lcd_wr_data(0x00);
    lcd_wr_regno(0xE5);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x02);
    lcd_wr_data(0x00);
    lcd_wr_data(0x29);
    lcd_wr_data(0x00);
    lcd_wr_data(0x3C);
    lcd_wr_data(0x00);
    lcd_wr_data(0x4B);
    lcd_wr_data(0x00);
    lcd_wr_data(0x5D);
    lcd_wr_data(0x00);
    lcd_wr_data(0x74);
    lcd_wr_data(0x00);
    lcd_wr_data(0x84);
    lcd_wr_data(0x00);
    lcd_wr_data(0x93);
    lcd_wr_data(0x00);
    lcd_wr_data(0xA2);
    lcd_wr_data(0x00);
    lcd_wr_data(0xB3);
    lcd_wr_data(0x00);
    lcd_wr_data(0xBE);
    lcd_wr_data(0x00);
    lcd_wr_data(0xC4);
    lcd_wr_data(0x00);
    lcd_wr_data(0xCD);
    lcd_wr_data(0x00);
    lcd_wr_data(0xD3);
    lcd_wr_data(0x00);
    lcd_wr_data(0xDC);
    lcd_wr_data(0x00);
    lcd_wr_data(0xF3);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xE6);
    lcd_wr_data(0x11);
    lcd_wr_data(0x00);
    lcd_wr_data(0x34);
    lcd_wr_data(0x00);
    lcd_wr_data(0x56);
    lcd_wr_data(0x00);
    lcd_wr_data(0x76);
    lcd_wr_data(0x00);
    lcd_wr_data(0x77);
    lcd_wr_data(0x00);
    lcd_wr_data(0x66);
    lcd_wr_data(0x00);
    lcd_wr_data(0x88);
    lcd_wr_data(0x00);
    lcd_wr_data(0x99);
    lcd_wr_data(0x00);
    lcd_wr_data(0xBB);
    lcd_wr_data(0x00);
    lcd_wr_data(0x99);
    lcd_wr_data(0x00);
    lcd_wr_data(0x66);
    lcd_wr_data(0x00);
    lcd_wr_data(0x55);
    lcd_wr_data(0x00);
    lcd_wr_data(0x55);
    lcd_wr_data(0x00);
    lcd_wr_data(0x45);
    lcd_wr_data(0x00);
    lcd_wr_data(0x43);
    lcd_wr_data(0x00);
    lcd_wr_data(0x44);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xE7);
    lcd_wr_data(0x32);
    lcd_wr_data(0x00);
    lcd_wr_data(0x55);
    lcd_wr_data(0x00);
    lcd_wr_data(0x76);
    lcd_wr_data(0x00);
    lcd_wr_data(0x66);
    lcd_wr_data(0x00);
    lcd_wr_data(0x67);
    lcd_wr_data(0x00);
    lcd_wr_data(0x67);
    lcd_wr_data(0x00);
    lcd_wr_data(0x87);
    lcd_wr_data(0x00);
    lcd_wr_data(0x99);
    lcd_wr_data(0x00);
    lcd_wr_data(0xBB);
    lcd_wr_data(0x00);
    lcd_wr_data(0x99);
    lcd_wr_data(0x00);
    lcd_wr_data(0x77);
    lcd_wr_data(0x00);
    lcd_wr_data(0x44);
    lcd_wr_data(0x00);
    lcd_wr_data(0x56);
    lcd_wr_data(0x00);
    lcd_wr_data(0x23);
    lcd_wr_data(0x00);
    lcd_wr_data(0x33);
    lcd_wr_data(0x00);
    lcd_wr_data(0x45);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xE8);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x99);
    lcd_wr_data(0x00);
    lcd_wr_data(0x87);
    lcd_wr_data(0x00);
    lcd_wr_data(0x88);
    lcd_wr_data(0x00);
    lcd_wr_data(0x77);
    lcd_wr_data(0x00);
    lcd_wr_data(0x66);
    lcd_wr_data(0x00);
    lcd_wr_data(0x88);
    lcd_wr_data(0x00);
    lcd_wr_data(0xAA);
    lcd_wr_data(0x00);
    lcd_wr_data(0xBB);
    lcd_wr_data(0x00);
    lcd_wr_data(0x99);
    lcd_wr_data(0x00);
    lcd_wr_data(0x66);
    lcd_wr_data(0x00);
    lcd_wr_data(0x55);
    lcd_wr_data(0x00);
    lcd_wr_data(0x55);
    lcd_wr_data(0x00);
    lcd_wr_data(0x44);
    lcd_wr_data(0x00);
    lcd_wr_data(0x44);
    lcd_wr_data(0x00);
    lcd_wr_data(0x55);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xE9);
    lcd_wr_data(0xAA);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);

    lcd_wr_regno(0x00);
    lcd_wr_data(0xAA);

    lcd_wr_regno(0xCF);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xF0);
    lcd_wr_data(0x00);
    lcd_wr_data(0x50);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xF3);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xF9);
    lcd_wr_data(0x06);
    lcd_wr_data(0x10);
    lcd_wr_data(0x29);
    lcd_wr_data(0x00);

    lcd_wr_regno(0x3A);
    lcd_wr_data(0x55);  /* 66 */

    lcd_wr_regno(0x11);
    HAL_Delay(100);
    lcd_wr_regno(0x29);
    lcd_wr_regno(0x35);
    lcd_wr_data(0x00);

    lcd_wr_regno(0x51);
    lcd_wr_data(0xFF);
    lcd_wr_regno(0x53);
    lcd_wr_data(0x2C);
    lcd_wr_regno(0x55);
    lcd_wr_data(0x82);
    lcd_wr_regno(0x2c);
}

/**
 * @brief       NT35510寄存器初始化代码 
 * @param       无
 * @retval      无
 */
void lcd_ex_nt35510_reginit(void)
{
    lcd_write_reg(0xF000, 0x55);
    lcd_write_reg(0xF001, 0xAA);
    lcd_write_reg(0xF002, 0x52);
    lcd_write_reg(0xF003, 0x08);
    lcd_write_reg(0xF004, 0x01);
    /* AVDD Set AVDD 5.2V */
    lcd_write_reg(0xB000, 0x0D);
    lcd_write_reg(0xB001, 0x0D);
    lcd_write_reg(0xB002, 0x0D);
    /* AVDD ratio */
    lcd_write_reg(0xB600, 0x34);
    lcd_write_reg(0xB601, 0x34);
    lcd_write_reg(0xB602, 0x34);
    /* AVEE -5.2V */
    lcd_write_reg(0xB100, 0x0D);
    lcd_write_reg(0xB101, 0x0D);
    lcd_write_reg(0xB102, 0x0D);
    /* AVEE ratio */
    lcd_write_reg(0xB700, 0x34);
    lcd_write_reg(0xB701, 0x34);
    lcd_write_reg(0xB702, 0x34);
    /* VCL -2.5V */
    lcd_write_reg(0xB200, 0x00);
    lcd_write_reg(0xB201, 0x00);
    lcd_write_reg(0xB202, 0x00);
    /* VCL ratio */
    lcd_write_reg(0xB800, 0x24);
    lcd_write_reg(0xB801, 0x24);
    lcd_write_reg(0xB802, 0x24);
    /* VGH 15V (Free pump) */
    lcd_write_reg(0xBF00, 0x01);
    lcd_write_reg(0xB300, 0x0F);
    lcd_write_reg(0xB301, 0x0F);
    lcd_write_reg(0xB302, 0x0F);
    /* VGH ratio */
    lcd_write_reg(0xB900, 0x34);
    lcd_write_reg(0xB901, 0x34);
    lcd_write_reg(0xB902, 0x34);
    /* VGL_REG -10V */
    lcd_write_reg(0xB500, 0x08);
    lcd_write_reg(0xB501, 0x08);
    lcd_write_reg(0xB502, 0x08);
    lcd_write_reg(0xC200, 0x03);
    /* VGLX ratio */
    lcd_write_reg(0xBA00, 0x24);
    lcd_write_reg(0xBA01, 0x24);
    lcd_write_reg(0xBA02, 0x24);
    /* VGMP/VGSP 4.5V/0V */
    lcd_write_reg(0xBC00, 0x00);
    lcd_write_reg(0xBC01, 0x78);
    lcd_write_reg(0xBC02, 0x00);
    /* VGMN/VGSN -4.5V/0V */
    lcd_write_reg(0xBD00, 0x00);
    lcd_write_reg(0xBD01, 0x78);
    lcd_write_reg(0xBD02, 0x00);
    /* VCOM */
    lcd_write_reg(0xBE00, 0x00);
    lcd_write_reg(0xBE01, 0x64);
    /* Gamma Setting */
    lcd_write_reg(0xD100, 0x00);
    lcd_write_reg(0xD101, 0x33);
    lcd_write_reg(0xD102, 0x00);
    lcd_write_reg(0xD103, 0x34);
    lcd_write_reg(0xD104, 0x00);
    lcd_write_reg(0xD105, 0x3A);
    lcd_write_reg(0xD106, 0x00);
    lcd_write_reg(0xD107, 0x4A);
    lcd_write_reg(0xD108, 0x00);
    lcd_write_reg(0xD109, 0x5C);
    lcd_write_reg(0xD10A, 0x00);
    lcd_write_reg(0xD10B, 0x81);
    lcd_write_reg(0xD10C, 0x00);
    lcd_write_reg(0xD10D, 0xA6);
    lcd_write_reg(0xD10E, 0x00);
    lcd_write_reg(0xD10F, 0xE5);
    lcd_write_reg(0xD110, 0x01);
    lcd_write_reg(0xD111, 0x13);
    lcd_write_reg(0xD112, 0x01);
    lcd_write_reg(0xD113, 0x54);
    lcd_write_reg(0xD114, 0x01);
    lcd_write_reg(0xD115, 0x82);
    lcd_write_reg(0xD116, 0x01);
    lcd_write_reg(0xD117, 0xCA);
    lcd_write_reg(0xD118, 0x02);
    lcd_write_reg(0xD119, 0x00);
    lcd_write_reg(0xD11A, 0x02);
    lcd_write_reg(0xD11B, 0x01);
    lcd_write_reg(0xD11C, 0x02);
    lcd_write_reg(0xD11D, 0x34);
    lcd_write_reg(0xD11E, 0x02);
    lcd_write_reg(0xD11F, 0x67);
    lcd_write_reg(0xD120, 0x02);
    lcd_write_reg(0xD121, 0x84);
    lcd_write_reg(0xD122, 0x02);
    lcd_write_reg(0xD123, 0xA4);
    lcd_write_reg(0xD124, 0x02);
    lcd_write_reg(0xD125, 0xB7);
    lcd_write_reg(0xD126, 0x02);
    lcd_write_reg(0xD127, 0xCF);
    lcd_write_reg(0xD128, 0x02);
    lcd_write_reg(0xD129, 0xDE);
    lcd_write_reg(0xD12A, 0x02);
    lcd_write_reg(0xD12B, 0xF2);
    lcd_write_reg(0xD12C, 0x02);
    lcd_write_reg(0xD12D, 0xFE);
    lcd_write_reg(0xD12E, 0x03);
    lcd_write_reg(0xD12F, 0x10);
    lcd_write_reg(0xD130, 0x03);
    lcd_write_reg(0xD131, 0x33);
    lcd_write_reg(0xD132, 0x03);
    lcd_write_reg(0xD133, 0x6D);
    lcd_write_reg(0xD200, 0x00);
    lcd_write_reg(0xD201, 0x33);
    lcd_write_reg(0xD202, 0x00);
    lcd_write_reg(0xD203, 0x34);
    lcd_write_reg(0xD204, 0x00);
    lcd_write_reg(0xD205, 0x3A);
    lcd_write_reg(0xD206, 0x00);
    lcd_write_reg(0xD207, 0x4A);
    lcd_write_reg(0xD208, 0x00);
    lcd_write_reg(0xD209, 0x5C);
    lcd_write_reg(0xD20A, 0x00);

    lcd_write_reg(0xD20B, 0x81);
    lcd_write_reg(0xD20C, 0x00);
    lcd_write_reg(0xD20D, 0xA6);
    lcd_write_reg(0xD20E, 0x00);
    lcd_write_reg(0xD20F, 0xE5);
    lcd_write_reg(0xD210, 0x01);
    lcd_write_reg(0xD211, 0x13);
    lcd_write_reg(0xD212, 0x01);
    lcd_write_reg(0xD213, 0x54);
    lcd_write_reg(0xD214, 0x01);
    lcd_write_reg(0xD215, 0x82);
    lcd_write_reg(0xD216, 0x01);
    lcd_write_reg(0xD217, 0xCA);
    lcd_write_reg(0xD218, 0x02);
    lcd_write_reg(0xD219, 0x00);
    lcd_write_reg(0xD21A, 0x02);
    lcd_write_reg(0xD21B, 0x01);
    lcd_write_reg(0xD21C, 0x02);
    lcd_write_reg(0xD21D, 0x34);
    lcd_write_reg(0xD21E, 0x02);
    lcd_write_reg(0xD21F, 0x67);
    lcd_write_reg(0xD220, 0x02);
    lcd_write_reg(0xD221, 0x84);
    lcd_write_reg(0xD222, 0x02);
    lcd_write_reg(0xD223, 0xA4);
    lcd_write_reg(0xD224, 0x02);
    lcd_write_reg(0xD225, 0xB7);
    lcd_write_reg(0xD226, 0x02);
    lcd_write_reg(0xD227, 0xCF);
    lcd_write_reg(0xD228, 0x02);
    lcd_write_reg(0xD229, 0xDE);
    lcd_write_reg(0xD22A, 0x02);
    lcd_write_reg(0xD22B, 0xF2);
    lcd_write_reg(0xD22C, 0x02);
    lcd_write_reg(0xD22D, 0xFE);
    lcd_write_reg(0xD22E, 0x03);
    lcd_write_reg(0xD22F, 0x10);
    lcd_write_reg(0xD230, 0x03);
    lcd_write_reg(0xD231, 0x33);
    lcd_write_reg(0xD232, 0x03);
    lcd_write_reg(0xD233, 0x6D);
    lcd_write_reg(0xD300, 0x00);
    lcd_write_reg(0xD301, 0x33);
    lcd_write_reg(0xD302, 0x00);
    lcd_write_reg(0xD303, 0x34);
    lcd_write_reg(0xD304, 0x00);
    lcd_write_reg(0xD305, 0x3A);
    lcd_write_reg(0xD306, 0x00);
    lcd_write_reg(0xD307, 0x4A);
    lcd_write_reg(0xD308, 0x00);
    lcd_write_reg(0xD309, 0x5C);
    lcd_write_reg(0xD30A, 0x00);

    lcd_write_reg(0xD30B, 0x81);
    lcd_write_reg(0xD30C, 0x00);
    lcd_write_reg(0xD30D, 0xA6);
    lcd_write_reg(0xD30E, 0x00);
    lcd_write_reg(0xD30F, 0xE5);
    lcd_write_reg(0xD310, 0x01);
    lcd_write_reg(0xD311, 0x13);
    lcd_write_reg(0xD312, 0x01);
    lcd_write_reg(0xD313, 0x54);
    lcd_write_reg(0xD314, 0x01);
    lcd_write_reg(0xD315, 0x82);
    lcd_write_reg(0xD316, 0x01);
    lcd_write_reg(0xD317, 0xCA);
    lcd_write_reg(0xD318, 0x02);
    lcd_write_reg(0xD319, 0x00);
    lcd_write_reg(0xD31A, 0x02);
    lcd_write_reg(0xD31B, 0x01);
    lcd_write_reg(0xD31C, 0x02);
    lcd_write_reg(0xD31D, 0x34);
    lcd_write_reg(0xD31E, 0x02);
    lcd_write_reg(0xD31F, 0x67);
    lcd_write_reg(0xD320, 0x02);
    lcd_write_reg(0xD321, 0x84);
    lcd_write_reg(0xD322, 0x02);
    lcd_write_reg(0xD323, 0xA4);
    lcd_write_reg(0xD324, 0x02);
    lcd_write_reg(0xD325, 0xB7);
    lcd_write_reg(0xD326, 0x02);
    lcd_write_reg(0xD327, 0xCF);
    lcd_write_reg(0xD328, 0x02);
    lcd_write_reg(0xD329, 0xDE);
    lcd_write_reg(0xD32A, 0x02);
    lcd_write_reg(0xD32B, 0xF2);
    lcd_write_reg(0xD32C, 0x02);
    lcd_write_reg(0xD32D, 0xFE);
    lcd_write_reg(0xD32E, 0x03);
    lcd_write_reg(0xD32F, 0x10);
    lcd_write_reg(0xD330, 0x03);
    lcd_write_reg(0xD331, 0x33);
    lcd_write_reg(0xD332, 0x03);
    lcd_write_reg(0xD333, 0x6D);
    lcd_write_reg(0xD400, 0x00);
    lcd_write_reg(0xD401, 0x33);
    lcd_write_reg(0xD402, 0x00);
    lcd_write_reg(0xD403, 0x34);
    lcd_write_reg(0xD404, 0x00);
    lcd_write_reg(0xD405, 0x3A);
    lcd_write_reg(0xD406, 0x00);
    lcd_write_reg(0xD407, 0x4A);
    lcd_write_reg(0xD408, 0x00);
    lcd_write_reg(0xD409, 0x5C);
    lcd_write_reg(0xD40A, 0x00);
    lcd_write_reg(0xD40B, 0x81);

    lcd_write_reg(0xD40C, 0x00);
    lcd_write_reg(0xD40D, 0xA6);
    lcd_write_reg(0xD40E, 0x00);
    lcd_write_reg(0xD40F, 0xE5);
    lcd_write_reg(0xD410, 0x01);
    lcd_write_reg(0xD411, 0x13);
    lcd_write_reg(0xD412, 0x01);
    lcd_write_reg(0xD413, 0x54);
    lcd_write_reg(0xD414, 0x01);
    lcd_write_reg(0xD415, 0x82);
    lcd_write_reg(0xD416, 0x01);
    lcd_write_reg(0xD417, 0xCA);
    lcd_write_reg(0xD418, 0x02);
    lcd_write_reg(0xD419, 0x00);
    lcd_write_reg(0xD41A, 0x02);
    lcd_write_reg(0xD41B, 0x01);
    lcd_write_reg(0xD41C, 0x02);
    lcd_write_reg(0xD41D, 0x34);
    lcd_write_reg(0xD41E, 0x02);
    lcd_write_reg(0xD41F, 0x67);
    lcd_write_reg(0xD420, 0x02);
    lcd_write_reg(0xD421, 0x84);
    lcd_write_reg(0xD422, 0x02);
    lcd_write_reg(0xD423, 0xA4);
    lcd_write_reg(0xD424, 0x02);
    lcd_write_reg(0xD425, 0xB7);
    lcd_write_reg(0xD426, 0x02);
    lcd_write_reg(0xD427, 0xCF);
    lcd_write_reg(0xD428, 0x02);
    lcd_write_reg(0xD429, 0xDE);
    lcd_write_reg(0xD42A, 0x02);
    lcd_write_reg(0xD42B, 0xF2);
    lcd_write_reg(0xD42C, 0x02);
    lcd_write_reg(0xD42D, 0xFE);
    lcd_write_reg(0xD42E, 0x03);
    lcd_write_reg(0xD42F, 0x10);
    lcd_write_reg(0xD430, 0x03);
    lcd_write_reg(0xD431, 0x33);
    lcd_write_reg(0xD432, 0x03);
    lcd_write_reg(0xD433, 0x6D);
    lcd_write_reg(0xD500, 0x00);
    lcd_write_reg(0xD501, 0x33);
    lcd_write_reg(0xD502, 0x00);
    lcd_write_reg(0xD503, 0x34);
    lcd_write_reg(0xD504, 0x00);
    lcd_write_reg(0xD505, 0x3A);
    lcd_write_reg(0xD506, 0x00);
    lcd_write_reg(0xD507, 0x4A);
    lcd_write_reg(0xD508, 0x00);
    lcd_write_reg(0xD509, 0x5C);
    lcd_write_reg(0xD50A, 0x00);
    lcd_write_reg(0xD50B, 0x81);

    lcd_write_reg(0xD50C, 0x00);
    lcd_write_reg(0xD50D, 0xA6);
    lcd_write_reg(0xD50E, 0x00);
    lcd_write_reg(0xD50F, 0xE5);
    lcd_write_reg(0xD510, 0x01);
    lcd_write_reg(0xD511, 0x13);
    lcd_write_reg(0xD512, 0x01);
    lcd_write_reg(0xD513, 0x54);
    lcd_write_reg(0xD514, 0x01);
    lcd_write_reg(0xD515, 0x82);
    lcd_write_reg(0xD516, 0x01);
    lcd_write_reg(0xD517, 0xCA);
    lcd_write_reg(0xD518, 0x02);
    lcd_write_reg(0xD519, 0x00);
    lcd_write_reg(0xD51A, 0x02);
    lcd_write_reg(0xD51B, 0x01);
    lcd_write_reg(0xD51C, 0x02);
    lcd_write_reg(0xD51D, 0x34);
    lcd_write_reg(0xD51E, 0x02);
    lcd_write_reg(0xD51F, 0x67);
    lcd_write_reg(0xD520, 0x02);
    lcd_write_reg(0xD521, 0x84);
    lcd_write_reg(0xD522, 0x02);
    lcd_write_reg(0xD523, 0xA4);
    lcd_write_reg(0xD524, 0x02);
    lcd_write_reg(0xD525, 0xB7);
    lcd_write_reg(0xD526, 0x02);
    lcd_write_reg(0xD527, 0xCF);
    lcd_write_reg(0xD528, 0x02);
    lcd_write_reg(0xD529, 0xDE);
    lcd_write_reg(0xD52A, 0x02);
    lcd_write_reg(0xD52B, 0xF2);
    lcd_write_reg(0xD52C, 0x02);
    lcd_write_reg(0xD52D, 0xFE);
    lcd_write_reg(0xD52E, 0x03);
    lcd_write_reg(0xD52F, 0x10);
    lcd_write_reg(0xD530, 0x03);
    lcd_write_reg(0xD531, 0x33);
    lcd_write_reg(0xD532, 0x03);
    lcd_write_reg(0xD533, 0x6D);
    lcd_write_reg(0xD600, 0x00);
    lcd_write_reg(0xD601, 0x33);
    lcd_write_reg(0xD602, 0x00);
    lcd_write_reg(0xD603, 0x34);
    lcd_write_reg(0xD604, 0x00);
    lcd_write_reg(0xD605, 0x3A);
    lcd_write_reg(0xD606, 0x00);
    lcd_write_reg(0xD607, 0x4A);
    lcd_write_reg(0xD608, 0x00);
    lcd_write_reg(0xD609, 0x5C);
    lcd_write_reg(0xD60A, 0x00);
    lcd_write_reg(0xD60B, 0x81);

    lcd_write_reg(0xD60C, 0x00);
    lcd_write_reg(0xD60D, 0xA6);
    lcd_write_reg(0xD60E, 0x00);
    lcd_write_reg(0xD60F, 0xE5);
    lcd_write_reg(0xD610, 0x01);
    lcd_write_reg(0xD611, 0x13);
    lcd_write_reg(0xD612, 0x01);
    lcd_write_reg(0xD613, 0x54);
    lcd_write_reg(0xD614, 0x01);
    lcd_write_reg(0xD615, 0x82);
    lcd_write_reg(0xD616, 0x01);
    lcd_write_reg(0xD617, 0xCA);
    lcd_write_reg(0xD618, 0x02);
    lcd_write_reg(0xD619, 0x00);
    lcd_write_reg(0xD61A, 0x02);
    lcd_write_reg(0xD61B, 0x01);
    lcd_write_reg(0xD61C, 0x02);
    lcd_write_reg(0xD61D, 0x34);
    lcd_write_reg(0xD61E, 0x02);
    lcd_write_reg(0xD61F, 0x67);
    lcd_write_reg(0xD620, 0x02);
    lcd_write_reg(0xD621, 0x84);
    lcd_write_reg(0xD622, 0x02);
    lcd_write_reg(0xD623, 0xA4);
    lcd_write_reg(0xD624, 0x02);
    lcd_write_reg(0xD625, 0xB7);
    lcd_write_reg(0xD626, 0x02);
    lcd_write_reg(0xD627, 0xCF);
    lcd_write_reg(0xD628, 0x02);
    lcd_write_reg(0xD629, 0xDE);
    lcd_write_reg(0xD62A, 0x02);
    lcd_write_reg(0xD62B, 0xF2);
    lcd_write_reg(0xD62C, 0x02);
    lcd_write_reg(0xD62D, 0xFE);
    lcd_write_reg(0xD62E, 0x03);
    lcd_write_reg(0xD62F, 0x10);
    lcd_write_reg(0xD630, 0x03);
    lcd_write_reg(0xD631, 0x33);
    lcd_write_reg(0xD632, 0x03);
    lcd_write_reg(0xD633, 0x6D);
    /* LV2 Page 0 enable */
    lcd_write_reg(0xF000, 0x55);
    lcd_write_reg(0xF001, 0xAA);
    lcd_write_reg(0xF002, 0x52);
    lcd_write_reg(0xF003, 0x08);
    lcd_write_reg(0xF004, 0x00);
    /* Display control */
    lcd_write_reg(0xB100, 0xCC);
    lcd_write_reg(0xB101, 0x00);
    /* Source hold time */
    lcd_write_reg(0xB600, 0x05);
    /* Gate EQ control */
    lcd_write_reg(0xB700, 0x70);
    lcd_write_reg(0xB701, 0x70);
    /* Source EQ control (Mode 2) */
    lcd_write_reg(0xB800, 0x01);
    lcd_write_reg(0xB801, 0x03);
    lcd_write_reg(0xB802, 0x03);
    lcd_write_reg(0xB803, 0x03);
    /* Inversion mode (2-dot) */
    lcd_write_reg(0xBC00, 0x02);
    lcd_write_reg(0xBC01, 0x00);
    lcd_write_reg(0xBC02, 0x00);
    /* Timing control 4H w/ 4-delay */
    lcd_write_reg(0xC900, 0xD0);
    lcd_write_reg(0xC901, 0x02);
    lcd_write_reg(0xC902, 0x50);
    lcd_write_reg(0xC903, 0x50);
    lcd_write_reg(0xC904, 0x50);
    lcd_write_reg(0x3500, 0x00);
    lcd_write_reg(0x3A00, 0x55); /* 16-bit/pixel */
    lcd_wr_regno(0x1100);
    HAL_Delay(1);
    lcd_wr_regno(0x2900);
}

/**
 * @brief       SSD1963寄存器初始化代码 
 * @param       无
 * @retval      无
 */
void lcd_ex_ssd1963_reginit(void)
{
    lcd_wr_regno(0xE2); /* Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz */
    lcd_wr_data(0x1D);  /* 参数1 */
    lcd_wr_data(0x02);  /* 参数2 Divider M = 2, PLL = 300/(M+1) = 100MHz */
    lcd_wr_data(0x04);  /* 参数3 Validate M and N values */
    HAL_Delay(1);
    lcd_wr_regno(0xE0); /*  Start PLL command */
    lcd_wr_data(0x01);  /*  enable PLL */
    HAL_Delay(10);
    lcd_wr_regno(0xE0); /*  Start PLL command again */
    lcd_wr_data(0x03);  /*  now, use PLL output as system clock */
    HAL_Delay(12);
    lcd_wr_regno(0x01); /* 软复位 */
    HAL_Delay(10);

    lcd_wr_regno(0xE6); /* 设置像素频率,33Mhz */
    lcd_wr_data(0x2F);
    lcd_wr_data(0xFF);
    lcd_wr_data(0xFF);

    lcd_wr_regno(0xB0); /* 设置LCD模式 */
    lcd_wr_data(0x20);  /* 24位模式 */
    lcd_wr_data(0x00);  /* TFT 模式 */

    lcd_wr_data((SSD_HOR_RESOLUTION - 1) >> 8); /* 设置LCD水平像素 */
    lcd_wr_data(SSD_HOR_RESOLUTION - 1);
    lcd_wr_data((SSD_VER_RESOLUTION - 1) >> 8); /* 设置LCD垂直像素 */
    lcd_wr_data(SSD_VER_RESOLUTION - 1);
    lcd_wr_data(0x00);  /* RGB序列 */

    lcd_wr_regno(0xB4); /* Set horizontal period */
    lcd_wr_data((SSD_HT - 1) >> 8);
    lcd_wr_data(SSD_HT - 1);
    lcd_wr_data(SSD_HPS >> 8);
    lcd_wr_data(SSD_HPS);
    lcd_wr_data(SSD_HOR_PULSE_WIDTH - 1);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_regno(0xB6); /* Set vertical perio */
    lcd_wr_data((SSD_VT - 1) >> 8);
    lcd_wr_data(SSD_VT - 1);
    lcd_wr_data(SSD_VPS >> 8);
    lcd_wr_data(SSD_VPS);
    lcd_wr_data(SSD_VER_FRONT_PORCH - 1);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);

    lcd_wr_regno(0xF0); /* 设置SSD1963与CPU接口为16bit */
    lcd_wr_data(0x03);  /* 16-bit(565 format) data for 16bpp */

    lcd_wr_regno(0x29); /* 开启显示 */
    /* 设置PWM输出  背光通过占空比可调 */
    lcd_wr_regno(0xD0); /* 设置自动白平衡DBC */
    lcd_wr_data(0x00);  /* disable */

    lcd_wr_regno(0xBE); /* 配置PWM输出 */
    lcd_wr_data(0x05);  /* 1设置PWM频率 */
    lcd_wr_data(0xFE);  /* 2设置PWM占空比 */
    lcd_wr_data(0x01);  /* 3设置C */
    lcd_wr_data(0x00);  /* 4设置D */
    lcd_wr_data(0x00);  /* 5设置E */
    lcd_wr_data(0x00);  /* 6设置F */

    lcd_wr_regno(0xB8); /* 设置GPIO配置 */
    lcd_wr_data(0x03);  /* 2个IO口设置成输出 */
    lcd_wr_data(0x01);  /* GPIO使用正常的IO功能 */
    lcd_wr_regno(0xBA);
    lcd_wr_data(0X01);  /* GPIO[1:0]=01,控制LCD方向 */
}
/**
 * @brief       lil9488寄存器初始化代码 
 * @param       无
 * @retval      无
 */
void lcd_ex_lil9488_reginit(void)
{

    lcd_wr_regno(0xE0);   //P-Gamma 
    lcd_wr_data(0x00); 
    lcd_wr_data(0x13); 
    lcd_wr_data(0x18); 
    lcd_wr_data(0x04); 
    lcd_wr_data(0x0F); 
    lcd_wr_data(0x06); 
    lcd_wr_data(0x3A); 
    lcd_wr_data(0x56); 
    lcd_wr_data(0x4D); 
    lcd_wr_data(0x03); 
    lcd_wr_data(0x0A); 
    lcd_wr_data(0x06); 
    lcd_wr_data(0x30); 
    lcd_wr_data(0x3E); 
    lcd_wr_data(0x0F); 
     
    lcd_wr_regno(0XE1);   //N-Gamma 
    lcd_wr_data(0x00); 
    lcd_wr_data(0x13); 
    lcd_wr_data(0x18); 
    lcd_wr_data(0x01); 
    lcd_wr_data(0x11); 
    lcd_wr_data(0x06); 
    lcd_wr_data(0x38); 
    lcd_wr_data(0x34); 
    lcd_wr_data(0x4D); 
    lcd_wr_data(0x06); 
    lcd_wr_data(0x0D); 
    lcd_wr_data(0x0B); 
    lcd_wr_data(0x31); 
    lcd_wr_data(0x37); 
    lcd_wr_data(0x0F); 

    lcd_wr_regno(0XC0);   //Power Control 1 
    lcd_wr_data(0x17);  //Vreg1out 
    lcd_wr_data(0x15);  //Verg2out 
     
    lcd_wr_regno(0xC1);   //Power Control 2   
    lcd_wr_data(0x41);  //VGH,VGL 
     
    lcd_wr_regno(0xC5);   //Power Control 3 
    lcd_wr_data(0x00); 
    lcd_wr_data(0x1A);  //Vcom 
    lcd_wr_data(0x80);
     
    lcd_wr_regno(0x36);   //Memory Access 
    lcd_wr_data(0x48); 
     
    lcd_wr_regno(0x3A);   // Interface Pixel Format 
    lcd_wr_data(0x55); 
     
    lcd_wr_regno(0XB0);   // Interface Mode Control 
    lcd_wr_data(0x00);   
     
    lcd_wr_regno(0xB1);   //Frame rate 
    lcd_wr_data(0xA0);  //60Hz 
     
    lcd_wr_regno(0xB4);   //Display Inversion Control 
    lcd_wr_data(0x02);  //2-dot 
     
    lcd_wr_regno(0XB6);   //RGB/MCU Interface Control 
    lcd_wr_data(0x02);  //MCU 
    lcd_wr_data(0x02);  //Source,Gate scan dieection 

    lcd_wr_regno(0XE9);   // Set Image Function  
    lcd_wr_data(0x00);  // Disable 24 bit data input 
     
    // Adjust Control
    lcd_wr_regno(0xF7);     
    lcd_wr_data(0xA9);   
    lcd_wr_data(0x51);   
    lcd_wr_data(0x2C);   
    lcd_wr_data(0x82);  // D7 stream, loose 

    lcd_wr_regno(0X21);
     
    lcd_wr_regno(0x11);   //Sleep out 
    HAL_Delay(120); 
    lcd_wr_regno(0x29);
}

/**
 * @brief       LCD写数据
 * @param       data: 要写入的数据
 * @retval      无
 */
void lcd_wr_data(volatile uint16_t data)
{
    data = data;            /* 使用-O2优化的时候,必须插入的延时 */
    LCD->LCD_RAM = data;
}

/**
 * @brief       LCD写寄存器编号/地址函数
 * @param       regno: 寄存器编号/地址
 * @retval      无
 */
void lcd_wr_regno(volatile uint16_t regno)
{
    regno = regno;          /* 使用-O2优化的时候,必须插入的延时 */
    LCD->LCD_REG = regno;   /* 写入要写的寄存器序号 */
}

/**
 * @brief       LCD写寄存器
 * @param       regno:寄存器编号/地址
 * @param       data:要写入的数据
 * @retval      无
 */
void lcd_write_reg(uint16_t regno, uint16_t data)
{
    LCD->LCD_REG = regno;   /* 写入要写的寄存器序号 */
    LCD->LCD_RAM = data;    /* 写入数据 */
}

/**
 * @brief       LCD读数据
 * @param       无
 * @retval      读取到的数据
 */
static uint16_t lcd_rd_data(void)
{
    volatile uint16_t ram;  /* 防止被优化 */
    ram = LCD->LCD_RAM;
    return ram;
}

/**
 * @brief       LCD延时函数,仅用于部分在mdk -O1时间优化时需要设置的地方
 * @param       t:延时的数值
 * @retval      无
 */
static void lcd_opt_delay(uint32_t i)
{
    while (i--);
}

/**
 * @brief       准备写GRAM
 * @param       无
 * @retval      无
 */
void lcd_write_ram_prepare(void)
{
    LCD->LCD_REG = lcddev.wramcmd;
}

/**
 * @brief       读取个某点的颜色值
 * @param       x,y:坐标
 * @retval      此点的颜色(32位颜色,方便兼容LTDC)
 */
uint32_t lcd_read_point(uint16_t x, uint16_t y)
{
    uint16_t r = 0, g = 0, b = 0;

    if (x >= lcddev.width || y >= lcddev.height)return 0;   /* 超过了范围,直接返回 */

    lcd_set_cursor(x, y);       /* 设置坐标 */

    if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(0X2E00);   /* 5510 发送读GRAM指令 */
    }
    else
    {
        lcd_wr_regno(0X2E);     /* 9341/5310/1963/7789/7796 等发送读GRAM指令 */
    }

    r = lcd_rd_data();  /* 假读(dummy read) */

    if (lcddev.id == 0X1963)return r;   /* 1963 直接读就可以 */

    lcd_opt_delay(2);
    r = lcd_rd_data();  /* 实际坐标颜色 */
    if(lcddev.id == 0X7796|| lcddev.id == 0X9488)
    return r;
    /* 9341/5310/5510/7789/7796/9488 要分2次读出 */
    lcd_opt_delay(2);
    b = lcd_rd_data();
    g = r & 0XFF;       /* 对于 9341/5310/5510/7789/7796/9488, 第一次读取的是RG的值,R在前,G在后,各占8位 */
    g <<= 8;
    return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));  /* 9341/5310/5510/7789/7796/9488 需要公式转换一下 */
}

/**
 * @brief       LCD开启显示
 * @param       无
 * @retval      无
 */
void lcd_display_on(void)
{
    if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(0X2900);   /* 开启显示 */
    }
    else    /* 9341/5310/1963/7789/7796/9488 等发送开启显示指令 */
    {
        lcd_wr_regno(0X29);     /* 开启显示 */
    }
}

/**
 * @brief       LCD关闭显示
 * @param       无
 * @retval      无
 */
void lcd_display_off(void)
{
    if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(0X2800);   /* 关闭显示 */
    }
    else    /* 9341/5310/1963/7789/7796/9488 等发送关闭显示指令 */
    {
        lcd_wr_regno(0X28);     /* 关闭显示 */
    }
}

/**
 * @brief       设置光标位置(对RGB屏无效)
 * @param       x,y: 坐标
 * @retval      无
 */
void lcd_set_cursor(uint16_t x, uint16_t y)
{
    if (lcddev.id == 0X1963)
    {
        if (lcddev.dir == 0)    /* 竖屏模式, x坐标需要变换 */
        {
            x = lcddev.width - 1 - x;
            lcd_wr_regno(lcddev.setxcmd);
            lcd_wr_data(0);
            lcd_wr_data(0);
            lcd_wr_data(x >> 8);
            lcd_wr_data(x & 0XFF);
        }
        else                    /* 横屏模式 */
        {
            lcd_wr_regno(lcddev.setxcmd);
            lcd_wr_data(x >> 8);
            lcd_wr_data(x & 0XFF);
            lcd_wr_data((lcddev.width - 1) >> 8);
            lcd_wr_data((lcddev.width - 1) & 0XFF);
        }

        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(y >> 8);
        lcd_wr_data(y & 0XFF);
        lcd_wr_data((lcddev.height - 1) >> 8);
        lcd_wr_data((lcddev.height - 1) & 0XFF);

    }
    else if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(x >> 8);
        lcd_wr_regno(lcddev.setxcmd + 1);
        lcd_wr_data(x & 0XFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(y >> 8);
        lcd_wr_regno(lcddev.setycmd + 1);
        lcd_wr_data(y & 0XFF);
    }
    else if (lcddev.id == 0X9488)
    {
        uint16_t twidth, theight;
        twidth = x + lcddev.width - 1;
        theight = y + lcddev.height - 1;
      
        lcd_wr_regno(lcddev.setxcmd); //Set Column Address
        lcd_wr_data(x>>8);
        lcd_wr_data(x& 0XFF);
        lcd_wr_data(lcddev.width>>8);
        lcd_wr_data(lcddev.width& 0XFF);
        lcd_wr_regno(lcddev.setycmd); //Set Page Address
        lcd_wr_data(y>>8);
        lcd_wr_data(y& 0XFF);
        lcd_wr_data(lcddev.height>>8);
        lcd_wr_data(lcddev.height& 0XFF);
    }
    else    /* 9341/5310/7789/7796 等 设置坐标 */
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(x >> 8);
        lcd_wr_data(x & 0XFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(y >> 8);
        lcd_wr_data(y & 0XFF);
    }
}

/**
 * @brief       设置LCD的自动扫描方向(对RGB屏无效)
 *   @note
 *              9341/5310/5510/1963/7789/7796等IC已经实际测试
 *              注意:其他函数可能会受到此函数设置的影响(尤其是9341),
 *              所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
 *
 * @param       dir:0~7,代表8个方向(具体定义见lcd.h)
 * @retval      无
 */
void lcd_scan_dir(uint8_t dir)
{
    uint16_t regval = 0;
    uint16_t dirreg = 0;
    uint16_t temp;

    /* 横屏时，对1963不改变扫描方向, 其他IC改变扫描方向！竖屏时1963改变方向, 其他IC不改变扫描方向 */
    if ((lcddev.dir == 1 && lcddev.id != 0X1963) || (lcddev.dir == 0 && lcddev.id == 0X1963))
    {
        switch (dir)   /* 方向转换 */
        {
            case 0:
                dir = 6;
                break;

            case 1:
                dir = 7;
                break;

            case 2:
                dir = 4;
                break;

            case 3:
                dir = 5;
                break;

            case 4:
                dir = 1;
                break;

            case 5:
                dir = 0;
                break;

            case 6:
                dir = 3;
                break;

            case 7:
                dir = 2;
                break;
        }
    }
 
    /* 根据扫描方式 设置 0X36/0X3600 寄存器 bit 5,6,7 位的值 */
    switch (dir)
    {
        case L2R_U2D:/* 从左到右,从上到下 */
            regval |= (0 << 7) | (0 << 6) | (0 << 5);
            break;

        case L2R_D2U:/* 从左到右,从下到上 */
            regval |= (1 << 7) | (0 << 6) | (0 << 5);
            break;

        case R2L_U2D:/* 从右到左,从上到下 */
            regval |= (0 << 7) | (1 << 6) | (0 << 5);
            break;

        case R2L_D2U:/* 从右到左,从下到上 */
            regval |= (1 << 7) | (1 << 6) | (0 << 5);
            break;

        case U2D_L2R:/* 从上到下,从左到右 */
            regval |= (0 << 7) | (0 << 6) | (1 << 5);
            break;

        case U2D_R2L:/* 从上到下,从右到左 */
            regval |= (0 << 7) | (1 << 6) | (1 << 5);
            break;

        case D2U_L2R:/* 从下到上,从左到右 */
            regval |= (1 << 7) | (0 << 6) | (1 << 5);
            break;

        case D2U_R2L:/* 从下到上,从右到左 */
            regval |= (1 << 7) | (1 << 6) | (1 << 5);
            break;
    }

    dirreg = 0X36;  /* 对绝大部分驱动IC, 由0X36寄存器控制 */

    if (lcddev.id == 0X5510)
    {
        dirreg = 0X3600;    /* 对于5510, 和其他驱动ic的寄存器有差异 */
    }

    /* 9341 & 7789 & 7796 要设置BGR位 */
    if (lcddev.id == 0X9341 || lcddev.id == 0X7789 || lcddev.id == 0X7796|| lcddev.id == 0X9488)
    {
        regval |= 0X08;
        if (lcddev.id == 0X7796|| lcddev.id == 0X9488)    /* 7796 X坐标镜像 */
        {
            regval |= 1<<26;
        }
    }

    lcd_write_reg(dirreg, regval);

    if (lcddev.id != 0X1963)   /* 1963不做坐标处理 */
    {
        if (regval & 0X20)
        {
            if (lcddev.width < lcddev.height)   /* 交换X,Y */
            {
                temp = lcddev.width;
                lcddev.width = lcddev.height;
                lcddev.height = temp;
            }
        }
        else
        {
            if (lcddev.width > lcddev.height)   /* 交换X,Y */
            {
                temp = lcddev.width;
                lcddev.width = lcddev.height;
                lcddev.height = temp;
            }
        }
    }

    /* 设置显示区域(开窗)大小 */
    if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(0);
        lcd_wr_regno(lcddev.setxcmd + 1);
        lcd_wr_data(0);
        lcd_wr_regno(lcddev.setxcmd + 2);
        lcd_wr_data((lcddev.width - 1) >> 8);
        lcd_wr_regno(lcddev.setxcmd + 3);
        lcd_wr_data((lcddev.width - 1) & 0XFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(0);
        lcd_wr_regno(lcddev.setycmd + 1);
        lcd_wr_data(0);
        lcd_wr_regno(lcddev.setycmd + 2);
        lcd_wr_data((lcddev.height - 1) >> 8);
        lcd_wr_regno(lcddev.setycmd + 3);
        lcd_wr_data((lcddev.height - 1) & 0XFF);
    }
    else
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(0);
        lcd_wr_data(0);
        lcd_wr_data((lcddev.width - 1) >> 8);
        lcd_wr_data((lcddev.width - 1) & 0XFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(0);
        lcd_wr_data(0);
        lcd_wr_data((lcddev.height - 1) >> 8);
        lcd_wr_data((lcddev.height - 1) & 0XFF);
    }
}

/**
 * @brief       画点
 * @param       x,y: 坐标
 * @param       color: 点的颜色(32位颜色,方便兼容LTDC)
 * @retval      无
 */
void lcd_draw_point(uint16_t x, uint16_t y, uint32_t color)
{
    lcd_set_cursor(x, y);       /* 设置光标位置 */
    lcd_write_ram_prepare();    /* 开始写入GRAM */
    LCD->LCD_RAM = color;
}

/**
 * @brief       SSD1963背光亮度设置函数
 * @param       pwm: 背光等级,0~100.越大越亮.
 * @retval      无
 */
void lcd_ssd_backlight_set(uint8_t pwm)
{
    lcd_wr_regno(0xBE);         /* 配置PWM输出 */
    lcd_wr_data(0x05);          /* 1设置PWM频率 */
    lcd_wr_data(pwm * 2.55);    /* 2设置PWM占空比 */
    lcd_wr_data(0x01);          /* 3设置C */
    lcd_wr_data(0xFF);          /* 4设置D */
    lcd_wr_data(0x00);          /* 5设置E */
    lcd_wr_data(0x00);          /* 6设置F */
}

/**
 * @brief       设置LCD显示方向
 * @param       dir:0,竖屏; 1,横屏
 * @retval      无
 */
void lcd_display_dir(uint8_t dir)
{
    lcddev.dir = dir;   /* 竖屏/横屏 */

    if (dir == 0)       /* 竖屏 */
    {
        lcddev.width = 240;
        lcddev.height = 320;

        if (lcddev.id == 0x5510)
        {
            lcddev.wramcmd = 0X2C00;
            lcddev.setxcmd = 0X2A00;
            lcddev.setycmd = 0X2B00;
            lcddev.width = 480;
            lcddev.height = 800;
        }
        else if (lcddev.id == 0X1963)
        {
            lcddev.wramcmd = 0X2C;  /* 设置写入GRAM的指令 */
            lcddev.setxcmd = 0X2B;  /* 设置写X坐标指令 */
            lcddev.setycmd = 0X2A;  /* 设置写Y坐标指令 */
            lcddev.width = 480;     /* 设置宽度480 */
            lcddev.height = 800;    /* 设置高度800 */
        }
        else   /* 其他IC, 包括: 9341 / 5310 / 7796 / 7789/9488等IC */
        {
            lcddev.wramcmd = 0X2C;
            lcddev.setxcmd = 0X2A;
            lcddev.setycmd = 0X2B;
        }

        if (lcddev.id == 0X5310 || lcddev.id == 0X7796|| lcddev.id == 0X9488) /* 如果是5310 / 7796 则表示是 320*480分辨率 */
        {
            lcddev.width = 320;
            lcddev.height = 480;
        }    
    }
    else                /* 横屏 */
    {
        lcddev.width = 320;         /* 默认宽度 */
        lcddev.height = 240;        /* 默认高度 */

        if (lcddev.id == 0x5510)
        {
            lcddev.wramcmd = 0X2C00;
            lcddev.setxcmd = 0X2A00;
            lcddev.setycmd = 0X2B00;
            lcddev.width = 800;
            lcddev.height = 480;
        }
        else if (lcddev.id == 0X1963)
        {
            lcddev.wramcmd = 0X2C;  /* 设置写入GRAM的指令 */
            lcddev.setxcmd = 0X2A;  /* 设置写X坐标指令 */
            lcddev.setycmd = 0X2B;  /* 设置写Y坐标指令 */
            lcddev.width = 800;     /* 设置宽度800 */
            lcddev.height = 480;    /* 设置高度480 */
        }
        else   /* 其他IC, 包括: 9341 / 5310 / 7796 / 7789/9488等IC */
        {
            lcddev.wramcmd = 0X2C;
            lcddev.setxcmd = 0X2A;
            lcddev.setycmd = 0X2B;
        }

        if (lcddev.id == 0X5310 || lcddev.id == 0X7796|| lcddev.id == 0X9488) /* 如果是5310 / 7796 则表示是 320*480分辨率 */
        {
            lcddev.width = 480;
            lcddev.height = 320;
        }
    }

    lcd_scan_dir(DFT_SCAN_DIR);     /* 默认扫描方向 */
}

/**
 * @brief       设置窗口(对RGB屏无效),并自动设置画点坐标到窗口左上角(sx,sy).
 * @param       sx,sy:窗口起始坐标(左上角)
 * @param       width,height:窗口宽度和高度,必须大于0!!
 *   @note      窗体大小:width*height.
 *
 * @retval      无
 */
void lcd_set_window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    uint16_t twidth, theight;
    twidth = sx + width - 1;
    theight = sy + height - 1;

    if (lcddev.id == 0X1963 && lcddev.dir != 1)    /* 1963竖屏特殊处理 */
    {
        sx = lcddev.width - width - sx;
        height = sy + height - 1;
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(sx >> 8);
        lcd_wr_data(sx & 0XFF);
        lcd_wr_data((sx + width - 1) >> 8);
        lcd_wr_data((sx + width - 1) & 0XFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(sy >> 8);
        lcd_wr_data(sy & 0XFF);
        lcd_wr_data(height >> 8);
        lcd_wr_data(height & 0XFF);
    }
    else if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(sx >> 8);
        lcd_wr_regno(lcddev.setxcmd + 1);
        lcd_wr_data(sx & 0XFF);
        lcd_wr_regno(lcddev.setxcmd + 2);
        lcd_wr_data(twidth >> 8);
        lcd_wr_regno(lcddev.setxcmd + 3);
        lcd_wr_data(twidth & 0XFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(sy >> 8);
        lcd_wr_regno(lcddev.setycmd + 1);
        lcd_wr_data(sy & 0XFF);
        lcd_wr_regno(lcddev.setycmd + 2);
        lcd_wr_data(theight >> 8);
        lcd_wr_regno(lcddev.setycmd + 3);
        lcd_wr_data(theight & 0XFF);
    }
    else if (lcddev.id == 0X9488)
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(sx >> 8);
        lcd_wr_data(sx);
        lcd_wr_data(twidth >> 8);
        lcd_wr_data(twidth);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(sy >> 8);
        lcd_wr_data(sy);
        lcd_wr_data(theight >> 8);
        lcd_wr_data(theight);
        lcd_wr_regno(0x2c);
    }
    else    /* 9341/5310/7789/7796/1963横屏 等 设置窗口 */
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(sx >> 8);
        lcd_wr_data(sx & 0XFF);
        lcd_wr_data(twidth >> 8);
        lcd_wr_data(twidth & 0XFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(sy >> 8);
        lcd_wr_data(sy & 0XFF);
        lcd_wr_data(theight >> 8);
        lcd_wr_data(theight & 0XFF);
    }
}
#if 0
/**
 * @brief       LCD写数据
 * @param       data: 要写入的数据
 * @retval      无
 */
void lcd_wr_data(volatile uint16_t data)
{
    data = data;            /* 使用-O2优化的时候,必须插入的延时 */
    LCD->LCD_RAM = data;
}

/**
 * @brief       LCD写寄存器编号/地址函数
 * @param       regno: 寄存器编号/地址
 * @retval      无
 */
void lcd_wr_regno(volatile uint16_t regno)
{
    regno = regno;          /* 使用-O2优化的时候,必须插入的延时 */
    LCD->LCD_REG = regno;   /* 写入要写的寄存器序号 */
}

/**
 * @brief       LCD写寄存器
 * @param       regno:寄存器编号/地址
 * @param       data:要写入的数据
 * @retval      无
 */
void lcd_write_reg(uint16_t regno, uint16_t data)
{
    LCD->LCD_REG = regno;   /* 写入要写的寄存器序号 */
    LCD->LCD_RAM = data;    /* 写入数据 */
}

/**
 * @brief       LCD读数据
 * @param       无
 * @retval      读取到的数据
 */
static uint16_t lcd_rd_data(void)
{
    volatile uint16_t ram;  /* 防止被优化 */
    ram = LCD->LCD_RAM;
    return ram;
}

/**
 * @brief       LCD延时函数,仅用于部分在mdk -O1时间优化时需要设置的地方
 * @param       t:延时的数值
 * @retval      无
 */
static void lcd_opt_delay(uint32_t i)
{
    while (i--);
}

/**
 * @brief       准备写GRAM
 * @param       无
 * @retval      无
 */
void lcd_write_ram_prepare(void)
{
    LCD->LCD_REG = lcddev.wramcmd;
}

/**
 * @brief       读取个某点的颜色值
 * @param       x,y:坐标
 * @retval      此点的颜色(32位颜色,方便兼容LTDC)
 */
uint32_t lcd_read_point(uint16_t x, uint16_t y)
{
    uint16_t r = 0, g = 0, b = 0;

    if (x >= lcddev.width || y >= lcddev.height)return 0;   /* 超过了范围,直接返回 */

    lcd_set_cursor(x, y);       /* 设置坐标 */

    if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(0X2E00);   /* 5510 发送读GRAM指令 */
    }
    else
    {
        lcd_wr_regno(0X2E);     /* 9341/5310/1963/7789 等发送读GRAM指令 */
    }


    r = lcd_rd_data();          /* 假读(dummy read) */

    if (lcddev.id == 0X1963)return r;   /* 1963直接读就可以 */

    lcd_opt_delay(2);
    r = lcd_rd_data();          /* 实际坐标颜色 */
    /* 9341/NT35310/NT35510要分2次读出 */
    lcd_opt_delay(2);
    b = lcd_rd_data();
    g = r & 0XFF;               /* 对于9341/5310/5510,第一次读取的是RG的值,R在前,G在后,各占8位 */
    g <<= 8;
    return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));  /* ILI9341/NT35310/NT35510需要公式转换一下 */
}

/**
 * @brief       LCD开启显示
 * @param       无
 * @retval      无
 */
void lcd_display_on(void)
{
    if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(0X2900);   /* 开启显示 */
    }
    else                        /* 9341/5310/1963/7789 等发送开启显示指令 */
    {
        lcd_wr_regno(0X29);     /* 开启显示 */
    }

}

/**
 * @brief       LCD关闭显示
 * @param       无
 * @retval      无
 */
void lcd_display_off(void)
{
    if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(0X2800);   /* 关闭显示 */
    }
    else    /* 9341/5310/1963/7789 等发送关闭显示指令 */
    {
        lcd_wr_regno(0X28);     /* 关闭显示 */
    }

}

/**
 * @brief       设置光标位置(对RGB屏无效)
 * @param       x,y: 坐标
 * @retval      无
 */
void lcd_set_cursor(uint16_t x, uint16_t y)
{
    if (lcddev.id == 0X1963)
    {
        if (lcddev.dir == 0)    /* 竖屏模式, x坐标需要变换 */
        {
            x = lcddev.width - 1 - x;
            lcd_wr_regno(lcddev.setxcmd);
            lcd_wr_data(0);
            lcd_wr_data(0);
            lcd_wr_data(x >> 8);
            lcd_wr_data(x & 0XFF);
        }
        else                    /* 横屏模式 */
        {
            lcd_wr_regno(lcddev.setxcmd);
            lcd_wr_data(x >> 8);
            lcd_wr_data(x & 0XFF);
            lcd_wr_data((lcddev.width - 1) >> 8);
            lcd_wr_data((lcddev.width - 1) & 0XFF);
        }

        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(y >> 8);
        lcd_wr_data(y & 0XFF);
        lcd_wr_data((lcddev.height - 1) >> 8);
        lcd_wr_data((lcddev.height - 1) & 0XFF);

    }
    else if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(x >> 8);
        lcd_wr_regno(lcddev.setxcmd + 1);
        lcd_wr_data(x & 0XFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(y >> 8);
        lcd_wr_regno(lcddev.setycmd + 1);
        lcd_wr_data(y & 0XFF);
    }
    else    /* 9341/5310/7789 等 设置坐标 */
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(x >> 8);
        lcd_wr_data(x & 0XFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(y >> 8);
        lcd_wr_data(y & 0XFF);
    }
}

/**
 * @brief       设置LCD的自动扫描方向(对RGB屏无效)
 *   @note
 *              9341/5310/5510/1963等IC已经实际测试
 *              注意:其他函数可能会受到此函数设置的影响(尤其是9341),
 *              所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
 *
 * @param       dir:0~7,代表8个方向(具体定义见lcd.h)
 * @retval      无
 */
void lcd_scan_dir(uint8_t dir)
{
    uint16_t regval = 0;
    uint16_t dirreg = 0;
    uint16_t temp;

    /* 横屏时，对1963不改变扫描方向！竖屏时1963改变方向(这里仅用于1963的特殊处理,对其他驱动IC无效) */
    if ((lcddev.dir == 1 && lcddev.id != 0X1963) || (lcddev.dir == 0 && lcddev.id == 0X1963))
    {
        switch (dir)   /* 方向转换 */
        {
            case 0:
                dir = 6;
                break;

            case 1:
                dir = 7;
                break;

            case 2:
                dir = 4;
                break;

            case 3:
                dir = 5;
                break;

            case 4:
                dir = 1;
                break;

            case 5:
                dir = 0;
                break;

            case 6:
                dir = 3;
                break;

            case 7:
                dir = 2;
                break;
        }
    }


    /* 根据扫描方式 设置 0X36/0X3600 寄存器 bit 5,6,7 位的值 */
    switch (dir)
    {
        case L2R_U2D:/* 从左到右,从上到下 */
            regval |= (0 << 7) | (0 << 6) | (0 << 5);
            break;

        case L2R_D2U:/* 从左到右,从下到上 */
            regval |= (1 << 7) | (0 << 6) | (0 << 5);
            break;

        case R2L_U2D:/* 从右到左,从上到下 */
            regval |= (0 << 7) | (1 << 6) | (0 << 5);
            break;

        case R2L_D2U:/* 从右到左,从下到上 */
            regval |= (1 << 7) | (1 << 6) | (0 << 5);
            break;

        case U2D_L2R:/* 从上到下,从左到右 */
            regval |= (0 << 7) | (0 << 6) | (1 << 5);
            break;

        case U2D_R2L:/* 从上到下,从右到左 */
            regval |= (0 << 7) | (1 << 6) | (1 << 5);
            break;

        case D2U_L2R:/* 从下到上,从左到右 */
            regval |= (1 << 7) | (0 << 6) | (1 << 5);
            break;

        case D2U_R2L:/* 从下到上,从右到左 */
            regval |= (1 << 7) | (1 << 6) | (1 << 5);
            break;
    }

    dirreg = 0X36;  /* 对绝大部分驱动IC, 由0X36寄存器控制 */

    if (lcddev.id == 0X5510)
    {
        dirreg = 0X3600;    /* 对于5510, 和其他驱动ic的寄存器有差异 */
    }

     /* 9341 & 7789 要设置BGR位 */
    if (lcddev.id == 0X9341 || lcddev.id == 0X7789)
    {
        regval |= 0X08;
    }

        lcd_write_reg(dirreg, regval);

        if (lcddev.id != 0X1963)                    /* 1963不做坐标处理 */
        {
            if (regval & 0X20)
            {
                if (lcddev.width < lcddev.height)   /* 交换X,Y */
                {
                    temp = lcddev.width;
                    lcddev.width = lcddev.height;
                    lcddev.height = temp;
                }
            }
            else
            {
                if (lcddev.width > lcddev.height)   /* 交换X,Y */
                {
                    temp = lcddev.width;
                    lcddev.width = lcddev.height;
                    lcddev.height = temp;
                }
            }
        }

        /* 设置显示区域(开窗)大小 */
        if (lcddev.id == 0X5510)
        {
            lcd_wr_regno(lcddev.setxcmd);
            lcd_wr_data(0);
            lcd_wr_regno(lcddev.setxcmd + 1);
            lcd_wr_data(0);
            lcd_wr_regno(lcddev.setxcmd + 2);
            lcd_wr_data((lcddev.width - 1) >> 8);
            lcd_wr_regno(lcddev.setxcmd + 3);
            lcd_wr_data((lcddev.width - 1) & 0XFF);
            lcd_wr_regno(lcddev.setycmd);
            lcd_wr_data(0);
            lcd_wr_regno(lcddev.setycmd + 1);
            lcd_wr_data(0);
            lcd_wr_regno(lcddev.setycmd + 2);
            lcd_wr_data((lcddev.height - 1) >> 8);
            lcd_wr_regno(lcddev.setycmd + 3);
            lcd_wr_data((lcddev.height - 1) & 0XFF);
        }
        else
        {
            lcd_wr_regno(lcddev.setxcmd);
            lcd_wr_data(0);
            lcd_wr_data(0);
            lcd_wr_data((lcddev.width - 1) >> 8);
            lcd_wr_data((lcddev.width - 1) & 0XFF);
            lcd_wr_regno(lcddev.setycmd);
            lcd_wr_data(0);
            lcd_wr_data(0);
            lcd_wr_data((lcddev.height - 1) >> 8);
            lcd_wr_data((lcddev.height - 1) & 0XFF);
        }
    }

/**
 * @brief       画点
 * @param       x,y: 坐标
 * @param       color: 点的颜色(32位颜色,方便兼容LTDC)
 * @retval      无
 */
void lcd_draw_point(uint16_t x, uint16_t y, uint32_t color)
{
    lcd_set_cursor(x, y);       /* 设置光标位置 */
    lcd_write_ram_prepare();    /* 开始写入GRAM */
    LCD->LCD_RAM = color;
}

/**
 * @brief       SSD1963背光亮度设置函数
 * @param       pwm: 背光等级,0~100.越大越亮.
 * @retval      无
 */
void lcd_ssd_backlight_set(uint8_t pwm)
{
    lcd_wr_regno(0xBE);         /* 配置PWM输出 */
    lcd_wr_data(0x05);          /* 1设置PWM频率 */
    lcd_wr_data(pwm * 2.55);    /* 2设置PWM占空比 */
    lcd_wr_data(0x01);          /* 3设置C */
    lcd_wr_data(0xFF);          /* 4设置D */
    lcd_wr_data(0x00);          /* 5设置E */
    lcd_wr_data(0x00);          /* 6设置F */
}

/**
 * @brief       设置LCD显示方向
 * @param       dir:0,竖屏; 1,横屏
 * @retval      无
 */
void lcd_display_dir(uint8_t dir)
{
    lcddev.dir = dir;   /* 竖屏/横屏 */

    if (dir == 0)       /* 竖屏 */
    {
        lcddev.width = 240;
        lcddev.height = 320;

        if (lcddev.id == 0x5510)
        {
            lcddev.wramcmd = 0X2C00;
            lcddev.setxcmd = 0X2A00;
            lcddev.setycmd = 0X2B00;
            lcddev.width = 480;
            lcddev.height = 800;
        }
        else if (lcddev.id == 0X1963)
        {
            lcddev.wramcmd = 0X2C;  /* 设置写入GRAM的指令 */
            lcddev.setxcmd = 0X2B;  /* 设置写X坐标指令 */
            lcddev.setycmd = 0X2A;  /* 设置写Y坐标指令 */
            lcddev.width = 480;     /* 设置宽度480 */
            lcddev.height = 800;    /* 设置高度800 */
        }
        else   /* 其他IC, 包括: 9341 / 5310 / 7789等IC */
        {
            lcddev.wramcmd = 0X2C;
            lcddev.setxcmd = 0X2A;
            lcddev.setycmd = 0X2B;
        }

        if (lcddev.id == 0X5310)    /* 如果是5310 则表示是 320*480分辨率 */
        {
            lcddev.width = 320;
            lcddev.height = 480;
        }         
    }/*dir = 0*/
    else        /* 横屏 */
    {
        lcddev.width = 320;         /* 默认宽度 */
        lcddev.height = 240;        /* 默认高度 */

        if (lcddev.id == 0x5510)
        {
            lcddev.wramcmd = 0X2C00;
            lcddev.setxcmd = 0X2A00;
            lcddev.setycmd = 0X2B00;
            lcddev.width = 800;
            lcddev.height = 480;
        }
        else if (lcddev.id == 0X1963)
        {
            lcddev.wramcmd = 0X2C;  /* 设置写入GRAM的指令 */
            lcddev.setxcmd = 0X2A;  /* 设置写X坐标指令 */
            lcddev.setycmd = 0X2B;  /* 设置写Y坐标指令 */
            lcddev.width = 800;     /* 设置宽度800 */
            lcddev.height = 480;    /* 设置高度480 */
        }
        else   /* 其他IC, 包括: 9341 / 5310 / 7789等IC */
        {
            lcddev.wramcmd = 0X2C;
            lcddev.setxcmd = 0X2A;
            lcddev.setycmd = 0X2B;
        }

        if (lcddev.id == 0X5310)
        {
            lcddev.width = 480;
            lcddev.height = 320;
        }
    }

    lcd_scan_dir(DFT_SCAN_DIR);     /* 默认扫描方向 */
}

/**
 * @brief       设置窗口(对RGB屏无效), 并自动设置画点坐标到窗口左上角(sx,sy).
 * @param       sx,sy:窗口起始坐标(左上角)
 * @param       width,height:窗口宽度和高度,必须大于0!!
 *   @note      窗体大小:width*height.
 *
 * @retval      无
 */
void lcd_set_window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    uint16_t twidth, theight;
    twidth = sx + width - 1;
    theight = sy + height - 1;

   
   if (lcddev.id == 0X1963 && lcddev.dir != 1)     /* 1963竖屏特殊处理 */
    {
        sx = lcddev.width - width - sx;
        height = sy + height - 1;
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(sx >> 8);
        lcd_wr_data(sx & 0XFF);
        lcd_wr_data((sx + width - 1) >> 8);
        lcd_wr_data((sx + width - 1) & 0XFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(sy >> 8);
        lcd_wr_data(sy & 0XFF);
        lcd_wr_data(height >> 8);
        lcd_wr_data(height & 0XFF);
    }
    else if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(sx >> 8);
        lcd_wr_regno(lcddev.setxcmd + 1);
        lcd_wr_data(sx & 0XFF);
        lcd_wr_regno(lcddev.setxcmd + 2);
        lcd_wr_data(twidth >> 8);
        lcd_wr_regno(lcddev.setxcmd + 3);
        lcd_wr_data(twidth & 0XFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(sy >> 8);
        lcd_wr_regno(lcddev.setycmd + 1);
        lcd_wr_data(sy & 0XFF);
        lcd_wr_regno(lcddev.setycmd + 2);
        lcd_wr_data(theight >> 8);
        lcd_wr_regno(lcddev.setycmd + 3);
        lcd_wr_data(theight & 0XFF);
    }
    else    /* 9341/5310/7789/1963横屏 等 设置窗口 */
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(sx >> 8);
        lcd_wr_data(sx & 0XFF);
        lcd_wr_data(twidth >> 8);
        lcd_wr_data(twidth & 0XFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(sy >> 8);
        lcd_wr_data(sy & 0XFF);
        lcd_wr_data(theight >> 8);
        lcd_wr_data(theight & 0XFF);
    }
}
#endif
#if 0
/**
 * @brief       SRAM底层驱动，时钟使能，引脚分配
 * @note        此函数会被HAL_SRAM_Init()调用,初始化读写总线引脚
 * @param       hsram:SRAM句柄
 * @retval      无
 */
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
    GPIO_InitTypeDef gpio_init_struct;

    __HAL_RCC_FSMC_CLK_ENABLE();            /* 使能FMC时钟 */
    __HAL_RCC_GPIOD_CLK_ENABLE();           /* 使能GPIOD时钟 */
    __HAL_RCC_GPIOE_CLK_ENABLE();           /* 使能GPIOE时钟 */

    /* 初始化PD0,1, 8,9,10,14,15 */
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 \
                           | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* 推挽复用 */
    gpio_init_struct.Pull = GPIO_PULLUP;                /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* 高速 */
    gpio_init_struct.Alternate = GPIO_AF12_FSMC;        /* 复用为FSMC */

    HAL_GPIO_Init(GPIOD, &gpio_init_struct);            /* 初始化 */

    /* 初始化PE7,8,9,10,11,12,13,14,15 */
    gpio_init_struct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 \
                           | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);
}
#endif

/**
 * @brief       初始化LCD
 *   @note      该初始化函数可以初始化各种型号的LCD(详见本.c文件最前面的描述)
 *
 * @param       无
 * @retval      无
 */
void lcd_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    FSMC_NORSRAM_TimingTypeDef fsmc_read_handle;
    FSMC_NORSRAM_TimingTypeDef fsmc_write_handle;

    LCD_CS_GPIO_CLK_ENABLE();   /* LCD_CS脚时钟使能 */
    LCD_WR_GPIO_CLK_ENABLE();   /* LCD_WR脚时钟使能 */
    LCD_RD_GPIO_CLK_ENABLE();   /* LCD_RD脚时钟使能 */
    LCD_RS_GPIO_CLK_ENABLE();   /* LCD_RS脚时钟使能 */
    LCD_BL_GPIO_CLK_ENABLE();   /* LCD_BL脚时钟使能 */
    LCD_RST_GPIO_CLK_ENABLE();
    
    gpio_init_struct.Pin = LCD_CS_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* 推挽复用 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    gpio_init_struct.Alternate = GPIO_AF12_FSMC;            /* 复用为FSMC */
    HAL_GPIO_Init(LCD_CS_GPIO_PORT, &gpio_init_struct);     /* 初始化LCD_CS引脚 */

    gpio_init_struct.Pin = LCD_WR_GPIO_PIN;
    HAL_GPIO_Init(LCD_WR_GPIO_PORT, &gpio_init_struct);     /* 初始化LCD_WR引脚 */

    gpio_init_struct.Pin = LCD_RD_GPIO_PIN;
    HAL_GPIO_Init(LCD_RD_GPIO_PORT, &gpio_init_struct);     /* 初始化LCD_RD引脚 */

    gpio_init_struct.Pin = LCD_RS_GPIO_PIN;
    HAL_GPIO_Init(LCD_RS_GPIO_PORT, &gpio_init_struct);     /* 初始化LCD_RS引脚 */

//    gpio_init_struct.Pin = LCD_BL_GPIO_PIN;
//    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
//    HAL_GPIO_Init(LCD_BL_GPIO_PORT, &gpio_init_struct);     /* LCD_BL引脚模式设置(推挽输出) */
    
//    gpio_init_struct.Pin = LCD_RST_GPIO_PIN;
//    HAL_GPIO_Init(LCD_RST_GPIO_PORT, &gpio_init_struct);
//    HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET);
//    HAL_Delay(12);
//    HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET);
    
    hsram1.Instance = FSMC_NORSRAM_DEVICE;
    hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
    
    hsram1.Init.NSBank = FSMC_NORSRAM_BANK4;                        /*使用NE4*/
    hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;     /*地址/数据线不复用*/
    hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;    /*16位数据宽度*/
    hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;   /*是否使能突发访问,仅对同步突发存储器有效,此处未用到*/
    hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW; /*等待信号的极性,仅在突发模式访问下有用*/
    hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;      /* 存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT */
    hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;       /* 存储器写使能 */
    hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;              /* 等待使能位,此处未用到 */
    hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;           /* 读写使用不同的时序 */
    hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;  /* 是否使能同步传输模式下的等待信号,此处未用到 */
    hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;              /* 禁止突发写 */
    
    //FMC读时序控制寄存器
    fsmc_read_handle.AddressSetupTime = 0x08;           /* 地址建立时间(ADDSET)为15个fmc_ker_ck 1/220M=4.5ns*15=67.5ns */
    fsmc_read_handle.AddressHoldTime = 0x00;
    fsmc_read_handle.DataSetupTime = 0x0F;              /* 数据保存时间(DATAST)为78个fmc_ker_ck=4.5*78=351ns */
                                                        /* 因为液晶驱动IC的读数据的时候，速度不能太快,尤其是个别奇葩芯片 */
    fsmc_read_handle.AccessMode = FSMC_ACCESS_MODE_A;   /* 模式A */
    //FMC写时序控制寄存器
    fsmc_write_handle.BusTurnAroundDuration = 0;
    fsmc_write_handle.AddressSetupTime = 4;             /* 地址建立时间(ADDSET)为15个fmc_ker_ck=67.5ns */
    fsmc_write_handle.AddressHoldTime = 0x00;
    fsmc_write_handle.DataSetupTime = 4;                /* 数据保存时间(DATAST)为15个fmc_ker_ck=67.5ns */
                                                        /* 15个fmc_ker_ck（fmc_ker_ck=220Mhz）,某些液晶驱动IC的写信号脉宽，最少也得50ns。 */
    fsmc_write_handle.AccessMode = FSMC_ACCESS_MODE_A;  /* 模式A */
    
    HAL_SRAM_Init(&hsram1, &fsmc_read_handle, &fsmc_write_handle);
    lcd_opt_delay(0x5FFFF);
    
    /* 尝试9341/7796 ID的读取 */
    lcd_wr_regno(0XD3);
    lcddev.id = lcd_rd_data();  /* dummy read */
    lcddev.id = lcd_rd_data();  /* 读到0X00 */
    lcddev.id = lcd_rd_data();  /* 读取0X93/0X77 */
    lcddev.id <<= 8;
    lcddev.id |= lcd_rd_data(); /* 读取0X41/0X96 */
    if (lcddev.id != 0X9341 && lcddev.id != 0X7796&& lcddev.id != 0X9488) /* 不是 9341 和 7796、9488,尝试看看是不是 ST7789 */
    {
        lcd_wr_regno(0X04);
        lcddev.id = lcd_rd_data();      /* dummy read */
        lcddev.id = lcd_rd_data();      /* 读到0X85 */
        lcddev.id = lcd_rd_data();      /* 读取0X85 */
        lcddev.id <<= 8;
        lcddev.id |= lcd_rd_data();     /* 读取0X52 */
        
        if (lcddev.id == 0X8552)        /* 将8552的ID转换成7789 */
        {
            lcddev.id = 0x7789;
        }

        if (lcddev.id != 0x7789)        /* 也不是ST7789, 尝试是不是 NT35310 */
        {
            lcd_wr_regno(0XD4);
            lcddev.id = lcd_rd_data();  /* dummy read */
            lcddev.id = lcd_rd_data();  /* 读回0X01 */
            lcddev.id = lcd_rd_data();  /* 读回0X53 */
            lcddev.id <<= 8;
            lcddev.id |= lcd_rd_data(); /* 这里读回0X10 */

            if (lcddev.id != 0X5310)    /* 也不是NT35310,尝试看看是不是NT35510 */
            {
                lcd_wr_regno(0XDA00);
                lcddev.id = lcd_rd_data();  /* 读回0X00 */
                lcd_wr_regno(0XDB00);
                lcddev.id = lcd_rd_data();  /* 读回0X80 */
                lcddev.id <<= 8;
                lcd_wr_regno(0XDC00);
                lcddev.id |= lcd_rd_data(); /* 读回0X00 */

                if (lcddev.id == 0x8000)lcddev.id = 0x5510; /* NT35510读回的ID是8000H,为方便区分,我们强制设置为5510 */

                if (lcddev.id != 0X5510)        /* 也不是NT5510,尝试看看是不是SSD1963 */
                {
                    lcd_wr_regno(0XA1);
                    lcddev.id = lcd_rd_data();
                    lcddev.id = lcd_rd_data();  /* 读回0X57 */
                    lcddev.id <<= 8;
                    lcddev.id |= lcd_rd_data(); /* 读回0X61 */

                    if (lcddev.id == 0X5761)lcddev.id = 0X1963; /* SSD1963读回的ID是5761H,为方便区分,我们强制设置为1963 */
                }
            }
        }
    }
    
//    /* 尝试9341 ID的读取 */
//    lcd_wr_regno(0XD3);
//    lcddev.id = lcd_rd_data();  /* dummy read */
//    lcddev.id = lcd_rd_data();  /* 读到0X00 */
//    lcddev.id = lcd_rd_data();  /* 读取93 */
//    lcddev.id <<= 8;
//    lcddev.id |= lcd_rd_data(); /* 读取41 */

//    if (lcddev.id != 0X9341)    /* 不是 9341 , 尝试看看是不是 ST7789 */
//    {
//        lcd_wr_regno(0X04);
//        lcddev.id = lcd_rd_data();      /* dummy read */
//        lcddev.id = lcd_rd_data();      /* 读到0X85 */
//        lcddev.id = lcd_rd_data();      /* 读取0X85 */
//        lcddev.id <<= 8;
//        lcddev.id |= lcd_rd_data();     /* 读取0X52 */
//        
//        if (lcddev.id == 0X8552)        /* 将8552的ID转换成7789 */
//        {
//            lcddev.id = 0x7789;
//        }

//        if (lcddev.id != 0x7789)        /* 也不是ST7789, 尝试是不是 NT35310 */
//        {
//            lcd_wr_regno(0XD4);
//            lcddev.id = lcd_rd_data();  /* dummy read */
//            lcddev.id = lcd_rd_data();  /* 读回0X01 */
//            lcddev.id = lcd_rd_data();  /* 读回0X53 */
//            lcddev.id <<= 8;
//            lcddev.id |= lcd_rd_data(); /* 这里读回0X10 */

//            if (lcddev.id != 0X5310)    /* 也不是NT35310,尝试看看是不是NT35510 */
//            {
//                lcd_wr_regno(0XDA00);
//                lcddev.id = lcd_rd_data();  /* 读回0X00 */
//                lcd_wr_regno(0XDB00);
//                lcddev.id = lcd_rd_data();  /* 读回0X80 */
//                lcddev.id <<= 8;
//                lcd_wr_regno(0XDC00);
//                lcddev.id |= lcd_rd_data(); /* 读回0X00 */

//                if (lcddev.id == 0x8000)lcddev.id = 0x5510; /* NT35510读回的ID是8000H,为方便区分,我们强制设置为5510 */

//                if (lcddev.id != 0X5510)        /* 也不是NT5510,尝试看看是不是SSD1963 */
//                {
//                    lcd_wr_regno(0XA1);
//                    lcddev.id = lcd_rd_data();
//                    lcddev.id = lcd_rd_data();  /* 读回0X57 */
//                    lcddev.id <<= 8;
//                    lcddev.id |= lcd_rd_data(); /* 读回0X61 */

//                    if (lcddev.id == 0X5761)lcddev.id = 0X1963; /* SSD1963读回的ID是5761H,为方便区分,我们强制设置为1963 */
//                }
//            }
//        }
//    }

    /* 特别注意, 如果在main函数里面屏蔽串口1初始化, 则会卡死在printf
     * 里面(卡死在f_putc函数), 所以, 必须初始化串口1, 或者屏蔽掉下面
     * 这行 printf 语句 !!!!!!!
     */
    
//    char buf[120];
//    sprintf(buf,"LCD ID:%x\r\n", lcddev.id);
//    CDC_Transmit_FS(buf,strlen((const char *)buf));
    //printf("LCD ID:%x\r\n", lcddev.id); /* 打印LCD ID */
    
//    if (lcddev.id == 0X7789)
//    {
//        lcd_ex_st7789_reginit();        /* 执行ST7789初始化 */
//    }
//    else if (lcddev.id == 0X9341)
//    {
//        lcd_ex_ili9341_reginit();       /* 执行ILI9341初始化 */
//    }
//    else if (lcddev.id == 0x5310)
//    {
//        lcd_ex_nt35310_reginit();       /* 执行NT35310初始化 */
//    }
//    else if (lcddev.id == 0x5510)
//    {
//        lcd_ex_nt35510_reginit();       /* 执行NT35510初始化 */
//    }
//    else if (lcddev.id == 0X1963)
//    {
//        lcd_ex_ssd1963_reginit();       /* 执行SSD1963初始化 */
//        lcd_ssd_backlight_set(100);     /* 背光设置为最亮 */
//    }

//    /* 初始化完成以后,提速 */
//    if (lcddev.id == 0X9341 || lcddev.id == 0X1963)  /* 如果是这几个IC, 则设置WR时序为最快 */
//    {
//        /* 重新配置写时序控制寄存器的时序 */
//        fsmc_write_handle.AddressSetupTime = 3;
//        fsmc_write_handle.DataSetupTime = 3;         
//        FSMC_NORSRAM_Extended_Timing_Init(hsram1.Extended, &fsmc_write_handle, hsram1.Init.NSBank, hsram1.Init.ExtendedMode);
//    }

//    if (lcddev.id == 0X5310 || lcddev.id == 0X5510)  /* 如果是这几个IC, 则设置WR时序为最快 */
//    {
//        /* 重新配置写时序控制寄存器的时序 */
//        fsmc_write_handle.AddressSetupTime = 2;
//        fsmc_write_handle.DataSetupTime = 2;
//        FSMC_NORSRAM_Extended_Timing_Init(hsram1.Extended, &fsmc_write_handle, hsram1.Init.NSBank, hsram1.Init.ExtendedMode);
//    }

    if (lcddev.id == 0X7789)
    {
        lcd_ex_st7789_reginit();    /* 执行ST7789初始化 */
    }
    else if (lcddev.id == 0X7796)
    {
        lcd_ex_st7796_reginit();    /* 执行ST7796初始化 */
    }
    else if (lcddev.id == 0X9341)
    {
        lcd_ex_ili9341_reginit();   /* 执行ILI9341初始化 */
    }
    else if (lcddev.id == 0x5310)
    {
        lcd_ex_nt35310_reginit();   /* 执行NT35310初始化 */
    }
    else if (lcddev.id == 0x5510)
    {
        lcd_ex_nt35510_reginit();   /* 执行NT35510初始化 */
    }
    else if (lcddev.id == 0X1963)
    {
        lcd_ex_ssd1963_reginit();   /* 执行SSD1963初始化 */
        lcd_ssd_backlight_set(100); /* 背光设置为最亮 */
    }
    else if (lcddev.id == 0x9488)
    {
        lcd_ex_lil9488_reginit();   /* 执行9488初始化 */
    }
    
    lcd_display_dir(1); /* 默认为竖屏 */
    LCD_BL(1);          /* 点亮背光 */
    lcd_clear(BLACK);
    
    lcd_wr_regno(0x36);   //Memory Access 
    //lcd_wr_data(0x48);//竖屏左上
    //lcd_wr_data(0x88);//竖屏右上
    lcd_wr_data(0x28);//横屏正显
    //lcd_wr_data(0xE8);//横屏倒显
    
    /* 重新配置写时序控制寄存器的时序 */
    fsmc_write_handle.AddressSetupTime = 3;
    fsmc_write_handle.DataSetupTime = 3;         
    FSMC_NORSRAM_Extended_Timing_Init(hsram1.Extended, &fsmc_write_handle, hsram1.Init.NSBank, hsram1.Init.ExtendedMode);
}
#if 0
/**
 * @brief       清屏函数
 * @param       color: 要清屏的颜色
 * @retval      无
 */
void lcd_clear(uint16_t color)
{
    uint32_t index = 0;
    uint32_t totalpoint = lcddev.width;
    totalpoint *= lcddev.height;    /* 得到总点数 */
    lcd_set_cursor(0x00, 0x0000);   /* 设置光标位置 */
    lcd_write_ram_prepare();        /* 开始写入GRAM */

    for (index = 0; index < totalpoint; index++)
    {
        LCD->LCD_RAM = color;
    }
}

/**
 * @brief       在指定区域内填充单个颜色
 * @param       (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex - sx + 1) * (ey - sy + 1)
 * @param       color:  要填充的颜色(32位颜色,方便兼容LTDC)
 * @retval      无
 */
void lcd_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{
    uint16_t i, j;
    uint16_t xlen = 0;
    xlen = ex - sx + 1;

    for (i = sy; i <= ey; i++)
    {
        lcd_set_cursor(sx, i);      /* 设置光标位置 */
        lcd_write_ram_prepare();    /* 开始写入GRAM */

        for (j = 0; j < xlen; j++)
        {
            LCD->LCD_RAM = color;   /* 显示颜色 */
        }
    }
}

/**
 * @brief       在指定区域内填充指定颜色块
 * @param       (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex - sx + 1) * (ey - sy + 1)
 * @param       color: 要填充的颜色数组首地址
 * @retval      无
 */
void lcd_color_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{
    uint16_t height, width;
    uint16_t i, j;
    width = ex - sx + 1;            /* 得到填充的宽度 */
    height = ey - sy + 1;           /* 高度 */

    for (i = 0; i < height; i++)
    {
        lcd_set_cursor(sx, sy + i); /* 设置光标位置 */
        lcd_write_ram_prepare();    /* 开始写入GRAM */

        for (j = 0; j < width; j++)
        {
            LCD->LCD_RAM = color[i * width + j]; /* 写入数据 */
        }
    }
}
#endif
/**
 * @brief       清屏函数
 * @param       color: 要清屏的颜色
 * @retval      无
 */
void lcd_clear(uint16_t color)
{
    uint32_t index = 0;
    uint32_t totalpoint = lcddev.width;
    totalpoint *= lcddev.height;    /* 得到总点数 */
    lcd_set_cursor(0x00, 0x0000);   /* 设置光标位置 */
    lcd_write_ram_prepare();        /* 开始写入GRAM */

    for (index = 0; index < totalpoint; index++)
    {
        LCD->LCD_RAM = color;
   }
}

/**
 * @brief       在指定区域内填充单个颜色
 * @param       (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex - sx + 1) * (ey - sy + 1)
 * @param       color: 要填充的颜色(32位颜色,方便兼容LTDC)
 * @retval      无
 */
void lcd_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{
    uint16_t i, j;
    uint16_t xlen = 0;
    xlen = ex - sx + 1;

    for (i = sy; i <= ey; i++)
    {
        lcd_set_cursor(sx, i);      /* 设置光标位置 */
        lcd_write_ram_prepare();    /* 开始写入GRAM */

        for (j = 0; j < xlen; j++)
        {
            LCD->LCD_RAM = color;   /* 显示颜色 */
        }
    }
}

/**
 * @brief       在指定区域内填充指定颜色块
 * @param       (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex - sx + 1) * (ey - sy + 1)
 * @param       color: 要填充的颜色数组首地址
 * @retval      无
 */
void lcd_color_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{
    uint16_t height, width;
    uint16_t i, j;
    width = ex - sx + 1;            /* 得到填充的宽度 */
    height = ey - sy + 1;           /* 高度 */

    for (i = 0; i < height; i++)
    {
        lcd_set_cursor(sx, sy + i); /* 设置光标位置 */
        lcd_write_ram_prepare();    /* 开始写入GRAM */

        for (j = 0; j < width; j++)
        {
            LCD->LCD_RAM = color[i * width + j]; /* 写入数据 */
        }
    }
}

/**
 * @brief       画线
 * @param       x1,y1: 起点坐标
 * @param       x2,y2: 终点坐标
 * @param       color: 线的颜色
 * @retval      无
 */
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    delta_x = x2 - x1;              /* 计算坐标增量 */
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)incx = 1;       /* 设置单步方向 */
    else if (delta_x == 0)incx = 0; /* 垂直线 */
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0; /* 水平线 */
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if ( delta_x > delta_y)distance = delta_x;  /* 选取基本增量坐标轴 */
    else distance = delta_y;

    for (t = 0; t <= distance + 1; t++ )        /* 画线输出 */
    {
        lcd_draw_point(row, col, color);        /* 画点 */
        xerr += delta_x ;
        yerr += delta_y ;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

/**
 * @brief       画水平线
 * @param       x0,y0: 起点坐标
 * @param       len  : 线长度
 * @param       color: 矩形的颜色
 * @retval      无
 */
void lcd_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
{
    if ((len == 0) || (x > lcddev.width) || (y > lcddev.height))return;

    lcd_fill(x, y, x + len - 1, y, color);
}

/**
 * @brief       画矩形
 * @param       x1,y1: 起点坐标
 * @param       x2,y2: 终点坐标
 * @param       color: 矩形的颜色
 * @retval      无
 */
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    lcd_draw_line(x1, y1, x2, y1, color);
    lcd_draw_line(x1, y1, x1, y2, color);
    lcd_draw_line(x1, y2, x2, y2, color);
    lcd_draw_line(x2, y1, x2, y2, color);
}

/**
 * @brief       画圆
 * @param       x,y  : 圆中心坐标
 * @param       r    : 半径
 * @param       color: 圆的颜色
 * @retval      无
 */
void lcd_draw_circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);       /* 判断下个点位置的标志 */

    while (a <= b)
    {
        lcd_draw_point(x0 + a, y0 - b, color);  /* 5 */
        lcd_draw_point(x0 + b, y0 - a, color);  /* 0 */
        lcd_draw_point(x0 + b, y0 + a, color);  /* 4 */
        lcd_draw_point(x0 + a, y0 + b, color);  /* 6 */
        lcd_draw_point(x0 - a, y0 + b, color);  /* 1 */
        lcd_draw_point(x0 - b, y0 + a, color);
        lcd_draw_point(x0 - a, y0 - b, color);  /* 2 */
        lcd_draw_point(x0 - b, y0 - a, color);  /* 7 */
        a++;

        /* 使用Bresenham算法画圆 */
        if (di < 0)
        {
            di += 4 * a + 6;
        }
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}

/**
 * @brief       填充实心圆
 * @param       x0,y0: 圆中心坐标
 * @param       r    : 半径
 * @param       color: 圆的颜色
 * @retval      无
 */
void lcd_fill_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    uint32_t i;
    uint32_t imax = ((uint32_t)r * 707) / 1000 + 1;
    uint32_t sqmax = (uint32_t)r * (uint32_t)r + (uint32_t)r / 2;
    uint32_t xr = r;

    lcd_draw_hline(x - r, y, 2 * r, color);

    for (i = 1; i <= imax; i++)
    {
        if ((i * i + xr * xr) > sqmax)
        {
            /* draw lines from outside */
            if (xr > imax)
            {
                lcd_draw_hline (x - i + 1, y + xr, 2 * (i - 1), color);
                lcd_draw_hline (x - i + 1, y - xr, 2 * (i - 1), color);
            }
            xr--;
        }
        /* draw lines from inside (center) */
        lcd_draw_hline(x - xr, y + i, 2 * xr, color);
        lcd_draw_hline(x - xr, y - i, 2 * xr, color);
    }
}

/**
 * @brief       在指定位置显示一个字符
 * @param       x,y  : 坐标
 * @param       chr  : 要显示的字符:" "--->"~"
 * @param       size : 字体大小 12/16/24/32
 * @param       mode : 叠加方式(1); 非叠加方式(0);
 * @retval      无
 */
void lcd_show_char(uint16_t x, uint16_t y, char chr, uint8_t size, uint8_t mode, uint16_t color)
{
    uint8_t temp, t1, t;
    uint16_t y0 = y;
    uint8_t csize = 0;
    uint8_t *pfont = 0;

    csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); /* 得到字体一个字符对应点阵集所占的字节数 */
    chr = chr - ' ';    /* 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库） */

    switch (size)
    {
        case 12:
            pfont = (uint8_t *)asc2_1206[chr];  /* 调用1206字体 */
            break;

        case 16:
            pfont = (uint8_t *)asc2_1608[chr];  /* 调用1608字体 */
            break;

        case 24:
            pfont = (uint8_t *)asc2_2412[chr];  /* 调用2412字体 */
            break;

        case 32:
            pfont = (uint8_t *)asc2_3216[chr];  /* 调用3216字体 */
            break;

        default:
            return ;
    }

    for (t = 0; t < csize; t++)
    {
        temp = pfont[t];                            /* 获取字符的点阵数据 */

        for (t1 = 0; t1 < 8; t1++)                  /* 一个字节8个点 */
        {
            if (temp & 0x80)                        /* 有效点,需要显示 */
            {
                lcd_draw_point(x, y, color);        /* 画点出来,要显示这个点 */
            }
            else if (mode == 0)                     /* 无效点,不显示 */
            {
                lcd_draw_point(x, y, g_back_color); /* 画背景色,相当于这个点不显示(注意背景色由全局变量控制) */
            }

            temp <<= 1;                             /* 移位, 以便获取下一个位的状态 */
            y++;

            if (y >= lcddev.height)return;          /* 超区域了 */

            if ((y - y0) == size)                   /* 显示完一列了? */
            {
                y = y0; /* y坐标复位 */
                x++;    /* x坐标递增 */

                if (x >= lcddev.width)return;       /* x坐标超区域了 */

                break;
            }
        }
    }
}

/**
 * @brief       平方函数, m^n
 * @param       m: 底数
 * @param       n: 指数
 * @retval      m的n次方
 */
static uint32_t lcd_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while (n--)result *= m;

    return result;
}

/**
 * @brief       显示len个数字
 * @param       x,y : 起始坐标
 * @param       num : 数值(0 ~ 2^32)
 * @param       len : 显示数字的位数
 * @param       size: 选择字体 12/16/24/32
 * @retval      无
 */
void lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)   /* 按总显示位数循环 */
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;   /* 获取对应位的数字 */

        if (enshow == 0 && t < (len - 1))               /* 没有使能显示,且还有位要显示 */
        {
            if (temp == 0)
            {
                lcd_show_char(x + (size / 2)*t, y, ' ', size, 0, color);/* 显示空格,占位 */
                continue;   /* 继续下个一位 */
            }
            else
            {
                enshow = 1; /* 使能显示 */
            }

        }

        lcd_show_char(x + (size / 2)*t, y, temp + '0', size, 0, color); /* 显示字符 */
    }
}

/**
 * @brief       扩展显示len个数字(高位是0也显示)
 * @param       x,y : 起始坐标
 * @param       num : 数值(0 ~ 2^32)
 * @param       len : 显示数字的位数
 * @param       size: 选择字体 12/16/24/32
 * @param       mode: 显示模式
 *              [7]:0,不填充;1,填充0.
 *              [6:1]:保留
 *              [0]:0,非叠加显示;1,叠加显示.
 *
 * @retval      无
 */
void lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode, uint16_t color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)   /* 按总显示位数循环 */
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;    /* 获取对应位的数字 */

        if (enshow == 0 && t < (len - 1))   /* 没有使能显示,且还有位要显示 */
        {
            if (temp == 0)
            {
                if (mode & 0X80)            /* 高位需要填充0 */
                {
                    lcd_show_char(x + (size / 2)*t, y, '0', size, mode & 0X01, color);  /* 用0占位 */
                }
                else
                {
                    lcd_show_char(x + (size / 2)*t, y, ' ', size, mode & 0X01, color);  /* 用空格占位 */
                }

                continue;
            }
            else
            {
                enshow = 1; /* 使能显示 */
            }

        }

        lcd_show_char(x + (size / 2)*t, y, temp + '0', size, mode & 0X01, color);
    }
}

/**
 * @brief       显示字符串
 * @param       x,y         : 起始坐标
 * @param       width,height: 区域大小
 * @param       size        : 选择字体 12/16/24/32
 * @param       p           : 字符串首地址
 * @retval      无
 */
void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color)
{
    uint8_t x0 = x;
    width += x;
    height += y;

    while ((*p <= '~') && (*p >= ' '))   /* 判断是不是非法字符! */
    {
        if (x >= width)
        {
            x = x0;
            y += size;
        }

        if (y >= height)break;          /* 退出 */

        lcd_show_char(x, y, *p, size, 0, color);
        x += size / 2;
        p++;
    }
}















