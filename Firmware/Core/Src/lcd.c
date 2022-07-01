#include "lcd.h"
#include "lcdfont.h"

extern SRAM_HandleTypeDef hsram1;   /* SRAM���(���ڿ���LCD) */

/* LCD�Ļ�����ɫ�ͱ���ɫ */
uint32_t g_point_color = WHITE;    /* ������ɫ */
uint32_t g_back_color  = BLACK;    /* ����ɫ */

/* ����LCD��Ҫ���� */
_lcd_dev lcddev;

/**
 * @brief       ST7789 �Ĵ�����ʼ������
 * @param       ��
 * @retval      ��
 */
void lcd_ex_st7789_reginit(void)
{
    lcd_wr_regno(0x11);

    HAL_Delay(120); 

    lcd_wr_regno(0x21);//��P100��Ļ��ɫ��ʾ������
    
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
 * @brief       ST7796 �Ĵ�����ʼ������
 * @param       ��
 * @retval      ��
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
 * @brief       ILI9341�Ĵ�����ʼ������
 * @param       ��
 * @retval      ��
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
 * @brief       NT35310�Ĵ�����ʼ������ 
 * @param       ��
 * @retval      ��
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
 * @brief       NT35510�Ĵ�����ʼ������ 
 * @param       ��
 * @retval      ��
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
 * @brief       SSD1963�Ĵ�����ʼ������ 
 * @param       ��
 * @retval      ��
 */
void lcd_ex_ssd1963_reginit(void)
{
    lcd_wr_regno(0xE2); /* Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz */
    lcd_wr_data(0x1D);  /* ����1 */
    lcd_wr_data(0x02);  /* ����2 Divider M = 2, PLL = 300/(M+1) = 100MHz */
    lcd_wr_data(0x04);  /* ����3 Validate M and N values */
    HAL_Delay(1);
    lcd_wr_regno(0xE0); /*  Start PLL command */
    lcd_wr_data(0x01);  /*  enable PLL */
    HAL_Delay(10);
    lcd_wr_regno(0xE0); /*  Start PLL command again */
    lcd_wr_data(0x03);  /*  now, use PLL output as system clock */
    HAL_Delay(12);
    lcd_wr_regno(0x01); /* ��λ */
    HAL_Delay(10);

    lcd_wr_regno(0xE6); /* ��������Ƶ��,33Mhz */
    lcd_wr_data(0x2F);
    lcd_wr_data(0xFF);
    lcd_wr_data(0xFF);

    lcd_wr_regno(0xB0); /* ����LCDģʽ */
    lcd_wr_data(0x20);  /* 24λģʽ */
    lcd_wr_data(0x00);  /* TFT ģʽ */

    lcd_wr_data((SSD_HOR_RESOLUTION - 1) >> 8); /* ����LCDˮƽ���� */
    lcd_wr_data(SSD_HOR_RESOLUTION - 1);
    lcd_wr_data((SSD_VER_RESOLUTION - 1) >> 8); /* ����LCD��ֱ���� */
    lcd_wr_data(SSD_VER_RESOLUTION - 1);
    lcd_wr_data(0x00);  /* RGB���� */

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

    lcd_wr_regno(0xF0); /* ����SSD1963��CPU�ӿ�Ϊ16bit */
    lcd_wr_data(0x03);  /* 16-bit(565 format) data for 16bpp */

    lcd_wr_regno(0x29); /* ������ʾ */
    /* ����PWM���  ����ͨ��ռ�ձȿɵ� */
    lcd_wr_regno(0xD0); /* �����Զ���ƽ��DBC */
    lcd_wr_data(0x00);  /* disable */

    lcd_wr_regno(0xBE); /* ����PWM��� */
    lcd_wr_data(0x05);  /* 1����PWMƵ�� */
    lcd_wr_data(0xFE);  /* 2����PWMռ�ձ� */
    lcd_wr_data(0x01);  /* 3����C */
    lcd_wr_data(0x00);  /* 4����D */
    lcd_wr_data(0x00);  /* 5����E */
    lcd_wr_data(0x00);  /* 6����F */

    lcd_wr_regno(0xB8); /* ����GPIO���� */
    lcd_wr_data(0x03);  /* 2��IO�����ó���� */
    lcd_wr_data(0x01);  /* GPIOʹ��������IO���� */
    lcd_wr_regno(0xBA);
    lcd_wr_data(0X01);  /* GPIO[1:0]=01,����LCD���� */
}
/**
 * @brief       lil9488�Ĵ�����ʼ������ 
 * @param       ��
 * @retval      ��
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
 * @brief       LCDд����
 * @param       data: Ҫд�������
 * @retval      ��
 */
void lcd_wr_data(volatile uint16_t data)
{
    data = data;            /* ʹ��-O2�Ż���ʱ��,����������ʱ */
    LCD->LCD_RAM = data;
}

/**
 * @brief       LCDд�Ĵ������/��ַ����
 * @param       regno: �Ĵ������/��ַ
 * @retval      ��
 */
void lcd_wr_regno(volatile uint16_t regno)
{
    regno = regno;          /* ʹ��-O2�Ż���ʱ��,����������ʱ */
    LCD->LCD_REG = regno;   /* д��Ҫд�ļĴ������ */
}

/**
 * @brief       LCDд�Ĵ���
 * @param       regno:�Ĵ������/��ַ
 * @param       data:Ҫд�������
 * @retval      ��
 */
void lcd_write_reg(uint16_t regno, uint16_t data)
{
    LCD->LCD_REG = regno;   /* д��Ҫд�ļĴ������ */
    LCD->LCD_RAM = data;    /* д������ */
}

/**
 * @brief       LCD������
 * @param       ��
 * @retval      ��ȡ��������
 */
static uint16_t lcd_rd_data(void)
{
    volatile uint16_t ram;  /* ��ֹ���Ż� */
    ram = LCD->LCD_RAM;
    return ram;
}

/**
 * @brief       LCD��ʱ����,�����ڲ�����mdk -O1ʱ���Ż�ʱ��Ҫ���õĵط�
 * @param       t:��ʱ����ֵ
 * @retval      ��
 */
static void lcd_opt_delay(uint32_t i)
{
    while (i--);
}

/**
 * @brief       ׼��дGRAM
 * @param       ��
 * @retval      ��
 */
void lcd_write_ram_prepare(void)
{
    LCD->LCD_REG = lcddev.wramcmd;
}

/**
 * @brief       ��ȡ��ĳ�����ɫֵ
 * @param       x,y:����
 * @retval      �˵����ɫ(32λ��ɫ,�������LTDC)
 */
uint32_t lcd_read_point(uint16_t x, uint16_t y)
{
    uint16_t r = 0, g = 0, b = 0;

    if (x >= lcddev.width || y >= lcddev.height)return 0;   /* �����˷�Χ,ֱ�ӷ��� */

    lcd_set_cursor(x, y);       /* �������� */

    if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(0X2E00);   /* 5510 ���Ͷ�GRAMָ�� */
    }
    else
    {
        lcd_wr_regno(0X2E);     /* 9341/5310/1963/7789/7796 �ȷ��Ͷ�GRAMָ�� */
    }

    r = lcd_rd_data();  /* �ٶ�(dummy read) */

    if (lcddev.id == 0X1963)return r;   /* 1963 ֱ�Ӷ��Ϳ��� */

    lcd_opt_delay(2);
    r = lcd_rd_data();  /* ʵ��������ɫ */
    if(lcddev.id == 0X7796|| lcddev.id == 0X9488)
    return r;
    /* 9341/5310/5510/7789/7796/9488 Ҫ��2�ζ��� */
    lcd_opt_delay(2);
    b = lcd_rd_data();
    g = r & 0XFF;       /* ���� 9341/5310/5510/7789/7796/9488, ��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ */
    g <<= 8;
    return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));  /* 9341/5310/5510/7789/7796/9488 ��Ҫ��ʽת��һ�� */
}

