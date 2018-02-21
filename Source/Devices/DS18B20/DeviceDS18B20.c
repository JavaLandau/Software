/**
  \file    DeviceDS18B20.c 
  \brief   ����������� ���� �������� ������� ����������� DS18B20
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#include "DeviceDS18B20.h"
#include "GPIOfun.h"
#include "TIMDelay.h"

/**
  \defgroup module_service_DS18B20 ��������� ������� ��� ������ � DS18B20
  \brief ������ ��������� �������, ����������� ��� ������ � ����������� ������� ����������� DS18B20
@{
*/

#define COUNT_BITS_OF_BYTE              8               ///<���������� ��� � �����

#define TIME_RESET_PULSE                460             ///<����� �������� Reset � �������������
#define TIME_PRESENCE_WAIT              15              ///<����� ������� �� ������� � �������������

#define TIME_BETWEEN_SLOT               1               ///<����� ����� ������� � �������������

#define TIME_INIT_WRITE_SLOT            1               ///<����� ������������� ����� ������ � �������������
#define TIME_WRITE_0_SLOT               50              ///<����� ������ "0" � �������������
#define TIME_WRITE_1_SLOT               35              ///<����� ������ "1" � �������������

#define TIME_INIT_READ_SLOT             1               ///<����� ������������� ����� ������ � �������������
#define TIME_READ_SLOT                  4               ///<���� ������
#define TIME_READ_SLOT_PART             35              ///<����� ����� ������

#define TIME_WAIT                       10              ///<����� ��������

#define TIME_TEMP_CONV                  800000          ///<����� ����������� ������ �����������

#define DS18B20_SCRATCHPAD_MEM_SIZE    9               ///<������ ������ ������� ����������� � ������
#define DS18B20_COUNT_TRYING           10              ///<���������� ������� ��� �������/�������� ������

static DeviceDS18B20 DS18B20Dev;                      ///<������ �������� ������� �����������
static uint8_t StatusReadConvertT = FALSE;              ///<���� ��������� ����������� ������

/**������������� �������� ������ �������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
static uint32_t prvDeviceDS18B20InitSequence(ExtCodeDeviceDS18B20* pExCode);

/**������ ������ ������ �������
  \param[out] pRecBytes ����������� ������
  \return ��������� ���������� ������� 
*/ 
static uint32_t prvDeviceDS18B20ReadScratchPad(uint8_t* pRecBytes);

/**�������� ������ � ������ 
  \param[in] pSendBytes ������������ ������
  \param[in] Size ������ ������
  \return ��������� ���������� ������� 
*/ 
static uint32_t prvDeviceDS18B20WriteBytes(uint8_t* pSendBytes, uint32_t Size);

/**���������� ����������� ����� CRC-8-Dallas/Maxim
  \param[in] Data ������������ ������
  \param[in] Size ������ ������
  \return ��������� ���������� ������� 
*/ 
static uint32_t prvDeviceDS18B20crc8(uint8_t* Data, uint32_t Size);

/*���������� ����������� ����� CRC-8-Dallas/Maxim*/
static uint32_t prvDeviceDS18B20crc8(uint8_t* Data, uint32_t Size)
{
  uint8_t crc = 0;
  for ( uint32_t i = 0; i < Size; ++i )
  {
      uint8_t inbyte = Data[i];
      for ( uint8_t j = 0; j < 8; ++j )
      {
          uint8_t mix = (crc ^ inbyte) & 0x01;
          crc >>= 1;
          if ( mix ) crc ^= 0x8C;
          inbyte >>= 1;
      }
  }
  return crc;
}

