/**
\file
  \brief ������������ ���� ����������� ������ GPIO PCA9554
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017  
*/

#ifndef __DEVICE_PCA9554
#define __DEVICE_PCA9554

#include "stm32f0xx_hal.h"

/**
  \addtogroup module_PCA9554
@{
*/

#define PORT_PIN_INPUT                  0x1	        ///<���� GPIO ���� "����"
#define PORT_PIN_OUTPUT                 0x0             ///<���� GPIO ���� "�����"

#define PORT_PIN_POLARITY_RETAINED      0x0             ///<���� GPIO �����������������
#define PORT_PIN_POLARITY_INVERTED      0x1             ///<���� GPIO ���������������

#define PCA9554_NUM_GPIO                8               ///<����� ������ GPIO � PCA9554

///��������� ��������
typedef struct _DevicePCA9554 {
  I2C_HandleTypeDef I2CDrv;                             ///<��������� �� ��������� �������� \f$I^2C\f$
  
  GPIO_TypeDef*     pGPIOINTPort;                       ///<��������� �� ��������� GPIO ��� ������� ���������� INT
  uint16_t          GPIOINTNum;                         ///<��������� �� ����� GPIO ��� ������� ���������� INT
  uint8_t           DevAddress;                         ///<����� \f$I^2C\f$ ����������
  uint8_t           ConfRegister;                       ///<����� ����������������� �������� PCA9554
  uint8_t           OutputPortRegister;                 ///<����� �������� ������� PCA9554   
} DevicePCA9554;

///��������� ���������� ������������� ��������
typedef struct _DevicePCA9554Param {
  I2C_TypeDef*      pInstanceI2CDrv;                    ///<��������� �� ��������� �������� \f$I^2C\f$     
  uint8_t           AddrDevice;                         ///<������� ����� \f$I^2C\f$ ����������
  char*             GPIOINTPin;                         ///<������ �������� ������ ���� ����������������, ������������� ��� ������� INT
} DevicePCA9554Param;

///����������� ���� ������
typedef enum _ExtCodeDevicePCA9554 {
  DEVICE_PCA9554_NOT_CODE = 0,                          ///<������ ���
  DEVICE_PCA9554_ERROR_INIT_I2C,                        ///<������ ������������� \f$I^2C\f$
  DEVICE_PCA9554_ERROR_INIT_GPIO,                       ///<������ ������������� GPIO
  DEVICE_PCA9554_ERROR_PROCESS_I2C,                     ///<������ ��� ������ � \f$I^2C\f$
  DEVICE_PCA9554_ERROR_WRITE_I2C                        ///<������ ������ ������ � ����������
} ExtCodeDevicePCA9554;

/**������������� �������� PCA9554
  \param[in] pDevParam ��������� �� ��������� ���������� ��� ������������� ��������
  \param[out] pDev ��������� �� ��������� ��������
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/
uint32_t DevicePCA9554Create(DevicePCA9554Param* pDevParam, DevicePCA9554* pDev, ExtCodeDevicePCA9554* pExCode);

/**���������� �������� ���� ������ GPIO
  \param[in] pDev ��������� �� ��������� ��������
  \param[out] pPortState ��������� ������ GPIO 
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/
uint32_t DevicePCA9554GetAllInputPorts(DevicePCA9554* pDev, GPIO_PinState* pPortState, ExtCodeDevicePCA9554* pExCode);

/**���������� �������� ���������� ����� GPIO
  \param[in] pDev ��������� �� ��������� ��������
  \param[in] NumPort ����� ����� GPIO 
  \param[out] pPortState ��������� ����� GPIO 
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/
uint32_t DevicePCA9554GetInputPort(DevicePCA9554* pDev, uint8_t NumPort, GPIO_PinState* pPortState, ExtCodeDevicePCA9554* pExCode);

/**��������� �������� ���� ������� GPIO
  \param[in] pDev ��������� �� ��������� ��������
  \param[in] pPortState �������� ������� GPIO 
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/
uint32_t DevicePCA9554SetAllOutputPorts(DevicePCA9554* pDev, GPIO_PinState* pPortState, ExtCodeDevicePCA9554* pExCode);

/**��������� �������� ���������� ������ GPIO
  \param[in] pDev ��������� �� ��������� ��������
  \param[in] NumPort ����� ����� GPIO 
  \param[in] PortState �������� ������ GPIO 
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/
uint32_t DevicePCA9554SetOutputPort(DevicePCA9554* pDev, uint8_t NumPort, GPIO_PinState PortState, ExtCodeDevicePCA9554* pExCode);

/**���������������� ���� ������ GPIO
  \param[in] pDev ��������� �� ��������� ��������
  \param[in] pPortDirection ����������� ������ GPIO 
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/
uint32_t DevicePCA9554SetConfRegister(DevicePCA9554* pDev, uint8_t* pPortDirection, ExtCodeDevicePCA9554* pExCode);


/**���������������� ��������� ������ GPIO
  \param[in] pDev ��������� �� ��������� ��������
  \param[in] NumPort ����� ����� GPIO
  \param[in] PortDirection ����������� ����� GPIO
  \param[out] pExCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/
uint32_t DevicePCA9554SetConfPort(DevicePCA9554* pDev, uint8_t NumPort, uint8_t PortDirection, ExtCodeDevicePCA9554* pExCode);

/**
@}
*/

#endif