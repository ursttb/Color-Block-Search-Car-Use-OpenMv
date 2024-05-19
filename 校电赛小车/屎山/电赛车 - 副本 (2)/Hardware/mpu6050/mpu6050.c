#include "mpu6050.h"

/*
	应用说明：
	在访问MPU6050_IIC设备前，请先调用 MPU6050_IIC_CheckDevice() 检测MPU6050_IIC设备是否正常，该函数会配置GPIO
*/
MPU6050_Data_TypeDef MPU6050_Data;
/*
*********************************************************************************************************
*	函 数 名: MPU6050_IIC_Delay
*	功能说明: MPU6050_IIC总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MPU6050_IIC_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_IIC_Start
*	功能说明: CPU发起MPU6050_IIC总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MPU6050_IIC_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示MPU6050_IIC总线启动信号 */
	MPU6050_IIC_SDA_1();
	MPU6050_IIC_SCL_1();
	MPU6050_IIC_Delay();
	MPU6050_IIC_SDA_0();
	MPU6050_IIC_Delay();
	MPU6050_IIC_SCL_0();
	MPU6050_IIC_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_IIC_Start
*	功能说明: CPU发起MPU6050_IIC总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MPU6050_IIC_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示MPU6050_IIC总线停止信号 */
	MPU6050_IIC_SDA_0();
	MPU6050_IIC_SCL_1();
	MPU6050_IIC_Delay();
	MPU6050_IIC_SDA_1();
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_IIC_SendByte
*	功能说明: CPU向MPU6050_IIC总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
static void MPU6050_IIC_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			MPU6050_IIC_SDA_1();
		}
		else
		{
			MPU6050_IIC_SDA_0();
		}
		MPU6050_IIC_Delay();
		MPU6050_IIC_SCL_1();
		MPU6050_IIC_Delay();	
		MPU6050_IIC_SCL_0();
		if (i == 7)
		{
			 MPU6050_IIC_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		MPU6050_IIC_Delay();
	}
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_IIC_ReadByte
*	功能说明: CPU从MPU6050_IIC总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
static uint8_t MPU6050_IIC_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		MPU6050_IIC_SCL_1();
		MPU6050_IIC_Delay();
		if (MPU6050_IIC_SDA_READ())
		{
			value++;
		}
		MPU6050_IIC_SCL_0();
		MPU6050_IIC_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_IIC_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
