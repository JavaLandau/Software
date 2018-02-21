/**
  \file    BitsMaskOperations.h 
  \brief   ������������ ���� �������� ������� ������������� �������� ��� ������ ����������
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __BITS_MASK_OPERATIONS__
#define __BITS_MASK_OPERATIONS__

#define RESET		       0        ///<����� ����/�����/��������
#define SET		       1        ///<��������� ����/�����/��������

/**
  \defgroup module_bits_op_4_var ������� ������������� �������� ��� ����������
  \brief ������ ������� ������������� �������� ��� ������ � �����������
@{
*/

///�����/��������� ��������� ���� � �������� �����
#define CHANGE_BIT(SRC,NUM,VALUE)    if(VALUE) (SRC)|=(char)(1<<(NUM));\
                                     else (SRC)&=(char)(~(1<<(NUM)));
///����� ��������� ���� � �������� �����
#define RESET_BIT(SRC,NUM)           ((SRC)&=(char)(~(1<<(NUM))))

///��������� ��������� ���� � �������� �����
#define SET_BIT(SRC,NUM)   	     ((SRC)|=(char)(1<<(NUM))

///�������� ��������� ���� � �������� �����
#define GET_BIT(SRC, NUM)            ((char)(((SRC)>>(NUM))&0xFF))
/**  
  @}
*/

/**
  \defgroup module_bits_op_4_reg ������� ������������� �������� ��� ���������
  \brief ������ ������� ������������� �������� ��� ������ � ���������� ���������� ��� �����������
@{
*/

///�����/��������� ��������� ���� � �������� ��������
#define CHANGE_REG_BIT(SRC,NUM,VALUE) if(VALUE) (*SRC)|=(char)(1<<(NUM));\
                                      else (*SRC)&=(char)(~(1<<(NUM)));

///����� ��������� ���� � �������� ��������                                      
#define RESET_REG_BIT(SRC,NUM)        ((*SRC)&=(char)(~(1<<(NUM))))

///��������� ��������� ���� � �������� ��������                                      
#define SET_REG_BIT(SRC,NUM)          ((*SRC)|=(char)(1<<(NUM)))

///�������� ��������� ���� � �������� ��������                                      
#define GET_REG_BIT(SRC, NUM)         ((char)(((*SRC)>>(NUM))&0xFF))

///��������� ��������� �������� �� �������� ��������                                         
#define CHANGE_REG(SRC,VALUE)	      (*(SRC) = (VALUE))

///�������� ��������� ��������                                      
#define GET_REG(SRC)	              (*(SRC))

///��������� ��������� ��������                                      
#define SET_REG(SRC)                  (*(SRC) = 0xFF)  

///����� ��������� ��������                                      
#define RESET_REG(SRC)                (*(SRC) = 0x00)  
/**  
  @}
*/
                                      
#endif