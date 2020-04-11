/*------------------------------------------------------------------------------
 *      RL-ARM
 *------------------------------------------------------------------------------
 *      Name:    Serial.c
 *      Purpose: Serial Input Output for NXP LPC17xx
 *      Note(s): Possible defines to select the used communication interface:
 *               __DBG_ITM   - ITM SWO interface (USART is used by default)
 *               __UART0     - COM0 (UART0) interface
 *                           - COM1 (UART1) interface  (default)
 *------------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC17xx.h>
#include "Serial.h"

#ifdef __DBG_ITM
volatile int32_t ITM_RxBuffer;
#endif

#ifdef __UART0
  #define UART    LPC_UART0
#else
  #define UART    LPC_UART1
#endif


/*------------------------------------------------------------------------------
 *       SER_Init:  Initialize Serial Interface
 *----------------------------------------------------------------------------*/

void SER_Init (void) {

#ifndef __DBG_ITM
#ifdef __UART0                             /* UART0 */
  LPC_SC->PCONP |= ((1 << 3) | (1 << 15)); /* enable power to UART0 & IOCON   */

  LPC_PINCON->PINSEL0 |=  (1 << 4);        /* Pin P0.2 used as TXD0           */
  LPC_PINCON->PINSEL0 |=  (1 << 6);        /* Pin P0.3 used as RXD0           */
#else                                      /* UART1 */
  LPC_SC->PCONP |= ((1 << 4) | (1 << 15)); /* enable power to UART1 & IOCON   */

  LPC_PINCON->PINSEL4 |=  (2 << 0);        /* Pin P2.0 used as TXD1           */
  LPC_PINCON->PINSEL4 |=  (2 << 2);        /* Pin P2.1 used as RXD1           */
#endif

  UART->LCR    = 0x83;                     /* 8 bits, no Parity, 1 Stop bit   */
  UART->DLL    = 9;                        /* 115200 Baud Rate @ 25.0 MHZ PCLK*/
  UART->FDR    = 0x21;                     /* FR 1,507, DIVADDVAL=1, MULVAL=2 */
  UART->DLM    = 0;                        /* High divisor latch = 0          */
  UART->LCR    = 0x03;                     /* DLAB = 0                        */
#endif
}


/*------------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to the Serial Port
 *----------------------------------------------------------------------------*/

int32_t SER_PutChar (int32_t ch) {

#ifdef __DBG_ITM
  ITM_SendChar(ch);
#else
  while (!(UART->LSR & 0x20));
  UART->THR = ch;
#endif
  return (ch);
}


/*------------------------------------------------------------------------------
 *       SER_GetChar:  Read a character from the Serial Port (non-blocking)
 *----------------------------------------------------------------------------*/

int32_t SER_GetChar (void) {

#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return (ITM_ReceiveChar());
#else
  if (UART->LSR & 0x01) 
    return (UART->RBR);
#endif
  return (-1);
}

/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
