#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Serial.h"
#include "GPS.h"

#define X_ACCEL_OFFSET -560
#define Y_ACCEL_OFFSET -110
#define Z_ACCEL_OFFSET -1320
#define X_GYRO_OFFSET 22 `
#define Y_GYRO_OFFSET -9 
#define Z_GYRO_OFFSET 50

double N1,E1,N2,E2;
uint8_t flag1;
double detN,detE,detE1;

typedef struct Angle
{
    double X_Angle;
    double Y_Angle;
    double Z_Angle;
    
} MPU6050_Angle ;

MPU6050_Angle data;

int16_t AX, AY, AZ, GX, GY, GZ, CX ,CY ,CZ; 
char AX1[15], AY1[15], AZ1[15], GX1[15], GY1[15], GZ1[15];
char AX2[15], AY2[15], AZ2[15], GX2[15], GY2[15], GZ2[15];
float Deg;
float Deg2;


int main(void)
{
	OLED_Init();
	MPU6050_Init();
	HMC5883L_Init();
	LED_Init();
	Serial_Init();
	GPS_Init();
	
	LED1_ON();
	LED2_ON();
	LED3_ON();
	LED4_ON();
	
	while (1)
	{
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		HMC5883L_GetData(&CX, &CY, &CZ);
			
		sprintf(AX1,"%f",(AX + X_ACCEL_OFFSET) / 16384.0);
		sprintf(AY1,"%f",(AY + Y_ACCEL_OFFSET) / 16384.0);
		sprintf(AZ1,"%f",(AZ + Z_ACCEL_OFFSET) / 16384.0);

		data.X_Angle = acos(atof(AX1))* 57.2;
    data.Y_Angle = acos(atof(AY1))* 57.2;
    data.Z_Angle = acos(atof(AZ1))* 57.2;
		
		
		sprintf(AX2,"%f",data.X_Angle);
		sprintf(AY2,"%f",data.Y_Angle);
		sprintf(AZ2,"%f",data.Z_Angle);
		
//		OLED_ShowString(1, 1, AX2);
//		OLED_ShowString(2, 1, AY2);
//		OLED_ShowString(3, 1, AZ2);
		
		if(data.X_Angle<87){
			LED2_ON();
			LED1_OFF();
		}
		else if(data.X_Angle>93){
			LED1_ON();
			LED2_OFF();
		}
		else{
			LED1_ON();
			LED2_ON();
		}
		
		if(data.Y_Angle<87){
			LED4_ON();
			LED3_OFF();
		}
		else if(data.Y_Angle>93){
			LED3_ON();
			LED4_OFF();
		}
		else{
			LED3_ON();
			LED4_ON();
		}//水平仪
		

		
//		OLED_ShowString(2, 1, AX1);//角速度
//		OLED_ShowString(3, 1, strcat(AZ2 ,"DU"));
	
//		OLED_ShowSignedNum(2, 8, GY, 5);//角速度
//		OLED_ShowSignedNum(3, 8, GZ, 5);
		
		
		if((CX > 0)&&(CY > 0)) Deg = atan(CY/CX)*57;
			else if((CX > 0)&&(CY < 0)) Deg = 0;
			else if((CX == 0)&&(CY > 0)) Deg = 90;
			else if((CX == 0)&&(CY < 0)) Deg = 270;
			else if(CX < 0) Deg = 0;
		 if(Deg < 2||Deg >358) LEDN_ON(); else LEDN_OFF();
		
//********************************************************************
//  	OLED_ShowSignedNum(1, 1, AX, 5);
//		OLED_ShowSignedNum(2, 1, AY, 5);
//		OLED_ShowSignedNum(3, 1, AZ, 5);
//********************************************************************	
		
//		
//		OLED_ShowString(4, 1, "Degree:");
//		OLED_ShowSignedNum(4, 9, atan(CY/CX), 3);
		if(Serial_GetRxFlag() == 1){
			
			
		char *pl = strtok(Serial_RxPacket,",");
		char pl2 = pl[0];
		for(uint8_t l = 0;l<strlen(pl);l++){
			pl[l] = pl[l+1];
			if(l==strlen(pl)-1) pl[l]=0;
		}
	
		if(pl2 == 'N') N2 = atof(pl);
		if(pl2 == 'S') N2 = -atof(pl);
		if(pl2 == 'W') E2 = -atof(pl);
		if(pl2 == 'E') E2 = atof(pl);
		
//		char fffff[10];
//		sprintf(fffff,"%f",N2);
//		Serial_Printf(fffff);
		
		pl = strtok(NULL,",");
		pl2 = pl[0];
		for(uint8_t l = 0;l<strlen(pl);l++){
			pl[l] = pl[l+1];
			if(l==strlen(pl)-1) pl[l]=0;
		}
		
		if(pl2 == 'N') N2 = atof(pl);
		if(pl2 == 'S') N2 = -atof(pl);
		if(pl2 == 'W') E2 = -atof(pl);
		if(pl2 == 'E') E2 = atof(pl);
		
		char hikj1[15],hikj2[15];
		sprintf(hikj1,"%f",N2);
		OLED_ShowString(1,1,hikj1);
		sprintf(hikj2,"%f",E2);
		OLED_ShowString(2,1,hikj2);
	}
		
		if (GPS_GetRxFlag() == 1)
		{
			N1 = GPS_GetRxDataN();
			E1 = GPS_GetRxDataE();
		}//当前位置
		
			detN = N2 - N1;
			detE = E2 - E1;
			if(detE > 180){
				detE = -(360 - detE);
			}
			if(detE < -180){
				detE = 360 + detE;
			}
			
			if((detE > 0)&&(detN > 0)) Deg2 = atan(detN/detE)*57;
			else if((detE > 0)&&(detN < 0)) Deg2 = 0;
			else if((detE == 0)&&(detN > 0)) Deg2 = 90;
			else if((detE == 0)&&(detN < 0)) Deg2 = 270;
			else if(detE < 0) Deg2 = 0;
			
			if(Deg - 1 < Deg2 < Deg + 1)
				LEDzx_ON();
			else  LEDzx_OFF();
			Delay_ms(50);
			
			
	}
}