/**
 * @brief       LCD������ʾ
 * @param       ��
 * @retval      ��
 */
void lcd_display_on(void)
{
    if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(0X2900);   /* ������ʾ */
    }
    else    /* 9341/5310/1963/7789/7796/9488 �ȷ��Ϳ�����ʾָ�� */
    {
        lcd_wr_regno(0X29);     /* ������ʾ */
    }
}

/**
 * @brief       LCD�ر���ʾ
 * @param       ��
 * @retval      ��
 */
void lcd_display_off(void)
{
    if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(0X2800);   /* �ر���ʾ */
    }
    else    /* 9341/5310/1963/7789/7796/9488 �ȷ��͹ر���ʾָ�� */
    {
        lcd_wr_regno(0X28);     /* �ر���ʾ */
    }
}

/**
 * @brief       ���ù��λ��(��RGB����Ч)
 * @param       x,y: ����
 * @retval      ��
 */
void lcd_set_cursor(uint16_t x, uint16_t y)
{
    if (lcddev.id == 0X1963)
    {
        if (lcddev.dir == 0)    /* ����ģʽ, x������Ҫ�任 */
        {
            x = lcddev.width - 1 - x;
            lcd_wr_regno(lcddev.setxcmd);
            lcd_wr_data(0);
            lcd_wr_data(0);
            lcd_wr_data(x >> 8);
            lcd_wr_data(x & 0XFF);
        }
        else                    /* ����ģʽ */
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
    else    /* 9341/5310/7789/7796 �� �������� */
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
 * @brief       ����LCD���Զ�ɨ�跽��(��RGB����Ч)
 *   @note
 *              9341/5310/5510/1963/7789/7796��IC�Ѿ�ʵ�ʲ���
 *              ע��:�����������ܻ��ܵ��˺������õ�Ӱ��(������9341),
 *              ����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
 *
 * @param       dir:0~7,����8������(���嶨���lcd.h)
 * @retval      ��
 */
void lcd_scan_dir(uint8_t dir)
{
    uint16_t regval = 0;
    uint16_t dirreg = 0;
    uint16_t temp;

    /* ����ʱ����1963���ı�ɨ�跽��, ����IC�ı�ɨ�跽������ʱ1963�ı䷽��, ����IC���ı�ɨ�跽�� */
    if ((lcddev.dir == 1 && lcddev.id != 0X1963) || (lcddev.dir == 0 && lcddev.id == 0X1963))
    {
        switch (dir)   /* ����ת�� */
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
 
    /* ����ɨ�跽ʽ ���� 0X36/0X3600 �Ĵ��� bit 5,6,7 λ��ֵ */
    switch (dir)
    {
        case L2R_U2D:/* ������,���ϵ��� */
            regval |= (0 << 7) | (0 << 6) | (0 << 5);
            break;

        case L2R_D2U:/* ������,���µ��� */
            regval |= (1 << 7) | (0 << 6) | (0 << 5);
            break;

        case R2L_U2D:/* ���ҵ���,���ϵ��� */
            regval |= (0 << 7) | (1 << 6) | (0 << 5);
            break;

        case R2L_D2U:/* ���ҵ���,���µ��� */
            regval |= (1 << 7) | (1 << 6) | (0 << 5);
            break;

        case U2D_L2R:/* ���ϵ���,������ */
            regval |= (0 << 7) | (0 << 6) | (1 << 5);
            break;

        case U2D_R2L:/* ���ϵ���,���ҵ��� */
            regval |= (0 << 7) | (1 << 6) | (1 << 5);
            break;

        case D2U_L2R:/* ���µ���,������ */
            regval |= (1 << 7) | (0 << 6) | (1 << 5);
            break;

        case D2U_R2L:/* ���µ���,���ҵ��� */
            regval |= (1 << 7) | (1 << 6) | (1 << 5);
            break;
    }

    dirreg = 0X36;  /* �Ծ��󲿷�����IC, ��0X36�Ĵ������� */

    if (lcddev.id == 0X5510)
    {
        dirreg = 0X3600;    /* ����5510, ����������ic�ļĴ����в��� */
    }

    /* 9341 & 7789 & 7796 Ҫ����BGRλ */
    if (lcddev.id == 0X9341 || lcddev.id == 0X7789 || lcddev.id == 0X7796|| lcddev.id == 0X9488)
    {
        regval |= 0X08;
        if (lcddev.id == 0X7796|| lcddev.id == 0X9488)    /* 7796 X���꾵�� */
        {
            regval |= 1<<26;
        }
    }

    lcd_write_reg(dirreg, regval);

    if (lcddev.id != 0X1963)   /* 1963�������괦�� */
    {
        if (regval & 0X20)
        {
            if (lcddev.width < lcddev.height)   /* ����X,Y */
            {
                temp = lcddev.width;
                lcddev.width = lcddev.height;
                lcddev.height = temp;
            }
        }
        else
        {
            if (lcddev.width > lcddev.height)   /* ����X,Y */
            {
                temp = lcddev.width;
                lcddev.width = lcddev.height;
                lcddev.height = temp;
            }
        }
    }

    /* ������ʾ����(����)��С */
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
 * @brief       ����
 * @param       x,y: ����
 * @param       color: �����ɫ(32λ��ɫ,�������LTDC)
 * @retval      ��
 */
void lcd_draw_point(uint16_t x, uint16_t y, uint32_t color)
{
    lcd_set_cursor(x, y);       /* ���ù��λ�� */
    lcd_write_ram_prepare();    /* ��ʼд��GRAM */
    LCD->LCD_RAM = color;
}

/**
 * @brief       SSD1963�����������ú���
 * @param       pwm: ����ȼ�,0~100.Խ��Խ��.
 * @retval      ��
 */
void lcd_ssd_backlight_set(uint8_t pwm)
{
    lcd_wr_regno(0xBE);         /* ����PWM��� */
    lcd_wr_data(0x05);          /* 1����PWMƵ�� */
    lcd_wr_data(pwm * 2.55);    /* 2����PWMռ�ձ� */
    lcd_wr_data(0x01);          /* 3����C */
    lcd_wr_data(0xFF);          /* 4����D */
    lcd_wr_data(0x00);          /* 5����E */
    lcd_wr_data(0x00);          /* 6����F */
}

/**
 * @brief       ����LCD��ʾ����
 * @param       dir:0,����; 1,����
 * @retval      ��
 */
void lcd_display_dir(uint8_t dir)
{
    lcddev.dir = dir;   /* ����/���� */

    if (dir == 0)       /* ���� */
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
            lcddev.wramcmd = 0X2C;  /* ����д��GRAM��ָ�� */
            lcddev.setxcmd = 0X2B;  /* ����дX����ָ�� */
            lcddev.setycmd = 0X2A;  /* ����дY����ָ�� */
            lcddev.width = 480;     /* ���ÿ��480 */
            lcddev.height = 800;    /* ���ø߶�800 */
        }
        else   /* ����IC, ����: 9341 / 5310 / 7796 / 7789/9488��IC */
        {
            lcddev.wramcmd = 0X2C;
            lcddev.setxcmd = 0X2A;
            lcddev.setycmd = 0X2B;
        }

        if (lcddev.id == 0X5310 || lcddev.id == 0X7796|| lcddev.id == 0X9488) /* �����5310 / 7796 ���ʾ�� 320*480�ֱ��� */
        {
            lcddev.width = 320;
            lcddev.height = 480;
        }    
    }
    else                /* ���� */
    {
        lcddev.width = 320;         /* Ĭ�Ͽ�� */
        lcddev.height = 240;        /* Ĭ�ϸ߶� */

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
            lcddev.wramcmd = 0X2C;  /* ����д��GRAM��ָ�� */
            lcddev.setxcmd = 0X2A;  /* ����дX����ָ�� */
            lcddev.setycmd = 0X2B;  /* ����дY����ָ�� */
            lcddev.width = 800;     /* ���ÿ��800 */
            lcddev.height = 480;    /* ���ø߶�480 */
        }
        else   /* ����IC, ����: 9341 / 5310 / 7796 / 7789/9488��IC */
        {
            lcddev.wramcmd = 0X2C;
            lcddev.setxcmd = 0X2A;
            lcddev.setycmd = 0X2B;
        }

        if (lcddev.id == 0X5310 || lcddev.id == 0X7796|| lcddev.id == 0X9488) /* �����5310 / 7796 ���ʾ�� 320*480�ֱ��� */
        {
            lcddev.width = 480;
            lcddev.height = 320;
        }
    }

    lcd_scan_dir(DFT_SCAN_DIR);     /* Ĭ��ɨ�跽�� */
}

/**
 * @brief       ���ô���(��RGB����Ч),���Զ����û������굽�������Ͻ�(sx,sy).
 * @param       sx,sy:������ʼ����(���Ͻ�)
 * @param       width,height:���ڿ�Ⱥ͸߶�,�������0!!
 *   @note      �����С:width*height.
 *
 * @retval      ��
 */
void lcd_set_window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    uint16_t twidth, theight;
    twidth = sx + width - 1;
    theight = sy + height - 1;

    if (lcddev.id == 0X1963 && lcddev.dir != 1)    /* 1963�������⴦�� */
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
    else    /* 9341/5310/7789/7796/1963���� �� ���ô��� */
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
 * @brief       LCDд����
 * @param       data: Ҫд�������
 * @retval      ��
 */
void lcd_wr_data(volatile uint16_t data)
{
    data = data;            /* ʹ��-O2�Ż���ʱ��,����������ʱ */
    LCD->LCD_RAM = data;
}

/**
 * @brief       LCDд�Ĵ������/��ַ����
 * @param       regno: �Ĵ������/��ַ
 * @retval      ��
 */
void lcd_wr_regno(volatile uint16_t regno)
{
    regno = regno;          /* ʹ��-O2�Ż���ʱ��,����������ʱ */
    LCD->LCD_REG = regno;   /* д��Ҫд�ļĴ������ */
}

/**
 * @brief       LCDд�Ĵ���
 * @param       regno:�Ĵ������/��ַ
 * @param       data:Ҫд�������
 * @retval      ��
 */
void lcd_write_reg(uint16_t regno, uint16_t data)
{
    LCD->LCD_REG = regno;   /* д��Ҫд�ļĴ������ */
    LCD->LCD_RAM = data;    /* д������ */
}

/**
 * @brief       LCD������
 * @param       ��
 * @retval      ��ȡ��������
 */
static uint16_t lcd_rd_data(void)
{
    volatile uint16_t ram;  /* ��ֹ���Ż� */
    ram = LCD->LCD_RAM;
    return ram;
}

/**
 * @brief       LCD��ʱ����,�����ڲ�����mdk -O1ʱ���Ż�ʱ��Ҫ���õĵط�
 * @param       t:��ʱ����ֵ
 * @retval      ��
 */
static void lcd_opt_delay(uint32_t i)
{
    while (i--);
}

/**
 * @brief       ׼��дGRAM
 * @param       ��
 * @retval      ��
 */
void lcd_write_ram_prepare(void)
{
    LCD->LCD_REG = lcddev.wramcmd;
}

/**
 * @brief       ��ȡ��ĳ�����ɫֵ
 * @param       x,y:����
 * @retval      �˵����ɫ(32λ��ɫ,�������LTDC)
 */
uint32_t lcd_read_point(uint16_t x, uint16_t y)
{
    uint16_t r = 0, g = 0, b = 0;

    if (x >= lcddev.width || y >= lcddev.height)return 0;   /* �����˷�Χ,ֱ�ӷ��� */

    lcd_set_cursor(x, y);       /* �������� */

    if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(0X2E00);   /* 5510 ���Ͷ�GRAMָ�� */
    }
    else
    {
        lcd_wr_regno(0X2E);     /* 9341/5310/1963/7789 �ȷ��Ͷ�GRAMָ�� */
    }


    r = lcd_rd_data();          /* �ٶ�(dummy read) */

    if (lcddev.id == 0X1963)return r;   /* 1963ֱ�Ӷ��Ϳ��� */

    lcd_opt_delay(2);
    r = lcd_rd_data();          /* ʵ��������ɫ */
    /* 9341/NT35310/NT35510Ҫ��2�ζ��� */
    lcd_opt_delay(2);
    b = lcd_rd_data();
    g = r & 0XFF;               /* ����9341/5310/5510,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ */
    g <<= 8;
    return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));  /* ILI9341/NT35310/NT35510��Ҫ��ʽת��һ�� */
}

