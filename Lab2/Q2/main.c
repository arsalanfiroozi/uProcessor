#include "main.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"


GPIO_InitTypeDef GPIO_InitStruct;
EXTI_InitTypeDef EXTI_InitStruct;

int val3,val2,val1,val=0;
int stat1=1;
int stat2=1;
int stat3=1;
void Delay(int a);

void EXTI15_10_IRQHandler(void);

void EXTI15_10_IRQHandler(void){
	// Debouncing
	Delay(36000000/3/100);
	val = GPIO_ReadInputData(GPIOC) & (GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	if((val & GPIO_Pin_13)!=0){
		GPIO_SetBits(GPIOB, GPIO_Pin_1);
	}
	else {
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	}
		
	if((val & GPIO_Pin_14)!=0){
		GPIO_SetBits(GPIOA, GPIO_Pin_7);
	}
	else{
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
	}
	
	if((val & GPIO_Pin_15)!=0){
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
	}
	else{
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	}
}
	
int main(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	// Enabling Clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	// Init Port B
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	// Init Port C
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);

	// Init Port A
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);
	
	//Init Intrupts
	EXTI_InitStruct.EXTI_Line = EXTI_Line13 | EXTI_Line14 | EXTI_Line15;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PC | AFIO_EXTICR4_EXTI14_PC | AFIO_EXTICR4_EXTI15_PC;
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_4);
	while(1){
		__NOP();
	}
}

void Delay(int a){
  int i;
  for(i=0;i<a;i=i+1)
    __NOP();
}
