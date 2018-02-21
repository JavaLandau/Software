/**
  \file    DeviceAD5370.c 
  \brief   ����������� ���� �������� ��� AD5370
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "DeviceAD5370.h"
#include "TypesDefine.h"
#include "GPIOfun.h"

/**
  \defgroup module_service_AD5370 ��������� ������� ��� ������ � AD5370
  \brief ������ ��������� �������, ����������� ��� ������ � ����������� ��� AD5370
@{
*/

#define OS_WAIT_FOR_RESET               1               ///<����� �������� ��� ������������ �������� Reset

#define LENGTH_PACKET                   3               ///<����� �������

#define OS_TIME_WAIT_SPI                100             ///<������������ ����� �������� ��������� ��������/������ �� SPI

#define NUM_DAC_GROUPS                  5               ///<���������� ����� ������� ���
#define NUM_DAC_CHANNELS                8               ///<���������� ������� � ������

/** @name ��������� �������� ����� ������� � ���
*/                        
///@{
#define MODE_SPECIAL                    0x00            ///<����������� �����, ������������ ������� ������� �������
#define MODE_WRITE_X_REG                0xC0            ///<������ � ������� X1A ��� X1B
#define MODE_WRITE_C_REG                0x80            ///<������ � ������� C
#define MODE_WRITE_M_REG                0x40            ///<������ � ������� M
///@}

/** @name ��������� ��������� ������� ��� ������ ������
*/                        
///@{
#define CHANNEL_0                       0x00            ///<����� 1
#define CHANNEL_1                       0x01            ///<����� 2
#define CHANNEL_2                       0x02            ///<����� 3
#define CHANNEL_3                       0x03            ///<����� 4
#define CHANNEL_4                       0x04            ///<����� 5
#define CHANNEL_5                       0x05            ///<����� 6
#define CHANNEL_6                       0x06            ///<����� 7
#define CHANNEL_7                       0x07            ///<����� 8
///@}

/** @name ��������� ��������� ����� ������� ���
*/                        
///@{
#define GROUP_0                         0x08            ///<������ 1
#define GROUP_1                         0x10            ///<������ 2
#define GROUP_2                         0x18            ///<������ 3
#define GROUP_3                         0x20            ///<������ 4
#define GROUP_4                         0x28            ///<������ 5
#define ALL_GROUPS                      0x30            ///<��� ������
#define GROUP_1234                      0x38            ///<������ � 1-� �� 4-�
///@}

/** @name ��������� ��������� ����� ������� � ������� � ������ ���
*/                        
///@{
#define ALL_GROUPS_ALL_CHANNELS         CHANNEL_0       ///<��� ������ � ������
#define GROUP_0_ALL_CHANNELS            CHANNEL_1       ///<��� ������ ����� 1
#define GROUP_1_ALL_CHANNELS            CHANNEL_2       ///<��� ������ ����� 2
#define GROUP_2_ALL_CHANNELS            CHANNEL_3       ///<��� ������ ����� 3
#define GROUP_3_ALL_CHANNELS            CHANNEL_4       ///<��� ������ ����� 4
#define GROUP_4_ALL_CHANNELS            CHANNEL_5       ///<��� ������ ����� 5
///@}

///��������� ������ � ������ Readback
#define READBACK_CHANNEL(CHANNEL)       ((0x8 + (CHANNEL)) << 0x7)

///����� �������� ���������� �������� ����������� ���
#define SPI_TIME_WAITING                100

