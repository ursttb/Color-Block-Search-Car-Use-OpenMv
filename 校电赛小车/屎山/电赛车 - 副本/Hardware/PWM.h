#ifndef __PWM_H
#define __PWM_H

//PWM的定时器
#define PWM_TIM TIM1
#define RCC_PWM_TIM RCC_APB2Periph_TIM1//若修改其他定时器记得去源文件修改使能函数

//PWM输出的GPIO口
#define PWM_TIM_GPIO GPIOA//由于两路PWM输出的GPIO口都是GPIOA，所以只定义了一个宏
#define RCC_PWM_TIM_GPIO RCC_APB2Periph_GPIOA

//PWM输出控制电机1的GPIO口Pin选、PWM输出通道选择
#define PWM_A_TIM_GPIO_Pin GPIO_Pin_8
#define PWM_A_TIM_OC_Init(x,y) TIM_OC1Init(x,y)//通道初始化
#define OCA_PreloadConfig(x,y) TIM_OC1PreloadConfig(x,y)//通道预装载使能
#define OCA_SetCompare(x,y) TIM_SetCompare1(x,y)//PWM占空比设置
#define OCA_PolarityConfig(x,y) TIM_OC1PolarityConfig(x,y)//反转电机时应该设置

//PWM输出控制电机2的GPIO口Pin选、PWM输出通道选择
#define PWM_B_TIM_GPIO_Pin GPIO_Pin_9
#define PWM_B_TIM_OC_Init(x,y) TIM_OC2Init(x,y)
#define OCB_PreloadConfig(x,y) TIM_OC2PreloadConfig(x,y)
#define OCB_SetCompare(x,y) TIM_SetCompare2(x,y)
#define OCB_PolarityConfig(x,y) TIM_OC2PolarityConfig(x,y)//反转电机时应该设置

//PWM输出控制电机3的GPIO口Pin选、PWM输出通道选择
#define PWM_C_TIM_GPIO_Pin GPIO_Pin_10
#define PWM_C_TIM_OC_Init(x,y) TIM_OC3Init(x,y)
#define OCC_PreloadConfig(x,y) TIM_OC3PreloadConfig(x,y)
#define OCC_SetCompare(x,y) TIM_SetCompare3(x,y)
#define OCC_PolarityConfig(x,y) TIM_OC3PolarityConfig(x,y)//反转电机时应该设置

//PWM输出控制电机4的GPIO口Pin选、PWM输出通道选择
#define PWM_D_TIM_GPIO_Pin GPIO_Pin_11
#define PWM_D_TIM_OC_Init(x,y) TIM_OC4Init(x,y)
#define OCD_PreloadConfig(x,y) TIM_OC4PreloadConfig(x,y)
#define OCD_SetCompare(x,y) TIM_SetCompare4(x,y)
#define OCD_PolarityConfig(x,y) TIM_OC4PolarityConfig(x,y)//反转电机时应该设置

void PWM_Init(void);

#endif
