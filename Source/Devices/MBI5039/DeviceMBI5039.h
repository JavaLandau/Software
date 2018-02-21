/**
  \file    DeviceMBI5039.h 
  \brief   ������������ ���� �������� ����������� MBI5039
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __DEVICE_MBI5039
#define __DEVICE_MBI5039

#include "stm32f0xx_hal.h"
#include "TypesDefine.h"

/**
  \addtogroup module_MBI5039
@{
*/

///��������� �������� �����������
typedef struct _DeviceMBI5039 {
  GPIO_TypeDef*     pGPIONSSPort;               ///<��������� �� ��������� GPIO ��� ������� NSS
  uint16_t          GPIONSSNum;                 ///<����� GPIO ��� ������� NSS
  SPI_HandleTypeDef SPIDrv;                     ///<��������� �� ��������� �������� SPI
  uint8_t           devNum;                     ///<����� ��������������� ������������ ���������
} DeviceMBI5039;

///��������� ���������� ������������� �������� �����������
typedef struct _DeviceMBI5039Param {    
  SPI_TypeDef*      pInstanceSPIDrv;            ///<��������� �� ��������� �������� SPI          
  uint32_t          BaudRateSPIPrescaler;       ///<�������� ������ SPI 
  char*             GPIONSSPin;                 ///<������ �������� ������ ���� ���������������� ��� ������� NSS
  uint8_t           devNum;                     ///<����� ��������������� ������������ ���������
} DeviceMBI5039Param;

///����������� ���� ������
typedef enum _ExtCodeDeviceMBI5039 {
  DEVICE_MBI5039_NOT_CODE = 0,                  ///<������ ���
  DEVICE_MBI5039_ERROR_INIT_SPI,                ///<������ ������������� SPI
  DEVICE_MBI5039_ERROR_INIT_GPIO,               ///<������ ������������� GPIO
  DEVICE_MBI5039_ERROR_PROCESS_SPI,             ///<������ � �������� ������ � SPI
  DEVICE_MBI5039_ERROR_WRITE_SPI                ///<������ ������ ������ �� SPI
} ExtCodeDeviceMBI5039;

/**������������� �������� �����������
  \param[in] pDevParam ��������� �� ��������� ���������� ��� ������������� ��������
  \param[in] pDev ��������� �� ��������� ��������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
uint32_t DeviceMBI5039Create(DeviceMBI5039Param* pDevParam, DeviceMBI5039* pDev, ExtCodeDeviceMBI5039* pExCode);

/**��������� ������� �������� 
  \param[in] pDev ��������� �� ��������� ��������
  \param[in] pOutput �������� ������� ��������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
uint32_t DeviceMBI5039Set(DeviceMBI5039* pDev, uint16_t* pOutput, ExtCodeDeviceMBI5039* pExCode);

/**����� ������� �������� 
  \param[in] pDev ��������� �� ��������� ��������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
uint32_t DeviceMBI5039Reset(DeviceMBI5039* pDev,  ExtCodeDeviceMBI5039* pExCode);

/**
@}
*/

#endif