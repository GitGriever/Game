/*-----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Serial Input Output for SONiX SN32F707
 * Note(s): Possible defines to select the used communication interface:
 *          __UART0  - UART0 interface
 *                    - UART1 is used by default
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

#include <SN32F100.h>
#include "Serial.h"

#ifdef __UART0
  #define SN_UARTx     SN_UART0
#else
  #define SN_UARTx     SN_UART1
#endif

/*-----------------------------------------------------------------------------
 *       SER_Init:  Initialize Serial Interface
 *----------------------------------------------------------------------------*/
void SER_Init (void) {

  SN_SYS1->AHBCLKEN |= (1UL <<  3);     /* enable clock for GPIO              */

#ifdef __UART0
  SN_SYS1->AHBCLKEN |= (1UL << 16);     /* enable clock for USART0            */
#else
  SN_SYS1->AHBCLKEN |= (1UL << 17);     /* enable clock for USART1            */
#endif

  /* Init USARTx                                                              */
  SN_UARTx->LC     = 0x83;              /* 8 bits, no Parity, 1 Stop bit      */
  SN_UARTx->DLM    = 0;
  SN_UARTx->DLL    = 4;                 /* 115200 Baudrate @ 12 MHz  IHRC     */
  SN_UARTx->FD     = 0x75;              /* FR 1.625, DIVADDVAL=5, MULVAL=8    */
  SN_UARTx->LC     = 0x03;              /* DLAB = 0                           */

  SN_UARTx->CTRL = ((1UL << 7) |        /* Transmitter enable                 */
                    (1UL << 6) |        /* receiver enable                    */
                    (1UL << 0)  );      /* USART enable                       */
}


/*-----------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to the Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_PutChar (int32_t ch) {

  while (!(SN_UARTx->LS & 0x20));
  SN_UARTx->TH = ch;

  return (ch);
}


/*-----------------------------------------------------------------------------
 *       SER_GetChar:  Read a character from the Serial Port (non-blocking)
 *----------------------------------------------------------------------------*/
int32_t SER_GetChar (void) {

  if (SN_UARTx->LS & 0x01) {
    return (SN_UARTx->RB);
  }

  return (-1);
}
