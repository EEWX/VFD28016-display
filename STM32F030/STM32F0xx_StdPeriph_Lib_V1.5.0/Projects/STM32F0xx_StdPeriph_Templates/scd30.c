#include "SCD30.h"

void SCD30_Init(void)
{
  GPIO_InitTypeDef GPIO_Type;
  I2C_InitTypeDef I2C_Type;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
  CRC_PolynomialSizeSelect(CRC_PolSize_8);
  CRC_SetPolynomial(0x31);
  
  GPIO_Type.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Type.GPIO_OType = GPIO_OType_OD;
  GPIO_Type.GPIO_Pin = SCD_SCL_PIN;
  GPIO_Type.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Type.GPIO_Speed = GPIO_Speed_Level_1;
  
  GPIO_Init(SCD_SCL_PORT,&GPIO_Type); //SCL
  GPIO_Type.GPIO_Pin = SCD_SDA_PIN;
  GPIO_Init(SCD_SDA_PORT,&GPIO_Type); //SDA
  
  I2C_Type.I2C_Ack = I2C_Ack_Disable;
  I2C_Type.I2C_Mode = I2C_Mode_I2C;
  
  I2C_Init(I2C1,&I2C_Type);
  I2C_Cmd(I2C1,ENABLE);
}

/**
  * @brief  Compute CRC value for input message
  * @param  data: pointer at uint8_t
  * @param  size: the size of the input message
  * @retval The computed CRC value
  */
static uint8_t CRC_8BitsCompute(uint8_t* data, uint32_t size)
{
  uint8_t* dataEnd = data+size;

  /* Reset CRC data register to avoid overlap when computing new data stream */
  CRC_ResetDR();

  while(data < dataEnd)
  {
    /* Write the input data in the CRC data register */
    CRC_CalcCRC8bits(*data++);
  }
  /* Return the CRC value */
  return (uint8_t)CRC_GetCRC();
}

void SCD30_ContinousMeasurewWithPressure(uint16_t pre)
{
  uint8_t buf[6];
  buf[0] = SCD_ADDRESS;
  buf[1] = SCD_CMD_START_MEASURE>>8;
  buf[2] = SCD_CMD_START_MEASURE&0x00ff;
  buf[3] = pre>>8;
  buf[4] = pre&0x00ff;
  buf[5] = CRC_8BitsCompute(buf,5);
  I2C_GenerateSTART(I2C1,ENABLE);
  for(uint8_t cnt = 0; cnt<6;cnt++)
  {
    I2C_SendData(I2C1,buf[cnt]);
    while(SET != I2C_GetFlagStatus(I2C1, I2C_FLAG_TC));
  }
  I2C_GenerateSTOP(I2C1,ENABLE);
}

void SCD30_StopContinousMeasure(void)
{
  uint8_t buf[4];
  buf[0] = SCD_ADDRESS;
  buf[1] = SCD_CMD_STOP_MEASURE>>8;
  buf[2] = SCD_CMD_STOP_MEASURE&0x00ff;
  buf[3] = CRC_8BitsCompute(buf,3);
  I2C_GenerateSTART(I2C1,ENABLE);
  for(uint8_t cnt = 0; cnt<4;cnt++)
  {
    I2C_SendData(I2C1,buf[cnt]);
    while(SET != I2C_GetFlagStatus(I2C1, I2C_FLAG_TC));
  }
  I2C_GenerateSTOP(I2C1,ENABLE);
}

void SCD30_SetMeasurewInterval(uint16_t tim)
{
  uint8_t buf[6];
  buf[0] = SCD_ADDRESS;
  buf[1] = SCD_CMD_SET_INTERVAL>>8;
  buf[2] = SCD_CMD_SET_INTERVAL&0x00ff;
  buf[3] = tim>>8;
  buf[4] = tim&0x00ff;
  buf[5] = CRC_8BitsCompute(buf,5);
  I2C_GenerateSTART(I2C1,ENABLE);
  for(uint8_t cnt = 0; cnt<6;cnt++)
  {
    I2C_SendData(I2C1,buf[cnt]);
    while(SET != I2C_GetFlagStatus(I2C1, I2C_FLAG_TC));
  }
  I2C_GenerateSTOP(I2C1,ENABLE);
}

uint16_t SCD30_ReadMeasurewInterval(void)
{
  uint8_t buf[6];
  uint16_t interval;
  buf[0] = SCD_ADDRESS;
  buf[1] = SCD_CMD_SET_INTERVAL>>8;
  buf[2] = SCD_CMD_SET_INTERVAL&0x00ff;
  I2C_GenerateSTART(I2C1,ENABLE);
  for(uint8_t cnt = 0; cnt<3;cnt++)
  {
    I2C_SendData(I2C1,buf[cnt]);
    while(SET != I2C_GetFlagStatus(I2C1, I2C_FLAG_TC));
  }
  I2C_GenerateSTOP(I2C1,ENABLE);
  buf[0] = SCD_ADDRESS | 0x01;  //read address
  I2C_GenerateSTART(I2C1,ENABLE);
  I2C_SendData(I2C1,buf[0]);
  while(SET != I2C_GetFlagStatus(I2C1, I2C_FLAG_TC));
  for(uint8_t cnt = 0; cnt<3;cnt++)
  {
    while(SET != I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE));
    buf[cnt] = I2C_ReceiveData(I2C1);
    I2C_AcknowledgeConfig(I2C1,ENABLE);
  }
  buf[4] = CRC_8BitsCompute(buf,2);
  if(buf[4] == buf[3])
  {
    interval = buf[0];
    interval <<= 8;
    return(interval | buf[1]);
  }
  else
  {
    return(0);
  }
}