///������������ ����������� �����, �������� ����� ������ ��� AD5370 � ������ @ref MODE_SPECIAL
typedef enum _DeviceAD5370SpecialCode {
  SPEC_NOP                        = 0x0,                ///<������ ��������
  SPEC_CTRL_REG                   = 0x1,                ///<������ � ������� Control
  SPEC_OFS0_REG                   = 0x2,                ///<������ � ������� OFS0
  SPEC_OFS1_REG                   = 0x3,                ///<������ � ������� OFS1
  SPEC_READBACK                   = 0x5,                ///<����� �������� ��� ������
  SPEC_SELECT_AB_REG_0            = 0x6,                ///<������ ������ � ������� \f$\overline{A}\backslash B\f$ Select 0
  SPEC_SELECT_AB_REG_1            = 0x7,                ///<������ ������ � ������� \f$\overline{A}\backslash B\f$ Select 1
  SPEC_SELECT_AB_REG_2            = 0x8,                ///<������ ������ � ������� \f$\overline{A}\backslash B\f$ Select 2
  SPEC_SELECT_AB_REG_3            = 0x9,                ///<������ ������ � ������� \f$\overline{A}\backslash B\f$ Select 3
  SPEC_SELECT_AB_REG_4            = 0xA,                ///<������ ������ � ������� \f$\overline{A}\backslash B\f$ Select 4
  SPEC_SELECT_AB_BLOCK_REG        = 0xB                 ///<������� ������ ������ � �������� \f$\overline{A}\backslash B\f$ Select 
} DeviceAD5370SpecialCode;


///������������ ����� ������ ��������� ��� AD5370 ��� ������ � ������ @ref SPEC_READBACK
typedef enum _DeviceAD5370ReadbackRegisters {
  READBACK_X1A_REG                = 0x0000,             ///<������ �������� X1A
  READBACK_X1B_REG                = 0x2000,             ///<������ �������� X1B
  READBACK_C_REG                  = 0x4000,             ///<������ �������� C
  READBACK_M_REG                  = 0x6000,             ///<������ �������� M
  READBACK_CTRL_REG               = 0x8080,             ///<������ �������� Control
  READBACK_OFS0_REG               = 0x8100,             ///<������ �������� OFS0
  READBACK_OFS1_REG               = 0x8180,             ///<������ �������� OFS1
  READBACK_SELECT_AB_0_REG        = 0x8300,             ///<������ �������� \f$\overline{A}\backslash B\f$ Select 0
  READBACK_SELECT_AB_1_REG        = 0x8380,             ///<������ �������� \f$\overline{A}\backslash B\f$ Select 1        
  READBACK_SELECT_AB_2_REG        = 0x8400,             ///<������ �������� \f$\overline{A}\backslash B\f$ Select 2
  READBACK_SELECT_AB_3_REG        = 0x8480,             ///<������ �������� \f$\overline{A}\backslash B\f$ Select 3
  READBACK_SELECT_AB_4_REG        = 0x8500              ///<������ �������� \f$\overline{A}\backslash B\f$ Select 4
} DeviceAD5370ReadbackRegisters;

///�������� � ��������� ����������� ��� ���������� �������� ����������� ���
#define SPI_WAITING() for(uint8_t SPIIncrement = 0; SPIIncrement < SPI_TIME_WAITING; SPIIncrement++) asm("NOP");

/**��������� ���� ������ �������� ��� ������
  \param[in] DevSpecCode ��� ������ ������ ���
  \return ��� ������ �������� ��� ������
*/ 
static DeviceAD5370ReadbackRegisters prvGetCodeReadbackRegister(DeviceAD5370SpecialCode DevSpecCode);

