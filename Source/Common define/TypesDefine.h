/**
  \file    TypesDefine.h 
  \brief   ������������ ���� ����������� ����� ����������, ����� ������ � ������
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __FUNC_DEFINE__
#define __FUNC_DEFINE__

/** @name ���� ������ �������
*/                        
///@{
#define FUNC_OK                 0               ///<������ ���
#define FUNC_ERROR              1               ///<����������� ������
#define FUNC_INVALID_PARAM      2               ///<������������ ������ ������� ������
#define FUNC_HEAP_MEM_ERROR     3               ///<������ ��������� ������ � ����
///@}

/** @name ����������� ������� ��������
*/                        
///@{
#define FALSE                   0               ///<���������� "����"
#define TRUE                    1               ///<���������� "������"
///@}

typedef void (*HwrIntFunc)(void*);              ///<��������� �� �������-����������� ����������

#endif