/*������������� �������� ������ �������*/
static uint32_t prvDeviceDS18B20InitSequence(ExtCodeDeviceDS18B20* pExCode)
{
  /*�������� ������� ������*/
  if(!pExCode)
    return FUNC_INVALID_PARAM;  
    
  *pExCode = DEVICE_DS18B20_NOT_CODE;  
  StatusReadConvertT = FALSE;
  
  /*Reset*/
  HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_RESET);
  TIMDelay(TIME_RESET_PULSE);
  HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_SET);
  
  TIMDelay(TIME_WAIT);
  
  /*����� �������� ������� �� �������*/
  uint8_t presenceFlag = FALSE;
   
  for(uint8_t i = 0; i < DS18B20_COUNT_TRYING; i++)
  {
    TIMDelay(TIME_PRESENCE_WAIT);
    
    if(HAL_GPIO_ReadPin(DS18B20Dev.pGPIORXPort, DS18B20Dev.GPIORXNum) == GPIO_PIN_RESET)
    {
      presenceFlag = TRUE; 
      break;
    }
  }
  
  if(!presenceFlag)
  {
    *pExCode = DEVICE_DS18B20_ERROR_NOT_RESP;
    return FUNC_ERROR;
  }
  
  /*�������� ������������ ����*/
  while(HAL_GPIO_ReadPin(DS18B20Dev.pGPIORXPort, DS18B20Dev.GPIORXNum) == GPIO_PIN_RESET); 
  
  return FUNC_OK;
}

/*������ ������ ������ �������*/
static uint32_t prvDeviceDS18B20ReadScratchPad(uint8_t* pRecBytes)
{
  /*�������� ������� ������*/
  if(!pRecBytes)
    return FUNC_INVALID_PARAM;     
  
  uint32_t Res;  
  uint8_t CurByte;
  
  /*��������� ������ ������*/
  uint8_t Cmd = SKIP_ROM;
  Res = prvDeviceDS18B20WriteBytes(&Cmd, 1);  
  
  Cmd = READ_SCRATCHPAD;
  Res = prvDeviceDS18B20WriteBytes(&Cmd, 1);
  
  /*������ ������ �� ������*/
  if(Res == FUNC_OK)
  {
    for(uint8_t i = 0; i < DS18B20_SCRATCHPAD_MEM_SIZE; i++)
    {      
      CurByte = 0;
      uint8_t CurBit;
      for(uint8_t j = 0; j < COUNT_BITS_OF_BYTE; j++)
      {
        HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_RESET);
        TIMDelay(TIME_INIT_READ_SLOT);
        HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_SET);
        TIMDelay(TIME_READ_SLOT);
        CurBit = HAL_GPIO_ReadPin(DS18B20Dev.pGPIORXPort, DS18B20Dev.GPIORXNum);
        
        TIMDelay(TIME_READ_SLOT_PART);
        
        if(CurBit == GPIO_PIN_SET)
          CurByte |= (1<<j);        
      }
      pRecBytes[i] = CurByte;
    }
  }
  
  return Res;
}

/*�������� ������ � ������*/
static uint32_t prvDeviceDS18B20WriteBytes(uint8_t* pSendBytes, uint32_t Size)
{
  /*�������� ������� ������*/
  if(!pSendBytes)
    return FUNC_INVALID_PARAM;  
  
  StatusReadConvertT = FALSE;
  
  /*������ ������*/
  for(uint32_t i = 0; i < Size; i++)  
  {
    uint8_t CurByte = pSendBytes[i];
    
    for(uint8_t j = 0; j < COUNT_BITS_OF_BYTE; j++)
    {
      HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_RESET);
      if((CurByte >> j) & 0x1)
      {                        
        TIMDelay(TIME_INIT_WRITE_SLOT);        
        HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_SET);
        TIMDelay(TIME_WRITE_1_SLOT);        
      }
      else{        
        TIMDelay(TIME_WRITE_0_SLOT);
        HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_SET);
      }
      TIMDelay(TIME_BETWEEN_SLOT);
    }
  }
  
  return FUNC_OK;
}

/**
@}
  \defgroup module_DS18B20 ������������ ������� ��� ������ � DS18B20
  \brief ������, ��������������� ������������ ����������� ���������� ��� ������ � ����������� ������� ����������� DS18B20
@{
*/

