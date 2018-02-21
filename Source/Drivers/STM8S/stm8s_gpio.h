/**
  \file    stm8s_gpio.h
  \brief   ������������ ���� �������� GPIO ���� STM8S
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __STM8S_GPIO__
#define __STM8S_GPIO__

#include "TypesDefine.h"

/**
  \addtogroup module_STM8S_GPIO
@{
*/

///����� ������ GPIO
typedef enum _GPIOMode {
  GPIO_FLOATING_INPUT                = 0x0,             ///<��������� ����
  GPIO_INPUT_WITH_PULLUP             = 0x2,             ///<���� � ��������� � �������
  GPIO_OUTPUT_PSEUDO_OPEN_DRAIN      = 0x1,             ///<����� � ��������� ������ "� �������� ������"
  GPIO_OUTPUT_PUSHPULL               = 0x3              ///<����������� �����
} GPIOMode;

///�������� ������ GPIO
typedef enum _GPIOSpeed {
  GPIO_SPEED_2MHZ                    = 0x0,             ///<2 ���
  GPIO_SPEED_10MHZ                   = 0x1              ///<10 ���
} GPIOSpeed;

///������������ ����� GPIO
typedef enum _GPIOPort {
  GPIO_PORT_A                        = 0x00,            ///<���� A
  GPIO_PORT_B                        = 0x05,            ///<���� B  
  GPIO_PORT_C                        = 0x0A,            ///<���� C
  GPIO_PORT_D                        = 0x0F,            ///<���� D
  GPIO_PORT_E                        = 0x14,            ///<���� E
  GPIO_PORT_F                        = 0x19,            ///<���� F
  GPIO_PORT_G                        = 0x1E,            ///<���� G
  GPIO_PORT_H                        = 0x23,            ///<���� H
  GPIO_PORT_I                        = 0x28             ///<���� I
} GPIOPort;

///��������� GPIO
typedef struct _GPIOType {
  GPIOMode          Mode;                               ///<����� 
  GPIOSpeed         Speed;                              ///<��������
  HwrIntFunc        FuncCallback;                       ///<���������� ����������
  void*             UserData;                           ///<��������� �� ���������������� ������          
  char              NumPin;                             ///<����� ������
  GPIOPort          NumPort;                            ///<����� �����
} GPIOType;

#define GPIO_OUTPUT_LOW             0                   ///<����� "0"
#define GPIO_OUTPUT_HIGH            1                   ///<����� "1"

/**������������� GPIO
  \param[in] GPIODef ��������� GPIO
  \return ��������� ���������� �������
*/
uint32_t stm8s_gpio_Init(GPIOType* GPIODef);

/**��������� ������ GPIO
  \param[in] NumPort ����� ����� GPIO
  \param[in] NumPin ����� ������ GPIO
  \param[in] GPIOValue �������� ������
  \return ��������� ���������� �������
*/
uint32_t stm8s_gpio_Set(GPIOPort NumPort, char NumPin, char GPIOValue);

/**
@}
*/

#endif