static uint8_t MPU6050_IIC_WaitAck(void)
{
	uint8_t re;

	MPU6050_IIC_SDA_1();	/* CPU释放SDA总线 */
	MPU6050_IIC_Delay();
	MPU6050_IIC_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	MPU6050_IIC_Delay();
	if (MPU6050_IIC_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	MPU6050_IIC_SCL_0();
	MPU6050_IIC_Delay();
	return re;
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_IIC_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MPU6050_IIC_Ack(void)
{
	MPU6050_IIC_SDA_0();	/* CPU驱动SDA = 0 */
	MPU6050_IIC_Delay();
	MPU6050_IIC_SCL_1();	/* CPU产生1个时钟 */
	MPU6050_IIC_Delay();
	MPU6050_IIC_SCL_0();
	MPU6050_IIC_Delay();
	MPU6050_IIC_SDA_1();	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_IIC_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MPU6050_IIC_NAck(void)
{
	MPU6050_IIC_SDA_1();	/* CPU驱动SDA = 1 */
	MPU6050_IIC_Delay();
	MPU6050_IIC_SCL_1();	/* CPU产生1个时钟 */
	MPU6050_IIC_Delay();
	MPU6050_IIC_SCL_0();
	MPU6050_IIC_Delay();	
}
/*
*********************************************************************************************************
*	函 数 名: MPU6050_IIC_GPIO_Config
*	功能说明: 配置MPU6050_IIC总线的GPIO，采用模拟IO的方式实现
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void MPU6050_IIC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(MPU6050_IIC_RCC_PORT, ENABLE);	/* 打开GPIO时钟 */

	GPIO_InitStructure.GPIO_Pin = MPU6050_IIC_SCL_PIN | MPU6050_IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* 开漏输出 */
	GPIO_Init(MPU6050_IIC_GPIO_PORT, &GPIO_InitStructure);

	/* 给一个停止信号, 复位MPU6050_IIC总线上的所有设备到待机模式 */
	MPU6050_IIC_Stop();
}


/*
*********************************************************************************************************
*	函 数 名: MPU6050_IIC_Check
*	功能说明: 检测MPU6050_IIC总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的MPU6050_IIC总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t MPU6050_IIC_Check(uint8_t _Address)
{
	uint8_t ucAck;

	MPU6050_IIC_GPIO_Config();		/* 配置GPIO */

	do
	{
		MPU6050_IIC_Start();		/* 发送启动信号 */

		/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
		MPU6050_IIC_SendByte(_Address<<1);
		ucAck = MPU6050_IIC_WaitAck();	/* 检测设备的ACK应答 */

		MPU6050_IIC_Stop();
		if(ucAck)
			printf("MPU6050 Initialization failed！\r\n");
		else
			printf("MPU6050 Initialization Succeed！\r\n");
	}
	while(ucAck);
	return ucAck;
}


/*
*********************************************************************************************************
*	函 数 名: MPU6050_Write_Len
*	功能说明: IIC连续写
*	形    参：addr:器件地址 reg:寄存器地址 len:写入长度 buf:数据区
*	返 回 值: 返回值:0,成功 其他,错误代码
*********************************************************************************************************
*/
u8 MPU6050_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
  MPU6050_IIC_Start(); 
	MPU6050_IIC_SendByte((MPU6050_ADDR<<1)|0);//发送器件地址+写命令	
	if(MPU6050_IIC_WaitAck())	//等待应答
	{
		MPU6050_IIC_Stop();		 
		return 1;		
	}
    MPU6050_IIC_SendByte(reg);	//写寄存器地址
    MPU6050_IIC_WaitAck();		//等待应答
	for(i=0;i<len;i++)
	{
		MPU6050_IIC_SendByte(buf[i]);	//发送数据
		if(MPU6050_IIC_WaitAck())		//等待ACK
		{
			MPU6050_IIC_Stop();	 
			return 1;		 
		}		
	}    
    MPU6050_IIC_Stop();	 
	return 0;	
} 

