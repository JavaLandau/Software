/**
  \file    stm8s_gpio.c 
  \brief   ����������� ���� �������� GPIO ���� STM8S
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#include "stm8s_gpio.h"
#include "BitsMaskOperations.h"

/**
  \defgroup module_service_STM8S_GPIO ��������� ������� ��� ������ � GPIO ���� STM8S
  \brief ������ ��������� �������, ����������� ��� ������ � GPIO ���� STM8S
@{
*/

/** @name �������� GPIO
*/                        
///@{
//Port A
#define PA_ODR                  (((volatile char*)(0x005000)))
#define PA_IDR                  (((volatile char*)(0x005001)))
#define PA_DDR                  (((volatile char*)(0x005002)))
#define PA_CR1                  (((volatile char*)(0x005003)))
#define PA_CR2                  (((volatile char*)(0x005004)))
//Port B
#define PB_ODR                  (((volatile char*)(0x005005)))
#define PB_IDR                  (((volatile char*)(0x005006)))
#define PB_DDR                  (((volatile char*)(0x005007)))
#define PB_CR1                  (((volatile char*)(0x005008)))
#define PB_CR2                  (((volatile char*)(0x005009)))
//Port C
#define PC_ODR                  (((volatile char*)(0x00500A)))
#define PC_IDR                  (((volatile char*)(0x00500B)))
#define PC_DDR                  (((volatile char*)(0x00500C)))
#define PC_CR1                  (((volatile char*)(0x00500D)))
#define PC_CR2                  (((volatile char*)(0x00500E)))
//Port D
#define PD_ODR                  (((volatile char*)(0x00500F)))
#define PD_IDR                  (((volatile char*)(0x005010)))
#define PD_DDR                  (((volatile char*)(0x005011)))
#define PD_CR1                  (((volatile char*)(0x005012)))
#define PD_CR2                  (((volatile char*)(0x005013)))
//Port E
#define PE_ODR                  (((volatile char*)(0x005014)))
#define PE_IDR                  (((volatile char*)(0x005015)))
#define PE_DDR                  (((volatile char*)(0x005016)))
#define PE_CR1                  (((volatile char*)(0x005017)))
#define PE_CR2                  (((volatile char*)(0x005018)))
//Port F
#define PF_ODR                  (((volatile char*)(0x005019)))
#define PF_IDR                  (((volatile char*)(0x00501A)))
#define PF_DDR                  (((volatile char*)(0x00501B)))
#define PF_CR1                  (((volatile char*)(0x00501C)))
#define PF_CR2                  (((volatile char*)(0x00501D)))
//Port G
#define PG_ODR                  (((volatile char*)(0x00501E)))
#define PG_IDR                  (((volatile char*)(0x00501F)))
#define PG_DDR                  (((volatile char*)(0x005020)))
#define PG_CR1                  (((volatile char*)(0x005021)))
#define PG_CR2                  (((volatile char*)(0x005022)))
//Port H
#define PH_ODR                  (((volatile char*)(0x005023)))
#define PH_IDR                  (((volatile char*)(0x005024)))
#define PH_DDR                  (((volatile char*)(0x005025)))
#define PH_CR1                  (((volatile char*)(0x005026)))
#define PH_CR2                  (((volatile char*)(0x005027)))
//Port I
#define PI_ODR                  (((volatile char*)(0x005028)))
#define PI_IDR                  (((volatile char*)(0x005029)))
#define PI_DDR                  (((volatile char*)(0x00502A)))
#define PI_CR1                  (((volatile char*)(0x00502B)))
#define PI_CR2                  (((volatile char*)(0x00502C)))
///@}

///���������� ������� GPIO �� ����
#define NUM_PIN_FOR_PORT        8

/**�������� ������������ ������������ ����� GPIO
  \param[in] Port ���� GPIO
  \return ��������� ���������� �������
*/
uint32_t prv_stm8s_gpio_CheckPort(GPIOPort Port);

/**�������� ������������ ������������ ������ ������ GPIO
  \param[in] Mode ����� ������ GPIO
  \return ��������� ���������� �������
*/
uint32_t prv_stm8s_gpio_CheckMode(GPIOMode Mode);

/**�������� ������������ ������������ �������� ������ GPIO
  \param[in] Speed �������� ������ GPIO
  \return ��������� ���������� �������
*/
uint32_t prv_stm8s_gpio_CheckSpeed(GPIOSpeed Speed);

/**
@}
  \defgroup module_STM8S_GPIO ������������ ������� ��� ������ � GPIO ���� STM8S
  \brief ������, ��������������� ������������ ����������� ���������� ��� ������ � GPIO ���� STM8S
@{
*/

