#ifndef FILAMENT_H
#define FILAMENT_H
#include "stm32f0xx.h"


#define FILAMENT_PWM_L_GPIO GPIOA
#define FILAMENT_PWM_L_PIN GPIO_Pin_6

#define FILAMENT_PWM_R_GPIO GPIOA
#define FILAMENT_PWM_R_PIN GPIO_Pin_7

#define FILAMENT_ADC_GPIO GPIOA
#define FILAMENT_ADC_PIN GPIO_Pin_0

#define FILAMENT_PWM_DEFAULT 35
#define FILAMENT_PWM_LIMIT 500

extern uint16_t FilamentCurrent;
void FILAMENT_Init(void);
#endif