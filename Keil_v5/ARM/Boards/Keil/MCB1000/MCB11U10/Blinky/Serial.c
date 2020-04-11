/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
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

#include "LPC11Uxx.h"                           /* LPC11Uxx definitions       */
#include "Serial.h"


/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
   
  /* configure PINs GPIO1.6, GPIO1.7 for UART */
  LPC_SYSCON->SYSAHBCLKCTRL |= ((1UL <<  6) |   /* enable clock for GPIO      */
                                (1UL << 16) );  /* enable clock for IOCON     */

  LPC_IOCON->PIO0_18  =  (1UL <<  0);           /* P0.18 is RxD               */
  LPC_IOCON->PIO0_19  =  (1UL <<  0);           /* P0.19 is TxD               */

  /* configure UART0 */
  LPC_SYSCON->SYSAHBCLKCTRL |=  (1UL << 12);    /* Enable clock to UART       */
  LPC_SYSCON->UARTCLKDIV     =  (4UL <<  0);    /* UART clock =  CCLK / 4     */

  LPC_USART->LCR = 0x83;                  /* 8 bits, no Parity, 1 Stop bit    */
  LPC_USART->DLL = 4;                     /* 115200 Baud Rate @ 12.0 MHZ PCLK */
  LPC_USART->FDR = 0x85;                  /* FR 1.627, DIVADDVAL 5, MULVAL 8  */
  LPC_USART->DLM = 0;                     /* High divisor latch = 0           */
  LPC_USART->LCR = 0x03;                  /* DLAB = 0                         */
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int c) {

  while (!(LPC_USART->LSR & 0x20));
  LPC_USART->THR = c;

  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER_GetChar (void) {

  while (!(LPC_USART->LSR & 0x01));
  return (LPC_USART->RBR);
}
