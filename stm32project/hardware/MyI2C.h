#ifndef __MYI2C_H
#define __MYI2C_H
void MyI2C_Init();

void MyI2C_Start();

void MyI2C_Stop();

void MyI2C_SendByte(uint8_t Byte);

uint8_t MyI2C_ReceiveByte();

void MyI2C_SendAck(uint8_t AckBit);

uint8_t MyI2C_ReceiveAck();

#endif