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
 * Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <sam3u.h>

#include "Serial.h"

#ifdef __DBG_ITM
volatile int32_t ITM_RxBuffer;
#endif

/* Clock Definitions */
#define BAUD(b) ((SystemCoreClock + 8*b)/(16*b))

/*-----------------------------------------------------------------------------
 *       SER_Init:  Initialize Serial Interface
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifdef __DBG_ITM
  ITM_RxBuffer = ITM_RXBUFFER_EMPTY;

#else
  PMC->PMC_WPMR = 0x504D4300;           /* Disable write protect              */
 #ifdef __UART
  PMC->PMC_PCER0 = (1UL << ID_PIOA) |   /* Enable PIOA clock                  */
                   (1UL << ID_UART) ;   /* Enable UART clock                  */  

  /* Configure UART for 115200 baud. */
  PIOA->PIO_IDR    =  (PIO_PA11A_URXD | PIO_PA12A_UTXD);
  PIOA->PIO_PUDR   =  (PIO_PA11A_URXD | PIO_PA12A_UTXD);
  PIOA->PIO_PDR    =  (PIO_PA11A_URXD | PIO_PA12A_UTXD);
  PIOA->PIO_ABSR  &= ~(PIO_PA11A_URXD | PIO_PA12A_UTXD);

  UART->UART_CR    = UART_CR_RSTRX   | UART_CR_RSTTX;
  UART->UART_IDR   = 0xFFFFFFFF;
  UART->UART_BRGR  = BAUD(115200);
  UART->UART_MR    = (0x4 <<  9);       /* (UART) No Parity                   */
  UART->UART_PTCR  = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
  UART->UART_CR    = UART_CR_RXEN     | UART_CR_TXEN;

 #else
  PMC->PMC_PCER0 = (1UL << ID_PIOA  ) | /* Enable PIOA   clock                */
                   (1UL << ID_USART1) ; /* Enable USART1 clock                */

  /* Configure USART1 for 115200 baud. */
  PIOA->PIO_IDR    =  (PIO_PA21A_RXD1 | PIO_PA20A_TXD1);
  PIOA->PIO_PUDR   =  (PIO_PA21A_RXD1 | PIO_PA20A_TXD1);
  PIOA->PIO_PDR    =  (PIO_PA21A_RXD1 | PIO_PA20A_TXD1);
  PIOA->PIO_ABSR  &= ~(PIO_PA21A_RXD1 | PIO_PA20A_TXD1);

  USART1->US_CR    = US_CR_RSTRX   | US_CR_RSTTX;
  USART1->US_IDR   = 0xFFFFFFFF;
  USART1->US_BRGR  = BAUD(115200);
  USART1->US_MR    = (0x0 <<  0) |      /* (USART) Normal                     */
                     (0x0 <<  4) |      /* (USART) Clock                      */
                     (0x3 <<  6) |      /* (USART) Character Length: 8 bits   */
                     (0x4 <<  9) |      /* (USART) No Parity                  */
                     (0x0 << 12) |      /* (USART) 1 stop bit                 */
                     (0x0 << 14) ;      /* (USART) Normal Mode                */
  USART1->US_PTCR  = US_PTCR_RXTDIS | US_PTCR_TXTDIS;
  USART1->US_CR    = US_CR_RXEN     | US_CR_TXEN;
 #endif
  PMC->PMC_WPMR = 0x504D4301;           /* Enable write protect               */
#endif
}


/*-----------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to the Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int ch) {
#ifdef __DBG_ITM
  int32_t i;
  ITM_SendChar (ch & 0xFF);
  for (i = 10000; i; i--);
#else
  #ifdef __UART
    while (!(UART->UART_SR  & UART_SR_TXRDY));
    UART->UART_THR = (ch & 0xFF);
  #else
    while (!(USART1->US_CSR & US_CSR_TXRDY));
    USART1->US_THR = (ch & 0xFF);
  #endif
#endif
  return (ch & 0xFF);
}


/*-----------------------------------------------------------------------------
 *       SER_GetChar:  Read a character from the Serial Port
 *----------------------------------------------------------------------------*/
int SER_GetChar (void) {
#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return ITM_ReceiveChar();
#else
  #ifdef __UART
  if (UART->UART_SR  & UART_SR_RXRDY) {
    return (UART->UART_RHR);
  }
  #else
  if (USART1->US_CSR & US_CSR_RXRDY) {
    return (USART1->US_RHR);
  }
  #endif
  return (-1);
#endif
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
