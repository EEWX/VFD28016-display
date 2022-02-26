/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   Main program body
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
#include "main.h"
#include "mn28016.h"
#include "usart.h"
/** @addtogroup STM32F0xx_StdPeriph_Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */ 
      
  /* Add your application code here
     */
  NVIC_InitTypeDef NVIC_Type;
  
  NVIC_Type.NVIC_IRQChannel = SysTick_IRQn;
  NVIC_Type.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Type.NVIC_IRQChannelPriority = 2;
  NVIC_Init(&NVIC_Type);
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  if (SysTick_Config(SystemCoreClock / 6000))
  { 
    /* Capture error */ 
    while (1);
  }
  Usart_Init();
  Usart_PrintInstructions();
  MN28016_Init();
// MN28016_RAM_Put6x8Char(0,0,'1');
  MN28016_RAM_Put8x16Str(0,0,"UART:115200 N 8 1,type CMD? for tip");
  //MN28016_RAM_Put6x8Str(0,7,"VFD28016 V1.0 UART setting 115200 N 8 1");
 // MN28016_RAM_Put6x8Str(0,0,"Send CMD? for the instructions list");
 // MN28016_DrawLine(5,5,250,16);
//  MN28016_WritePixel(10,10,1);
  SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
  
  
/* Infinite loop */
  while (1)
  {
      //MN28016_Refresh();
//    if(DisplayUpdateFlag != RESET)
//    {
//      
//      DisplayUpdateFlag = RESET;
//      MN28016_SPI_Refresh();
//      DMA_Cmd(DMA1_Channel3,DISABLE);
//      DMA_SetCurrDataCounter(DMA1_Channel3,12);
//      DMA_Cmd(DMA1_Channel3,ENABLE);
//  //    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
//    }
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
