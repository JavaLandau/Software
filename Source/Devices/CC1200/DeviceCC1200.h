/**
  \file    DeviceCC1200.h
  \brief   ������������ ���� �������� ���������������� CC1200
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __DEVICE_CC1200__
#define __DEVICE_CC1200__

#include "TypesDefine.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

/**
@}
  \addtogroup module_CC1200
@{
*/

///����� �������� ����� ������ �� ����������
#define CC1200_RECEIVE_WAIT_FOREVER     0                       

///������ ������ ����������������
typedef enum _DeviceCC1200Mode {
  DEVICE_CC1200_IDLE = 0x0,                     ///<��������
  DEVICE_CC1200_SLEEP,                          ///<���
  DEVICE_CC1200_RECEIVE,                        ///<����
  DEVICE_CC1200_TRANSMIT                        ///<��������
} DeviceCC1200Mode;

///��������� �������� ����������������
typedef struct _DeviceCC1200 {
  SPI_HandleTypeDef SPIDrv;                     ///<��������� �� ��������� �������� SPI
  DMA_HandleTypeDef  DMATXDrv;                  ///<��������� �� ��������� �������� SPI DMA TX
  DMA_HandleTypeDef  DMARXDrv;                  ///<��������� �� ��������� �������� SPI DMA RX
  DeviceCC1200Mode   DevMode;                   ///<����� ������ ����������������
  uint8_t            DevAddr;                   ///<����� ���������� � ������������ ����
  
  osSemaphoreId      SemIDSyncDMA;              ///<������� �������� ��������� �����/�������� ������ �� DMA
} DeviceCC1200;

///��������� ���������� ������������� �������� ����������������
typedef struct _DeviceCC1200Param {
  SPI_TypeDef*          pInstanceSPIDrv;        ///<��������� �� ��������� �������� SPI
  uint32_t              BaudRateSPIPrescaler;   ///<�������� ������ SPI 
  DMA_Stream_TypeDef*   pInstanceDMATXDrv;      ///<��������� �� ��������� �������� SPI DMA TX
  DMA_Stream_TypeDef*   pInstanceDMARXDrv;      ///<��������� �� ��������� �������� SPI DMA RX
  uint32_t              DMATXChannel;           ///<����� ������ SPI DMA TX
  uint32_t              DMARXChannel;           ///<����� ������ SPI DMA RX
  uint8_t               DevAddr;                ///<����� ���������� � ������������ ����
} DeviceCC1200Param;

///����������� ���� ������
typedef enum _ExtCodeDeviceCC1200 {
  DEVICE_CC1200_NOT_CODE = 0,                   ///<������ ���
  DEVICE_CC1200_ERROR_TIMEOUT,                  ///<����� �������� �������
  DEVICE_CC1200_ERROR_INIT_SPI,                 ///<������ ������������� SPI
  DEVICE_CC1200_ERROR_INIT_DMA,                 ///<������ ������������� DMA
  DEVICE_CC1200_ERROR_PROCESS_DMA,              ///<������ ��� ������ � DMA
  DEVICE_CC1200_ERROR_WRITE_SPI,                ///<������ ������ ������ �� SPI
  DEVICE_CC1200_ERROR_RX_BUF_TIMEOUT,           ///<����� �������� ����� ������ �������
  DEVICE_CC1200_ERROR_RX_BUF_UNDERFLOW,         ///<����� RX ������
  DEVICE_CC1200_ERROR_RX_BUF_OVERFLOW,          ///<����� RX ����������
  DEVICE_CC1200_ERROR_RX_BUF_ERROR,             ///<������ ��� ������ � ������� RX
  DEVICE_CC1200_ERROR_TX_BUF_UNDERFLOW,         ///<����� TX ������
  DEVICE_CC1200_ERROR_TX_BUF_OVERFLOW,          ///<����� TX ����������
  DEVICE_CC1200_ERROR_TX_BUF_ERROR,             ///<������ ��� ������ � ������� TX
  DEVICE_CC1200_ERROR_RECEIVE                   ///<������ ����� ������
} ExtCodeDeviceCC1200;

/**������������� �������� ����������������
  \param[in] pDevParam ��������� �� ��������� ���������� ��� ������������� ��������
  \param[in] pDev ��������� �� ��������� ��������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
uint32_t DeviceCC1200Create(DeviceCC1200Param* pDevParam, DeviceCC1200* pDev, ExtCodeDeviceCC1200* pExCode);

/**���� ������ ������ �������������� �������
  \param[in] pDev ��������� �� ��������� ��������
  \param[in] FixNumPacket ������ ������ ������
  \param[out] pPacket ����������� ����� ������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \param[in] Waiting ����� �������� ���� ������
  \return ��������� ���������� ������� 
*/ 
uint32_t DeviceCC1200ReceiveFixPacket(DeviceCC1200* pDev, uint8_t FixNumPacket, uint8_t* pPacket, ExtCodeDeviceCC1200* pExCode, uint32_t Waiting);

/**�������� ������ ������ �������������� �������
  \param[in] pDev ��������� �� ��������� ��������
  \param[in] FixNumPacket ������ ������ ������
  \param[out] pPacket ������������ ����� ������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
uint32_t DeviceCC1200TransmitFixPacket(DeviceCC1200* pDev, uint8_t FixNumPacket, uint8_t* pPacket, ExtCodeDeviceCC1200* pExCode);

/**������������ ����������������
  \param[in] pDev ��������� �� ��������� ��������
  \param[out] pExCode ����������� ��� ������ ���������� �������  
  \return ��������� ���������� ������� 
*/
uint32_t DeviceCC1200Reset(DeviceCC1200* pDev, ExtCodeDeviceCC1200* pExCode);

/**������ ��������� ����������������
  \param[in] pDev ��������� �� ��������� ��������
  \param[out] pChipStatusByte ��������� ����������������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
uint32_t DeviceCC1200ReadChipStatus(DeviceCC1200* pDev, uint8_t* pChipStatusByte, ExtCodeDeviceCC1200* pExCode);

/**
@}
*/

#endif