/*==========================================================
Description  : Bldc Hall Square Contorl
Project      : WuYunTaXue_ZhenLinV1.00
Mcu          : STM32G030C8T6
Date         : 2022.10.23
File         : Protect.h
Function     : Fault Monitor
Author       ���ŷ�ʵս����
Website      ��www.wuyuntaxue.com����վ���������ϣ�
PCBA����     ���Ա���������"�ŷɵ�����ҵ��"
==========================================================*/
#include "..\Include\Define.h"
#include "..\Include\Device.h"
/*========================================================*/
void ProtectInit(void);
void ProtectApp(void);
void ProtectInt(void);

static void VoltageDetect(void);
static void CurrentDetect(void);
static void HallDetect(void);
/*========================================================*/
tProtect_Type tProtect;
UNION_ERRORFLAGS Error_Flag;

extern tBldc_Type tBldc;
/*=============================Copyright belongs to ZhangFeiShiZhanDianZi=============================*/
