/**
  \file    stm8s_flash.h 
  \brief   ������������ ���� �������� ���������� ����-������ STM8S
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __STM8S_FLASH__
#define __STM8S_FLASH__

#include "TypesDefine.h"

/**���������� ������� � option bytes ��� ����-������
  \return ��������� ���������� �������
*/
uint32_t stm8s_flash_OptionBytesEnable(void);

/**������ ������� � option bytes ��� ����-������
  \return ��������� ���������� �������
*/
uint32_t stm8s_flash_OptionBytesDisable(void);

#endif
