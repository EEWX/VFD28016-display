#ifndef BOOST_H
#define BOOST_H
#include "stm32f0xx.h"
#include "mn28016.h"

#define BOOST_PWM_GPIO GPIOB
#define BOOST_PWM_PIN GPIO_Pin_1

#define BOOST_ADC_GPIO GPIOA
#define BOOST_ADC_PIN GPIO_Pin_1

#define BOOST_PWM_LIMIT 1000

extern uint8_t BoostVoltagePWM;
extern uint16_t BoostVoltage;
extern uint16_t BoostVoltageSetpoint;
void BOOST_Init(void);
void BOOST_Control(void);
#endif