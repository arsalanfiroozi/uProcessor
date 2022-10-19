#include "main.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_usart.h"
#include <math.h> 

void Delay(int a);
void ADC1_2_IRQHandler(void);
const int n = 100;
volatile uint32_t temp[n];
volatile float temp_avg = 0;

void ADC1_2_IRQHandler(void){
	int j;
	for(j=n-2;j>=0;j=j-1){
		temp[j+1]=temp[j];
	}
	temp[0] = ADC_GetConversionValue(ADC1);
	uint32_t t;
	t=0;
	for(j=0;j<n;j=j+1){
		t = t + temp[j];
	}
	temp_avg = t / n * 3300/4096 * (-0.7) + 472.0;
	USART_SendData(USART2, floor(temp_avg));
}
	
int main(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	USART_InitTypeDef 	USART_InitStruct;
	USART_ClockInitTypeDef 	USART_ClockInitStruct;
	
	// Enabling Clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	// Init USART
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_Init(USART2, &USART_InitStruct);
	
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;
	USART_ClockInit(USART2, &USART_ClockInitStruct);
	
	// Init Port A2
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	// Init Port A6
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	// Init ADC
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &ADC_InitStruct);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_1Cycles5);
	
	ADC_StartCalibration(ADC1);
	ADC_Cmd(ADC1, ENABLE);
	while(ADC_GetCalibrationStatus(ADC1)==SET){
		__NOP();
	}
	
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_Init(&NVIC_InitStruct);
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	USART_Cmd(USART2, ENABLE);
	while(1){
		__NOP();
	}
}
