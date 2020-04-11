/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *            __UART      - UART  (UART1)  interface
 *                        - disabled  (default)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <SAM4S.h>                        /* SAM4S definitions                */
#include "Serial.h"

#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */
#endif

/* Clock Definitions */
#define BAUD(b) ((SystemCoreClock + 8*b)/(16*b))

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifndef __DBG_ITM

  PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

#ifdef __UART
  PMC->PMC_PCER0 = ((1UL << ID_PIOB) |    /* enable PIOA clock                */
                    (1UL << ID_UART1)  ); /* enable UART clock                */

  /* Configure UART1 Pins (PB3 = TX, PB2 = RX). */
  PIOB->PIO_IDR        =  (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);
  PIOB->PIO_PUDR       =  (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);
  PIOB->PIO_ABCDSR[0] &= ~(PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);
  PIOB->PIO_ABCDSR[1] &= ~(PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);
  PIOB->PIO_PDR        =  (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);

  /* Configure UART for 115200 baud. */
  UART1->UART_CR   = (UART_CR_RSTRX | UART_CR_RSTTX) |
                     (UART_CR_RXDIS | UART_CR_TXDIS);
  UART1->UART_IDR  = 0xFFFFFFFF;
  UART1->UART_BRGR   = BAUD(115200);
  UART1->UART_MR   =  (0x4 <<  9);        /* (UART) No Parity                 */
  UART1->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
  UART1->UART_CR   = UART_CR_RXEN | UART_CR_TXEN;
#endif

  PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect             */
#endif
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int c) {

#ifdef __DBG_ITM
    ITM_SendChar(c);
#else
  #ifdef __UART
    while ((UART1->UART_SR & UART_SR_TXEMPTY) == 0);
    UART1->UART_THR = c;
  #endif
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
  #ifdef __UART
    while((UART1->UART_SR & UART_SR_RXRDY) == 0);
    return UART1->UART_RHR;
  #endif
#endif
}
