/**
  \file    DeviceAD5370.h 
  \brief   ������������ ���� �������� ��� AD5370
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __DEVICE_AD5370
#define __DEVICE_AD5370

#include "stm32f0xx_hal.h"
#include "cmsis_os.h"

/**
  \addtogroup module_AD5370
@{
*/

#define NUM_CHANNELS            40              ///<����� ���������� �������

#define SELECT_A                0x0             ///<����� �������� X2A ��� ������� ���
#define SELECT_B                0x4             ///<����� �������� X2B ��� ������� ���

#define TEMP_SHUTDOWN_ENABLE    0x2             ///<��������� ���������� ���������� ��� ���������
#define TEMP_SHUTDOWN_DISABLE   0x0             ///<��������� ���������� ���������� ��� ���������

#define SOFT_POWER_DOWN         0x1             ///<��� ��������
#define SOFT_POWER_UP           0x0             ///<��� �������

///������� ���������� ��� ���
typedef enum _DeviceAD5370Vref {
  VREF_3V = 0x0,                                ///<������� ���������� 3 �
  VREF_5V                                       ///<������� ���������� 5 �
} DeviceAD5370Vref;

///��������� �������� ���
typedef struct _DeviceAD5370 {
  SPI_HandleTypeDef SPIDrv;                    ///<��������� �� ��������� �������� SPI
  
  GPIO_TypeDef*     pGPIOSyncPort;             ///<��������� �� ��������� GPIO ��� ������� \f$\overline{SYNC}\f$ 
  GPIO_TypeDef*     pGPIOResetPort;            ///<��������� �� ��������� GPIO ��� ������� \f$\overline{RESET}\f$  
  GPIO_TypeDef*     pGPIOBusyPort;             ///<��������� �� ��������� GPIO ��� ������� \f$\overline{BUSY}\f$ 
  GPIO_TypeDef*     pGPIOLDACPort;             ///<��������� �� ��������� GPIO ��� ������� \f$\overline{LDAC}\f$  
  GPIO_TypeDef*     pGPIOCLRPort;              ///<��������� �� ��������� GPIO ��� ������� \f$\overline{CLR}\f$    
  
  uint16_t          GPIOSyncNum;               ///<����� GPIO ��� ������� \f$\overline{SYNC}\f$  
  uint16_t          GPIOResetNum;              ///<����� GPIO ��� ������� \f$\overline{RESET}\f$   
  uint16_t          GPIOBusyNum;               ///<����� GPIO ��� ������� \f$\overline{BUSY}\f$   
  uint16_t          GPIOLDACNum;               ///<����� GPIO ��� ������� \f$\overline{LDAC}\f$   
  uint16_t          GPIOCLRNum;                ///<����� GPIO ��� ������� \f$\overline{CLR}\f$   
} DeviceAD5370;

///��������� ���������� ������������� �������� ���
typedef struct _DeviceAD5370Param {
  DeviceAD5370Vref  Vref;                      ///<������� ���������� ��� 
  SPI_TypeDef*      pInstanceSPIDrv;           ///<��������� �� ��������� �������� SPI          
  uint32_t          BaudRateSPIPrescaler;      ///<�������� ������ SPI 
  
  char*             GPIOSyncPin;               ///<������ �������� ������ ���� ���������������� ��� ������� \f$\overline{SYNC}\f$  
  char*             GPIOResetPin;              ///<������ �������� ������ ���� ���������������� ��� ������� \f$\overline{RESET}\f$   
  char*             GPIOBusyPin;               ///<������ �������� ������ ���� ���������������� ��� ������� \f$\overline{BUSY}\f$   
  char*             GPIOLDACPin;               ///<������ �������� ������ ���� ���������������� ��� ������� \f$\overline{LDAC}\f$   
  char*             GPIOCLRPin;                ///<������ �������� ������ ���� ���������������� ��� ������� \f$\overline{CLR}\f$   
} DeviceAD5370Param;

///����������� ���� ������
typedef enum _ExtCodeDeviceAD5370 {
  DEVICE_AD5370_NOT_CODE = 0,                   ///<������ ���
  DEVICE_AD5370_ERROR_INIT_SPI,                 ///<������ ������������� SPI
  DEVICE_AD5370_ERROR_INIT_GPIO,                ///<������ ������������� GPIO
  DEVICE_AD5370_ERROR_PROCESS_SPI,              ///<������ � �������� ������ � SPI
  DEVICE_AD5370_ERROR_WRITE_SPI                 ///<������ ������ ������ � ��� �� SPI
} ExtCodeDeviceAD5370;

/**������������� �������� ���
  \param[in] pDevParam ��������� �� ��������� ���������� ��� ������������� ��������
  \param[in] pDev ��������� �� ��������� ��������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
uint32_t DeviceAD5370Create(DeviceAD5370Param* pDevParam, DeviceAD5370* pDev, ExtCodeDeviceAD5370* pExCode);

/**��������� ���� ������� ���
  \param[in] pDev ��������� �� ��������� ��������
  \param[in] pOutput �������� ��� ���� ������� ���
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/ 
uint32_t DeviceAD5370SetAllChannels(DeviceAD5370* pDev, uint16_t* pOutput, ExtCodeDeviceAD5370* pExCode);

/**������������ ���
  \param[in] pDev ��������� �� ��������� ��������
  \return ��������� ���������� ������� 
*/
uint32_t DeviceAD5370Reset(DeviceAD5370* pDev);

/**��������� ��������� ������ ���
  \param[in] pDev ��������� �� ��������� ��������
  \param[in] ValueChannel �������� ������
  \param[in] NumChannel ����� ������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/
uint32_t DeviceAD5370SetOneChannel(DeviceAD5370* pDev, uint16_t ValueChannel, uint8_t NumChannel, ExtCodeDeviceAD5370* pExCode);

/**
@}
*/

#endif