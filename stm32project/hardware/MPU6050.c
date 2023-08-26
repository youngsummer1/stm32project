#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS		0xD0

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);//MPU6050地址
	
	MyI2C_ReceiveAck();//记得在此处判断应答信号
	
	
	MyI2C_SendByte(RegAddress);//寄存器地址
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}//指定地址写一个时序

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	//重复起始 
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01); //改成读0xD1
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);//给0,给从机应答,会继续发送数据
	MyI2C_Stop();
	
	return Data;
}//指定地址读一个时序

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_Init(void)
{
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);//电源管理寄存器1 解除睡眠 设备不复位 不循环 选用陀螺仪时钟 温度传感器失能
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);//电源管理寄存器2  循环模式唤醒频率不需要 后6位每一个寄存器都不需要待机
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);//采样率分频 决定数据输出的快慢 10分频
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);//配置寄存器 外部同步都赋0 不需要      数字低通滤波器给110(最平滑的滤波)
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);//陀螺仪配置寄存器  前三位自测失能给0  满量程选择,此处选择最大量程11
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x01);//加速度计配置寄存器  前三位自测失能给0  满量程选择,此处选择最大量程11 不用高通滤波器给00
	
	MPU6050_WriteReg(0x6A, 0x00);
	MPU6050_WriteReg(0x37, 0x02);
}//此时陀螺仪和加速度计的数据已经开始不断进行数据转换了,并且存放在数据寄存器里


void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H); //加速度寄存器的高8位
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L); //加速度寄存器的低8位
	*AccX = (DataH << 8) | DataL; //进行拼接(虽然是8位但是会自动进行类型转换,移出去的位不会消失)
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}//用指针的地址传递实现多返回值  此处也可以通过在基地址累加,通过for循环来一次性读取所有值
