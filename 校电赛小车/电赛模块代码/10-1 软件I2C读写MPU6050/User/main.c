#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"

uint8_t ID;
int16_t GX, GY, GZ;
float AX, AY, AZ;

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	
	OLED_ShowString(1, 1, "ID:");
	ID = MPU6050_GetID();
	OLED_ShowHexNum(1, 4, ID, 2);
	
	while (1)
	{
    	MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
       
        OLED_ShowSignedNum(2, 1, AX, 5);         //(AX*16)/32748
		OLED_ShowSignedNum(3, 1, AY, 5);         //(AY*16)/32748
		OLED_ShowSignedNum(4, 1, AZ, 5);         //(AZ*16)/32748
		OLED_ShowSignedNum(2, 8, GX, 5);
		OLED_ShowSignedNum(3, 8, GY, 5);
		OLED_ShowSignedNum(4, 8, GZ, 5);
        
	}
    
 }
