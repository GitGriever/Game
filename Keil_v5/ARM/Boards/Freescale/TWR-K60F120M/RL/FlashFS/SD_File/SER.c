/*------------------------------------------------------------------------------
 * Name:    SER.c
 * Purpose: Serial I/O functions for the 
 *          Freescale Kinetis TWR-K60F120M board
 * Note(s): 
 *------------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <MK60F12.H>
#include "SER.h"

#ifdef __DBG_ITM
volatile int32_t ITM_RxBuffer;
#endif


/*------------------------------------------------------------------------------
 * SER_Init:        Initialize Serial Interface
 *----------------------------------------------------------------------------*/

void SER_Init (void) {

#ifdef __DBG_ITM
  ITM_RxBuffer = ITM_RXBUFFER_EMPTY;
#else
  /* Configure UART                                                           */
  /* Settings: 115200 baud @ 60MHz, 8 data bits, 1 stop bit, no flow control  */
  SIM->SCGC1 |= SIM_SCGC1_UART5_MASK;   /* Enable UART gate clocking          */
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;   /* Enable PORTE gate clocking         */ 

  PORTE->PCR[ 8] = (3UL <<  8);         /* Pin mux configured as ALT3         */
  PORTE->PCR[ 9] = (3UL <<  8);         /* Pin mux configured as ALT3         */

  UART5->BDH =  0;
  UART5->BDL = 32;                      /* SBR = 32                           */

  UART5->C1  =  0; 
  UART5->C4  =  1;                      /* BRFA = 1                           */

  UART5->C2  =  UART_C2_RE_MASK |       /* Receiver enable                    */
                UART_C2_TE_MASK;        /* Transmitter enable                 */
  UART5->S2  =  0;
  UART5->C3  =  0;

  UART5->RWFIFO = UART_RWFIFO_RXWATER(1);
  UART5->TWFIFO = UART_TWFIFO_TXWATER(0);
#endif
}


/*------------------------------------------------------------------------------
 * SER_Uninit:      Uninitialize Serial Interface
 *----------------------------------------------------------------------------*/

void SER_Uninit (void) {

#ifndef __DBG_ITM
  UART5->C2  = 0;
  UART5->C4  = 0;
  UART5->BDL = 4;

  PORTE->PCR[ 8] = 0;
  PORTE->PCR[ 9] = 0;

  SIM->SCGC1 &= ~SIM_SCGC1_UART5_MASK;
#endif
}


/*------------------------------------------------------------------------------
 * SER_PutChar:     Write a character to the Serial Port
 *----------------------------------------------------------------------------*/

int32_t SER_PutChar (int32_t ch) {

#ifdef __DBG_ITM
  int i;
  ITM_SendChar (ch & 0xFF);
  for (i = 10000; i; i--);
#else
  while (!(UART5->S1 & UART_S1_TDRE_MASK));
  UART5->D = (ch & 0xFF);
#endif
  return (ch);
}


/*------------------------------------------------------------------------------
 * SER_GetChar:     Read a character from the Serial Port (non-blocking)
 *----------------------------------------------------------------------------*/

int32_t SER_GetChar (void) {

#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return (ITM_ReceiveChar());
#else
  if (UART5->S1 & UART_S1_RDRF_MASK) 
    return (UART5->D);
#endif
  return (-1);
}


/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
