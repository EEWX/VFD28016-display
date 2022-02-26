#include "config.h"
#include "usart.h"
#include "hc595.h"
#include "pwm.h"

void Clock_Init(void);
void Gpio_Init(void);
void Timer_Init(void);

void System_Init(void)
{
    Clock_Init();
    HC595_Init();
    Usart_Init();
    Timer_Init();
    PWM_Init();
	
}

//At startup the master clock source is automatically selected as HSI RC clock output(16M) divided by 8 (fHSI/8 = 2MHz).
void Clock_Init(void)
{
  CLK_DeInit();
  CLK_HSECmd(ENABLE);
  CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO,CLK_SOURCE_HSE,DISABLE,CLK_CURRENTCLOCKSTATE_ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

void Timer_Init(void)
{
  TIM4_DeInit();
  TIM4_TimeBaseInit(TIM4_PRESCALER_128,200);//11059200/128=86400, 86400/200=432
  TIM4_ITConfig(TIM2_IT_UPDATE,ENABLE);
  TIM4_Cmd(ENABLE);
}



