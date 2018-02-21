/**
  \file    GPIOfun.h 
  \brief   ������������ ���� ������� ��� ������ � GPIO
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef _GPIOFUN_
#define _GPIOFUN_

/**��������� ������ GPIO �� ���������� ��������
  \param[in] StrGPIOPin ��������� �������� ������ GPIO
  \param[out] ppGPIOPort ��������� �� ���� GPIO
  \param[out] pGPIONum ����� GPIO
  \return ��������� ���������� ������� 
*/
uint32_t GPIOGetPort(const char* StrGPIOPin, GPIO_TypeDef** ppGPIOPort, uint16_t* pGPIONum);

#endif
