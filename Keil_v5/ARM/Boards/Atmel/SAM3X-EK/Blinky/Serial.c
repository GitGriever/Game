/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *            __UART      - UART interface
 *                        - USART (USART0) interface  (default)
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

#include <sam3xa.h>
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
#ifndef __UART
  uint32_t i;
#endif

  PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

#ifdef __UART
  PMC->PMC_PCER0 = ((1UL << ID_PIOA) |    /* enable PIOA clock                */
                    (1UL << ID_UART)  );  /* enable UART clock                */

  /* Configure UART for 115200 baud. */
  PIOA->PIO_IDR    =
  PIOA->PIO_PUDR   =
  PIOA->PIO_PDR    =  (PIO_PA8A_URXD | PIO_PA9A_UTXD);
  PIOA->PIO_ABSR  &= ~(PIO_PA8A_URXD | PIO_PA9A_UTXD);

  UART->UART_CR    = UART_CR_RSTRX   | UART_CR_RSTTX;
  UART->UART_IDR   = 0xFFFFFFFF;
  UART->UART_BRGR  = BAUD(115200);
  UART->UART_MR    = (0x4 <<  9);         /* (UART) No Parity                 */
  UART->UART_PTCR  = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
  UART->UART_CR    = UART_CR_RXEN     | UART_CR_TXEN;
#else
  PMC->PMC_PCER0 = ((1UL << ID_PIOA  ) |  /* enable PIOA   clock              */
                    (1UL << ID_PIOE  ) |  /* enable PIOE   clock              */
                    (1UL << ID_USART0)  );/* enable USART0 clock              */

  /* Configure USART0 for 115200 baud. */
  PIOE->PIO_PER  =
  PIOE->PIO_OER  =
  PIOE->PIO_PUDR = (PIO_PE14);            /* Setup PA14 for USART0 enable     */
  PIOE->PIO_CODR = (PIO_PE14);            /* set Pin LOW to enable driver     */
  for (i = 0; i < 0x500; i++) __NOP();    /* delay to power up driver         */

  PIOA->PIO_IDR    =
  PIOA->PIO_PUDR   =
  PIOA->PIO_PDR    =  (PIO_PA10A_RXD0 | PIO_PA11A_TXD0);
  PIOA->PIO_ABSR  &= ~(PIO_PA10A_RXD0 | PIO_PA11A_TXD0);

  USART0->US_CR    = US_CR_RSTRX   | US_CR_RSTTX;
  USART0->US_IDR   = 0xFFFFFFFF;
  USART0->US_BRGR  = BAUD(115200);
  USART0->US_MR    = (0x0 <<  0) |        /* (USART) Normal                   */
                     (0x0 <<  4) |        /* (USART) Clock                    */
                     (0x3 <<  6) |        /* (USART) Character Length: 8 bits */
                     (0x4 <<  9) |        /* (USART) No Parity                */
                     (0x0 << 12) |        /* (USART) 1 stop bit               */
                     (0x0 << 14) ;        /* (USART) Normal Mode              */
  USART0->US_PTCR  = US_PTCR_RXTDIS | US_PTCR_TXTDIS;
  USART0->US_CR    = US_CR_RXEN     | US_CR_TXEN;
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
    while (!(UART->UART_SR  & UART_SR_TXRDY));
    UART->UART_THR = c;
  #else
    while (!(USART0->US_CSR & US_CSR_TXRDY));
    USART0->US_THR = c;
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
    while (!(USART0->US_CSR & US_CSR_RXRDY));
    return (USART0->US_RHR);
  #endif
#endif
}
