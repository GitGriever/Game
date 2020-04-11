/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used interface:
 *            __DBG_ITM   - output redirect to SWO
 *                        - output redirect to SC0 (default)
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

#include <TMPM369.H>
#include "Serial.h"

#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */
#endif

/*SC0 Pins */
#define SC0_TXD        (1UL << 2)                /* PE.2 = TXD                */
#define SC0_RXD        (1UL << 1)                /* PE.1 = RXD                */
#define SC0_PINS       (SC0_RXD | SC0_TXD)

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
      Baudrate calculation with Baudrate generator:
      Baudrate = fc / 16 / (N + (16-K)/16) / Tx
 *----------------------------------------------------------------------------*/
void SER_Init (void) {

#ifndef __DBG_ITM
  TSB_PE->IE     &= ~SC0_TXD;                    /* Disable Input  PE0        */
  TSB_PE->CR     |=  SC0_TXD;                    /* Enable  Output PE0        */
  TSB_PE->IE     |=  SC0_RXD;                    /* Enable  Input  PE1        */
  TSB_PE->CR     &= ~SC0_RXD;                    /* Disable Output PE1        */
  TSB_PE->FR1    |=  SC0_PINS;                   /* PE0..1 used for SC0 TX,RX */
  TSB_PE->FR2    |=  0;

  TSB_SC0->EN    =  (1UL << 0);                  /* enable SC0                */
                                                 /* 115200 @ 80MHz (fPeriph/1)*/
  TSB_SC0->BRADD =   ( 9UL << 0);                /* K = 9                     */
  TSB_SC0->BRCR  =  (( 1UL << 6) |               /* set BR0ADDE               */
                     ( 1UL << 4) |               /* select T4 clock           */
                     ( 5UL << 0) );              /* N =  5                    */
  TSB_SC0->CR    =     0;                        /* No Parity                 */
  TSB_SC0->MOD0  =  (( 1UL << 0) |               /* select Baudrate Generator */
                     ( 2UL << 2) );              /* 8bit data                 */
  TSB_SC0->MOD2  =     0;                        /* 1 Stop bit                */
  TSB_SC0->MOD1  =   ( 1UL << 4);                /* Enable TX                 */
  TSB_SC0->MOD0 |=   ( 1UL << 5);                /* Enable RX                 */
#endif
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int c) {

#ifdef __DBG_ITM
  ITM_SendChar(c);
#else
  while ((TSB_SC0->MOD2 & 0xA0) != 0x80);
  TSB_SC0->BUF = (unsigned char)c;
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
  while (!(TSB_SC0->MOD2 & 0x40));
  return (TSB_SC0->BUF);
#endif
}
