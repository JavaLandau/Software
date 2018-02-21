/**
  \file    DeviceDS18B20.h
  \brief   ������������ ���� �������� ������� ����������� DS18B20
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __DEVICE_DS18B20
#define __DEVICE_DS18B20

#include "stm32f0xx_hal.h"
#include "TypesDefine.h"

/**
  \addtogroup module_DS18B20
@{
*/

/** @name ������� ��� ������� �����������
*/                        
///@{
#define  SEARCH_ROM          0xF0               ///<����� ���������� �� ROM ����
#define  READ_ROM            0x33               ///<������ ROM ���� ����������
#define  MATCH_ROM           0x55               ///<��������� � ���������� ���������� �� ROM ����
#define  SKIP_ROM            0xCC               ///<��������� � ���������� ��� ROM ����
#define  ALARM_SEARCH        0xEC               ///<��������� ������ ���������
#define  CONVERT_T           0x44               ///<����������� ������ � �����������
#define  READ_SCRATCHPAD     0xBE               ///<������ ����������� ������ ����������
#define  WRITE_SCRATCHPAD    0x4E               ///<������ � ������ ����������
#define  COPY_SCRATCHPAD     0x48               ///<����������� ������ ������ ����������
#define  RECALL_E2           0xB8               ///<������ ������� ��������� ��������� �������� �����������
#define  READ_POWER_SUPPLY   0xB4               ///<������ ������ ������� ����������
///@}

/** @name ����� ���������� ������ ������ �����������
*/                        
///@{
#define TEMP_NOT_READY       0x0                ///<������ �� ������
#define TEMP_READY           0x1                ///<������ ������
///@}

///��������� �������� ������� �����������
typedef struct _DeviceDS18B20 {
  GPIO_TypeDef*     pGPIOTXPort;                ///<��������� �� ��������� GPIO ��� ������� TX
  GPIO_TypeDef*     pGPIORXPort;                ///<��������� �� ��������� GPIO ��� ������� RX
  uint16_t          GPIOTXNum;                  ///<����� GPIO ��� ������� TX
  uint16_t          GPIORXNum;                  ///<����� GPIO ��� ������� RX
} DeviceDS18B20;

///��������� ���������� ������������� ������� �����������
typedef struct _DeviceDS18B20Param {    
  char*             GPIOTXPin;                  ///<������ �������� ������ ���� ���������������� ��� ������� TX
  char*             GPIORXPin;                  ///<������ �������� ������ ���� ���������������� ��� ������� RX
} DeviceDS18B20Param;

///����������� ���� ������
typedef enum _ExtCodeDeviceDS18B20 {
  DEVICE_DS18B20_NOT_CODE = 0,                  ///<������ ���
  DEVICE_DS18B20_ERROR_INIT_GPIO,               ///<������ ������������� GPIO
  DEVICE_DS18B20_ERROR_NOT_RESP,                ///<������ ����������� �� ��������
  DEVICE_DS18B20_ERROR_CRC                      ///<������ ����������� ������
} ExtCodeDeviceDS18B20;

/**������������� �������� ������� �����������
  \param[in] pDevParam ��������� �� ��������� ���������� ��� ������������� ��������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
uint32_t DeviceDS18B20Create(DeviceDS18B20Param* pDevParam, ExtCodeDeviceDS18B20* pExCode);

/**������ ��������� ���������� ������ ������ � �����������
  \param[out] pTempReady ���� ����������
  \return ��������� ���������� ������� 
*/ 
uint32_t DeviceDS18B20GetReadyTemperature(uint8_t* pTempReady);

/**������ �������� ����������� ������ � �����������
  \return ��������� ���������� ������� 
*/ 
uint32_t DeviceDS18B20ConversionTemperature();

/**������ ����������� � �������
  \param[out] pTemp ����������� � �������� �������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
uint32_t DeviceDS18B20GetTemperature(int8_t* pTemp, ExtCodeDeviceDS18B20* pExCode);

/**
@}
*/

#endif