/**
 * @brief       LCD������ʾ
 * @param       ��
 * @retval      ��
 */
void lcd_display_on(void)
{
    if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(0X2900);   /* ������ʾ */
    }
    else                        /* 9341/5310/1963/7789 �ȷ��Ϳ�����ʾָ�� */
    {
        lcd_wr_regno(0X29);     /* ������ʾ */
    }

}

/**
 * @brief       LCD�ر���ʾ
 * @param       ��
 * @retval      ��
 */
void lcd_display_off(void)
{
    if (lcddev.id == 0X5510)
    {
        lcd_wr_regno(0X2800);   /* �ر���ʾ */
    }
    else    /* 9341/5310/1963/7789 �ȷ��͹ر���ʾָ�� */
    {
        lcd_wr_regno(0X28);     /* �ر���ʾ */
    }

}

/**
 * @brief       ���ù��λ��(��RGB����Ч)
 * @param       x,y: ����
 * @retval      ��
 */
void lcd_set_cursor(uint16_t x, uint16_t y)
{
    if (lcddev.id == 0X1963)
    {
        if (lcddev.dir == 0)    /* ����ģʽ, x������Ҫ�任 */
        {
            x = lcddev.width - 1 - x;
            lcd_wr_regno(lcddev.setxcmd);
            lcd_wr_data(0);
            lcd_wr_data(0);
            lcd_wr_data(x >> 8);
            lcd_wr_data(x & 0XFF);
        }
        else                    /* ����ģʽ */
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
    else    /* 9341/5310/7789 �� �������� */
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
 * @brief       ����LCD���Զ�ɨ�跽��(��RGB����Ч)
 *   @note
 *              9341/5310/5510/1963��IC�Ѿ�ʵ�ʲ���
 *              ע��:�����������ܻ��ܵ��˺������õ�Ӱ��(������9341),
 *              ����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
 *
 * @param       dir:0~7,����8������(���嶨���lcd.h)
 * @retval      ��
 */
void lcd_scan_dir(uint8_t dir)
{
    uint16_t regval = 0;
    uint16_t dirreg = 0;
    uint16_t temp;

    /* ����ʱ����1963���ı�ɨ�跽������ʱ1963�ı䷽��(���������1963�����⴦��,����������IC��Ч) */
    if ((lcddev.dir == 1 && lcddev.id != 0X1963) || (lcddev.dir == 0 && lcddev.id == 0X1963))
    {
        switch (dir)   /* ����ת�� */
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


    /* ����ɨ�跽ʽ ���� 0X36/0X3600 �Ĵ��� bit 5,6,7 λ��ֵ */
    switch (dir)
    {
        case L2R_U2D:/* ������,���ϵ��� */
            regval |= (0 << 7) | (0 << 6) | (0 << 5);
            break;

        case L2R_D2U:/* ������,���µ��� */
            regval |= (1 << 7) | (0 << 6) | (0 << 5);
            break;

        case R2L_U2D:/* ���ҵ���,���ϵ��� */
            regval |= (0 << 7) | (1 << 6) | (0 << 5);
            break;

        case R2L_D2U:/* ���ҵ���,���µ��� */
            regval |= (1 << 7) | (1 << 6) | (0 << 5);
            break;

        case U2D_L2R:/* ���ϵ���,������ */
            regval |= (0 << 7) | (0 << 6) | (1 << 5);
            break;

        case U2D_R2L:/* ���ϵ���,���ҵ��� */
            regval |= (0 << 7) | (1 << 6) | (1 << 5);
            break;

        case D2U_L2R:/* ���µ���,������ */
            regval |= (1 << 7) | (0 << 6) | (1 << 5);
            break;

        case D2U_R2L:/* ���µ���,���ҵ��� */
            regval |= (1 << 7) | (1 << 6) | (1 << 5);
            break;
    }

    dirreg = 0X36;  /* �Ծ��󲿷�����IC, ��0X36�Ĵ������� */

    if (lcddev.id == 0X5510)
    {
        dirreg = 0X3600;    /* ����5510, ����������ic�ļĴ����в��� */
    }

     /* 9341 & 7789 Ҫ����BGRλ */
    if (lcddev.id == 0X9341 || lcddev.id == 0X7789)
    {
        regval |= 0X08;
    }

        lcd_write_reg(dirreg, regval);

        if (lcddev.id != 0X1963)                    /* 1963�������괦�� */
        {
            if (regval & 0X20)
            {
                if (lcddev.width < lcddev.height)   /* ����X,Y */
                {
                    temp = lcddev.width;
                    lcddev.width = lcddev.height;
                    lcddev.height = temp;
                }
            }
            else
            {
                if (lcddev.width > lcddev.height)   /* ����X,Y */
                {
                    temp = lcddev.width;
                    lcddev.width = lcddev.height;
                    lcddev.height = temp;
                }
            }
        }

        /* ������ʾ����(����)��С */
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
 * @brief       ����
 * @param       x,y: ����
 * @param       color: �����ɫ(32λ��ɫ,�������LTDC)
 * @retval      ��
 */
void lcd_draw_point(uint16_t x, uint16_t y, uint32_t color)
{
    lcd_set_cursor(x, y);       /* ���ù��λ�� */
    lcd_write_ram_prepare();    /* ��ʼд��GRAM */
    LCD->LCD_RAM = color;
}

/**
 * @brief       SSD1963�����������ú���
 * @param       pwm: ����ȼ�,0~100.Խ��Խ��.
 * @retval      ��
 */
void lcd_ssd_backlight_set(uint8_t pwm)
{
    lcd_wr_regno(0xBE);         /* ����PWM��� */
    lcd_wr_data(0x05);          /* 1����PWMƵ�� */
    lcd_wr_data(pwm * 2.55);    /* 2����PWMռ�ձ� */
    lcd_wr_data(0x01);          /* 3����C */
    lcd_wr_data(0xFF);          /* 4����D */
    lcd_wr_data(0x00);          /* 5����E */
    lcd_wr_data(0x00);          /* 6����F */
}

/**
 * @brief       ����LCD��ʾ����
 * @param       dir:0,����; 1,����
 * @retval      ��
 */
void lcd_display_dir(uint8_t dir)
{
    lcddev.dir = dir;   /* ����/���� */

    if (dir == 0)       /* ���� */
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
            lcddev.wramcmd = 0X2C;  /* ����д��GRAM��ָ�� */
            lcddev.setxcmd = 0X2B;  /* ����дX����ָ�� */
            lcddev.setycmd = 0X2A;  /* ����дY����ָ�� */
            lcddev.width = 480;     /* ���ÿ��480 */
            lcddev.height = 800;    /* ���ø߶�800 */
        }
        else   /* ����IC, ����: 9341 / 5310 / 7789��IC */
        {
            lcddev.wramcmd = 0X2C;
            lcddev.setxcmd = 0X2A;
            lcddev.setycmd = 0X2B;
        }

        if (lcddev.id == 0X5310)    /* �����5310 ���ʾ�� 320*480�ֱ��� */
        {
            lcddev.width = 320;
            lcddev.height = 480;
        }         
    }/*dir = 0*/
    else        /* ���� */
    {
        lcddev.width = 320;         /* Ĭ�Ͽ�� */
        lcddev.height = 240;        /* Ĭ�ϸ߶� */

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
            lcddev.wramcmd = 0X2C;  /* ����д��GRAM��ָ�� */
            lcddev.setxcmd = 0X2A;  /* ����дX����ָ�� */
            lcddev.setycmd = 0X2B;  /* ����дY����ָ�� */
            lcddev.width = 800;     /* ���ÿ��800 */
            lcddev.height = 480;    /* ���ø߶�480 */
        }
        else   /* ����IC, ����: 9341 / 5310 / 7789��IC */
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

    lcd_scan_dir(DFT_SCAN_DIR);     /* Ĭ��ɨ�跽�� */
}

/**
 * @brief       ���ô���(��RGB����Ч), ���Զ����û������굽�������Ͻ�(sx,sy).
 * @param       sx,sy:������ʼ����(���Ͻ�)
 * @param       width,height:���ڿ�Ⱥ͸߶�,�������0!!
 *   @note      �����С:width*height.
 *
 * @retval      ��
 */
void lcd_set_window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    uint16_t twidth, theight;
    twidth = sx + width - 1;
    theight = sy + height - 1;

   
   if (lcddev.id == 0X1963 && lcddev.dir != 1)     /* 1963�������⴦�� */
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
    else    /* 9341/5310/7789/1963���� �� ���ô��� */
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
 * @brief       SRAM�ײ�������ʱ��ʹ�ܣ����ŷ���
 * @note        �˺����ᱻHAL_SRAM_Init()����,��ʼ����д��������
 * @param       hsram:SRAM���
 * @retval      ��
 */
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
    GPIO_InitTypeDef gpio_init_struct;

    __HAL_RCC_FSMC_CLK_ENABLE();            /* ʹ��FMCʱ�� */
    __HAL_RCC_GPIOD_CLK_ENABLE();           /* ʹ��GPIODʱ�� */
    __HAL_RCC_GPIOE_CLK_ENABLE();           /* ʹ��GPIOEʱ�� */

    /* ��ʼ��PD0,1, 8,9,10,14,15 */
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 \
                           | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* ���츴�� */
    gpio_init_struct.Pull = GPIO_PULLUP;                /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* ���� */
    gpio_init_struct.Alternate = GPIO_AF12_FSMC;        /* ����ΪFSMC */

    HAL_GPIO_Init(GPIOD, &gpio_init_struct);            /* ��ʼ�� */

    /* ��ʼ��PE7,8,9,10,11,12,13,14,15 */
    gpio_init_struct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 \
                           | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);
}
#endif

