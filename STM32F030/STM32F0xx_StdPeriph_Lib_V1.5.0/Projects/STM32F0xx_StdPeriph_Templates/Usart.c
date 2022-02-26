#include "usart.h"

uint8_t Usart_Send_Buf[10] = {0};
__IO uint8_t Usart_Timeout = 0;
__IO FunctionalState Usart_Timeout_State = DISABLE;
__IO uint8_t Usart_Receive_Pointer = 0;

#define CMD_NUMBER 9

  
const uint8_t CommandTable[CMD_NUMBER][4]=
{
  {'N','U','L','L'},/*"ASCH",high case display, total 35*/
  {'A','S','C','H'},/*"ASCH",high case display, total 35*/
  {'A','S','C','L'},/*"ASCH",high case display, total 35*/
  {'C','U','R','R'},/*"ASCH",high case display, total 35*/
  {'A','D','D','R'},/*"ASCH",high case display, total 35*/
  {'D','O','T','C'},/*"ASCH",high case display, total 35*/
  {'C','L','R','S'},/*"ASCH",high case display, total 35*/
  {'C','M','D','?'},/*"ASCH",high case display, total 35*/
  {'W','R','A','M'},/*"ASCH",high case display, total 35*/
};

const uint8_t Instructions[INSTRUCTION_NUMBER][40]=
{
  //1234567890123456789012345678901234567890
  {"-------------VFD28016 V1.1--------------"},
  {"---------------Wesley Xu----------------"},
  {"-------------Commands list--------------"},
  {"CLRS :Clearscreen                       "},
  {"ASCH *..*:Write 8x16 ASCII,max 35 inputs"},
  {"ASCL *..*:Write 6x8 ASCII,max 46 inputs "},
  {"DOTC xxx,yy,d:Write pixel at x,y on/off "},
  {"WRAM *..* :Write RAM data in hex        "},
};


void Usart_Init(void)
{
  GPIO_InitTypeDef GPIO_Type;
  USART_InitTypeDef UART_Type;
  NVIC_InitTypeDef NVIC_Type;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
  //RCC_USARTCLKConfig(RCC_USART1CLK_PCLK);
  
  GPIO_Type.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Type.GPIO_OType = GPIO_OType_PP;
  GPIO_Type.GPIO_Pin = UART1_TXD_PIN;
  GPIO_Type.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Type.GPIO_Speed = GPIO_Speed_Level_1;
  
  GPIO_Init(UART1_TXD_GPIO,&GPIO_Type); //UART1 TXD
  
  GPIO_Type.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Type.GPIO_OType = GPIO_OType_OD;
  GPIO_Type.GPIO_Pin = UART1_RXD_PIN;
  GPIO_Type.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Type.GPIO_Speed = GPIO_Speed_Level_1;
  GPIO_Init(UART1_RXD_GPIO,&GPIO_Type); //UART1 RXD
  
  GPIO_PinAFConfig(UART1_RXD_GPIO, GPIO_PinSource10, GPIO_AF_1); //UART1 RX
  GPIO_PinAFConfig(UART1_TXD_GPIO, GPIO_PinSource9, GPIO_AF_1); //UART1 TX
  
  UART_Type.USART_BaudRate = 115200;
  UART_Type.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  UART_Type.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  UART_Type.USART_Parity = USART_Parity_No;
  UART_Type.USART_StopBits = USART_StopBits_1;
  UART_Type.USART_WordLength = USART_WordLength_8b;
  
  NVIC_Type.NVIC_IRQChannel = USART1_IRQn;
  NVIC_Type.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Type.NVIC_IRQChannelPriority = 4;
  NVIC_Init(&NVIC_Type);
  
  USART_DeInit(USART1);
  USART_Init(USART1,&UART_Type);
  USART_SetReceiverTimeOut(USART1,1000);
  USART_ReceiverTimeOutCmd(USART1,ENABLE);
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
  USART_ITConfig(USART1,USART_IT_RTO,ENABLE);
  USART_Cmd(USART1,ENABLE);
}

void Usart_Send_Byte(uint8_t data)
{
  //GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
  USART_SendData(USART1,data);
  while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
  //GPIO_WriteLow(GPIOD,GPIO_PIN_4);
}

void Usart_Send_Bytes(uint8_t *p,uint8_t length)
{
  //GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
  while(length--)
  {
    USART_SendData(USART1,*p);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
    p++;
  }
  //GPIO_WriteLow(GPIOD,GPIO_PIN_4);
}

void Usart_PrintInstructions(void)
{
  uint8_t cnt;
  uint8_t *ptr;
  ptr = (uint8_t *)Instructions;
  for(cnt = 0;cnt<INSTRUCTION_NUMBER;cnt++)
  {
    Usart_Send_Bytes(ptr,40);
    Usart_Send_Byte(0x0d);
    Usart_Send_Byte(0x0a);
    ptr += 40;
  }
  
}

uint8_t CommandCompare(uint8_t *cmd)
{
  uint8_t *cp;
  uint8_t cnt,cmd_index;
  cp = (uint8_t *)CommandTable;
  for(cmd_index = 0;cmd_index<CMD_NUMBER;cmd_index++)
  {
    for(cnt = 0;cnt<4;cnt++)
    {
      if(*(cp+cnt) != *(cmd+cnt)) //not match
      {
        break;
      }
    }
    cp += 4;
    if(cnt == 4)
    {
      
      return(cmd_index);
    }
  }
}