/*��������� ������ GPIO*/
uint32_t stm8s_gpio_Set(GPIOPort NumPort, char NumPin, char GPIOValue)
{
  /*�������� �������� ������*/
  if(GPIOValue != GPIO_OUTPUT_LOW && GPIOValue != GPIO_OUTPUT_HIGH)
    return FUNC_INVALID_PARAM;
    
  if(prv_stm8s_gpio_CheckPort(NumPort) != FUNC_OK || 
     NumPin > (NUM_PIN_FOR_PORT - 1))    
  {
    return FUNC_INVALID_PARAM;
  }    
  
  /*��������� �������� � ��������������� ������� Px_ODR*/
  volatile char* GPIORegAddr = PA_ODR + (char)NumPort;
  CHANGE_REG_BIT(GPIORegAddr, NumPin, GPIOValue);

  /*�����������*/
  if(GET_REG_BIT(GPIORegAddr, NumPin) != GPIOValue)
    return FUNC_ERROR;

  return FUNC_OK;
}

/*������������� GPIO*/
uint32_t stm8s_gpio_Init(GPIOType* GPIODef)
{
  /*�������� �������� ������*/
  if(!GPIODef)
    return FUNC_INVALID_PARAM;
      
  if(prv_stm8s_gpio_CheckMode(GPIODef->Mode) != FUNC_OK || 
     prv_stm8s_gpio_CheckPort(GPIODef->NumPort) != FUNC_OK ||
     prv_stm8s_gpio_CheckSpeed(GPIODef->Speed) != FUNC_OK ||
     GPIODef->NumPin > (NUM_PIN_FOR_PORT - 1))    
  {
    return FUNC_INVALID_PARAM;
  }
 
  /*��������� ������ ���������������� �������� Px_ODR*/
  volatile char* GPIORegAddr = PA_ODR + (char)GPIODef->NumPort;
  
  /*��������� ������ � ��������� ���������������� �������� Px_DDR*/
  GPIORegAddr+=0x2;
  CHANGE_REG_BIT(GPIORegAddr, GPIODef->NumPin, GET_BIT((char)GPIODef->Mode,0));
  
  /*��������� ������ � ��������� ���������������� �������� Px_CR1*/
  GPIORegAddr++;
  CHANGE_REG_BIT(GPIORegAddr, GPIODef->NumPin, GET_BIT((char)GPIODef->Mode,1));
  
  /*��������� ������ � ��������� ���������������� �������� Px_CR2*/
  GPIORegAddr++;
  if(GET_BIT(GPIODef->Mode,0) == RESET)
  {
    if(GPIODef->FuncCallback == NULL)
      RESET_REG_BIT(GPIORegAddr, GPIODef->NumPin);
    else
      SET_REG_BIT(GPIORegAddr, GPIODef->NumPin);
  }else
    CHANGE_REG_BIT(GPIORegAddr, GPIODef->NumPin, GPIODef->Speed);
  
  
  return FUNC_OK;
}

/*�������� ������������ ������������ �������� ������ GPIO*/
uint32_t prv_stm8s_gpio_CheckSpeed(GPIOSpeed Speed)
{
  if(Speed != GPIO_SPEED_2MHZ && Speed != GPIO_SPEED_10MHZ)
    return FUNC_ERROR;
    
  return FUNC_OK;  
}

/*�������� ������������ ������������ ������ ������ GPIO*/
uint32_t prv_stm8s_gpio_CheckMode(GPIOMode Mode)
{
  if(Mode != GPIO_FLOATING_INPUT && Mode != GPIO_INPUT_WITH_PULLUP &&
     Mode != GPIO_OUTPUT_PSEUDO_OPEN_DRAIN && Mode != GPIO_OUTPUT_PUSHPULL)
  {
    return FUNC_ERROR;
  }
  
  return FUNC_OK;
}

/*�������� ������������ ������������ ����� GPIO*/
uint32_t prv_stm8s_gpio_CheckPort(GPIOPort Port)
{
  if(Port != GPIO_PORT_A && Port != GPIO_PORT_B && Port != GPIO_PORT_C && 
     Port != GPIO_PORT_D && Port != GPIO_PORT_E && Port != GPIO_PORT_F &&
     Port != GPIO_PORT_G && Port != GPIO_PORT_H && Port != GPIO_PORT_I)
  {
    return FUNC_ERROR;
  }
  
  return FUNC_OK;
}

/**
@}
*/