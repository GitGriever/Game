/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *            __USART     - USART  (USART2)  interface
 *                        - disabled  (default)
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

#include <SAM4L.h>
#include "Serial.h"

#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */
#endif

/* Clock Definitions */
#define BAUD(b) ((40000000UL)/(16*b))

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifndef __DBG_ITM

#ifdef __USART
  PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu) |   /* Disable write protect           */
                  PM_UNLOCK_ADDR((uint32_t)&PM->PM_PBAMASK - (uint32_t)PM);
  PM->PM_PBAMASK |= PM_PBAMASK_USART2;    /* enable USART2 clock              */

  /* Configure USART2 Pins (PC12 = TX, PC11 = RX). */
  GPIO->GPIO_PORT[2].GPIO_PMR0S  = (GPIO_PC11 | GPIO_PC12);
  GPIO->GPIO_PORT[2].GPIO_PMR1C  = (GPIO_PC11 | GPIO_PC12);
  GPIO->GPIO_PORT[2].GPIO_PMR2C  = (GPIO_PC11 | GPIO_PC12);

  GPIO->GPIO_PORT[2].GPIO_GPERC  = (GPIO_PC11 | GPIO_PC12);

  /* Configure USART2 for 115200 baud. */
  USART2->US_WPMR = US_WPMR_WPKEY(0x555341u) | US_WPMR_WPEN_0;

  USART2->US_CR   = (US_CR_USART_RSTRX | US_CR_USART_RSTTX) |
                    (US_CR_USART_RXDIS | US_CR_USART_TXDIS);
  USART2->US_IDR  = 0xFFFFFFFF;
  USART2->US_BRGR = BAUD(115200);
  USART2->US_MR   = (0x0 <<  0) |         /* (USART) Normal                   */
                    (0x0 <<  4) |         /* (USART) Clock                    */
                    (0x3 <<  6) |         /* (USART) Character Length: 8 bits */
                    (0x4 <<  9) |         /* (USART) No Parity                */
                    (0x0 << 12) |         /* (USART) 1 stop bit               */
                    (0x0 << 14) ;         /* (USART) Normal Mode              */
  USART2->US_CR    = US_CR_USART_RXEN | US_CR_USART_TXEN;
#endif

#endif
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int c) {

#ifdef __DBG_ITM
    ITM_SendChar(c);
#else
  #ifdef __USART
    while ((USART2->US_CSR & US_CSR_TXEMPTY) == 0);
    USART2->US_THR = c;
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
  #ifdef __USART
    while((USART2->US_CSR & US_CSR_RXRDY) == 0);
    return USART2->US_RHR;
  #else
    return -1;
  #endif
#endif
}