/**
 * @brief       ��ʼ��LCD
 *   @note      �ó�ʼ���������Գ�ʼ�������ͺŵ�LCD(�����.c�ļ���ǰ�������)
 *
 * @param       ��
 * @retval      ��
 */
void lcd_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    FSMC_NORSRAM_TimingTypeDef fsmc_read_handle;
    FSMC_NORSRAM_TimingTypeDef fsmc_write_handle;

    LCD_CS_GPIO_CLK_ENABLE();   /* LCD_CS��ʱ��ʹ�� */
    LCD_WR_GPIO_CLK_ENABLE();   /* LCD_WR��ʱ��ʹ�� */
    LCD_RD_GPIO_CLK_ENABLE();   /* LCD_RD��ʱ��ʹ�� */
    LCD_RS_GPIO_CLK_ENABLE();   /* LCD_RS��ʱ��ʹ�� */
    LCD_BL_GPIO_CLK_ENABLE();   /* LCD_BL��ʱ��ʹ�� */
    LCD_RST_GPIO_CLK_ENABLE();
    
    gpio_init_struct.Pin = LCD_CS_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* ���츴�� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    gpio_init_struct.Alternate = GPIO_AF12_FSMC;            /* ����ΪFSMC */
    HAL_GPIO_Init(LCD_CS_GPIO_PORT, &gpio_init_struct);     /* ��ʼ��LCD_CS���� */

    gpio_init_struct.Pin = LCD_WR_GPIO_PIN;
    HAL_GPIO_Init(LCD_WR_GPIO_PORT, &gpio_init_struct);     /* ��ʼ��LCD_WR���� */

    gpio_init_struct.Pin = LCD_RD_GPIO_PIN;
    HAL_GPIO_Init(LCD_RD_GPIO_PORT, &gpio_init_struct);     /* ��ʼ��LCD_RD���� */

    gpio_init_struct.Pin = LCD_RS_GPIO_PIN;
    HAL_GPIO_Init(LCD_RS_GPIO_PORT, &gpio_init_struct);     /* ��ʼ��LCD_RS���� */

