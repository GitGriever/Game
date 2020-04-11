/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *                        - USART interface  (default)
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

#include "HT32F125x.h"
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

  HT_AFIO->GPACFGR &= ~((3UL <<  8*2) |      /* configure PA8  as GPIO        */
                        (3UL <<  9*2) );     /* configure PA9  as GPIO        */
  HT_AFIO->GPACFGR |=  ((2UL <<  8*2) |      /* configure PA8  as USART RX    */
                        (2UL <<  9*2) );     /* configure PA9  as USART TX    */

  /* configure USART                                                          */
  HT_CKCU->APBCCR0 |=  ((1UL <<   8 ) );     /* enable USART Clock            */

  HT_USART->LCR  =  ((1UL <<   0) |          /* set word length to 8 Bit      */
                     (0UL <<   2) |          /* 1 stop bit                    */
                     (0UL <<   3) );         /* no parity                     */
  HT_USART->MCR  =  ((0UL <<   0) |          /* normal mode                   */
                     (0UL <<   2) );         /* LSB first                     */

  HT_USART->DLR  =  (72000000UL / 115200);   /* 115200 @ 72MHz                */

#endif
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int c) {

#ifdef __DBG_ITM
    ITM_SendChar(c);
#else
  while (!(HT_USART->LSR & (1UL << 5)));
  HT_USART->TBR = (c & 0x1FF);
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
  while (!(HT_USART->LSR & (1UL << 0)));
  return (HT_USART->RBR & 0x1FF);
#endif
}


/*----------------------------------------------------------------------------
  Check if a character is received
 *----------------------------------------------------------------------------*/
int SER_CheckChar (void) {

#ifdef __DBG_ITM
  return (ITM_CheckChar());
#else
  if (HT_USART->LSR & (1UL << 0))
    return (1);
  else
    return (0);
#endif
}
