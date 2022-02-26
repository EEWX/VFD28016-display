/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "mn28016.h"
#include "usart.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void DMA1_Channel2_3_IRQHandler(void);
void USART1_IRQHandler(void);
/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  if(DisplayUpdateFlag != RESET)
  {
    
    DisplayUpdateFlag = RESET;
    MN28016_SPI_Refresh();
    RESET_LATCH();
    DMA_Cmd(DMA1_Channel3,DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel3,12);
    DMA_Cmd(DMA1_Channel3,ENABLE);
//    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
  }
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */

void DMA1_Channel2_3_IRQHandler(void)
{
  //SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,DISABLE);
  //DMA_SetCurrDataCounter(DMA1_Channel3,12);
  
  SET_BLK();
  DisplayUpdateFlag = SET;
  SET_LATCH();
  RESET_BLK();
  DMA_ClearITPendingBit(DMA1_IT_TC3);
}


void USART1_IRQHandler(void)
{
  static uint8_t received_buf[570] = {0};
  static uint16_t receive_cnt = 0;
  uint8_t cmd_code = 0;
  uint8_t *p = (unsigned char *)MN28016_RAM;
  if(USART_GetITStatus(USART1,USART_IT_RTO))
  {
    USART_ClearITPendingBit(USART1,USART_IT_RTO);
    if(receive_cnt)
    {
      cmd_code = CommandCompare(received_buf);
      switch(cmd_code)
      {
      case 0:
        break;
      case 1: //ASCH
        MN28016_RAM_Put8x16Str(0,0,received_buf+5);
        break;
      case 2: //ASCL
        MN28016_RAM_Put6x8Str(0,0,received_buf+5);
        break;
      case 5: //DOTC
       // received_buf[6]
       // MN28016_WritePixel();
        break;
      case 6: //CLRS
        MN28016_ClearScreen();
        break;
      case 7:
        Usart_PrintInstructions();
        break;
      case 8: //WRAM
        receive_cnt -=5;
        while(receive_cnt--)
        {
          *(p + receive_cnt + 2)=received_buf[receive_cnt+5];
        }
        break;
      default:
        break;
      }
      receive_cnt = 0;
    }
    
    
  }
  else
  {
    received_buf[receive_cnt] = USART_ReceiveData(USART1);
    receive_cnt++;  
    
  }
  //USART_ClearITPendingBit(USART1,USART_IT_TC);
}
/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
