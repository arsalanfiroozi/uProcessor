#include "main.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

GPIO_InitTypeDef GPIO_InitStruct;
void Delay(int a);
	
int main(void){
	int val1,val2,val3;
	uint16_t stat;
	
	// Enabling Clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	// Init Port A
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	// Init Port B
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	// Init Port C
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	stat=0;
	while(1){
		val1 = GPIO_ReadInputData(GPIOC) & GPIO_Pin_13;
		val2 = GPIO_ReadInputData(GPIOC) & GPIO_Pin_14;
		val3 = GPIO_ReadInputData(GPIOC) & GPIO_Pin_15;
		if(val1==0)
			GPIO_ResetBits(GPIOB, GPIO_Pin_1);
		if(val2==0)
			GPIO_ResetBits(GPIOA, GPIO_Pin_7);
		if(val3==0)
			GPIO_ResetBits(GPIOB, GPIO_Pin_0);
		if(val1!=0 || val2!=0 || val3!=0){
			// Debouncing
			Delay(36000000/3/100);
			val1 = GPIO_ReadInputData(GPIOC) & GPIO_Pin_13;
			val2 = GPIO_ReadInputData(GPIOC) & GPIO_Pin_14;
			val3 = GPIO_ReadInputData(GPIOC) & GPIO_Pin_15;
		}
		if(val1!=0 || val2!=0 || val3!=0){
			if(val1!=0)
				GPIO_SetBits(GPIOB, GPIO_Pin_1);
			else
				GPIO_ResetBits(GPIOB, GPIO_Pin_1);
			
			if(val2!=0)
				GPIO_SetBits(GPIOA, GPIO_Pin_7);
			else
				GPIO_ResetBits(GPIOA, GPIO_Pin_7);
			
			if(val3!=0)
				GPIO_SetBits(GPIOB, GPIO_Pin_0);
			else
				GPIO_ResetBits(GPIOB, GPIO_Pin_0);
		}
	}
}

void Delay(int a){
  int i;
  for(i=0;i<a;i=i+1)
    __NOP();
}