/**������ � ������� ���
  \param[in] pDev ��������� �� ��������� ��������
  \param[in] WriteReg ������ ��� ������ � �������
  \param[in] DevSpecCode ��� ������ ������ ��� ��� ������ �������� ��� ������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
static uint32_t prvDeviceAD5370WriteRegister(DeviceAD5370* pDev, uint16_t WriteReg, 
                                             DeviceAD5370SpecialCode DevSpecCode, ExtCodeDeviceAD5370* pExCode);

/**������ �������� ���
  \param[in] pDev ��������� �� ��������� ��������
  \param[in] DevReg ��� ������ �������� ��� ������
  \param[in] NumChannel ��� ������ ������ ������ ��� ������
  \param[in] pReadData ����������� ������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
static uint32_t prvDeviceAD5370ReadRegister(DeviceAD5370* pDev, DeviceAD5370ReadbackRegisters DevReg, 
                                            uint8_t NumChannel, uint16_t* pReadData, ExtCodeDeviceAD5370* pExCode);

/**����� ���� ������� ��� X2A ��� X2B ���������
  \param[in] pDev ��������� �� ��������� ��������
  \param[in] SelectAB ����� �������� X2A ��� X2B
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
static uint32_t prvDeviceAD5370BlockSelectAB(DeviceAD5370* pDev, uint8_t SelectAB, ExtCodeDeviceAD5370* pExCode);

/**��������� ���������� ��� �� ���������
  \param[in] pDev ��������� �� ��������� ��������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
static uint32_t prvDeviceAD5370DefParams(DeviceAD5370* pDev, ExtCodeDeviceAD5370* pExCode);

/**��������� �������� ��������� ������ ���
  \param[in] pDev ��������� �� ��������� ��������
  \param[in] Addr ����� ������ ���
  \param[in] InitCode �������� � ������ ���
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
static uint32_t prvDeviceAD5370SetDACChannel(DeviceAD5370* pDev, uint16_t Addr, uint16_t InitCode, ExtCodeDeviceAD5370* pExCode);

/**
@}
  \defgroup module_AD5370 ������������ ������� ��� ������ � AD5370
  \brief ������, ��������������� ������������ ����������� ���������� ��� ������ � ����������� ��� AD5370
@{
*/

