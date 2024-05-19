/*==========================================================
Description  : Bldc Hall Square Contorl
Project      : WuYunTaXue_ZhenLinV1.00
Mcu          : STM32G030C8T6
Date         : 2022.10.23
File         : DisPlay.h
Function     : Fault Led DisPlay
Author       ：张飞实战电子
Website      ：www.wuyuntaxue.com（网站可下载资料）
PCBA购买     ：淘宝搜索店铺"张飞电子企业店"
==========================================================*/
#include "..\Include\Define.h"
#include "..\Include\Device.h"
/*========================================================*/
#define     LED_FLICK_TIME      250
#define     LED_OFF_TIME        2000
#define     OFF                 1
/*========================================================*/
void LedInit(void);
void LedInt(void);
void LedApp(unsigned char N);

static void LedCount(void);
/*========================================================*/
tLed_Type tLed;
/*=============================Copyright belongs to ZhangFeiShiZhanDianZi=============================*/
