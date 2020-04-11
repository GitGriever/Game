/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *            __USART1    - USART1 interface
 *                        - USART3 interface  (default)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "stm32l1xx.h"                  /* STM32L1xx.h definitions            */
#include "Serial.h"

#ifdef __DBG_ITM
volatile int ITM_RxBuffer;              /*  CMSIS Debug Input                 */
#endif

/*----------------------------------------------------------------------------
 Define  USART
 *----------------------------------------------------------------------------*/
#ifdef __USART1
  #define USARTx  USART1
#else
  #define USARTx  USART3
#endif


/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifdef __DBG_ITM
  ITM_RxBuffer = ITM_RXBUFFER_EMPTY;       /*  CMSIS Debug Input              */
#else

#ifdef __USART1                            /* USART1 */
  RCC->AHBENR   |=   ( 1UL <<  0);         /* Enable GPIOA clock              */
  RCC->APB2ENR  |=   ( 1UL << 14);         /* Enable USART#1 clock            */

  /* Configure PA.10 to USART1_RX (AF07), PA.9 to USART1_TX (AF07) */
  GPIOA->AFR[1] &= ~((15UL << 4* 2) | (15UL << 4* 1) );
  GPIOA->AFR[1] |=  (( 7UL << 4* 2) | ( 7UL << 4* 1) );
  GPIOA->MODER  &= ~(( 3UL << 2*10) | ( 3UL << 2* 9) );
  GPIOA->MODER  |=  (( 2UL << 2*10) | ( 2UL << 2* 9) );
#else
  RCC->AHBENR   |=   ( 1UL <<  1);         /* Enable GPIOB clock              */
  RCC->APB1ENR  |=   ( 1UL << 18);         /* Enable USART#3 clock            */

  /* Configure PB.11 to USART3_RX (AF07), PB.10 to USART3_TX (AF07) */
  GPIOB->AFR[1] &= ~((15UL << 4* 3) | (15UL << 4* 2) );
  GPIOB->AFR[1] |=  (( 7UL << 4* 3) | ( 7UL << 4* 2) );
  GPIOB->MODER  &= ~(( 3UL << 2*11) | ( 3UL << 2*10) );
  GPIOB->MODER  |=  (( 2UL << 2*11) | ( 2UL << 2*10) );
#endif

  USARTx->BRR    = (17 << 4) | 6;          /* 115200 baud @ 32MHz             */
  USARTx->CR3    = 0x0000;                 /* no flow control                 */
  USARTx->CR2    = 0x0000;                 /* 1 stop bit                      */
  USARTx->CR1    = ((   1UL <<  2) |       /* enable RX                       */
                    (   1UL <<  3) |       /* enable TX                       */
                    (   0UL << 12) |       /* 1 start bit, 8 data bits        */
                    (   1UL << 13) );      /* enable USART                    */

#endif
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int ch) {
#ifdef __DBG_ITM
  ITM_SendChar (ch & 0xFF);
#else
  while (!(USARTx->SR & 0x0080));
  USARTx->DR = (ch & 0xFF);
#endif

  return (ch);
}

/*----------------------------------------------------------------------------
  Read character from Serial Port
 *----------------------------------------------------------------------------*/
int SER_GetChar (void) {
#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return ITM_ReceiveChar();
#else
  if (USARTx->SR & 0x0020)
    return (USARTx->DR);
#endif
  return (-1);
}

