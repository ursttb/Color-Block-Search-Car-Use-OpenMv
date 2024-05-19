/*==========================================================
Description  : Bldc Hall Square Contorl
Project      : WuYunTaXue_ZhenLinV1.00
Mcu          : STM32G030C8T6
Date         : 2022.10.23
File         : Initialize.h
Function     : Mcu Peripheral Init
Author       ：张飞实战电子
Website      ：www.wuyuntaxue.com（网站可下载资料）
PCBA购买     ：淘宝搜索店铺"张飞电子企业店"
==========================================================*/
#include "..\Include\Define.h"
#include "..\Include\Device.h"
/*========================================================*/
void Initialize(void);

static void McuRccInitialize(void);
static void McuGpioInitialize(void);
static void McuTimer1Initialize(void);
static void McuAdcInitialize(void);
static void McuDmaInitialize(void);
static void McuNvicInitialize(void);

extern void LedInit(void);
extern void ProtectInit(void);

/*========================================================*/
extern tBldc_Type tBldc;
/*=============================Copyright belongs to ZhangFeiShiZhanDianZi=============================*/
