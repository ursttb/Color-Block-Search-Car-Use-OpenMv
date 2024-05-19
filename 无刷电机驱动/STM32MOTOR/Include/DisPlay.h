/*==========================================================
Description  : Bldc Hall Square Contorl
Project      : WuYunTaXue_ZhenLinV1.00
Mcu          : STM32G030C8T6
Date         : 2022.10.23
File         : DisPlay.h
Function     : Fault Led DisPlay
Author       ���ŷ�ʵս����
Website      ��www.wuyuntaxue.com����վ���������ϣ�
PCBA����     ���Ա���������"�ŷɵ�����ҵ��"
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
