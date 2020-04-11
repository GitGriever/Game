/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *                        - USART0 interface  (default)
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

#include <HT32F175x_275x.h>
#include "Serial.h"

#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */
#endif


/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifndef __DBG_ITM

  /* configure Pins used for USART                                            */
  HT_CKCU->APBCCR0 |=  ((1UL <<  14 ) );     /* enable AFIO Clock             */

  HT_AFIO->GPCCFGR &= ~((3UL <<  8*2) |      /* configure PC8  as GPIO        */
                        (3UL << 10*2) );     /* configure PC10 as GPIO        */
  HT_AFIO->GPCCFGR |=  ((3UL <<  8*2) |      /* configure PC8  as USART0 TX   */
                        (3UL << 10*2) );     /* configure PC10 as USART0 RX   */

  /* configure USART                                                          */
  HT_CKCU->APBCCR0 |=  ((1UL <<   8 ) );     /* enable USART0 Clock           */

  HT_USART0->LCR =  ((1UL <<   0) |          /* set word length to 8 Bit      */
                     (0UL <<   2) |          /* 1 stop bit                    */
                     (0UL <<   3) );         /* no parity                     */
  HT_USART0->MCR =  ((0UL <<   2) );         /* HW flow control disable       */
  HT_USART0->MDR =  ((0UL <<   2) |          /* LSB First                     */
                     (0UL <<   0) );         /* Normal operation              */
  HT_USART0->FCR =  ((0UL <<   4) |          /* 0 1yte TX FIFO trigger level  */
                     (0UL <<   6) );         /* 1 byte RX FIFO trigger level  */

  HT_USART0->DLR =  (SystemCoreClock / 115200);  /* 115200 @ 72MHz            */
  
  HT_USART0->FCR =  ((1UL <<   8) |          /* USART TX enable               */
                     (1UL <<   9) );         /* USART RX enable               */
#endif
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int c) {

#ifdef __DBG_ITM
    ITM_SendChar(c);
#else
  while (!(HT_USART0->LSR & (1UL << 5)));
  HT_USART0->TBR = (c & 0x1FF);
#endif
  return (c);
}


/*----------------------------------------------------------------------------
  Read a character from the Serial Port (non-blocking)
 *----------------------------------------------------------------------------*/
int32_t SER_GetChar (void) {

#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return (ITM_ReceiveChar());
#else
  if (HT_USART0->LSR & (1UL << 0))
    return (HT_USART0->RBR & 0x1FF);
#endif
  return (-1);
}


/*----------------------------------------------------------------------------
  Check if a character is received
 *----------------------------------------------------------------------------*/
int SER_CheckChar (void) {

#ifdef __DBG_ITM
  return (ITM_CheckChar());
#else
  if (HT_USART0->LSR & (1UL << 0))
    return (1);
  else
    return (0);
#endif
}
