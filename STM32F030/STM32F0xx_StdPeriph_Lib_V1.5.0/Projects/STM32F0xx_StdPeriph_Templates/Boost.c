#include "boost.h"

uint16_t BoostVoltage = 0;
uint8_t BoostVoltagePWM = 0;
uint16_t BoostVoltageSetpoint = 355;      //50V -> 455, 58V -> 528  43k 2k divider
void BOOST_Init(void)
{
  GPIO_InitTypeDef GPIO_Type;
  TIM_TimeBaseInitTypeDef TIM_Type;
  TIM_OCInitTypeDef TIM_OC_Type;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
    
  GPIO_Type.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Type.GPIO_OType = GPIO_OType_PP;
  GPIO_Type.GPIO_Pin = BOOST_PWM_PIN;
  GPIO_Type.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Type.GPIO_Speed = GPIO_Speed_Level_1;
  
  GPIO_Init(BOOST_PWM_GPIO,&GPIO_Type);		
  
  GPIO_Type.GPIO_Mode = GPIO_Mode_AN;
  GPIO_Type.GPIO_OType = GPIO_OType_OD;
  GPIO_Type.GPIO_Pin = BOOST_ADC_PIN;
  GPIO_Type.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Type.GPIO_Speed = GPIO_Speed_Level_1;
  
  GPIO_Init(BOOST_ADC_GPIO,&GPIO_Type);		//ADC
  
  GPIO_PinAFConfig(BOOST_PWM_GPIO, GPIO_PinSource1, GPIO_AF_0); //TIM14 CH1
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
  
  TIM_Type.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_Type.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_Type.TIM_Period = BOOST_PWM_LIMIT;
  TIM_Type.TIM_Prescaler = 0;
  TIM_Type.TIM_RepetitionCounter = 0;
  
  TIM_DeInit(TIM14);
  TIM_TimeBaseInit(TIM14,&TIM_Type);
  TIM_Cmd(TIM14,ENABLE);
  
  TIM_OC_Type.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OC_Type.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OC_Type.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
  TIM_OC_Type.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OC_Type.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC_Type.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OC_Type.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OC_Type.TIM_Pulse = 750;
  TIM_OC1Init(TIM14, &TIM_OC_Type);
}

void BOOST_Control(void)
{
  int error;
  long pwm_temp = 0;
  static int ierror = 0;
  error = BoostVoltageSetpoint - BoostVoltage;
  //ierror+=error;
  if(ierror > 5000)
  {
    ierror = 5000;
  }
  if(ierror < -5000)
  {
    ierror = -5000;
  }
  pwm_temp += error;
  if(pwm_temp > PWM_LIMIT-70)
  {
    pwm_temp = PWM_LIMIT-70;
  }
  if(pwm_temp < 0)
  {
    pwm_temp = 0;
  }
  BoostVoltagePWM = pwm_temp;
}