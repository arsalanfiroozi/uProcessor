#include "main.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

GPIO_InitTypeDef GPIO_InitStruct;
void Delay(int a);
	
int main(void){
	int val;
	uint16_t stat;
	
	
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
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	stat=0;
	GPIO_ResetBits(GPIOA, GPIO_Pin_15);
	
	while(1){
		val = GPIO_ReadInputData(GPIOB) & GPIO_Pin_5;
		if(val!=0){
			// Debouncing
			Delay(36000000/3/100);
			val = GPIO_ReadInputData(GPIOB) & GPIO_Pin_5;
			if(val==0 && stat==0){
				GPIO_SetBits(GPIOA, GPIO_Pin_15);
				stat = 1;
			} else if(val==0) {
				GPIO_ResetBits(GPIOA, GPIO_Pin_15);
				stat = 0;
			}
		}
	}
}

void Delay(int a){
  int i;
  for(i=0;i<a;i=i+1)
    __NOP();
}
