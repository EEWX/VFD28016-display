#ifndef MN28016_H
#define MN28016_H

#include "stm32f0xx.h"
#include "boost.h"
#include "filament.h"


#define MN28016_BLK_GPIO GPIOA
#define MN28016_BLK_PIN GPIO_Pin_8

#define MN28016_SCK_GPIO GPIOB
#define MN28016_SCK_PIN GPIO_Pin_13

#define MN28016_SDO_GPIO GPIOB
#define MN28016_SDO_PIN GPIO_Pin_15

#define MN28016_LATCH_GPIO GPIOB
#define MN28016_LATCH_PIN GPIO_Pin_12

#define SET_SDO() GPIO_SetBits(MN28016_SDO_GPIO,MN28016_SDO_PIN)
#define RESET_SDO() GPIO_ResetBits(MN28016_SDO_GPIO,MN28016_SDO_PIN)

#define SET_SCK() GPIO_SetBits(MN28016_SCK_GPIO,MN28016_SCK_PIN)
#define RESET_SCK() GPIO_ResetBits(MN28016_SCK_GPIO,MN28016_SCK_PIN)

#define SET_BLK() GPIO_SetBits(MN28016_BLK_GPIO,MN28016_BLK_PIN)
#define RESET_BLK() GPIO_ResetBits(MN28016_BLK_GPIO,MN28016_BLK_PIN)

#define SET_LATCH() GPIO_SetBits(MN28016_LATCH_GPIO,MN28016_LATCH_PIN)
#define RESET_LATCH() GPIO_ResetBits(MN28016_LATCH_GPIO,MN28016_LATCH_PIN)

#define PWM_LIMIT 100

extern uint16_t MN28016_RAM[281];
extern FlagStatus PID_UpdateFlag;
extern FlagStatus DisplayUpdateFlag;

void MN28016_Init(void);
void MN28016_Refresh(void);
void MN28016_SPI_Refresh(void);
void MN28016_ClearScreen(void);
void MN28016_WritePixel(uint16_t x,uint8_t y,uint8_t data);
void MN28016_DrawLine(uint16_t x1, uint8_t y1, uint16_t x2, uint8_t y2);
void MN28016_RAM_Put6x8Char(uint16_t x, uint8_t y, char asc);
void MN28016_RAM_Put6x8Str(uint16_t x, uint8_t y, char *ptr);
void MN28016_RAM_Put8x16Char(uint16_t x, uint8_t y, char asc);
void MN28016_RAM_Put8x16Str(uint16_t x, uint8_t y, char *ptr);
void ADC_ISR(void);
void TIM1_UPD_ISR(void);
#endif