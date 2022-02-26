#ifndef USART_H
#define USART_H
#include "stm32f0xx.h"

#define UART1_TXD_GPIO GPIOA
#define UART1_TXD_PIN GPIO_Pin_9

#define UART1_RXD_GPIO GPIOA
#define UART1_RXD_PIN GPIO_Pin_10

#define INSTRUCTION_NUMBER 8

extern __IO uint8_t Usart_Timeout;
extern __IO FunctionalState Usart_Timeout_State;
#define USART_TIMEOUT_THRESHOLD 1 //in 100ms
extern __IO uint8_t Usart_Receive_Pointer;
extern uint8_t Usart_Send_Buf[10];
extern const uint8_t Instructions[INSTRUCTION_NUMBER][40];

void Usart_Init(void);
void Usart_Send_Byte(uint8_t data);
void Usart_Send_Bytes(uint8_t *p,uint8_t length);
uint8_t CommandCompare(uint8_t *cmd);
void Usart_PrintInstructions(void);

#endif