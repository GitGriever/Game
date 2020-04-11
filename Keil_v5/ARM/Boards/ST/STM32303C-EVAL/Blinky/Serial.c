/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines used for target MPS
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *                        - USART1  (default)
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
  /* configure PC4 USART1 TX, PE1 USART1 RX                                   */
  RCC->AHBENR   |=  ((1UL <<  19) |                 /* Enable GPIOC clock     */
	                   (1UL <<  21)  );               /* Enable GPIOE clock     */

  /* Connect PC4 to Alternate Function                                        */
  GPIOC->AFR[0] &= ~(0xF << 4*4);
  GPIOC->AFR[0] |=  (0x7 << 4*4);                   /* PC4 AF7                */
  GPIOC->MODER  &= ~(3UL << 4*2);
  GPIOC->MODER  |=  (2UL << 4*2);                   /* PC4 AF                 */

  /* Connect PE1 to Alternate Function                                        */
  GPIOE->AFR[0] &= ~(0xF << 1*4);
  GPIOE->AFR[0] |=  (0x7 << 1*4);                   /* PE1 AF7                */
  GPIOE->MODER  &= ~(3UL << 1*2);
  GPIOE->MODER  |=  (2UL << 1*2);                   /* PE1 AF                 */

  /* Configure USART1 for 115200 baud                                         */
  RCC->APB2ENR |= (1UL << 14);                      /* Enable USART#1 clock   */
  USART1->BRR   = (39 << 4) | 1;                    /* 115200 @ 72   MHz      */
  USART1->CR3   =  0;
  USART1->CR2   =  (0UL << 12);                     /* 1 stop bit             */
  USART1->CR1   = ((0UL << 12) |                    /* 8 Data Bits            */
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
  while (!(USART1->ISR & (1UL << 7)));
  USART1->TDR = (c & 0x1FF);
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
  if (USART1->ISR & (1UL << 5))
    return (USART1->RDR & 0x1FF);
#endif
  return (-1);
}
