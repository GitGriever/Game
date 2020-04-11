/*------------------------------------------------------------------------------
 *      RL-ARM
 *------------------------------------------------------------------------------
 *      Name:    Serial.c
 *      Purpose: Serial Input Output for ST STM32F10x
 *      Note(s): Possible defines to select the used communication interface:
 *                           - UART2 interface  (default)
 *------------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f10x_cl.h>
#include "Serial.h"


/*------------------------------------------------------------------------------
 *       SER_Init:  Initialize Serial Interface
 *----------------------------------------------------------------------------*/

void SER_Init (void) {

  /* Configure UART2 for 115200 baud. */
  RCC->APB2ENR |= 0x00000021;
  AFIO->MAPR   |= 0x00000008;
  GPIOD->CRL   &= 0xF00FFFFF;
  GPIOD->CRL   |= 0x04B00000;

  RCC->APB1ENR |= 0x00020000;
  USART2->BRR   = 0x0135;
  USART2->CR3   = 0x0000;
  USART2->CR2   = 0x0000;
  USART2->CR1   = 0x200C;
}


/*------------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to the Serial Port
 *----------------------------------------------------------------------------*/

int32_t SER_PutChar (int32_t ch) {

  while (!(USART2->SR & 0x0080));
  USART2->DR = ch;
  return (ch);
}


/*------------------------------------------------------------------------------
 *       SER_GetChar:  Read a character from the Serial Port (non-blocking)
 *----------------------------------------------------------------------------*/

int32_t SER_GetChar (void) {

  if (USART2->SR & 0x0020) 
    return (USART2->DR);
  return (-1);
}

/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
