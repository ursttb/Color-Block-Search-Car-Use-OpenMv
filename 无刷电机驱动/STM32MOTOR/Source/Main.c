/*==========================================================
Description  : Bldc Hall Square Contorl
Project      : WuYunTaXue_ZhenLinV1.00
Mcu          : STM32G030C8T6
Date         : 2022.10.23
File         : Main.c
Function     : Main Loop
Author       ：张飞实战电子
Website      ：www.wuyuntaxue.com（网站可下载资料）
PCBA购买     ：淘宝搜索店铺"张飞电子企业店"
==========================================================*/
#include "..\Include\Main.h"
/*========================================================*/
void Main(void)
{
    Initialize();
    
    do
    {
        BldcApp();
        LedApp(tProtect.Code);
        ProtectApp(); 
    }while(1);
}
/*========================================================*/
void DMA1_Channel1_IRQHandler(void)
{
    unsigned int Temp;
    
    Temp = DMA1 -> ISR;
    Temp &= 0x00000002;
    if(Temp != 0)
    {
        DMA1 -> IFCR |= 0x00000002; //Clear Transfer flag
        BldcInt();
        LedInt();
        ProtectInt();
    }
}
/*========================================================*/
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    unsigned int Temp;
    
    Temp = TIM1 -> SR;
    Temp &= 0x00000080;
    
    if(Temp != 0)
    {
        TIM1 -> SR &= 0xffffff7f;
        if((GPIOB -> IDR & 0xe000) == 0x0)
        {
            Error_Flag.Bits.Hoc = 1;
            TIM1 -> DIER &= 0xff7f; //Disable Break Interrupt
        }
    }
}
/*=============================Copyright belongs to ZhangFeiShiZhanDianZi=============================*/
