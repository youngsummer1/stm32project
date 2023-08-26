#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitValue);
	Delay_us(10);//防止MPU6050时序跟不上
}


void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitValue);
	Delay_us(10);
}


uint8_t MyI2C_R_SDA()
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	Delay_us(10);
	
	return BitValue;
}


void MyI2C2_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_5,(BitAction)BitValue);
	Delay_us(10);//防止MPU6050时序跟不上
}


void MyI2C2_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_4,(BitAction)BitValue);
	Delay_us(10);
}


uint8_t MyI2C2_R_SDA()
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);
	Delay_us(10);
	
	return BitValue;
}



void MyI2C_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//开漏输出(既可以输出,也可以输入)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB,&GPIO_InitStructure);


	
	
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);//默认置高电平

}//软件I2C,只需要GPIO的读写,与库无关



//完成6个时序单元1
void MyI2C_Start()
{
   MyI2C_W_SDA(1);
	 MyI2C_W_SCL(1);
	 MyI2C_W_SDA(0);
	 MyI2C_W_SCL(0);
}//起始条件 兼容起始和中途的Sr重复起始

void MyI2C_Stop()
{
	 MyI2C_W_SDA(0);
	 MyI2C_W_SCL(1);
	 MyI2C_W_SDA(1);
}//终止条件

void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for(i = 0;i < 8;i++){
		MyI2C_W_SDA(Byte & (0x80 >> i));//结果为0x80等或是0x00 但前面的函数已经规定不是0就是1
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}

uint8_t MyI2C_ReceiveByte()
{
	uint8_t i,Byte = 0x00;
	 MyI2C_W_SDA(1);//主机释放SDA,因为应答位主机将SDA下拉
	for(i=0;i<8;i++){
		MyI2C_W_SCL(1);
		if(MyI2C_R_SDA()==1)
			Byte|=(0x80>>i);//从机挂载数据
		MyI2C_W_SCL(0);
	}
	return Byte;
}


void MyI2C_SendAck(uint8_t AckBit)
{
		MyI2C_W_SDA(AckBit);
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
}//发送应答位

uint8_t MyI2C_ReceiveAck()
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit=MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}





