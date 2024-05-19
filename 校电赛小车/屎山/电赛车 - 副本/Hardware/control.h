#ifndef __CONTROL_H
#define __CONTROL_H

int Displacement(float Med, int SR04_Num);
int Velocity(float Med, float Encode_A, float Encode_B, float Acc);
int Final_PWM_Out(float Displacement_PWM_Out, float Encode_A, float Encode_B, float Acc);

#endif
