/*==========================================================
Description  : Bldc Hall Square Contorl
Project      : WuYunTaXue_ZhenLinV1.00
Mcu          : STM32G030C8T6
Date         : 2022.10.23
File         : Main.h
Function     : Main Loop
Author       ���ŷ�ʵս����
Website      ��www.wuyuntaxue.com����վ���������ϣ�
PCBA����     ���Ա���������"�ŷɵ�����ҵ��"
==========================================================*/
#include "..\Include\Define.h"
#include "..\Include\Device.h"
/*========================================================*/
void Main(void);
void DMA1_Channel1_IRQHandler(void);

extern void Initialize(void);
extern void BldcInt(void);
extern void BldcApp(void);
extern void LedInt(void);
extern void LedApp(unsigned char N);
extern void ProtectApp(void);
extern void ProtectInt(void);
/*========================================================*/
extern tProtect_Type tProtect;
extern UNION_ERRORFLAGS Error_Flag;
/*=============================Copyright belongs to ZhangFeiShiZhanDianZi=============================*/
