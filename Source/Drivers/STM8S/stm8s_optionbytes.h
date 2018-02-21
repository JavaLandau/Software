/**
  \file    stm8s_optionbytes.h
  \brief   ������������ ���� �������� ���������� Option Bytes ���� STM8S
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __STM8S_OPTIONBYTES__
#define __STM8S_OPTIONBYTES__

#include "TypesDefine.h"

/** @name �������� �������� ������� � ����-������
*/                        
///@{
#define OPT7_WAIT_0			0
#define OPT7_WAIT_1			1
///@}

/**��������� ������� � ����-������
  \param[in] WaitState ������� ������� � ����-������
  \return ��������� ���������� �������
*/
uint32_t stm8s_optionbytes_SetFlashWait(char WaitState);

#endif