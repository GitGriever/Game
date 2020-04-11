/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *            __UART      - UART interface
 *                        - USART (USART1) interface  (default)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <sam3u.h>
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

  PMC->PMC_WPMR = 0x504D4300;                /* Disable write protect         */
#ifdef __UART
  PMC->PMC_PCER0 = ((1UL << ID_PIOA) |       /* enable PIOA clock             */
                    (1UL << ID_UART)  );     /* enable UART clock             */  

  /* Configure UART0 Pins (PA12 = TX, PA10 = RX). */
  PIOA->PIO_IDR    =
  PIOA->PIO_PUDR   =
  PIOA->PIO_PDR    =  (PIO_PA11A_URXD | PIO_PA12A_UTXD);
  PIOA->PIO_ABSR  &= ~(PIO_PA11A_URXD | PIO_PA12A_UTXD);

  /* Configure UART for 115200 baud. */
  UART->UART_CR    = (UART_CR_RSTRX | UART_CR_RSTTX) |
                     (UART_CR_RXDIS | UART_CR_TXDIS);
  UART->UART_IDR   = 0xFFFFFFFF;
  UART->UART_MR    = (0x4 <<  9);            /* (UART) No Parity              */
  UART->UART_BRGR  = BAUD(115200);
  UART->UART_PTCR  = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
  UART->UART_CR    = UART_CR_RXEN     | UART_CR_TXEN;
#else
  PMC->PMC_PCER0 = ((1UL << ID_PIOA  ) |     /* enable PIOA   clock           */
                    (1UL << ID_USART1)  );   /* enable USART1 clock           */  

  /* Configure USART1 Pins (PA20 = TX, PA21 = RX). */
  PIOA->PIO_IDR    =
  PIOA->PIO_PUDR   =
  PIOA->PIO_PDR    =  (PIO_PA21A_RXD1 | PIO_PA20A_TXD1);
  PIOA->PIO_ABSR  &= ~(PIO_PA21A_RXD1 | PIO_PA20A_TXD1);

  /* Configure USART1 for 115200 baud. */
  USART1->US_CR    = US_CR_RSTRX   | US_CR_RSTTX;
  USART1->US_IDR   = 0xFFFFFFFF;
  USART1->US_BRGR  = BAUD(115200);
  USART1->US_MR    = (0x0 <<  0) |           /* Normal                        */
                     (0x0 <<  4) |           /* Clock                         */
                     (0x3 <<  6) |           /* Character Length: 8 bits      */
                     (0x4 <<  9) |           /* No Parity                     */
                     (0x0 << 12) |           /* 1 stop bit                    */
                     (0x0 << 14) ;           /* Normal Mode                   */
  USART1->US_PTCR  = US_PTCR_RXTDIS | US_PTCR_TXTDIS;
  USART1->US_CR    = US_CR_RXEN     | US_CR_TXEN;
#endif
  PMC->PMC_WPMR = 0x504D4301;                /* Enable write protect          */

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
    while (!(UART->UART_SR  & UART_SR_TXRDY));
    UART->UART_THR = c;
  #else
    while (!(USART1->US_CSR & US_CSR_TXRDY));
    USART1->US_THR = c;
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
    while (!(UART->UART_SR  & UART_SR_RXRDY));
    return (UART->UART_RHR);
  #else
    while (!(USART1->US_CSR & US_CSR_RXRDY));
    return (USART1->US_RHR);
  #endif
#endif
}
