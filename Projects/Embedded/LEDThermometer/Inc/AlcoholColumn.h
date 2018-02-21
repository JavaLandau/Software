/**
  \file    AlcoholColumn.h 
  \brief   ������������ ���� ������ ���������� ������������ ��������� �������
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef _ALCOHOL_COLUMN_
#define _ALCOHOL_COLUMN_

/**
  \addtogroup module_AlcoholColumn
@{
*/

/** @name ���������, �������� ������������� ��������
*/                        
///@{
#define TEMPERATURE_MAX         40
#define TEMPERATURE_MIN         -5
///@}

/**������������� ������ ���������� ������������ ��������� �������
  \return ��������� ���������� ������� 
*/
uint32_t AlcoholColumnCreate(void);

/**��������� ��������� �������� �����������
  \param[in] Value �������� �����������
  \return ��������� ���������� ������� 
*/
uint32_t AlcoholColumnSet(int8_t Value);

/**����� �������� �����������
  \return ��������� ���������� ������� 
*/
uint32_t AlcoholColumnReset();

/**  
@}
*/

#endif