#ifndef __PWM_H
#define __PWM_H

//PWM�Ķ�ʱ��
#define PWM_TIM TIM1
#define RCC_PWM_TIM RCC_APB2Periph_TIM1//���޸�������ʱ���ǵ�ȥԴ�ļ��޸�ʹ�ܺ���

//PWM�����GPIO��
#define PWM_TIM_GPIO GPIOA//������·PWM�����GPIO�ڶ���GPIOA������ֻ������һ����
#define RCC_PWM_TIM_GPIO RCC_APB2Periph_GPIOA

//PWM������Ƶ��1��GPIO��Pinѡ��PWM���ͨ��ѡ��
#define PWM_A_TIM_GPIO_Pin GPIO_Pin_8
#define PWM_A_TIM_OC_Init(x,y) TIM_OC1Init(x,y)//ͨ����ʼ��
#define OCA_PreloadConfig(x,y) TIM_OC1PreloadConfig(x,y)//ͨ��Ԥװ��ʹ��
#define OCA_SetCompare(x,y) TIM_SetCompare1(x,y)//PWMռ�ձ�����
#define OCA_PolarityConfig(x,y) TIM_OC1PolarityConfig(x,y)//��ת���ʱӦ������

//PWM������Ƶ��2��GPIO��Pinѡ��PWM���ͨ��ѡ��
#define PWM_B_TIM_GPIO_Pin GPIO_Pin_9
#define PWM_B_TIM_OC_Init(x,y) TIM_OC2Init(x,y)
#define OCB_PreloadConfig(x,y) TIM_OC2PreloadConfig(x,y)
#define OCB_SetCompare(x,y) TIM_SetCompare2(x,y)
#define OCB_PolarityConfig(x,y) TIM_OC2PolarityConfig(x,y)//��ת���ʱӦ������

//PWM������Ƶ��3��GPIO��Pinѡ��PWM���ͨ��ѡ��
#define PWM_C_TIM_GPIO_Pin GPIO_Pin_10
#define PWM_C_TIM_OC_Init(x,y) TIM_OC3Init(x,y)
#define OCC_PreloadConfig(x,y) TIM_OC3PreloadConfig(x,y)
#define OCC_SetCompare(x,y) TIM_SetCompare3(x,y)
#define OCC_PolarityConfig(x,y) TIM_OC3PolarityConfig(x,y)//��ת���ʱӦ������

//PWM������Ƶ��4��GPIO��Pinѡ��PWM���ͨ��ѡ��
#define PWM_D_TIM_GPIO_Pin GPIO_Pin_11
#define PWM_D_TIM_OC_Init(x,y) TIM_OC4Init(x,y)
#define OCD_PreloadConfig(x,y) TIM_OC4PreloadConfig(x,y)
#define OCD_SetCompare(x,y) TIM_SetCompare4(x,y)
#define OCD_PolarityConfig(x,y) TIM_OC4PolarityConfig(x,y)//��ת���ʱӦ������

void PWM_Init(void);

#endif
