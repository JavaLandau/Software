/**
  \file    stm8s_itc.c 
  \brief   ����������� ���� ����������� ���������� ���� STM8S
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/
#include "TypesDefine.h"

/*�������� ��������� ����������*/
void stm8s_itc_InterruptsEnable(void)
{
  _asm("RIM");  
}