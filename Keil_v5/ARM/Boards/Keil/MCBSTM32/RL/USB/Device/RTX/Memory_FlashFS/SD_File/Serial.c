/*-----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Serial Input Output functions
 * Note(s):
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f10x.h>                    /* STM32F10x Definitions           */
#include "Serial.h"

#ifdef __DBG_ITM
volatile int32_t ITM_RxBuffer;
#endif

/*-----------------------------------------------------------------------------
 *       SER_Init:  Initialize Serial Interface
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifdef __DBG_ITM
  ITM_RxBuffer = ITM_RXBUFFER_EMPTY;

#else
  int i;

  RCC->APB2ENR |=  (   1UL <<  0);        /* enable clock Alternate Function */
  AFIO->MAPR   &= ~(   1UL <<  2);        /* clear USART1 remap              */

  RCC->APB2ENR |=  (   1UL <<  2);        /* enable GPIOA clock              */
  GPIOA->CRH   &= ~(0xFFUL <<  4);        /* clear PA9, PA10                 */
  GPIOA->CRH   |=  (0x0BUL <<  4);        /* USART1 Tx (PA9) output push-pull*/
  GPIOA->CRH   |=  (0x04UL <<  8);        /* USART1 Rx (PA10) input floating */

  RCC->APB2ENR |=  (   1UL << 14);        /* enable USART#1 clock            */

  USART1->BRR   = 0x0271;                 /* 115200 baud @ PCLK2 72MHz       */
  USART1->CR1   = ((   1UL <<  2) |       /* enable RX                       */
                   (   1UL <<  3) |       /* enable TX                       */
                   (   0UL << 12) );      /* 1 start bit, 8 data bits        */
  USART1->CR2   = 0x0000;                 /* 1 stop bit                      */
  USART1->CR3   = 0x0000;                 /* no flow control                 */
  for (i = 0; i < 0x1000; i++) __NOP();   /* avoid unwanted output           */
  USART1->CR1  |= ((   1UL << 13) );      /* enable USART                    */
#endif
}


/*-----------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to the Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_PutChar (int32_t ch) {
#ifdef __DBG_ITM
  int i;
  ITM_SendChar (ch & 0xFF);
  for (i = 10000; i; i--);
#else
  while (!(USART1->SR & USART_SR_TXE));
  USART1->DR = (ch & 0xFF);
#endif  

  return (ch);
}


/*-----------------------------------------------------------------------------
 *       SER_GetChar:  Read a character from the Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_GetChar (void) {
#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return ITM_ReceiveChar();
#else
  if (USART1->SR & USART_SR_RXNE)
    return (USART1->DR);
#endif
  return (-1);
}

/*-----------------------------------------------------------------------------
 * end of file
 *----------------------------------------------------------------------------*/
