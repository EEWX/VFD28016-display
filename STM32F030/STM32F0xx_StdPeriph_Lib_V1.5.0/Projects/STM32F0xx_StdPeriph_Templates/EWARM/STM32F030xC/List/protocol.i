


 







extern u8 Device_Adress;

void Store_Parameters(void);
void Restore_Parameters(void);


ErrorStatus Receive_Status = ERROR;

FlagStatus Parameters_Changed_Flag = RESET;

u8 Device_Adress = (u8)0x01;



void Flash_Write_Byte(uint32_t adress,u8 data)
{
    disableInterrupts();
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    IWDG_ReloadCounter();
    while(RESET == FLASH_GetFlagStatus(FLASH_FLAG_DUL));
    IWDG_ReloadCounter();
    FLASH_EraseByte(adress);
    IWDG_ReloadCounter();
    while(RESET == FLASH_GetFlagStatus(FLASH_FLAG_EOP));
    IWDG_ReloadCounter();
    FLASH_ProgramByte(adress,data);
    IWDG_ReloadCounter();
    while(RESET == FLASH_GetFlagStatus(FLASH_FLAG_EOP));
    IWDG_ReloadCounter();
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    IWDG_ReloadCounter();
    while(SET == FLASH_GetFlagStatus(FLASH_FLAG_DUL));
    enableInterrupts();
}

void Flash_Write_Bytes(uint32_t adress,u8 *p,u8 length)
{
	u8 count = 0;
        disableInterrupts();
	FLASH_Unlock(FLASH_MEMTYPE_DATA);
        IWDG_ReloadCounter();
	while(RESET == FLASH_GetFlagStatus(FLASH_FLAG_DUL));
	while(count < length)
	{
          IWDG_ReloadCounter();
          FLASH_EraseByte(adress+count);
          IWDG_ReloadCounter();
          while(RESET == FLASH_GetFlagStatus(FLASH_FLAG_EOP));
          IWDG_ReloadCounter();
          FLASH_ProgramByte((adress+count),*(p+count));
          IWDG_ReloadCounter();
          while(RESET == FLASH_GetFlagStatus(FLASH_FLAG_EOP));
          count++;
	}
	FLASH_Lock(FLASH_MEMTYPE_DATA);
	while(SET == FLASH_GetFlagStatus(FLASH_FLAG_DUL));
        enableInterrupts();
}


void Store_Parameters(void)
{
	u8 index = 0;
	u8 temp[5] = {0};
	
	temp[0] = 0xa5;
	temp[1] = Device_Adress;
	
	while(index < 4)
	{
	  temp[4] += temp[index++];
	}
	
	Flash_Write_Bytes(0x004000,temp,5);
}


void Restore_Parameters(void)
{
	u8 index = 0;
	u8 sum = 0;
	u8 temp[5] = {0};
	while(index < 5)
	{
          temp[index] = FLASH_ReadByte(0x004000+index);
          if(index < 4)
          {
            sum += temp[index];
          }
          index++;
	}
	
	if(temp[0] != 0xa5)
	{
	  Store_Parameters();
	}
	else
	{
            while(sum != temp[4])
            {
              index = 0;
              while(index < 5)
              {
                temp[index] = FLASH_ReadByte(0x004000+index);
                if(index < 4)
                {
                  sum += temp[index];
                }
                index++;
              }
            }
            Device_Adress = temp[1];
 
	}
}
