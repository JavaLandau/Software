/**
  \file    stm8s_tim.h
  \brief   ������������ ���� �������� ������� ���� STM8S
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __STM8S_TIM__
#define __STM8S_TIM__

#include "TypesDefine.h"

///����� �������
typedef enum _TIMTypeObj {
  TIM1 = 0x005250,                      ///<TIM1
  TIM2 = 0x005300,                      ///<TIM2
  TIM3 = 0x005320,                      ///<TIM3
  TIM4 = 0x005340                       ///<TIM4
} TIMTypeObj;

///��������� �������
typedef struct _TIMType  {
  TIMTypeObj        Instance;           ///<����� �������
  HwrIntFunc        FuncCallback;       ///<��������� �� ���������� ���������� �� �������
  void*             UserData;           ///<��������� �� ���������������� ������
  unsigned int      Prescaler;          ///<�������� ������������
  unsigned int      ARR;                ///<�������� ������� ��������
} TIMType;

/**������������� �������
  \param[in] TIMDef ��������� �������
  \return ��������� ���������� �������
*/
uint32_t stm8s_tim_Init(TIMType* TIMDef);

/**������ �������� �������
  \param[in] TIMDef ��������� �������
  \return ��������� ���������� �������
*/
uint32_t stm8s_tim_Start(TIMType* TIMDef);

/**��������� �������� �������
  \param[in] TIMDef ��������� �������
  \return ��������� ���������� �������
*/
uint32_t stm8s_tim_Stop(TIMType* TIMDef);

/**���������� ���������� �� ������� UPDATE �� ������� TIM1*/
void stm8s_TIM1_UpdateInterrupt(void);

/**���������� ���������� �� ������� UPDATE �� ������� TIM2*/
void stm8s_TIM2_UpdateInterrupt(void);

/**���������� ���������� �� ������� UPDATE �� ������� TIM3*/
void stm8s_TIM3_UpdateInterrupt(void);

#endif