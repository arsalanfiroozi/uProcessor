#include "main.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

GPIO_InitTypeDef GPIO_InitStruct;
void Delay(int a);
	
int main(void){
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	// Enabling Clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// Init Port A
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	// Init Port B
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	while(1){
		GPIO_SetBits(GPIOA, GPIO_Pin_15);
		GPIO_ResetBits(GPIOB, GPIO_Pin_3);
		Delay(36000000/3/2); // approximately 0.5 s
		GPIO_ResetBits(GPIOA, GPIO_Pin_15);
		GPIO_SetBits(GPIOB, GPIO_Pin_3);
		Delay(36000000/3/2); // approximately 0.5 s
	}
}

void Delay(int a){
	int i;
	for(i=0;i<a;i=i+1)
		__NOP();
}
