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
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "stm32f0xx.h"                  /* STM32F0xx Definitions              */

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate   PA9 USART1_TX, PA10 USART1_RX  (AF1))
 *----------------------------------------------------------------------------*/
void SER_Init (void) {

  /* Configure UART1 for 115200 baud                                          */
  RCC->AHBENR   |=  (1UL << 17);        /* Enable GPIOA clock                 */

  GPIOA->AFR[1] &= ~((15UL << 4*(9-8)) | (15UL << 4*(10-8)));
  GPIOA->AFR[1] |=  (( 1UL << 4*(9-8)) | ( 1UL << 4*(10-8)));
  GPIOA->MODER  &= ~(( 3UL << 2*(9  )) | ( 3UL << 2*(10  )));
  GPIOA->MODER  |=  (( 2UL << 2*(9  )) | ( 2UL << 2*(10  )));

  RCC->APB2ENR  |= (1UL << 14);         /* Enable UART#1 clock                */

  USART1->BRR  = 0x01A1;                /* Configure 115200 baud, @ 48MHz     */
  USART1->CR1  = 0x0000;                /* 8 bit, no parity                   */     
  USART1->CR2  = 0x0000;                /* 1 stop bit                         */
  USART1->CR1 |= ((1UL << 3) |          /* Transmitter enable                 */
                  (1UL << 2) |          /* Receiver enable                    */
                  (1UL << 0)  );        /* USART enable                       */
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int c) {

  while (!(USART1->ISR & (1UL << 7)));
  USART1->TDR = (c & 0x1FF);

  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER_GetChar (void) {

  while (!(USART1->ISR & (1UL << 5)));
  return (USART1->RDR);
}
