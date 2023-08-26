#ifndef __HMC5883L_H
#define __HMC5883L_H

void HMC5883L_WriteReg(uint8_t RegAddress, uint8_t Data);

uint8_t HMC5883L_ReadReg(uint8_t RegAddress);

void HMC5883L_Init();

void HMC5883L_GetData(int16_t *GSX, int16_t *GSY, int16_t *GSZ);

#endif