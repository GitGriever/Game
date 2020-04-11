/*-----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Serial Input Output for SONiX SN32F707
 * Note(s): Possible defines to select the used communication interface:
 *          __USART0  - USART0 interface
 *                    - USART1 is used by default
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <SN32F710.h>
#include "Serial.h"

#ifdef __USART0
  #define SN_USARTx     SN_USART0
#else
  #define SN_USARTx     SN_USART1
#endif

/*-----------------------------------------------------------------------------
 *       SER_Init:  Initialize Serial Interface
 *----------------------------------------------------------------------------*/
void SER_Init (void) {

  SN_SYS1->AHBCLKEN |= (1UL <<  3);     /* enable clock for GPIO              */

#ifdef __USART0
  SN_SYS1->AHBCLKEN |= (1UL << 16);     /* enable clock for USART0            */
#else
  SN_SYS1->AHBCLKEN |= (1UL << 17);     /* enable clock for USART1            */
#endif

  /* Init USARTx                                                              */
  SN_USARTx->LC     = 0x83;             /* 8 bits, no Parity, 1 Stop bit      */
  SN_USARTx->DLM    = 0;
  SN_USARTx->DLL    = 4;                /* 115200 Baudrate @ 12 MHz  IHRC     */
  SN_USARTx->FD     = 0x75;             /* FR 1.625, DIVADDVAL=5, MULVAL=8    */
  SN_USARTx->LC     = 0x03;             /* DLAB = 0                           */

  SN_USARTx->CTRL = ((1UL << 7) |       /* Transmitter enable                 */
                     (1UL << 6) |       /* receiver enable                    */
                     (0UL << 1) |       /* UART mode                          */
                     (1UL << 0)  );     /* USART enable                       */
}


/*-----------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to the Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_PutChar (int32_t ch) {

  while (!(SN_USARTx->LS & 0x20));
  SN_USARTx->TH = ch;

  return (ch);
}


/*-----------------------------------------------------------------------------
 *       SER_GetChar:  Read a character from the Serial Port (non-blocking)
 *----------------------------------------------------------------------------*/
int32_t SER_GetChar (void) {

  if (SN_USARTx->LS & 0x01) {
    return (SN_USARTx->RB);
  }

  return (-1);
}
