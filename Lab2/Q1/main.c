#include "main.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"


GPIO_InitTypeDef GPIO_InitStruct;
EXTI_InitTypeDef EXTI_InitStruct;

int val=0;
int stat1=1;
int stat2=1;
void Delay(int a);
void EXTI9_5_IRQHandler(void);

void EXTI9_5_IRQHandler(void){
	// Debouncing
	Delay(36000000/3/100);
	val = GPIO_ReadInputData(GPIOB) & (GPIO_Pin_5 | GPIO_Pin_6);
	if((val & GPIO_Pin_5)==0){
		if(stat1==1){
			GPIO_SetBits(GPIOA, GPIO_Pin_15);
			stat1 = 0;
		}else{
			GPIO_ResetBits(GPIOA, GPIO_Pin_15);
			stat1 = 1;
		}
	}else if((val & GPIO_Pin_6)==0){
		if(stat2==1){
			GPIO_SetBits(GPIOB, GPIO_Pin_3);
			stat2 = 0;
		}else{
			GPIO_ResetBits(GPIOB, GPIO_Pin_3);
			stat2 = 1;
		}
	}
	while((val & GPIO_Pin_6)==0 || (val & GPIO_Pin_5)==0)
		val = GPIO_ReadInputData(GPIOB) & (GPIO_Pin_5 | GPIO_Pin_6);
	return;
}
	
int main(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	// Enabling Clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	
	// Init Port B
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	// Init Port B
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	// Init Port A
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
	
	//Init Intrupts
	EXTI_InitStruct.EXTI_Line = EXTI_Line5 | EXTI_Line6;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PB;
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_15);
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
	
	while(1){
		__NOP();
	}
}

void Delay(int a){
  int i;
  for(i=0;i<a;i=i+1)
    __NOP();
}