/*������������� �������� ������� �����������*/
uint32_t DeviceDS18B20Create(DeviceDS18B20Param* pDevParam, ExtCodeDeviceDS18B20* pExCode)
{  
  /*�������� ������� ������*/
  if(!pDevParam || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode = DEVICE_DS18B20_NOT_CODE;
             
  /*������������� GPIO*/
  uint32_t Res;
  GPIO_InitTypeDef GPIO_InitStruct;
  DS18B20Dev.pGPIOTXPort = 0;
  
  Res = GPIOGetPort(pDevParam->GPIOTXPin,&DS18B20Dev.pGPIOTXPort,&DS18B20Dev.GPIOTXNum);
  
  if(Res != FUNC_OK)
  {
    *pExCode = DEVICE_DS18B20_ERROR_INIT_GPIO;
    return FUNC_ERROR;    
  }
    
  HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = DS18B20Dev.GPIOTXNum;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DS18B20Dev.pGPIOTXPort, &GPIO_InitStruct);    
  
  DS18B20Dev.pGPIORXPort = 0;
  Res = GPIOGetPort(pDevParam->GPIORXPin,&DS18B20Dev.pGPIORXPort,&DS18B20Dev.GPIORXNum);
  
  if(Res != FUNC_OK)
  {
    *pExCode = DEVICE_DS18B20_ERROR_INIT_GPIO;
    return FUNC_ERROR;    
  }
    
  GPIO_InitStruct.Pin = DS18B20Dev.GPIORXNum;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DS18B20Dev.pGPIORXPort, &GPIO_InitStruct);     
  
  StatusReadConvertT = FALSE;
  
  return FUNC_OK;  
}

/*������ ��������� ���������� ������ ������ � �����������*/
uint32_t DeviceDS18B20GetReadyTemperature(uint8_t* pTempReady)
{
  /*�������� ������� ������*/
  if(!pTempReady)
    return FUNC_INVALID_PARAM;  
     
  if(StatusReadConvertT)
  {
    HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_RESET);
    TIMDelay(TIME_INIT_READ_SLOT);
    HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_SET);
    TIMDelay(TIME_READ_SLOT);
    *pTempReady = HAL_GPIO_ReadPin(DS18B20Dev.pGPIORXPort, DS18B20Dev.GPIORXNum);    
    TIMDelay(TIME_READ_SLOT_PART);     
  }else
    return FUNC_ERROR;  
  
  return FUNC_OK;
}

/*������ �������� ����������� ������ � �����������*/
uint32_t DeviceDS18B20ConversionTemperature()
{    
  uint32_t Res;  
  uint8_t Cmd;
  uint8_t ReadyTemp;
  ExtCodeDeviceDS18B20 ExCode;   
  
  if(StatusReadConvertT && !DeviceDS18B20GetReadyTemperature(&ReadyTemp))
    return FUNC_ERROR;
      
  
  Res = prvDeviceDS18B20InitSequence(&ExCode);
  
  if(Res != FUNC_OK)
    return Res;

  Cmd = SKIP_ROM;
  Res = prvDeviceDS18B20WriteBytes(&Cmd, 1);
  
  Cmd = CONVERT_T;
  Res = prvDeviceDS18B20WriteBytes(&Cmd, 1);
  
  if(Res != FUNC_OK)
    return Res;

  StatusReadConvertT = TRUE;  
  return FUNC_OK;
}

/*������ ����������� � �������*/
uint32_t DeviceDS18B20GetTemperature(int8_t* pTemp, ExtCodeDeviceDS18B20* pExCode)
{
  /*�������� ������� ������*/
  if(!pExCode || !pTemp)
    return FUNC_INVALID_PARAM;  
    
  uint32_t Res;    
  uint8_t ArrayScratchPad[DS18B20_SCRATCHPAD_MEM_SIZE];
  
  *pExCode = DEVICE_DS18B20_NOT_CODE;    
  StatusReadConvertT = FALSE;
        
  Res = prvDeviceDS18B20InitSequence(pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  /*������ ������ � ������ �������*/  
  Res = prvDeviceDS18B20ReadScratchPad(ArrayScratchPad);
  
  if(Res != FUNC_OK)
    return Res;  
  
  uint32_t crc = prvDeviceDS18B20crc8(ArrayScratchPad, DS18B20_SCRATCHPAD_MEM_SIZE - 1);
  
  if(crc != ArrayScratchPad[DS18B20_SCRATCHPAD_MEM_SIZE - 1])
  {
    *pExCode = DEVICE_DS18B20_ERROR_CRC;
    return FUNC_ERROR;
  }
    
  uint16_t buffVar = (((int16_t)ArrayScratchPad[0])|(((int16_t)(ArrayScratchPad[1]))<<8))>>4;
  *pTemp = (int8_t)(buffVar & 0xFF);

  if(((ArrayScratchPad[0] >> 3)&0x1))
    (*pTemp)++;
      
  return FUNC_OK;
}

/**
@}
*/