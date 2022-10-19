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
	TIM_OCInitTypeDef TIM_OCInitTypeDefStructure;
	
	// Enabling Clocks
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	// Init Port A7 Green
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	// Init Port B1 Red B0 Blue
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	// Timer
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 256-1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	//
	TIM_OCInitTypeDefStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitTypeDefStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitTypeDefStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	
	//
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 36000-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
	// NVIC
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_Init(&NVIC_InitStruct);
	
	//TIM_ITConfig(TIM3, TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	
	// NVIC
	/*NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_Init(&NVIC_InitStruct);*/
	
	TIM_Cmd(TIM3, ENABLE);
	
	int r=255, g=0, b=0;
	
	while(1){
		TIM_OCInitTypeDefStructure.TIM_Pulse = g; // g
		TIM_OC2Init(TIM3, &TIM_OCInitTypeDefStructure);
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
		
		TIM_OCInitTypeDefStructure.TIM_Pulse = b; // b
		TIM_OC3Init(TIM3, &TIM_OCInitTypeDefStructure);
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		
		TIM_OCInitTypeDefStructure.TIM_Pulse = r; // r
		TIM_OC4Init(TIM3, &TIM_OCInitTypeDefStructure);
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
		
		for(;g<256;g++){
			TIM_OCInitTypeDefStructure.TIM_Pulse = g; // g
			TIM_OC2Init(TIM3, &TIM_OCInitTypeDefStructure);
			TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
			Delay(1);
		}
		for(;r>0;r--){
			TIM_OCInitTypeDefStructure.TIM_Pulse = r; // r
			TIM_OC4Init(TIM3, &TIM_OCInitTypeDefStructure);
			TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
			Delay(1);
		}
		for(;b<256;b++){
			TIM_OCInitTypeDefStructure.TIM_Pulse = b; // b
			TIM_OC3Init(TIM3, &TIM_OCInitTypeDefStructure);
			TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
			Delay(1);
		}
		for(;g>0;g--){
			TIM_OCInitTypeDefStructure.TIM_Pulse = g; // g
			TIM_OC2Init(TIM3, &TIM_OCInitTypeDefStructure);
			TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
			Delay(1);
		}
		for(;r<256;r++){
			TIM_OCInitTypeDefStructure.TIM_Pulse = r; // r
			TIM_OC4Init(TIM3, &TIM_OCInitTypeDefStructure);
			TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
			Delay(1);
		}
		for(;b>0;b--){
			TIM_OCInitTypeDefStructure.TIM_Pulse = b; // b
			TIM_OC3Init(TIM3, &TIM_OCInitTypeDefStructure);
			TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
			Delay(1);
		}
	}
}
