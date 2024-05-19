/*==========================================================
Description  : Bldc Hall Square Contorl
Project      : WuYunTaXue_ZhenLinV1.00
Mcu          : STM32G030C8T6
Date         : 2022.10.23
File         : DisPlay.c
Function     : Fault Led DisPlay
Author       ：张飞实战电子
Website      ：www.wuyuntaxue.com（网站可下载资料）
PCBA购买     ：淘宝搜索店铺"张飞电子企业店"
==========================================================*/
#include "..\Include\DisPlay.h"
/*========================================================*/
void LedInit(void)
{
    GPIOA -> ODR &= 0xfff7; //LED OFF
}
/*============================================================================*/
void LedInt(void)
{
    LedCount();
}
/*============================================================================*/
static void LedCount(void)
{
    tLed.Count ++;
    if(tLed.Count >= 20)
    {
        tLed.Count = 0;
        
        if(tLed.CountMs)
            tLed.CountMs --;
    }   
}
/*============================================================================*/
void LedApp(unsigned char N)
{
    unsigned int Temp;
    static unsigned char Cnt;
    
    if(N == 0)
    {
        Cnt = 0;
        tLed.CountMs = LED_OFF_TIME;
        GPIOA -> ODR &= 0xfff7; //LED OFF
    }
    else
    {
        Temp = tLed.CountMs;
        if(Temp == 0)
        {
            Cnt ++;
            if(Cnt <= N*2)
            {
                tLed.CountMs = LED_FLICK_TIME;
                if((GPIOA -> ODR & 0x0008) == 0x0)
                    GPIOA -> ODR |= 0x0008;
                else
                    GPIOA -> ODR &= 0xfff7;
            }
            else
            {
                Cnt = 0;
                tLed.CountMs = LED_OFF_TIME;
                GPIOA -> ODR &= 0xfff7; //LED OFF
            }
        }
    }
}
/*=============================Copyright belongs to ZhangFeiShiZhanDianZi=============================*/
