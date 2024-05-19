/*==========================================================
Description  : Bldc Hall Square Contorl
Project      : WuYunTaXue_ZhenLinV1.00
Mcu          : STM32G030C8T6
Date         : 2022.10.23
File         : Bldc.h
Function     : Bldc Motor Control
Author       ：张飞实战电子
Website      ：www.wuyuntaxue.com（网站可下载资料）
PCBA购买     ：淘宝搜索店铺"张飞电子企业店"
==========================================================*/
#include "..\Include\Define.h"
#include "..\Include\Device.h"
/*========================================================*/
#define    MIN_DUTY    256
#define    MAX_DUTY    3200 
/*========================================================*/
void BldcInt(void);
void BldcApp(void);

static void BldcCount(void);
static void GetHallValue(void);
static void StatusMachine(void);
static void AdcFilter(void);
static void PotValueProcess(void);
static void MotorRunStopControl(void);
/*========================================================*/
tBldc_Type tBldc;

extern tProtect_Type tProtect;
extern UNION_ERRORFLAGS Error_Flag;

const unsigned int PwmPhaseOutReg0[7] = {PWM_ALL_OFF_CCER,PWM_PHASE_ONE_CCER,PWM_PHASE_TWO_CCER,PWM_PHASE_THREE_CCER,
                                         PWM_PHASE_FOUR_CCER,PWM_PHASE_FIVE_CCER,PWM_PHASE_SIX_CCER};
const unsigned int PwmPhaseOutReg1[7] = {PWM_ALL_OFF_CCMR1,PWM_PHASE_ONE_CCMR1,PWM_PHASE_TWO_CCMR1,PWM_PHASE_THREE_CCMR1,
                                         PWM_PHASE_FOUR_CCMR1,PWM_PHASE_FIVE_CCMR1,PWM_PHASE_SIX_CCMR1};
const unsigned int PwmPhaseOutReg2[7] = {PWM_ALL_OFF_CCMR2,PWM_PHASE_ONE_CCMR2,PWM_PHASE_TWO_CCMR2,PWM_PHASE_THREE_CCMR2,
                                         PWM_PHASE_FOUR_CCMR2,PWM_PHASE_FIVE_CCMR2,PWM_PHASE_SIX_CCMR2};    
/*=============================Copyright belongs to ZhangFeiShiZhanDianZi=============================*/
