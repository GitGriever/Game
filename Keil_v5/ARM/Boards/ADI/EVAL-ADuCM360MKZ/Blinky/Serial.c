/*-----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Serial Input Output for SONiX SN32F707
 * Note(s): Possible defines to select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *                        - UART is used by default
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <ADuCM360.H>
#include "Serial.h"

#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */
#endif


/*-----------------------------------------------------------------------------
 *       SER_Init:  Initialize Serial Interface
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifndef __DBG_ITM

  pADI_GP0->GPCON = ((3UL << (1*2)) |                   /* P0.1 = UART RX     */
                     (3UL << (2*2))  );                 /* P0.2 = UART TX     */

  pADI_UART->COMLCR = ((0UL << 3) |                     /* no Parity          */
                       (0UL << 2) |                     /* 1 Stop bit         */
                       (3UL << 0)  );                   /* 8 Data bits        */
                                                        /* 115200Baud @ 16MHz */
  pADI_UART->COMDIV =  (2UL << 0);                      /* COMDIV = 2         */
  pADI_UART->COMFBR = ((  1UL << 15) |                  /* enable FBRG        */
                       (  2UL << 11) |                  /* DIVM = 2           */
                       (384UL <<  0)  );                /* DIVN = 384         */
#endif
}


/*-----------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to the Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_PutChar (int32_t ch) {

#ifdef __DBG_ITM
  ITM_SendChar (ch & 0xFF);
#else
  while (!(pADI_UART->COMLSR & 0x20));
  pADI_UART->COMTX = ch;
#endif  

  return (ch);
}


/*-----------------------------------------------------------------------------
 *       SER_GetChar:  Read a character from the Serial Port (non-blocking)
 *----------------------------------------------------------------------------*/
int32_t SER_GetChar (void) {

#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return ITM_ReceiveChar();
#else
  if (pADI_UART->COMLSR & 0x01) {
    return (pADI_UART->COMRX);
  }
#endif
  return (-1);
}
