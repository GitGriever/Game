/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines used for target MPS
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *                        - USART2  (default)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2014 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f3xx.h>
#include "Serial.h"

#ifdef __DBG_ITM
volatile int32_t ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input    */
#endif


/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Init (void) {

#ifndef __DBG_ITM
  /* configure PD5 USART2 TX, PD6 USART2 RX                                   */
  RCC->AHBENR   |=  (1UL <<  20);                   /* Enable GPIOD clock     */

  /* Connect PD5 and PD6 to Alternate Function                                */
  GPIOD->AFR[0] &= ~((0xF << 5*4) | (0xF << 6*4));
  GPIOD->AFR[0] |=  ((0x7 << 5*4) | (0x7 << 6*4));  /* PD5, PD6 AF7           */
  GPIOD->MODER  &= ~((3UL << 5*2) | (3UL << 6*2));
  GPIOD->MODER  |=  ((2UL << 5*2) | (2UL << 6*2));  /* PD5, PD6 AF            */

  /* Configure USART2 for 115200 baud                                         */
  RCC->APB1ENR |= (1UL << 17);                      /* Enable USART#2 clock   */
  USART2->BRR   = (19 << 4) | 8;                    /* 115200 @ 72/2 MHz      */
  USART2->CR3   =  0;
  USART2->CR2   =  (0UL << 12);                     /* 1 stop bit             */
  USART2->CR1   = ((0UL << 12) |                    /* 8 Data Bits            */
                   (0UL << 10) |                    /* no Parity              */
                   (1UL <<  3) |                    /* Transmitter enable     */
                   (1UL <<  2) |                    /* Receiver enable        */
                   (1UL <<  0)  );                  /* USART enable           */
#endif
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int c) {

#if defined __DBG_ITM
  ITM_SendChar(c);
#else
  while (!(USART2->ISR & (1UL << 7)));
  USART2->TDR = (c & 0x1FF);
#endif
  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port
 *----------------------------------------------------------------------------*/
int SER_GetChar (void) {

#if defined __DBG_ITM
  if (ITM_CheckChar())
    return ITM_ReceiveChar();
#else
  if (USART2->ISR & (1UL << 5))
    return (USART2->RDR & 0x1FF);
#endif
  return (-1);
}
