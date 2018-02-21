/**
  \file STM32Profile.h
  \brief ������������ ���� �������������� STM32
  \author  ������� �. �.
  \version 1.0
  \date    25.01.2018 
*/

/**
  \addtogroup module_STM32profile
@{
*/

#ifndef __STM32_PROFILE_
#define __STM32_PROFILE_

///�������� ��������� ������������� ����������
typedef enum {
  TIME_IN_CYCLES = 0,                           ///<� ������
  TIME_IN_NSEC,                                 ///<� ������������
  TIME_IN_UCSEC,                                ///<� �������������
  TIME_IN_MILSEC,                               ///<� �������������
  TIME_IN_SEC                                   ///<� ��������
} STM32TimeScale;

///��������� ������� �������
typedef struct {
  TIM_HandleTypeDef htim;                       ///<��������� �� ������ �������
  uint32_t TimerFreq;                           ///<������� ������������ �������
  uint32_t Period;                              ///<������ ����������� �������� �������
  uint32_t TimerCycles;                         ///<���������� ������
  STM32TimeScale TimeScale;                     ///<������� ������������� ����������
  uint32_t SelfDelay;                           ///<��������, �������� �������� ��� ������� �������, ����������� �������� � �. �.
  uint32_t SelfIntDelay;                        ///<��������, �������� �������� ��� ��������� ����������
} STM32ProfileStruct;

///�������� ������ ������� �������
typedef enum {
  STM32_PROFILE_ERROR_NOT_CODE = 0,             ///<������ ���
  STM32_PROFILE_ERROR_TIM1                      ///<������ ������������� ������� TIM1
} STM32ProfileErrorCode;


/** ������������� �������
        \param[in] FreqMCU ������� ����������������
	\param[in] Prescaler �������� ������������ �������
        \param[in] Period �������� ������� ����������� �������� �������
        \param[in] TimeScale ������� ������������� ����������
	\param[out] pErrorCode ��������� �� ����������, ���������� ���������� �� ������� ���������� �������	
	\return ������� ���������� �������
        \warning �������������� ������� ������������ ������� ���������������� ��� ���������� ���������� ����������,
                 ��� ��������� �������������� ������� FreqMCU �� Prescaler, ������� ������������� ������������� ��������
                 FreqMCU � Prescaler ������, ����� ��� �������� ������
*/
uint32_t STM32ProfileInit(uint32_t FreqMCU, uint32_t Prescaler, uint32_t Period, STM32TimeScale TimeScale, STM32ProfileErrorCode* pErrorCode);

/** ������ �������� �������
*/
void STM32ProfileStart(void);

/** ���������� �������� �������� �������
	\return ������������ ����� � ��������� �������� 
*/
uint32_t STM32ProfileStop(void);

#endif
/**  
  @}
*/