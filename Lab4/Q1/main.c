#include "main.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"

void Delay(int a);
void TIM2_IRQHandler(void);
int t = 0;
int Delay_t = 0; // In mSeconds

void Delay(int ms){
	Delay_t = ms;
	t = 0;
	while(t!=Delay_t) {
		__NOP();
	}
	t=0;
	return;
}


void TIM2_IRQHandler(void){
	t = t + 1;
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

int main(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//TIM_OCInitTypeDef TIM2_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	// Enabling Clocks
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	// Init Port A
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	// Timer
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 36000-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	/*
	TIM2_InitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM2_InitStruct.TIM_OCMode = TIM_OCMode_Active;
	TIM2_InitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM2_InitStruct.TIM_OCPolarity = 	TIM_OCPolarity_High;
	TIM2_InitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM2_InitStruct.TIM_Pulse = 
	*/
	
	// NVIC
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM2, ENABLE);
	
	while(1){
		Delay(500);
		GPIO_SetBits(GPIOA, GPIO_Pin_15);
		Delay(500);
		GPIO_ResetBits(GPIOA, GPIO_Pin_15);
	}
}
