#ifndef __motor_H
#define __motor_H
#include "sys.h"
#include	"led.h"
#include "PWMA.h"
//#include "control.h"
//#define AIN1 PBout(14)
//#define AIN2 PBout(15)
//#define BIN1 PBout(13)
//#define BIN2 PBout(12)

#define Ain1 GPIO_Pin_14
#define Ain2 GPIO_Pin_15
#define Bin1 GPIO_Pin_13
#define Bin2 GPIO_Pin_12						
void motor_init(void);
//void motor_mode(u16 number);

void Limit(int *MOTOA,int *MOTOB);
int GFP_abs (int p);

void Load(int moto1,int moto2);

void Stop(float *Med_Jiaodu,float *Jiaodu);

#endif




