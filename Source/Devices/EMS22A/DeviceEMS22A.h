/**
  \file    DeviceEMS22A.h 
  \brief   ������������ ���� �������� ����������� �������� EMS22A
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __DEVICE_EMS22A__
#define __DEVICE_EMS22A__

#include "TypesDefine.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

/**
  \addtogroup module_EMS22A
@{
*/

///��������� �������� ��������
typedef struct _DeviceEMS22A {
  SPI_HandleTypeDef SPIDrv;                     ///<��������� �� ��������� �������� SPI
} DeviceEMS22A;

///��������� ���������� ������������� �������� ��������
typedef struct _DeviceEMS22AParam {
  SPI_TypeDef*          pInstanceSPIDrv;        ///<��������� �� ��������� �������� SPI          
  uint32_t              BaudRateSPIPrescaler;   ///<�������� ������ SPI 
} DeviceEMS22AParam;

///����������� ���� ������
typedef enum _ExtCodeDeviceEMS22A {
  DEVICE_EMS22A_NOT_CODE = 0,                   ///<������ ���
  DEVICE_EMS22A_ERROR_INIT_SPI,                 ///<������ ������������� SPI
  DEVICE_EMS22A_ERROR_WRITE_SPI,                ///<������ ������ ������ �� SPI
  DEVICE_EMS22A_ERROR_PROCESS_SPI,              ///<������ � �������� ������ � SPI
  DEVICE_EMS22A_ERROR_CRC                       ///<������ ����������� ������
} ExtCodeDeviceEMS22A;

/**������������� �������� ��������
  \param[in] pDevParam ��������� �� ��������� ���������� ��� ������������� ��������
  \param[in] pDev ��������� �� ��������� ��������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
uint32_t DeviceEMS22ACreate(DeviceEMS22AParam* pDevParam, DeviceEMS22A* pDev, ExtCodeDeviceEMS22A* pExCode);

/**������ �������� ���� ��������
  \param[in] pDev ��������� �� ��������� ��������
  \param[in] pAng ���� ��������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
uint32_t DeviceEMS22AGetAngular(DeviceEMS22A* pDev, float* pAng, ExtCodeDeviceEMS22A* pExCode);
    
/**������ ������ � ��������
  \param[in] pDev ��������� �� ��������� ��������
  \param[in] pCode ����������� ������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
uint32_t DeviceEMS22AGetCode(DeviceEMS22A* pDev, uint16_t* pCode, ExtCodeDeviceEMS22A* pExCode);

/**
@}
*/

#endif