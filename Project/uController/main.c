#include "main.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_usart.h"
#include <math.h> 
#include "stm32f10x_crc.h"

void USART2_IRQHandler(void);
void Delay(int a);
uint32_t crc_val;
int stat=0;

void USART2_IRQHandler(void){
	if(stat==0){
		stat = 1;
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		CRC->DR = 0x00000000;
		CRC_ResetDR();
		crc_val = CRC_CalcCRC(((uint32_t)USART_ReceiveData(USART2)));
		//temp_avg = USART_ReceiveData(USART2);
		USART_SendData(USART2, (uint16_t)(crc_val & 0x0FF));
		while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE))
			__NOP();
		USART_SendData(USART2, (uint16_t)((crc_val >> 8) & 0x0FF));
		while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE))
			__NOP();
		USART_SendData(USART2, (uint16_t)((crc_val >> 16) & 0x0FF));
		while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE))
			__NOP();
		USART_SendData(USART2, (uint16_t)((crc_val >> 24) & 0x0FF));
		while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE))
			__NOP();
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		stat = 0;
	}
	return ;
}
	
int main(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	USART_InitTypeDef 	USART_InitStruct;
	USART_ClockInitTypeDef 	USART_ClockInitStruct;
	
	// Enabling Clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	// Init USART
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_Init(USART2, &USART_InitStruct);
	
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;
	USART_ClockInit(USART2, &USART_ClockInitStruct);
	
	// Init Port A2
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	// Init Port A6
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_SetGuardTime(USART2, 100);
	USART_Cmd(USART2, ENABLE);
	while(1){
		__NOP();
	}
}

void Delay(int a){
	int i;
  for(i=0;i<a;i=i+1)
    __NOP();
}