/*������������� �������� ���*/ 
uint32_t DeviceAD5370Create(DeviceAD5370Param* pDevParam, DeviceAD5370* pDev, ExtCodeDeviceAD5370* pExCode)
{  
  if(!pDevParam || !pDev || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode = DEVICE_AD5370_NOT_CODE;
    
  if(!pDevParam->GPIOBusyPin || !pDevParam->GPIOCLRPin || !pDevParam->GPIOLDACPin ||
     !pDevParam->GPIOResetPin || !pDevParam->GPIOSyncPin)
  {
    return FUNC_INVALID_PARAM;
  }
  
  if(!pDevParam->pInstanceSPIDrv)
    return FUNC_INVALID_PARAM;
  
  uint32_t Res;  
  
  /*������������� SPI*/
  pDev->SPIDrv.State = HAL_SPI_STATE_RESET;
  pDev->SPIDrv.Instance = pDevParam->pInstanceSPIDrv;
  pDev->SPIDrv.Init.Mode = SPI_MODE_MASTER;
  pDev->SPIDrv.Init.Direction = SPI_DIRECTION_2LINES;
  pDev->SPIDrv.Init.DataSize = SPI_DATASIZE_8BIT;
  pDev->SPIDrv.Init.CLKPolarity = SPI_POLARITY_LOW;
  pDev->SPIDrv.Init.CLKPhase = SPI_PHASE_2EDGE;
  pDev->SPIDrv.Init.NSS = SPI_NSS_SOFT;
  pDev->SPIDrv.Init.BaudRatePrescaler = pDevParam->BaudRateSPIPrescaler;
  pDev->SPIDrv.Init.FirstBit = SPI_FIRSTBIT_MSB;
  pDev->SPIDrv.Init.TIMode = SPI_TIMODE_DISABLE;
  pDev->SPIDrv.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  pDev->SPIDrv.Init.CRCPolynomial = 7;
  pDev->SPIDrv.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  pDev->SPIDrv.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&((pDev)->SPIDrv)) != HAL_OK)  
  {
    *pExCode = DEVICE_AD5370_ERROR_INIT_SPI;
    return FUNC_ERROR;
  }
  
  pDev->pGPIOBusyPort = NULL;
  pDev->pGPIOCLRPort = NULL;
  pDev->pGPIOLDACPort = NULL;
  pDev->pGPIOResetPort = NULL;
  pDev->pGPIOSyncPort = NULL;
    
  /*������������� GPIO*/
  /*Pin SYNC*/
  GPIO_InitTypeDef GPIO_InitStruct;
  
  Res = GPIOGetPort(pDevParam->GPIOSyncPin,&pDev->pGPIOSyncPort,&pDev->GPIOSyncNum);
  
  if(Res != FUNC_OK)
  {
    *pExCode = DEVICE_AD5370_ERROR_INIT_GPIO;
    return FUNC_ERROR;    
  }
    
  HAL_GPIO_WritePin(pDev->pGPIOSyncPort, pDev->GPIOSyncNum, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = pDev->GPIOSyncNum;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(pDev->pGPIOSyncPort, &GPIO_InitStruct);  
  
  /*Pin RESET*/
  Res = GPIOGetPort(pDevParam->GPIOResetPin,&pDev->pGPIOResetPort,&pDev->GPIOResetNum);
  
  if(Res != FUNC_OK)
  {
    *pExCode = DEVICE_AD5370_ERROR_INIT_GPIO;
    return FUNC_ERROR;    
  }
  
  HAL_GPIO_WritePin(pDev->pGPIOResetPort, pDev->GPIOResetNum, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = pDev->GPIOResetNum;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(pDev->pGPIOResetPort, &GPIO_InitStruct);    
  
  /*Pin LDAC*/
  Res = GPIOGetPort(pDevParam->GPIOLDACPin,&pDev->pGPIOLDACPort,&pDev->GPIOLDACNum);
  
  if(Res != FUNC_OK)
  {
    *pExCode = DEVICE_AD5370_ERROR_INIT_GPIO;
    return FUNC_ERROR;    
  }
  
  HAL_GPIO_WritePin(pDev->pGPIOLDACPort, pDev->GPIOLDACNum, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = pDev->GPIOLDACNum;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(pDev->pGPIOLDACPort, &GPIO_InitStruct);    
  
  /*Pin CLR*/
  Res = GPIOGetPort(pDevParam->GPIOCLRPin,&pDev->pGPIOCLRPort,&pDev->GPIOCLRNum);
  
  if(Res != FUNC_OK)
  {
    *pExCode = DEVICE_AD5370_ERROR_INIT_GPIO;
    return FUNC_ERROR;    
  }
  
  HAL_GPIO_WritePin(pDev->pGPIOCLRPort, pDev->GPIOCLRNum, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = pDev->GPIOCLRNum;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(pDev->pGPIOCLRPort, &GPIO_InitStruct);    
  
  /*Pin BUSY*/
  Res = GPIOGetPort(pDevParam->GPIOBusyPin,&pDev->pGPIOBusyPort,&pDev->GPIOBusyNum);
  
  if(Res != FUNC_OK)
  {
    *pExCode = DEVICE_AD5370_ERROR_INIT_GPIO;
    return FUNC_ERROR;    
  }
  
  GPIO_InitStruct.Pin = pDev->GPIOBusyNum;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(pDev->pGPIOBusyPort, &GPIO_InitStruct);     
        
  Res = DeviceAD5370Reset(pDev);
  
  if(Res != FUNC_OK)
    return Res;
  
  /*��������� ���������� �� ���������*/
  Res = prvDeviceAD5370DefParams(pDev, pExCode);    
    
  return Res;  
}

/*������������ ���*/
uint32_t DeviceAD5370Reset(DeviceAD5370* pDev)
{
  if(!pDev)
    return FUNC_INVALID_PARAM;
  
  if(!pDev->pGPIOResetPort)
    return FUNC_INVALID_PARAM;    
  
  /*������ Reset*/
  HAL_GPIO_WritePin(pDev->pGPIOResetPort, pDev->GPIOResetNum, GPIO_PIN_RESET);
  osDelay(OS_WAIT_FOR_RESET);
  HAL_GPIO_WritePin(pDev->pGPIOResetPort, pDev->GPIOResetNum, GPIO_PIN_SET);
  
  /*�������� ������������ ����������*/                         
  osDelay(OS_WAIT_FOR_RESET);                   
    
  return FUNC_OK;                    
}

/*��������� ���� ������� ���*/ 
uint32_t DeviceAD5370SetAllChannels(DeviceAD5370* pDev, uint16_t* pOutput, ExtCodeDeviceAD5370* pExCode)
{
  /*�������� ������� ����������*/
  if(!pDev || !pOutput || !pExCode)
    return FUNC_INVALID_PARAM;  
  
  uint32_t Res;
  *pExCode = DEVICE_AD5370_NOT_CODE;
  
  /*��������� �������*/
  for(uint8_t i = 0; i < NUM_DAC_GROUPS; i++)
  {
    for(uint8_t j = 0; j < NUM_DAC_CHANNELS; j++)
    {
      Res = prvDeviceAD5370SetDACChannel(pDev, j | ((i + 1) << 0x3), pOutput[NUM_DAC_CHANNELS*i + j], pExCode);
      if(Res != FUNC_OK)  
        return Res;
    }
  }
  
  return FUNC_OK;
}

/*��������� ��������� ������ ���*/
uint32_t DeviceAD5370SetOneChannel(DeviceAD5370* pDev, uint16_t ValueChannel, uint8_t NumChannel, ExtCodeDeviceAD5370* pExCode)
{
  /*�������� ������� ����������*/
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;  

  if(NumChannel > NUM_CHANNELS - 1)
    return FUNC_INVALID_PARAM;  
  
  uint32_t Res;
  *pExCode = DEVICE_AD5370_NOT_CODE;

  /*��������� ������*/
  uint8_t NumDACGroup = NumChannel / NUM_DAC_CHANNELS;
  uint8_t NumDACChannel = NumChannel - NUM_DAC_CHANNELS*NumDACGroup;
  
  Res = prvDeviceAD5370SetDACChannel(pDev, NumDACChannel | ((NumDACGroup + 1) << 0x3), ValueChannel, pExCode);
      
  return Res;
}

/**
@}
*/

/*��������� ���������� ��� �� ���������*/
static uint32_t prvDeviceAD5370DefParams(DeviceAD5370* pDev, ExtCodeDeviceAD5370* pExCode)
{
  /*�������� ������� ����������*/
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;  
  
  uint32_t Res;
  uint16_t WriteReg;
  
  /*������ � ������� Control*/
  WriteReg = SOFT_POWER_UP | TEMP_SHUTDOWN_ENABLE | SELECT_A;
  Res = prvDeviceAD5370WriteRegister(pDev,  WriteReg, SPEC_CTRL_REG, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  /*��������� ���� ������� �� ������� X2A*/
  Res = prvDeviceAD5370BlockSelectAB(pDev, SELECT_A, pExCode);

  if(Res != FUNC_OK)
    return Res;
  
  /*��������� �������� Offset*/
  WriteReg = 0x0;
  Res = prvDeviceAD5370WriteRegister(pDev,  WriteReg, SPEC_OFS0_REG, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceAD5370WriteRegister(pDev,  WriteReg, SPEC_OFS1_REG, pExCode);
  
  if(Res != FUNC_OK)
    return Res;    
  
  /*������� ���� ������� ���*/
  uint16_t DACChannels[NUM_CHANNELS];  
  memset(DACChannels, 0, NUM_CHANNELS*sizeof(uint16_t));
  
  Res = DeviceAD5370SetAllChannels(pDev, DACChannels, pExCode);
  
  return Res;
}

/*��������� �������� ��������� ������ ���*/
static uint32_t prvDeviceAD5370SetDACChannel(DeviceAD5370* pDev, uint16_t Addr, uint16_t InitCode, ExtCodeDeviceAD5370* pExCode)
{
  /*�������� ������� ����������*/
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;  
  
  *pExCode = DEVICE_AD5370_NOT_CODE;
  
  /*��������� ������*/
  HAL_StatusTypeDef HALRes;
  uint8_t WriteData[LENGTH_PACKET] = {0x0, 0x0, 0x0};
  WriteData[0] = MODE_WRITE_X_REG | Addr;
  WriteData[1] = (uint8_t)((InitCode >> 0x8) & 0xFF);
  WriteData[2] = (uint8_t)(InitCode & 0xFF);
  
  HAL_GPIO_WritePin(pDev->pGPIOSyncPort, pDev->GPIOSyncNum, GPIO_PIN_RESET);
  HALRes = HAL_SPI_Transmit(&pDev->SPIDrv, WriteData, LENGTH_PACKET, OS_TIME_WAIT_SPI);
  HAL_GPIO_WritePin(pDev->pGPIOSyncPort, pDev->GPIOSyncNum, GPIO_PIN_SET);
   
  if(HALRes != HAL_OK)
  {
    *pExCode = DEVICE_AD5370_ERROR_PROCESS_SPI;
    return FUNC_ERROR;
  }      
  
  /*�������� ���������� �������� ����������� ���*/
  while(HAL_GPIO_ReadPin(pDev->pGPIOBusyPort, pDev->GPIOBusyNum) == GPIO_PIN_RESET);
  
  /*��������� ������� ��� �� �����*/
  HAL_GPIO_WritePin(pDev->pGPIOLDACPort, pDev->GPIOLDACNum, GPIO_PIN_RESET);
  
  SPI_WAITING();
    
  HAL_GPIO_WritePin(pDev->pGPIOLDACPort, pDev->GPIOLDACNum, GPIO_PIN_SET);
  
  return FUNC_OK;
}

/*����� ���� ������� ��� X2A ��� X2B ���������*/
static uint32_t prvDeviceAD5370BlockSelectAB(DeviceAD5370* pDev, uint8_t SelectAB, ExtCodeDeviceAD5370* pExCode)     
{
  /*�������� ������� ����������*/
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;  
  
  *pExCode = DEVICE_AD5370_NOT_CODE;
  
  if(SelectAB != SELECT_A && SelectAB != SELECT_B)
    return FUNC_INVALID_PARAM;  
    
  /*������������ � �������� ������*/
  uint32_t Res; 
  HAL_StatusTypeDef HALRes;
  uint8_t WriteData[LENGTH_PACKET] = {0x0, 0x0, 0x0};
  WriteData[0] = MODE_SPECIAL | SPEC_SELECT_AB_BLOCK_REG;
  
  if(SelectAB == SELECT_B)
    WriteData[2] = 0xFF;
    
  HAL_GPIO_WritePin(pDev->pGPIOSyncPort, pDev->GPIOSyncNum, GPIO_PIN_RESET);
  HALRes = HAL_SPI_Transmit(&pDev->SPIDrv, WriteData, LENGTH_PACKET, OS_TIME_WAIT_SPI);
  HAL_GPIO_WritePin(pDev->pGPIOSyncPort, pDev->GPIOSyncNum, GPIO_PIN_SET);
   
  if(HALRes != HAL_OK)
  {
    *pExCode = DEVICE_AD5370_ERROR_PROCESS_SPI;
    return FUNC_ERROR;
  }
    
  /*����������� ���������� ������*/    
  uint16_t ReadReg[NUM_DAC_GROUPS] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
  Res = prvDeviceAD5370ReadRegister(pDev, READBACK_SELECT_AB_0_REG, NULL, &ReadReg[0], pExCode);
    
  if(Res != FUNC_OK)
    return Res;
  
  Res = prvDeviceAD5370ReadRegister(pDev, READBACK_SELECT_AB_1_REG, NULL, &ReadReg[1], pExCode);
    
  if(Res != FUNC_OK)
    return Res; 
  
  Res = prvDeviceAD5370ReadRegister(pDev, READBACK_SELECT_AB_2_REG, NULL, &ReadReg[2], pExCode);
    
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceAD5370ReadRegister(pDev, READBACK_SELECT_AB_3_REG, NULL, &ReadReg[3], pExCode);
    
  if(Res != FUNC_OK)
    return Res;    
 
  Res = prvDeviceAD5370ReadRegister(pDev, READBACK_SELECT_AB_4_REG, NULL, &ReadReg[4], pExCode);
    
  if(Res != FUNC_OK)
    return Res;    
  
  if(SelectAB == SELECT_A)
  {
    if(ReadReg[0] != 0x00 || ReadReg[1] != 0x00 || ReadReg[2] != 0x00 ||
       ReadReg[3] != 0x00 || ReadReg[4] != 0x00)
    {
      *pExCode = DEVICE_AD5370_ERROR_WRITE_SPI; 
      return FUNC_ERROR;
    }
  }else
  {
    if(ReadReg[0] != 0xFF || ReadReg[1] != 0xFF || ReadReg[2] != 0xFF ||
       ReadReg[3] != 0xFF || ReadReg[4] != 0xFF)
    {
      *pExCode = DEVICE_AD5370_ERROR_WRITE_SPI; 
      return FUNC_ERROR;
    }    
  }
  return FUNC_OK;  
}

/*������ �������� ���*/
static uint32_t prvDeviceAD5370ReadRegister(DeviceAD5370* pDev, DeviceAD5370ReadbackRegisters DevReg, 
                                            uint8_t NumChannel, uint16_t* pReadData, ExtCodeDeviceAD5370* pExCode)
{
  /*�������� ������� ����������*/
  if(!pDev || !pExCode || !pReadData)
    return FUNC_INVALID_PARAM;
  
  *pExCode = DEVICE_AD5370_NOT_CODE;
  
  if(NumChannel > NUM_CHANNELS - 1)
    return FUNC_INVALID_PARAM;
    
  /*������������ � �������� ������*/
  HAL_StatusTypeDef Res;
  uint8_t WriteData[LENGTH_PACKET] = {0x0, 0x0, 0x0};
  uint16_t ReadbackRegister;
  
  WriteData[0] = MODE_SPECIAL | ((uint8_t)SPEC_READBACK);
  
  if(DevReg == READBACK_X1A_REG || DevReg == READBACK_X1B_REG ||
     DevReg == READBACK_C_REG   || DevReg == READBACK_M_REG)
  {
    ReadbackRegister = (uint16_t)DevReg | READBACK_CHANNEL(NumChannel);
  }else
  {
    ReadbackRegister = (uint16_t)DevReg;
  }
  
  WriteData[1] = (uint8_t)((ReadbackRegister >> 0x8) & 0xFF);
  WriteData[2] = (uint8_t)(ReadbackRegister & 0xFF);
  
  HAL_GPIO_WritePin(pDev->pGPIOSyncPort, pDev->GPIOSyncNum, GPIO_PIN_RESET);
  Res = HAL_SPI_Transmit(&pDev->SPIDrv, WriteData, LENGTH_PACKET, OS_TIME_WAIT_SPI);
  HAL_GPIO_WritePin(pDev->pGPIOSyncPort, pDev->GPIOSyncNum, GPIO_PIN_SET);
  
  SPI_WAITING();
  
  if(Res != HAL_OK)
  {
    *pExCode = DEVICE_AD5370_ERROR_PROCESS_SPI;
    return FUNC_ERROR;
  }
    
  /*������ ���������� ������*/    
  uint8_t ReadData[LENGTH_PACKET] = {0xFF, 0xFF, 0xFF};
  
  WriteData[0] = (uint8_t)SPEC_NOP;
  WriteData[1] = 0x0;
  WriteData[2] = 0x0;
    
  HAL_GPIO_WritePin(pDev->pGPIOSyncPort, pDev->GPIOSyncNum, GPIO_PIN_RESET);
  Res = HAL_SPI_TransmitReceive(&pDev->SPIDrv, WriteData, ReadData, LENGTH_PACKET, OS_TIME_WAIT_SPI);  
  HAL_GPIO_WritePin(pDev->pGPIOSyncPort, pDev->GPIOSyncNum, GPIO_PIN_SET);
    
  *pReadData = ((uint16_t)ReadData[2])|((uint16_t)ReadData[1] << 0x8);
  return FUNC_OK;      
}

/*������ � ������� ���*/
static uint32_t prvDeviceAD5370WriteRegister(DeviceAD5370* pDev, uint16_t WriteReg, 
                                             DeviceAD5370SpecialCode DevSpecCode, ExtCodeDeviceAD5370* pExCode)
{
  /*�������� ������� ����������*/
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode = DEVICE_AD5370_NOT_CODE;
  
  if(DevSpecCode == SPEC_READBACK || DevSpecCode == SPEC_NOP ||
     DevSpecCode == SPEC_SELECT_AB_BLOCK_REG)
  {
    return FUNC_INVALID_PARAM;
  }
    
  if(DevSpecCode == SPEC_OFS0_REG || DevSpecCode == SPEC_OFS1_REG)
    HAL_GPIO_WritePin(pDev->pGPIOCLRPort, pDev->GPIOCLRNum, GPIO_PIN_RESET);
      
  /*������������ � �������� ������*/
  uint32_t Res; 
  HAL_StatusTypeDef HALRes;
  uint8_t WriteData[LENGTH_PACKET] = {0x0, 0x0, 0x0};
  WriteData[0] = MODE_SPECIAL | ((uint8_t)DevSpecCode);
  WriteData[1] = (uint8_t)((WriteReg >> 0x8) & 0xFF);
  WriteData[2] = (uint8_t)(WriteReg & 0xFF);
    
  HAL_GPIO_WritePin(pDev->pGPIOSyncPort, pDev->GPIOSyncNum, GPIO_PIN_RESET);
  HALRes = HAL_SPI_Transmit(&pDev->SPIDrv, WriteData, LENGTH_PACKET, OS_TIME_WAIT_SPI);
  HAL_GPIO_WritePin(pDev->pGPIOSyncPort, pDev->GPIOSyncNum, GPIO_PIN_SET);
   
  if(HALRes != HAL_OK)
  {
    *pExCode = DEVICE_AD5370_ERROR_PROCESS_SPI;
    return FUNC_ERROR;
  }
    
  if(DevSpecCode == SPEC_OFS0_REG || DevSpecCode == SPEC_OFS1_REG)
    HAL_GPIO_WritePin(pDev->pGPIOCLRPort, pDev->GPIOCLRNum, GPIO_PIN_SET);
    
  /*����������� ���������� ������*/    
  DeviceAD5370ReadbackRegisters DevReg;
  DevReg = prvGetCodeReadbackRegister(DevSpecCode);
    
  uint16_t ReadReg = 0xFFFF;
  Res = prvDeviceAD5370ReadRegister(pDev, DevReg, NULL, &ReadReg, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  if(ReadReg != WriteReg)
  {
    *pExCode = DEVICE_AD5370_ERROR_WRITE_SPI;
    return FUNC_ERROR;
  }
  
  return FUNC_OK;
}

/*��������� ���� ������ �������� ��� ������*/
static DeviceAD5370ReadbackRegisters prvGetCodeReadbackRegister(DeviceAD5370SpecialCode DevSpecCode)
{
  switch(DevSpecCode)
  {
  case SPEC_CTRL_REG:
    return READBACK_CTRL_REG;
  case SPEC_OFS0_REG:
    return READBACK_OFS0_REG;
  case SPEC_OFS1_REG:
    return READBACK_OFS1_REG;
  case SPEC_SELECT_AB_REG_0:
    return READBACK_SELECT_AB_0_REG; 
  case SPEC_SELECT_AB_REG_1:
    return READBACK_SELECT_AB_1_REG;     
  case SPEC_SELECT_AB_REG_2:
    return READBACK_SELECT_AB_2_REG;     
  case SPEC_SELECT_AB_REG_3:
    return READBACK_SELECT_AB_3_REG;     
  case SPEC_SELECT_AB_REG_4:
    return READBACK_SELECT_AB_4_REG;     
  default:
    return READBACK_CTRL_REG;
  }
}