#include "beep.h"
#include "tim.h"

void MY_TIM14_Init(void)
{
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 84-1;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 280;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_MspPostInit(&htim14);
}


#include "font_img.h"

// 蜂鸣器停止发声
void buzzerQuiet(void)
{
    __HAL_TIM_SetCompare(&htim14,TIM_CHANNEL_1,0);
}

//蜂鸣器发出指定频率的声音
//usFreq是发声频率，取值 (系统时钟/65536)+1 ～ 20000，单位：Hz
void buzzerSound(uint16_t usFreq)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    unsigned long ulVal;
    if((usFreq<=8000000/65536UL)||(usFreq>20000))
    {
        buzzerQuiet();
    }
    else
    {
        ulVal=8000000/usFreq;
        __HAL_TIM_SetAutoreload(&htim14,ulVal);//音调
        __HAL_TIM_SetCompare(&htim14,TIM_CHANNEL_1,ulVal /2); //音量
    }
}

#include "lcd.h"
#include "key.h"
#include "usbd_cdc_if.h"

// 演奏乐曲
void musicPlay(void)
{
    MX_TIM14_Init();
    HAL_TIM_PWM_Start(&htim14,TIM_CHANNEL_1);

    uint8_t flag = 0;
    uint16_t i = 0,p = 0,p_last = 200;
    uint8_t buf[32];
    uint32_t t_last = HAL_GetTick();
    g_back_color = 0;
    
    while(1)
    {
        for(p = 0;p < 28;p ++)
        {
            if(i < liric_pos[p])
            {
                break;
            }
        }
        if(p_last != p)
        {
            flag = 1;
            if(p==0)
            {
                show_str_mid(94,160 - 8 + 32,(char *)liric_arr[0],16,RED,240);
            }
            else if(p==1)
            {
                show_str_mid(94,160 - 8,     (char *)liric_arr[0],16,WHITE,240);
                show_str_mid(94,160 - 8 + 32,(char *)liric_arr[1],16,RED,240);
            }
            else if(p==26)
            {
                show_str_mid(94,160 - 8 - 32,(char *)liric_arr[25],16,WHITE,240);
                show_str_mid(94,160 - 8,     (char *)liric_arr[26],16,RED,240);
                show_str_mid(94,160 - 8 + 32,(char *)"                              ",16,RED,240);
            }
            else if(p < 26)
            {
                show_str_mid(94,160 - 8 - 32,(char *)liric_arr[p-2],16,WHITE,240);
                show_str_mid(94,160 - 8,     (char *)liric_arr[p-1],16,RED,240);
                show_str_mid(94,160 - 8 + 32,(char *)liric_arr[p],16,WHITE,240);
            }
            p_last = p;
        }
        if (MyScore[i].mTime == 0)
            break;
        buzzerSound(MyScore[i].mName);
        HAL_Delay(MyScore[i].mTime);
        i++;
        buzzerQuiet();  // 蜂鸣器静音
        if(get_key_code(KEY_ID_1) == CODE_CLICK)
        {
            clear_key(KEY_ID_1);
            break;
//            sprintf((char *)buf,"enc:%3d\r\n",i);
//            CDC_Transmit_FS(buf,19);
        }
    }
    
    MY_TIM14_Init();
}

