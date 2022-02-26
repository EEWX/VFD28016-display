#include "filament.h"

uint16_t FilamentCurrent = 0;

void FILAMENT_Init(void)
{
  GPIO_InitTypeDef GPIO_Type;
  TIM_TimeBaseInitTypeDef TIM_Type;
  TIM_OCInitTypeDef TIM_OC_Type;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
 
  GPIO_Type.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Type.GPIO_OType = GPIO_OType_PP;
  GPIO_Type.GPIO_Pin = FILAMENT_PWM_L_PIN;
  GPIO_Type.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Type.GPIO_Speed = GPIO_Speed_Level_1;
  
  GPIO_Init(FILAMENT_PWM_L_GPIO,&GPIO_Type); //PWM_L
  
  GPIO_Type.GPIO_Pin = FILAMENT_PWM_R_PIN;
  GPIO_Init(FILAMENT_PWM_R_GPIO,&GPIO_Type); //PWM R
  
  GPIO_Type.GPIO_Mode = GPIO_Mode_AN;
  GPIO_Type.GPIO_OType = GPIO_OType_OD;
  GPIO_Type.GPIO_Pin = FILAMENT_ADC_PIN;
  GPIO_Type.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Type.GPIO_Speed = GPIO_Speed_Level_1;
  
  GPIO_Init(FILAMENT_ADC_GPIO,&GPIO_Type);		//ADC
  
  GPIO_PinAFConfig(FILAMENT_PWM_L_GPIO, GPIO_PinSource6, GPIO_AF_1); //TIM14 CH1
  GPIO_PinAFConfig(FILAMENT_PWM_R_GPIO, GPIO_PinSource7, GPIO_AF_1); //TIM14 CH1
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
  
  TIM_Type.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_Type.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
  TIM_Type.TIM_Period = FILAMENT_PWM_LIMIT;
  TIM_Type.TIM_Prescaler = 0;
  TIM_Type.TIM_RepetitionCounter = 0;
  
  
  
  TIM_DeInit(TIM3);
  TIM_TimeBaseInit(TIM3,&TIM_Type);
  TIM_Cmd(TIM3,ENABLE);
  
  TIM_OC_Type.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OC_Type.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OC_Type.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
  TIM_OC_Type.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OC_Type.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC_Type.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OC_Type.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OC_Type.TIM_Pulse = FILAMENT_PWM_DEFAULT;
  TIM_OC1Init(TIM3, &TIM_OC_Type);
  
  TIM_OC_Type.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OC_Type.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OC_Type.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
  TIM_OC_Type.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OC_Type.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC_Type.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OC_Type.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OC_Type.TIM_Pulse = FILAMENT_PWM_LIMIT - FILAMENT_PWM_DEFAULT;
  TIM_OC2Init(TIM3, &TIM_OC_Type);
  
}