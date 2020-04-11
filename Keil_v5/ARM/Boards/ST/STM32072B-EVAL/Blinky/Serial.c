/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low level serial routines
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "stm32f0xx.h"                  /* STM32F0xx Definitions              */

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate   PD5 USART2_TX, PD6 USART2_RX  (AF1))
 *----------------------------------------------------------------------------*/
void SER_Init (void) {

  /* Configure USART2 for 115200 baud                                         */
  RCC->AHBENR |=  (1UL << 20);          /* Enable GPIOD clock                 */

  GPIOD->AFR[0] &= ~((15UL << 4* 5) | (15UL << 4* 5));
  GPIOD->MODER  &= ~(( 3UL << 2* 5) | ( 3UL << 2* 6));
  GPIOD->MODER  |=  (( 2UL << 2* 5) | ( 2UL << 2* 6));

  RCC->APB1ENR |= (1UL << 17);          /* Enable USART#2 clock               */

  USART2->BRR  = 0x01A1;                /* Configure 115200 baud, @ 48MHz     */
  USART2->CR1  = 0x0000;                /* 8 bit, no parity                   */
  USART2->CR2  = 0x0000;                /* 1 stop bit                         */
  USART2->CR1 |= ((1UL << 3) |          /* Transmitter enable                 */
                  (1UL << 2) |          /* Receiver enable                    */
                  (1UL << 0)  );        /* USART enable                       */
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int c) {

  while (!(USART2->ISR & (1UL << 7)));
  USART2->TDR = (c & 0x1FF);

  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER_GetChar (void) {

  while (!(USART2->ISR & (1UL << 5)));
  return (USART2->RDR);
}