/*
*********************************************************************************************************
*	函 数 名: MPU6050_Read_Len
*	功能说明: IIC连续读
*	形    参：addr:器件地址 reg:要读取的寄存器地址 len:要读取的长度 buf:读取到的数据存储区
*	返 回 值: 返回值:0,成功 其他,错误代码
*********************************************************************************************************
*/
u8 MPU6050_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	MPU6050_IIC_Start(); 
	MPU6050_IIC_SendByte((MPU6050_ADDR<<1)|0);//发送器件地址+写命令	
	if(MPU6050_IIC_WaitAck())	//等待应答
	{
		MPU6050_IIC_Stop();		 
		return 1;		
	}
    MPU6050_IIC_SendByte(reg);	//写寄存器地址
    MPU6050_IIC_WaitAck();		//等待应答
    MPU6050_IIC_Start();
		MPU6050_IIC_SendByte((MPU6050_ADDR<<1)|1);//发送器件地址+读命令	
    MPU6050_IIC_WaitAck();		//等待应答 
	while(len)
	{
		if(len==1)
		{
			*buf=MPU6050_IIC_ReadByte();//读数据
			MPU6050_IIC_NAck();//发送nACK 
		}
		else
		{
			*buf=MPU6050_IIC_ReadByte();		//读数据
			MPU6050_IIC_Ack();//发送ACK  
		}
		len--;
		buf++; 
	}    
    MPU6050_IIC_Stop();	//产生一个停止条件 
	return 0;	
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_Write_Byte
*	功能说明: IIC写一个字节
*	形    参： reg:寄存器地址 data:数据
*	返 回 值: 返回值:0,成功 其他,错误代码
*********************************************************************************************************
*/
static u8 MPU6050_Write_Byte(u8 reg,u8 data) 				 
{ 
	MPU6050_IIC_Start(); 
	MPU6050_IIC_SendByte((MPU6050_ADDR<<1)|0);//发送器件地址+写命令	
	if(MPU6050_IIC_WaitAck())	//等待应答
	{
		MPU6050_IIC_Stop();		 
		return 1;		
	}
		MPU6050_IIC_SendByte(reg);	//写寄存器地址
		MPU6050_IIC_WaitAck();		//等待应答 
	MPU6050_IIC_SendByte(data);//发送数据
	if(MPU6050_IIC_WaitAck())	//等待ACK
	{
		MPU6050_IIC_Stop();	 
		return 1;		 
	}		 
		MPU6050_IIC_Stop();	 
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_Read_Byte
*	功能说明: IIC读一个字节
*	形    参： reg:寄存器地址 
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
//static u8 MPU6050_Read_Byte(u8 reg)
//{
//	u8 res;
//	MPU6050_IIC_Start(); 
//	MPU6050_IIC_SendByte((MPU6050_ADDR<<1)|0);//发送器件地址+写命令	
//	MPU6050_IIC_WaitAck();		//等待应答 
//	MPU6050_IIC_SendByte(reg);	//写寄存器地址
//	MPU6050_IIC_WaitAck();		//等待应答
//	MPU6050_IIC_Start();
//	MPU6050_IIC_SendByte((MPU6050_ADDR<<1)|1);//发送器件地址+读命令	
//	MPU6050_IIC_WaitAck();		//等待应答 
//	res=MPU6050_IIC_ReadByte();		//读数据
//	MPU6050_IIC_NAck();//发送ACK  
//	MPU6050_IIC_Stop();			//产生一个停止条件 
//	return res;		
//}


/*
*********************************************************************************************************
*	函 数 名: MPU6050_Set_Gyro_Fsr
*	功能说明: 设置MPU6050陀螺仪传感器满量程范围
*	形    参：fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
*	返 回 值: 0,设置成功  其他,设置失败 
*********************************************************************************************************
*/
static u8 MPU6050_Set_Gyro_Fsr(u8 fsr)
{
	return MPU6050_Write_Byte(MPU6050_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_Set_Accel_Fsr
*	功能说明: 设置MPU6050加速度传感器满量程范围
*	形    参：fsr:0,±2g;1,±4g;2,±8g;3,±16g
*	返 回 值: 0,设置成功  其他,设置失败 
*********************************************************************************************************
*/
static u8 MPU6050_Set_Accel_Fsr(u8 fsr)
{
	return MPU6050_Write_Byte(MPU6050_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}
/*
*********************************************************************************************************
*	函 数 名: MPU6050_Set_LPF
*	功能说明: 设置MPU6050的数字低通滤波器
*	形    参：lpf:数字低通滤波频率(Hz)
*	返 回 值: 0,设置成功  其他,设置失败 
*********************************************************************************************************
*/
static u8 MPU6050_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU6050_Write_Byte(MPU6050_CFG_REG,data);//设置数字低通滤波器  
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_Set_Rate
*	功能说明: 设置MPU6050的采样率(假定Fs=1KHz)
*	形    参：rate:4~1000(Hz)
*	返 回 值: 0,设置成功  其他,设置失败 
*********************************************************************************************************
*/
static u8 MPU6050_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU6050_Write_Byte(MPU6050_SAMPLE_RATE_REG,data);//设置数字低通滤波器
 	return MPU6050_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_Get_Temperature
*	功能说明: 得到温度值
*	形    参：无
*	返 回 值: 温度值(扩大了100倍)
*********************************************************************************************************
*/
void MPU6050_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
		MPU6050_Read_Len(MPU6050_ADDR,MPU6050_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    MPU6050_Data.temp=(36.53+((double)raw)/340)*100; 
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_Get_Gyroscope
*	功能说明: 得到陀螺仪值(原始值)
*	形    参：无
*	返 回 值: 返回值:0,成功 其他,错误代码
*********************************************************************************************************
*/
uint8_t MPU6050_Get_Gyroscope(void)
{
  u8 buf[6],res;  
	res=MPU6050_Read_Len(MPU6050_ADDR,MPU6050_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		MPU6050_Data.gyrox=((u16)buf[0]<<8)|buf[1];  
		MPU6050_Data.gyroy=((u16)buf[2]<<8)|buf[3];  
		MPU6050_Data.gyroz=((u16)buf[4]<<8)|buf[5];
		return res;
	}
	else  
	{
		return res;
  }
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_Get_Accelerometer
*	功能说明: 得到加速度值(原始值)
*	形    参：无
*	返 回 值: 返回值:0,成功 其他,错误代码
*********************************************************************************************************
*/
uint8_t MPU6050_Get_Accelerometer(void)
{
    u8 buf[6],res;  
	res=MPU6050_Read_Len(MPU6050_ADDR,MPU6050_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		MPU6050_Data.aacx=((u16)buf[0]<<8)|buf[1];  
		MPU6050_Data.aacy=((u16)buf[2]<<8)|buf[3];  
		MPU6050_Data.aacz=((u16)buf[4]<<8)|buf[5];
		return res;
	}
	else  
	{
		return res;
  }
}
/*
*********************************************************************************************************
*	函 数 名: MPU6050_Config
*	功能说明: 初始化MPU6050模块
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void MPU6050_Config(void)
{
	MPU6050_IIC_Check(MPU6050_ADDR);
	MPU6050_Write_Byte(MPU6050_PWR_MGMT1_REG,0X80);	//复位MPU6050
  delay_ms(100);
	MPU6050_Write_Byte(MPU6050_PWR_MGMT1_REG,0X00);	//唤醒MPU6050 
	
	MPU6050_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
	MPU6050_Set_Accel_Fsr(0);					//加速度传感器,±2g
	MPU6050_Set_Rate(50);						//设置采样率50Hz
	MPU6050_Write_Byte(MPU6050_INT_EN_REG,0X00);	//关闭所有中断
	MPU6050_Write_Byte(MPU6050_USER_CTRL_REG,0X00);	//I2C主模式关闭
	MPU6050_Write_Byte(MPU6050_FIFO_EN_REG,0X00);	//关闭FIFO
	MPU6050_Write_Byte(MPU6050_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	MPU6050_Write_Byte(MPU6050_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
	MPU6050_Write_Byte(MPU6050_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
	MPU6050_Set_Rate(50);						//设置采样率为50Hz
}


/*
*********************************************************************************************************
*	函 数 名: MPU6050_Get_Data
*	功能说明: 获取MPU6050原始数据和欧拉角数据，并打印
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void MPU6050_Get_Data(void)
{
	static uint8_t flage=0;
	
	if(flage==0)
	{
		while(mpu_dmp_init())//mpu初始化
		{	
			printf("MPU6050 Error");
		}
		flage=!flage;
	}
	
	if(mpu_dmp_get_data(&MPU6050_Data.pitch,&MPU6050_Data.roll,&MPU6050_Data.yaw)==0)//获取欧拉角
		{ 
			MPU6050_Get_Temperature();	//得到温度值
			MPU6050_Get_Gyroscope();	//得到加速度传感器数据
			MPU6050_Get_Accelerometer();	//得到陀螺仪数据
			printf("\r\n加速度： %8d%8d%8d    ",MPU6050_Data.aacx,MPU6050_Data.aacy,MPU6050_Data.aacz);
			printf("陀螺仪： %8d%8d%8d    ",MPU6050_Data.gyrox,MPU6050_Data.gyroy,MPU6050_Data.gyroz);
			printf("温度： %8.2f    ",MPU6050_Data.temp);
			printf("pitch： %8.2f    ",MPU6050_Data.pitch);
			printf("roll :  %8.2f    ",MPU6050_Data.roll);
			printf("yaw  :  %8.2f    ",MPU6050_Data.yaw);
		}
}