//    gpio_init_struct.Pin = LCD_BL_GPIO_PIN;
//    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
//    HAL_GPIO_Init(LCD_BL_GPIO_PORT, &gpio_init_struct);     /* LCD_BL����ģʽ����(�������) */
    
//    gpio_init_struct.Pin = LCD_RST_GPIO_PIN;
//    HAL_GPIO_Init(LCD_RST_GPIO_PORT, &gpio_init_struct);
//    HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET);
//    HAL_Delay(12);
//    HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET);
    
    hsram1.Instance = FSMC_NORSRAM_DEVICE;
    hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
    
    hsram1.Init.NSBank = FSMC_NORSRAM_BANK4;                        /*ʹ��NE4*/
    hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;     /*��ַ/�����߲�����*/
    hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;    /*16λ���ݿ��*/
    hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;   /*�Ƿ�ʹ��ͻ������,����ͬ��ͻ���洢����Ч,�˴�δ�õ�*/
    hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW; /*�ȴ��źŵļ���,����ͻ��ģʽ����������*/
    hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;      /* �洢�����ڵȴ�����֮ǰ��һ��ʱ�����ڻ��ǵȴ������ڼ�ʹ��NWAIT */
    hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;       /* �洢��дʹ�� */
    hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;              /* �ȴ�ʹ��λ,�˴�δ�õ� */
    hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;           /* ��дʹ�ò�ͬ��ʱ�� */
    hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;  /* �Ƿ�ʹ��ͬ������ģʽ�µĵȴ��ź�,�˴�δ�õ� */
    hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;              /* ��ֹͻ��д */
    
    //FMC��ʱ����ƼĴ���
    fsmc_read_handle.AddressSetupTime = 0x08;           /* ��ַ����ʱ��(ADDSET)Ϊ15��fmc_ker_ck 1/220M=4.5ns*15=67.5ns */
    fsmc_read_handle.AddressHoldTime = 0x00;
    fsmc_read_handle.DataSetupTime = 0x0F;              /* ���ݱ���ʱ��(DATAST)Ϊ78��fmc_ker_ck=4.5*78=351ns */
                                                        /* ��ΪҺ������IC�Ķ����ݵ�ʱ���ٶȲ���̫��,�����Ǹ�������оƬ */
    fsmc_read_handle.AccessMode = FSMC_ACCESS_MODE_A;   /* ģʽA */
    //FMCдʱ����ƼĴ���
    fsmc_write_handle.BusTurnAroundDuration = 0;
    fsmc_write_handle.AddressSetupTime = 4;             /* ��ַ����ʱ��(ADDSET)Ϊ15��fmc_ker_ck=67.5ns */
    fsmc_write_handle.AddressHoldTime = 0x00;
    fsmc_write_handle.DataSetupTime = 4;                /* ���ݱ���ʱ��(DATAST)Ϊ15��fmc_ker_ck=67.5ns */
                                                        /* 15��fmc_ker_ck��fmc_ker_ck=220Mhz��,ĳЩҺ������IC��д�ź���������Ҳ��50ns�� */
    fsmc_write_handle.AccessMode = FSMC_ACCESS_MODE_A;  /* ģʽA */
    
    HAL_SRAM_Init(&hsram1, &fsmc_read_handle, &fsmc_write_handle);
    lcd_opt_delay(0x5FFFF);
    
    /* ����9341/7796 ID�Ķ�ȡ */
    lcd_wr_regno(0XD3);
    lcddev.id = lcd_rd_data();  /* dummy read */
    lcddev.id = lcd_rd_data();  /* ����0X00 */
    lcddev.id = lcd_rd_data();  /* ��ȡ0X93/0X77 */
    lcddev.id <<= 8;
    lcddev.id |= lcd_rd_data(); /* ��ȡ0X41/0X96 */
    if (lcddev.id != 0X9341 && lcddev.id != 0X7796&& lcddev.id != 0X9488) /* ���� 9341 �� 7796��9488,���Կ����ǲ��� ST7789 */
    {
        lcd_wr_regno(0X04);
        lcddev.id = lcd_rd_data();      /* dummy read */
        lcddev.id = lcd_rd_data();      /* ����0X85 */
        lcddev.id = lcd_rd_data();      /* ��ȡ0X85 */
        lcddev.id <<= 8;
        lcddev.id |= lcd_rd_data();     /* ��ȡ0X52 */
        
        if (lcddev.id == 0X8552)        /* ��8552��IDת����7789 */
        {
            lcddev.id = 0x7789;
        }

        if (lcddev.id != 0x7789)        /* Ҳ����ST7789, �����ǲ��� NT35310 */
        {
            lcd_wr_regno(0XD4);
            lcddev.id = lcd_rd_data();  /* dummy read */
            lcddev.id = lcd_rd_data();  /* ����0X01 */
            lcddev.id = lcd_rd_data();  /* ����0X53 */
            lcddev.id <<= 8;
            lcddev.id |= lcd_rd_data(); /* �������0X10 */

            if (lcddev.id != 0X5310)    /* Ҳ����NT35310,���Կ����ǲ���NT35510 */
            {
                lcd_wr_regno(0XDA00);
                lcddev.id = lcd_rd_data();  /* ����0X00 */
                lcd_wr_regno(0XDB00);
                lcddev.id = lcd_rd_data();  /* ����0X80 */
                lcddev.id <<= 8;
                lcd_wr_regno(0XDC00);
                lcddev.id |= lcd_rd_data(); /* ����0X00 */

                if (lcddev.id == 0x8000)lcddev.id = 0x5510; /* NT35510���ص�ID��8000H,Ϊ��������,����ǿ������Ϊ5510 */

                if (lcddev.id != 0X5510)        /* Ҳ����NT5510,���Կ����ǲ���SSD1963 */
                {
                    lcd_wr_regno(0XA1);
                    lcddev.id = lcd_rd_data();
                    lcddev.id = lcd_rd_data();  /* ����0X57 */
                    lcddev.id <<= 8;
                    lcddev.id |= lcd_rd_data(); /* ����0X61 */

                    if (lcddev.id == 0X5761)lcddev.id = 0X1963; /* SSD1963���ص�ID��5761H,Ϊ��������,����ǿ������Ϊ1963 */
                }
            }
        }
    }
    
