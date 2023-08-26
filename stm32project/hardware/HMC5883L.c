#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "HMC5883L_Reg.h"


#define HMC5883L_ADDRESS 0x3C; //写入地址


void HMC5883L_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	
	MyI2C_Start();
	MyI2C_SendByte(0x3C);
	
	MyI2C_ReceiveAck();//记得在此处判断应答信号
	
	
	MyI2C_SendByte(RegAddress);//寄存器地址
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}//指定地址写一个时序


uint8_t HMC5883L_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(0x3C);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	//重复起始 
	MyI2C_Start();
	MyI2C_SendByte(0X3D); 
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);//给0,给从机应答,会继续发送数据
	MyI2C_Stop();
	
	return Data;
}//指定地址读一个时序


void HMC5883L_Init()
{
	MyI2C_Init();
	
	HMC5883L_WriteReg(CONFIGA,0x70);
	HMC5883L_WriteReg(CONFIGB,0x20);
	HMC5883L_WriteReg(MODE,0x00);
}


void HMC5883L_GetData(int16_t *GSX, int16_t *GSY, int16_t *GSZ)
{
	uint8_t DataH, DataL;
	
	DataH = HMC5883L_ReadReg(DATAX_M); 
	DataL = HMC5883L_ReadReg(DATAX_L); 
	*GSX = (DataH << 8) | DataL; 
	
	DataH = HMC5883L_ReadReg(DATAY_M);
	DataL = HMC5883L_ReadReg(DATAY_L);
	*GSY = (DataH << 8) | DataL;
	
	DataH = HMC5883L_ReadReg(DATAZ_M);
	DataL = HMC5883L_ReadReg(DATAZ_L);
	*GSZ = (DataH << 8) | DataL;
	
}
