#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "MOTOR.h"
#include "DELAY.h"

uint8_t Achievement = 0;

int main()
{
	PWM_Init();
	Motot_Init();
	
	while(1)
	{
		Motor_PWMLoad(1000, 1000);
		Delay_s(2);
		Motor_PWMLoad(-1000, -1000);
	}
	return 0;
}