//    /* ����9341 ID�Ķ�ȡ */
//    lcd_wr_regno(0XD3);
//    lcddev.id = lcd_rd_data();  /* dummy read */
//    lcddev.id = lcd_rd_data();  /* ����0X00 */
//    lcddev.id = lcd_rd_data();  /* ��ȡ93 */
//    lcddev.id <<= 8;
//    lcddev.id |= lcd_rd_data(); /* ��ȡ41 */

//    if (lcddev.id != 0X9341)    /* ���� 9341 , ���Կ����ǲ��� ST7789 */
//    {
//        lcd_wr_regno(0X04);
//        lcddev.id = lcd_rd_data();      /* dummy read */
//        lcddev.id = lcd_rd_data();      /* ����0X85 */
//        lcddev.id = lcd_rd_data();      /* ��ȡ0X85 */
//        lcddev.id <<= 8;
//        lcddev.id |= lcd_rd_data();     /* ��ȡ0X52 */
//        
//        if (lcddev.id == 0X8552)        /* ��8552��IDת����7789 */
//        {
//            lcddev.id = 0x7789;
//        }

//        if (lcddev.id != 0x7789)        /* Ҳ����ST7789, �����ǲ��� NT35310 */
//        {
//            lcd_wr_regno(0XD4);
//            lcddev.id = lcd_rd_data();  /* dummy read */
//            lcddev.id = lcd_rd_data();  /* ����0X01 */
//            lcddev.id = lcd_rd_data();  /* ����0X53 */
//            lcddev.id <<= 8;
//            lcddev.id |= lcd_rd_data(); /* �������0X10 */

//            if (lcddev.id != 0X5310)    /* Ҳ����NT35310,���Կ����ǲ���NT35510 */
//            {
//                lcd_wr_regno(0XDA00);
//                lcddev.id = lcd_rd_data();  /* ����0X00 */
//                lcd_wr_regno(0XDB00);
//                lcddev.id = lcd_rd_data();  /* ����0X80 */
//                lcddev.id <<= 8;
//                lcd_wr_regno(0XDC00);
//                lcddev.id |= lcd_rd_data(); /* ����0X00 */

//                if (lcddev.id == 0x8000)lcddev.id = 0x5510; /* NT35510���ص�ID��8000H,Ϊ��������,����ǿ������Ϊ5510 */

//                if (lcddev.id != 0X5510)        /* Ҳ����NT5510,���Կ����ǲ���SSD1963 */
//                {
//                    lcd_wr_regno(0XA1);
//                    lcddev.id = lcd_rd_data();
//                    lcddev.id = lcd_rd_data();  /* ����0X57 */
//                    lcddev.id <<= 8;
//                    lcddev.id |= lcd_rd_data(); /* ����0X61 */

//                    if (lcddev.id == 0X5761)lcddev.id = 0X1963; /* SSD1963���ص�ID��5761H,Ϊ��������,����ǿ������Ϊ1963 */
//                }
//            }
//        }
//    }

    /* �ر�ע��, �����main�����������δ���1��ʼ��, ��Ῠ����printf
     * ����(������f_putc����), ����, �����ʼ������1, �������ε�����
     * ���� printf ��� !!!!!!!
     */
    
//    char buf[120];
//    sprintf(buf,"LCD ID:%x\r\n", lcddev.id);
//    CDC_Transmit_FS(buf,strlen((const char *)buf));
    //printf("LCD ID:%x\r\n", lcddev.id); /* ��ӡLCD ID */
    
//    if (lcddev.id == 0X7789)
//    {
//        lcd_ex_st7789_reginit();        /* ִ��ST7789��ʼ�� */
//    }
//    else if (lcddev.id == 0X9341)
//    {
//        lcd_ex_ili9341_reginit();       /* ִ��ILI9341��ʼ�� */
//    }
//    else if (lcddev.id == 0x5310)
//    {
//        lcd_ex_nt35310_reginit();       /* ִ��NT35310��ʼ�� */
//    }
//    else if (lcddev.id == 0x5510)
//    {
//        lcd_ex_nt35510_reginit();       /* ִ��NT35510��ʼ�� */
//    }
//    else if (lcddev.id == 0X1963)
//    {
//        lcd_ex_ssd1963_reginit();       /* ִ��SSD1963��ʼ�� */
//        lcd_ssd_backlight_set(100);     /* ��������Ϊ���� */
//    }

//    /* ��ʼ������Ժ�,���� */
//    if (lcddev.id == 0X9341 || lcddev.id == 0X1963)  /* ������⼸��IC, ������WRʱ��Ϊ��� */
//    {
//        /* ��������дʱ����ƼĴ�����ʱ�� */
//        fsmc_write_handle.AddressSetupTime = 3;
//        fsmc_write_handle.DataSetupTime = 3;         
//        FSMC_NORSRAM_Extended_Timing_Init(hsram1.Extended, &fsmc_write_handle, hsram1.Init.NSBank, hsram1.Init.ExtendedMode);
//    }

