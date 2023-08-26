#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>//可变参数
#include "OLED.h"
#include "Serial.h"
#include <string.h>
#include <stdlib.h>

char GPS_RxData[50]={0};
uint8_t GPS_RxFlag;

void GPS_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;    //PA2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//复位串口2
//  RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//停止复位

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600; //波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //选择是否需要RTS和CTS(流控)
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//选择发送还是输出，或是二者都要
	USART_InitStructure.USART_Parity = USART_Parity_No; //是否加上校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位长度
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长,无校验位,所以为8位
	USART_Init(USART2,&USART_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);


	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2,ENABLE);
}

uint8_t GPS_GetRxFlag(void)
{
	if (GPS_RxFlag == 1)
	{
		GPS_RxFlag = 0;
		return 1;
	}
	return 0;
} //标志位判断

uint8_t i = 0;
double N,E;
int16_t p1;

uint8_t GPS_GetRxDataN(void)
{
	return N;
}

uint8_t GPS_GetRxDataE(void)
{
	return E;
}

void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		if(USART_ReceiveData(USART2)!='\n'){
		GPS_RxData[i++] = USART_ReceiveData(USART2);
		}
		else{
			
			if(GPS_RxData[18] == 'A' ){ //判断读到正确的一行 , 已经通过串口工具配置过了
				char *p;
				p=strtok(GPS_RxData,",");
				p=strtok(NULL,",");
				p=strtok(NULL,",");
				p=strtok(NULL,",");
					
					N = atof(p);
					p1 = (int16_t)N;
					N = p1/100 + (N - p1/100*100)/60;//北纬
					
				p=strtok(NULL,",");
				if(strcmp(p,"S")==0)
					N=-N;
							
//				char ddd[15];
//				sprintf(ddd,"%f",N);
//				OLED_ShowString(2,1,ddd);
				
				p=strtok(NULL,",");
				
					
					E = atof(p);
					p1 = (int16_t)E;
					E = p1/100 + (E - p1/100*100)/60; //东经
				
				if(strcmp(p,"W")==0)
					E=-E;

//				char bbb[15];
//				sprintf(bbb,"%f",E);
//				OLED_ShowString(1,1,bbb);
			}
			
			for(int8_t a = 0;a<strlen(GPS_RxData);a++){
				GPS_RxData[a] = 0;
			}//清零
			i=0;
		}
		
		GPS_RxFlag = 1;
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}
