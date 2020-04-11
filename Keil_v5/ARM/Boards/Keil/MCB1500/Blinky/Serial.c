/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *                        - COM (UART0) interface  (default)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC15xx.h>                         /* LPC15xx definitions           */
#include "Serial.h"

#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */
#endif

#define UART    LPC_USART0


/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifndef __DBG_ITM

  /* configure PINs GPIO2.6, GPIO2.7 for UART */
  LPC_SYSCON->SYSAHBCLKCTRL0 |= ((1UL << 16) |  /* enable clock for GPIO2     */
                                 (1UL << 12)  );/* enable clock for SWM       */

  LPC_SWM->PINASSIGN0 &= ~((0xFF <<  0) |       /* clear PIN assign UART1_TXD */
                           (0xFF <<  8)  );     /* clear PIN assign UART1_RXD */
  LPC_SWM->PINASSIGN0 |=  ((  70 <<  0) |       /* PIN assign UART1_TXD  P2.6 */
                           (  71 <<  8)  );     /* PIN assign UART1_RXD  P2.7 */

  /* configure UART0 */
  LPC_SYSCON->SYSAHBCLKCTRL1 |=  (1UL << 17);   /* Enable clock to UART0      */

  /* 115200 baud @ 12MHz */
  LPC_SYSCON->UARTCLKDIV = 6;                   /* UART clock =  PCLK / 6     */
  LPC_SYSCON->FRGCTRL    = 0x15FF;
  UART->BRG  = ((12000000UL / 16 / 115200UL) -1);

  UART->CFG  = ((1UL << 0) |                    /* Enable USART               */
                (1UL << 2) |                    /* 8 data bits                */
                (0UL << 4) |                    /* no parity                  */
                (0UL << 6)  );                  /* 1 stop bit                 */
#endif
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int c) {

#ifdef __DBG_ITM
    ITM_SendChar(c);
#else
  while (!(UART->STAT & (1UL << 2)));
  UART->TXDATA = c;
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
  while (!(UART->STAT & (1UL << 0)));
  return (UART->RXDATA);
#endif
}
