#ifndef __TEXT_H__
#define __TEXT_H__

#include "main.h"

void Get_HzMat(char *code,uint8_t *mat,uint8_t size);//�õ����ֵĵ�����
void Show_Font(uint16_t x,uint16_t y,char *font,uint8_t size,uint8_t mode);//��ָ��λ����ʾһ������
void Show_String(uint16_t x,uint16_t y,uint16_t width,uint16_t height,char *str,uint8_t size,uint8_t mode);//��ָ��λ����ʾһ���ַ��� 
void Show_Str_Mid(uint16_t x,uint16_t y,char *str,uint8_t size,uint8_t len);

#endif
