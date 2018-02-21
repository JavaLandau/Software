/**
  \file    DeviceEMS22A.c 
  \brief   ����������� ���� �������� ����������� �������� EMS22A
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#include "DeviceEMS22A.h"

/**
  \defgroup module_service_EMS22A ��������� ������� ��� ������ � EMS22A
  \brief ������ ��������� �������, ����������� ��� ������ � ���������� ��������� EMS22A
@{
*/

///����� �������� ��������� �����/�������� ������ �� SPI
#define EMS22A_TIME_WAIT_SPI            1000                    

///����������� ��������� ����������� ��������� ��� ����
#define EMS22A_FULL_RANGE_CODE          0x400                   

///���������� �������� � ������ �����
#define EMS22A_FULL_ROTATE_DEG          360

#define GET_BIT(VALUE,NUM)              (((VALUE)>>(NUM))&0x1)  ///<�������� ��������� ����
#define ANGULAR_BITS(VALUE)             (((VALUE)>>0x6)&0x3FF)  ///<�������� ����
#define STATUS_BITS(VALUE)              (((VALUE)>>0x1)&0x1F)   ///<�������� ���� �������
#define PARITY_BIT(VALUE)               ((VALUE)&0x1)           ///<�������� ���� ����������� �����

/**�������� ����������� ����� ������ � ��������
  \param[in] Value ������ � �������
  \return ��������� ��������� ����������� ���� (1 - ���������, 0 - �� ���������)
*/ 
static uint8_t prvDeviceEMS22AEventParity(uint16_t Value)
{
  uint8_t ParityValue = 0;
  for(uint8_t i = 0; i < 16; i++)
    ParityValue+=GET_BIT(Value,i);
  
  return !(ParityValue % 2);
}

/**�������� ����� � ���������
  \param[in] pDev ��������� �� ��������� ��������
  \return ��������� ���������� �������
*/ 
static uint32_t prvDeviceEMS22ACheck(DeviceEMS22A* pDev)
{
  /*�������� ������� ������*/
  if(!pDev)
    return FUNC_INVALID_PARAM;  
  
  HAL_StatusTypeDef HALRes;
  uint16_t ReceiveHalfWord = 0xFFFF;
    
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
  HALRes = HAL_SPI_Receive(&pDev->SPIDrv, (uint8_t*)&ReceiveHalfWord, 1, EMS22A_TIME_WAIT_SPI);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
  
  if(HALRes != HAL_OK)    
    return FUNC_ERROR;

  if(!prvDeviceEMS22AEventParity(ReceiveHalfWord))
    return FUNC_ERROR;    
  
  return FUNC_OK;
}

/**
@}
  \defgroup module_EMS22A ������������ ������� ��� ������ � EMS22A
  \brief ������, ��������������� ������������ ����������� ���������� ��� ������ � ���������� ��������� EMS22A
@{
*/

/*������������� �������� ��������*/
uint32_t DeviceEMS22ACreate(DeviceEMS22AParam* pDevParam, DeviceEMS22A* pDev, ExtCodeDeviceEMS22A* pExCode)
{
  /*�������� ������� ������*/
  if(!pExCode)
    return FUNC_INVALID_PARAM;
    
  *pExCode = DEVICE_EMS22A_NOT_CODE;
  
  if(!pDevParam || !pDev)
    return FUNC_INVALID_PARAM;
    
  /*������������� SPI*/
  pDev->SPIDrv.State = HAL_SPI_STATE_RESET;
  pDev->SPIDrv.Instance = pDevParam->pInstanceSPIDrv;
  pDev->SPIDrv.Init.Mode = SPI_MODE_MASTER;
  pDev->SPIDrv.Init.Direction = SPI_DIRECTION_2LINES;
  pDev->SPIDrv.Init.DataSize = SPI_DATASIZE_16BIT;
  pDev->SPIDrv.Init.CLKPolarity = SPI_POLARITY_LOW;
  pDev->SPIDrv.Init.CLKPhase = SPI_PHASE_2EDGE;
  pDev->SPIDrv.Init.NSS = SPI_NSS_SOFT;
  pDev->SPIDrv.Init.BaudRatePrescaler = pDevParam->BaudRateSPIPrescaler;
  pDev->SPIDrv.Init.FirstBit = SPI_FIRSTBIT_MSB;
  pDev->SPIDrv.Init.TIMode = SPI_TIMODE_DISABLE;
  pDev->SPIDrv.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  pDev->SPIDrv.Init.CRCPolynomial = 10;
  pDev->SPIDrv.pUserData = (void*)pDev;
  if (HAL_SPI_Init(&pDev->SPIDrv) != HAL_OK)
  {
    *pExCode = DEVICE_EMS22A_ERROR_INIT_SPI;
    return FUNC_ERROR;
  }

  /*�������� ����� � ���������*/
  if(prvDeviceEMS22ACheck(pDev) != FUNC_OK)
    return FUNC_ERROR;
  
  return FUNC_OK;  
}

/*������ ������ � ��������*/
uint32_t DeviceEMS22AGetCode(DeviceEMS22A* pDev, uint16_t* pCode, ExtCodeDeviceEMS22A* pExCode)
{
  /*�������� ������� ������*/
  if(!pDev || !pCode || !pExCode)
    return FUNC_INVALID_PARAM;
    
  *pExCode = DEVICE_EMS22A_NOT_CODE;  
  
  HAL_StatusTypeDef HALRes;
  uint16_t ReceiveHalfWord = 0xFFFF;
    
  /*������ ������*/
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
  HALRes = HAL_SPI_Receive(&pDev->SPIDrv, (uint8_t*)&ReceiveHalfWord, 1, EMS22A_TIME_WAIT_SPI);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
  
  if(HALRes != HAL_OK)
  {
    *pExCode = DEVICE_EMS22A_ERROR_PROCESS_SPI;
    return FUNC_ERROR;
  }

  /*�������� CRC*/
  if(!prvDeviceEMS22AEventParity(ReceiveHalfWord))
  {
    *pExCode = DEVICE_EMS22A_ERROR_CRC;
    return FUNC_ERROR;    
  }
  
  *pCode = ANGULAR_BITS(ReceiveHalfWord);
      
  return FUNC_OK;  
}

/*������ �������� ���� ��������*/
uint32_t DeviceEMS22AGetAngular(DeviceEMS22A* pDev, float* pAng, ExtCodeDeviceEMS22A* pExCode)
{
  /*�������� ������� ������*/
  if(!pDev || !pAng || !pExCode)
    return FUNC_INVALID_PARAM;
    
  *pExCode = DEVICE_EMS22A_NOT_CODE;  
  
  uint32_t Res;
  uint16_t EncoderCode;
  
  /*������ ������ � ��������*/
  Res = DeviceEMS22AGetCode(pDev, &EncoderCode, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  /*���������� ���� ��������*/
  *pAng = EMS22A_FULL_ROTATE_DEG*(float)EncoderCode /(float)EMS22A_FULL_RANGE_CODE;
      
  return FUNC_OK;
}

/**
@}
*/