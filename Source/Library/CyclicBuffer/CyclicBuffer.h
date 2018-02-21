/**
  \file    CyclicBuffer.h 
  \brief   ������������ ���� ������������ ������
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __CYCLIC_BUFFER__
#define __CYCLIC_BUFFER__

#include "stm32f4xx_hal.h"

///��������� ������������ ������
typedef struct
{
  uint32_t      IndexWrite;             ///<����� ����� � ������, ������� ����� ������� 
  uint32_t      IndexRead;              ///<����� ����� � ������, ������� ����� ��������
  uint8_t*      pBuffer;                ///<��������� �� ������������ ������ ����
  uint32_t      SizeBuffer;             ///<������ ������
  uint8_t       isEmpty;                ///<���� ����, ��� ����� ������
  uint8_t       isFull;                 ///<���� ����, ��� ����� ������
} CyclicBuff;

///����������� ���� ������
typedef enum {
  CYCLIC_BUFF_NOT_CODE = 0,             ///<������ ���
  CYCLIC_BUFF_UNDERFLOW,                ///<������ ������ �� ������� ������
  CYCLIC_BUFF_OVERFLOW,                 ///<������������ ������
  CYCLIC_BUFF_DONT_INIT                 ///<����� �� ������������������
} ExtCodeCyclicBuff;

/**������������� ������������ ������
  \param[in] pCyclicBuff ��������� �� ��������� ������������ ������
  \param[in] SizeBuff ������ ������������ ������ � ������
  \return ��������� ���������� ������� 
*/
uint8_t CyclicBufferCreate(CyclicBuff* pCyclicBuff, uint32_t SizeBuff);

/**������ ����� � ����������� �����
  \param[in] pCyclicBuff ��������� �� ��������� ������������ ������
  \param[in] PutByte ������������ ����
  \param[out] pExtCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/
uint8_t CyclicBufferPut(CyclicBuff* pCyclicBuff, uint8_t PutByte, ExtCodeCyclicBuff* pExtCode);


/**������ ����� �� ������������ ������
  \param[in] pCyclicBuff ��������� �� ��������� ������������ ������
  \param[in] pGetByte ����������� ����
  \param[out] pExtCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/
uint8_t CyclicBufferGet(CyclicBuff* pCyclicBuff, uint8_t* pGetByte, ExtCodeCyclicBuff* pExtCode);

/**����� ������������ ������
  \param[in] pCyclicBuff ��������� �� ��������� ������������ ������
  \param[out] pExtCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/
uint8_t CyclicBufferReset(CyclicBuff* pCyclicBuff, ExtCodeCyclicBuff* pExtCode);

/**������ ������������ ������, ��������� ��� ������
  \param[in] pCyclicBuff ��������� �� ��������� ������������ ������
  \param[out] pWriteSize ������ ������, ��������� ��� ������
  \param[out] pExtCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� ������� 
*/
uint8_t CyclicBufferGetWriteSize(CyclicBuff* pCyclicBuff, uint32_t* pWriteSize, ExtCodeCyclicBuff* pExtCode);

/**������ ������������ ������, ��������� ��� ������
  \param[in] pCyclicBuff ��������� �� ��������� ������������ ������
  \param[out] pReadSize ������ ������, ��������� ��� ������
  \param[out] pExtCode ����������� ��� ������ ���������� �������
  \return ��������� ���������� �������
*/
uint8_t CyclicBufferGetReadSize(CyclicBuff* pCyclicBuff, uint32_t* pReadSize, ExtCodeCyclicBuff* pExtCode);

/**��������������� ������������ ������
  \param[in] pCyclicBuff ��������� �� ��������� ������������ ������
  \return ��������� ���������� �������
*/
uint8_t CyclicBufferDestroy(CyclicBuff* pCyclicBuff);

#endif