//    if (lcddev.id == 0X5310 || lcddev.id == 0X5510)  /* ������⼸��IC, ������WRʱ��Ϊ��� */
//    {
//        /* ��������дʱ����ƼĴ�����ʱ�� */
//        fsmc_write_handle.AddressSetupTime = 2;
//        fsmc_write_handle.DataSetupTime = 2;
//        FSMC_NORSRAM_Extended_Timing_Init(hsram1.Extended, &fsmc_write_handle, hsram1.Init.NSBank, hsram1.Init.ExtendedMode);
//    }

    if (lcddev.id == 0X7789)
    {
        lcd_ex_st7789_reginit();    /* ִ��ST7789��ʼ�� */
    }
    else if (lcddev.id == 0X7796)
    {
        lcd_ex_st7796_reginit();    /* ִ��ST7796��ʼ�� */
    }
    else if (lcddev.id == 0X9341)
    {
        lcd_ex_ili9341_reginit();   /* ִ��ILI9341��ʼ�� */
    }
    else if (lcddev.id == 0x5310)
    {
        lcd_ex_nt35310_reginit();   /* ִ��NT35310��ʼ�� */
    }
    else if (lcddev.id == 0x5510)
    {
        lcd_ex_nt35510_reginit();   /* ִ��NT35510��ʼ�� */
    }
    else if (lcddev.id == 0X1963)
    {
        lcd_ex_ssd1963_reginit();   /* ִ��SSD1963��ʼ�� */
        lcd_ssd_backlight_set(100); /* ��������Ϊ���� */
    }
    else if (lcddev.id == 0x9488)
    {
        lcd_ex_lil9488_reginit();   /* ִ��9488��ʼ�� */
    }
    
    lcd_display_dir(1); /* Ĭ��Ϊ���� */
    LCD_BL(1);          /* �������� */
    lcd_clear(BLACK);
    
    lcd_wr_regno(0x36);   //Memory Access 
    //lcd_wr_data(0x48);//��������
    //lcd_wr_data(0x88);//��������
    lcd_wr_data(0x28);//��������
    //lcd_wr_data(0xE8);//��������
    
    /* ��������дʱ����ƼĴ�����ʱ�� */
    fsmc_write_handle.AddressSetupTime = 3;
    fsmc_write_handle.DataSetupTime = 3;         
    FSMC_NORSRAM_Extended_Timing_Init(hsram1.Extended, &fsmc_write_handle, hsram1.Init.NSBank, hsram1.Init.ExtendedMode);
}
#if 0
/**
 * @brief       ��������
 * @param       color: Ҫ��������ɫ
 * @retval      ��
 */
void lcd_clear(uint16_t color)
{
    uint32_t index = 0;
    uint32_t totalpoint = lcddev.width;
    totalpoint *= lcddev.height;    /* �õ��ܵ��� */
    lcd_set_cursor(0x00, 0x0000);   /* ���ù��λ�� */
    lcd_write_ram_prepare();        /* ��ʼд��GRAM */

    for (index = 0; index < totalpoint; index++)
    {
        LCD->LCD_RAM = color;
    }
}

/**
 * @brief       ��ָ����������䵥����ɫ
 * @param       (sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex - sx + 1) * (ey - sy + 1)
 * @param       color:  Ҫ������ɫ(32λ��ɫ,�������LTDC)
 * @retval      ��
 */
void lcd_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{
    uint16_t i, j;
    uint16_t xlen = 0;
    xlen = ex - sx + 1;

    for (i = sy; i <= ey; i++)
    {
        lcd_set_cursor(sx, i);      /* ���ù��λ�� */
        lcd_write_ram_prepare();    /* ��ʼд��GRAM */

        for (j = 0; j < xlen; j++)
        {
            LCD->LCD_RAM = color;   /* ��ʾ��ɫ */
        }
    }
}

/**
 * @brief       ��ָ�����������ָ����ɫ��
 * @param       (sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex - sx + 1) * (ey - sy + 1)
 * @param       color: Ҫ������ɫ�����׵�ַ
 * @retval      ��
 */
void lcd_color_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{
    uint16_t height, width;
    uint16_t i, j;
    width = ex - sx + 1;            /* �õ����Ŀ�� */
    height = ey - sy + 1;           /* �߶� */

    for (i = 0; i < height; i++)
    {
        lcd_set_cursor(sx, sy + i); /* ���ù��λ�� */
        lcd_write_ram_prepare();    /* ��ʼд��GRAM */

        for (j = 0; j < width; j++)
        {
            LCD->LCD_RAM = color[i * width + j]; /* д������ */
        }
    }
}
#endif
/**
 * @brief       ��������
 * @param       color: Ҫ��������ɫ
 * @retval      ��
 */
void lcd_clear(uint16_t color)
{
    uint32_t index = 0;
    uint32_t totalpoint = lcddev.width;
    totalpoint *= lcddev.height;    /* �õ��ܵ��� */
    lcd_set_cursor(0x00, 0x0000);   /* ���ù��λ�� */
    lcd_write_ram_prepare();        /* ��ʼд��GRAM */

    for (index = 0; index < totalpoint; index++)
    {
        LCD->LCD_RAM = color;
   }
}

/**
 * @brief       ��ָ����������䵥����ɫ
 * @param       (sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex - sx + 1) * (ey - sy + 1)
 * @param       color: Ҫ������ɫ(32λ��ɫ,�������LTDC)
 * @retval      ��
 */
void lcd_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{
    uint16_t i, j;
    uint16_t xlen = 0;
    xlen = ex - sx + 1;

    for (i = sy; i <= ey; i++)
    {
        lcd_set_cursor(sx, i);      /* ���ù��λ�� */
        lcd_write_ram_prepare();    /* ��ʼд��GRAM */

        for (j = 0; j < xlen; j++)
        {
            LCD->LCD_RAM = color;   /* ��ʾ��ɫ */
        }
    }
}

/**
 * @brief       ��ָ�����������ָ����ɫ��
 * @param       (sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex - sx + 1) * (ey - sy + 1)
 * @param       color: Ҫ������ɫ�����׵�ַ
 * @retval      ��
 */
void lcd_color_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{
    uint16_t height, width;
    uint16_t i, j;
    width = ex - sx + 1;            /* �õ����Ŀ�� */
    height = ey - sy + 1;           /* �߶� */

    for (i = 0; i < height; i++)
    {
        lcd_set_cursor(sx, sy + i); /* ���ù��λ�� */
        lcd_write_ram_prepare();    /* ��ʼд��GRAM */

        for (j = 0; j < width; j++)
        {
            LCD->LCD_RAM = color[i * width + j]; /* д������ */
        }
    }
}

