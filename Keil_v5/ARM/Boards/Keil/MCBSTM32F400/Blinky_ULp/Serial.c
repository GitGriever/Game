/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *            __USART3    - USART3 interface
 *            __USART4    - USART4 interface
 *                        - USART1 interface  (default)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f4xx.h>                  /* STM32F4xx Definitions              */
#include "Serial.h"

#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */
#endif

#if   defined  __USART3
  #define USART  USART3
#elif defined  __USART4
  #define USART  UART4
#else
  #define USART  USART1
#endif

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifndef __DBG_ITM
  int i;

#if defined __USART3  || defined __USART4
#if defined __USART3
  RCC->APB1ENR  |= (1UL <<18);          /* Enable USART3 clock                */
#else
  RCC->APB1ENR  |= (1UL <<19);          /* Enable USART4 clock                */
#endif
  RCC->AHB1ENR  |= (1UL << 2);          /* Enable GPIOC clock                 */

  /* Connect PC10 and PC11 to Alternate Function */
  GPIOC->MODER  &= (3 << 2*10) | (3 << 2*11);
  GPIOC->MODER  |= (2 << 2*10) | (2 << 2*11);

  GPIOC->AFR[1] &= ~0x0000FF00;
#if defined __USART3
  GPIOC->AFR[1] |=  0x00007700;         /* USART3: Alternate Function 7 (AF7) */
#else
  GPIOC->AFR[1] |=  0x00008800;         /* USART4: Alternate Function 8 (AF8) */
  #endif
#else
  RCC->APB2ENR  |= (1UL << 4);          /* Enable USART1 clock                */
  RCC->AHB1ENR  |= (1UL << 1);          /* Enable GPIOB clock                 */

  /* Connect PB6 and PB7 to Alternate Function 7 (AF7) */
  GPIOB->MODER  &= (3 << 2*6) | (3 << 2*7);
  GPIOB->MODER  |= (2 << 2*6) | (2 << 2*7);
  GPIOB->AFR[0] &= ~0xFF000000;
  GPIOB->AFR[0] |=  0x77000000;
#endif

#if defined __USART3  || defined __USART4
  /* Configure USART3/4: 115200 baud @ 30MHz, 8 bits, 1 stop bit, no parity   */
  USART->BRR = (16 << 4) | 4;
#else
  /* Configure USART1: 115200 baud @ 60MHz, 8 bits, 1 stop bit, no parity     */
  USART->BRR = (32 << 4) | 9;
#endif
  USART->CR3 = 0x0000;
  USART->CR2 = 0x0000;
  for (i = 0; i < 0x1000; i++) __NOP(); /* avoid unwanted output              */
  USART->CR1 = 0x200C;

#endif
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int c) {

#ifdef __DBG_ITM
  ITM_SendChar(c);
#else
  while (!(USART->SR & 0x0080));
  USART->DR = (c & 0x1FF);
#endif
  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER_GetChar (void) {

#ifdef __DBG_ITM
  while (ITM_CheckChar() != 1) __NOP();
  return (ITM_ReceiveChar());
#else
  while (!(USART->SR & 0x0020));
  return (USART->DR);
#endif
}
