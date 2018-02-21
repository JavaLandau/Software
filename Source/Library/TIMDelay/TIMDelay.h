/**
  \file    TIMDelay.h 
  \brief   ������������ ���� ������� ��������
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef _TIMDELAY_
#define _TIMDELAY_

/**�������� �� �������� �����
  \param[in] microsec ����� �������� � �������������
*/
void TIMDelay(uint32_t microsec);

/**������������� ������� ��������
  \param[in] period ������ �������� ������� TIM1
  \return ��������� ���������� �������
*/
uint32_t InitTIM(uint32_t period);

#endif