/**
 * @brief       ����
 * @param       x1,y1: �������
 * @param       x2,y2: �յ�����
 * @param       color: �ߵ���ɫ
 * @retval      ��
 */
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    delta_x = x2 - x1;              /* ������������ */
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)incx = 1;       /* ���õ������� */
    else if (delta_x == 0)incx = 0; /* ��ֱ�� */
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0; /* ˮƽ�� */
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if ( delta_x > delta_y)distance = delta_x;  /* ѡȡ�������������� */
    else distance = delta_y;

    for (t = 0; t <= distance + 1; t++ )        /* ������� */
    {
        lcd_draw_point(row, col, color);        /* ���� */
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
 * @brief       ��ˮƽ��
 * @param       x0,y0: �������
 * @param       len  : �߳���
 * @param       color: ���ε���ɫ
 * @retval      ��
 */
void lcd_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
{
    if ((len == 0) || (x > lcddev.width) || (y > lcddev.height))return;

    lcd_fill(x, y, x + len - 1, y, color);
}

/**
 * @brief       ������
 * @param       x1,y1: �������
 * @param       x2,y2: �յ�����
 * @param       color: ���ε���ɫ
 * @retval      ��
 */
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    lcd_draw_line(x1, y1, x2, y1, color);
    lcd_draw_line(x1, y1, x1, y2, color);
    lcd_draw_line(x1, y2, x2, y2, color);
    lcd_draw_line(x2, y1, x2, y2, color);
}

/**
 * @brief       ��Բ
 * @param       x,y  : Բ��������
 * @param       r    : �뾶
 * @param       color: Բ����ɫ
 * @retval      ��
 */
void lcd_draw_circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);       /* �ж��¸���λ�õı�־ */

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

        /* ʹ��Bresenham�㷨��Բ */
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
 * @brief       ���ʵ��Բ
 * @param       x0,y0: Բ��������
 * @param       r    : �뾶
 * @param       color: Բ����ɫ
 * @retval      ��
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
 * @brief       ��ָ��λ����ʾһ���ַ�
 * @param       x,y  : ����
 * @param       chr  : Ҫ��ʾ���ַ�:" "--->"~"
 * @param       size : �����С 12/16/24/32
 * @param       mode : ���ӷ�ʽ(1); �ǵ��ӷ�ʽ(0);
 * @retval      ��
 */
void lcd_show_char(uint16_t x, uint16_t y, char chr, uint8_t size, uint8_t mode, uint16_t color)
{
    uint8_t temp, t1, t;
    uint16_t y0 = y;
    uint8_t csize = 0;
    uint8_t *pfont = 0;

    csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); /* �õ�����һ���ַ���Ӧ������ռ���ֽ��� */
    chr = chr - ' ';    /* �õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩ */

    switch (size)
    {
        case 12:
            pfont = (uint8_t *)asc2_1206[chr];  /* ����1206���� */
            break;

        case 16:
            pfont = (uint8_t *)asc2_1608[chr];  /* ����1608���� */
            break;

        case 24:
            pfont = (uint8_t *)asc2_2412[chr];  /* ����2412���� */
            break;

        case 32:
            pfont = (uint8_t *)asc2_3216[chr];  /* ����3216���� */
            break;

        default:
            return ;
    }

    for (t = 0; t < csize; t++)
    {
        temp = pfont[t];                            /* ��ȡ�ַ��ĵ������� */

        for (t1 = 0; t1 < 8; t1++)                  /* һ���ֽ�8���� */
        {
            if (temp & 0x80)                        /* ��Ч��,��Ҫ��ʾ */
            {
                lcd_draw_point(x, y, color);        /* �������,Ҫ��ʾ����� */
            }
            else if (mode == 0)                     /* ��Ч��,����ʾ */
            {
                lcd_draw_point(x, y, g_back_color); /* ������ɫ,�൱������㲻��ʾ(ע�ⱳ��ɫ��ȫ�ֱ�������) */
            }

            temp <<= 1;                             /* ��λ, �Ա��ȡ��һ��λ��״̬ */
            y++;

            if (y >= lcddev.height)return;          /* �������� */

            if ((y - y0) == size)                   /* ��ʾ��һ����? */
            {
                y = y0; /* y���긴λ */
                x++;    /* x������� */

                if (x >= lcddev.width)return;       /* x���곬������ */

                break;
            }
        }
    }
}

/**
 * @brief       ƽ������, m^n
 * @param       m: ����
 * @param       n: ָ��
 * @retval      m��n�η�
 */
static uint32_t lcd_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while (n--)result *= m;

    return result;
}

/**
 * @brief       ��ʾlen������
 * @param       x,y : ��ʼ����
 * @param       num : ��ֵ(0 ~ 2^32)
 * @param       len : ��ʾ���ֵ�λ��
 * @param       size: ѡ������ 12/16/24/32
 * @retval      ��
 */
void lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)   /* ������ʾλ��ѭ�� */
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;   /* ��ȡ��Ӧλ������ */

        if (enshow == 0 && t < (len - 1))               /* û��ʹ����ʾ,�һ���λҪ��ʾ */
        {
            if (temp == 0)
            {
                lcd_show_char(x + (size / 2)*t, y, ' ', size, 0, color);/* ��ʾ�ո�,ռλ */
                continue;   /* �����¸�һλ */
            }
            else
            {
                enshow = 1; /* ʹ����ʾ */
            }

        }

        lcd_show_char(x + (size / 2)*t, y, temp + '0', size, 0, color); /* ��ʾ�ַ� */
    }
}

/**
 * @brief       ��չ��ʾlen������(��λ��0Ҳ��ʾ)
 * @param       x,y : ��ʼ����
 * @param       num : ��ֵ(0 ~ 2^32)
 * @param       len : ��ʾ���ֵ�λ��
 * @param       size: ѡ������ 12/16/24/32
 * @param       mode: ��ʾģʽ
 *              [7]:0,�����;1,���0.
 *              [6:1]:����
 *              [0]:0,�ǵ�����ʾ;1,������ʾ.
 *
 * @retval      ��
 */
void lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode, uint16_t color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)   /* ������ʾλ��ѭ�� */
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;    /* ��ȡ��Ӧλ������ */

        if (enshow == 0 && t < (len - 1))   /* û��ʹ����ʾ,�һ���λҪ��ʾ */
        {
            if (temp == 0)
            {
                if (mode & 0X80)            /* ��λ��Ҫ���0 */
                {
                    lcd_show_char(x + (size / 2)*t, y, '0', size, mode & 0X01, color);  /* ��0ռλ */
                }
                else
                {
                    lcd_show_char(x + (size / 2)*t, y, ' ', size, mode & 0X01, color);  /* �ÿո�ռλ */
                }

                continue;
            }
            else
            {
                enshow = 1; /* ʹ����ʾ */
            }

        }

        lcd_show_char(x + (size / 2)*t, y, temp + '0', size, mode & 0X01, color);
    }
}

/**
 * @brief       ��ʾ�ַ���
 * @param       x,y         : ��ʼ����
 * @param       width,height: �����С
 * @param       size        : ѡ������ 12/16/24/32
 * @param       p           : �ַ����׵�ַ
 * @retval      ��
 */
void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color)
{
    uint8_t x0 = x;
    width += x;
    height += y;

    while ((*p <= '~') && (*p >= ' '))   /* �ж��ǲ��ǷǷ��ַ�! */
    {
        if (x >= width)
        {
            x = x0;
            y += size;
        }

        if (y >= height)break;          /* �˳� */

        lcd_show_char(x, y, *p, size, 0, color);
        x += size / 2;
        p++;
    }
}















