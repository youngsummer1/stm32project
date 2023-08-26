#include "stm32f10x.h" // Device header

void LED_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_1 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_1 | GPIO_Pin_3);
}

void LED1_ON(){
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}

void LED1_OFF(){
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}
void LED2_ON(){
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}

void LED2_OFF(){
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
}

void LED3_ON(){
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
}

void LED3_OFF(){
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
}
void LED4_ON(){
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}

void LED4_OFF(){
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
}

void LEDN_ON(){
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}

void LEDN_OFF(){
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
}

void LEDzx_ON(){
	GPIO_ResetBits(GPIOB,GPIO_Pin_3);
}

void LEDzx_OFF(){
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
}


//void LED1_Turn(){
//	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_1)==0){
//		GPIO_SetBits(GPIOA,GPIO_Pin_1);
//	}
//	else{
//		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
//	}
//	
//}

//void LED2_Turn(){
//	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_2)==0){
//		GPIO_SetBits(GPIOA,GPIO_Pin_2);
//	}
//	else{
//		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
//	}
//	
//}
