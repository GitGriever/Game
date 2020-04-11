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
 * Copyright (c) 2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC11Uxx.h>


/*----------------------------------------------------------------------------
  Initialize UART0 pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
  volatile uint32_t regVal;
   
  /* configure PINs GPIO0.18, GPIO0.19 for USART */
  LPC_SYSCON->SYSAHBCLKCTRL |= ((1UL <<  6) |   /* enable clock for GPIO0     */
                                (1UL << 16) );  /* enable clock for IOCON     */

  LPC_IOCON->PIO0_18 &= ~(7UL <<  0);           /* clear FUNC bits            */
  LPC_IOCON->PIO0_18 |=  (1UL <<  0);           /* P0.18 is RxD0              */
  LPC_IOCON->PIO0_19 &= ~(7UL <<  0);           /* clear FUNC bits            */
  LPC_IOCON->PIO0_19 |=  (1UL <<  0);           /* P0.19 is TxD0              */

  /* configure UART0 */
  LPC_SYSCON->SYSAHBCLKCTRL |=  (1UL << 12);    /* Enable clock to USART      */
  LPC_SYSCON->UARTCLKDIV     =  (4UL <<  0);    /* UART0 clock =  CCLK / 4    */

  LPC_USART->LCR = 0x83;                  /* 8 bits, no Parity, 1 Stop bit    */
  LPC_USART->DLL = 4;                     /* 115200 Baud Rate @ 12.0 MHZ PCLK */
  LPC_USART->FDR = 0x85;                  /* FR 1.627, DIVADDVAL 5, MULVAL 8  */
  LPC_USART->DLM = 0;                     /* High divisor latch = 0           */
  LPC_USART->LCR = 0x03;                  /* DLAB = 0                         */

  regVal = LPC_USART->LSR;                      /* clear status register      */
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
  Read character from Serial Port   (non-blocking)
 *----------------------------------------------------------------------------*/
int SER_GetChar (void) {

  if (LPC_USART->LSR & 0x01) { 
    return (LPC_USART->RBR);
  }
  return (-1);
}
