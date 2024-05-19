#ifndef __HCSR04_H
#define __HCSR04_H

void HC_SR04_Init(void);//初始化
int16_t sonar_mm(void);//返回距离，单位：毫米
float sonar(void);//返回距离，单位：米

#endif
