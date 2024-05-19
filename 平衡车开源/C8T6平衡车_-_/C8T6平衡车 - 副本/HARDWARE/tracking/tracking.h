#ifndef __TRACKING_H__
#define __TRACKING_H__
#include "sys.h"

#define Right_tracking  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)//右边的红外循迹模块
#define Left_tracking   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)//左边的红外循迹模块对应的out口

void Tracking_Init(void);
void Tracking_Mode(void);



 





#endif 


