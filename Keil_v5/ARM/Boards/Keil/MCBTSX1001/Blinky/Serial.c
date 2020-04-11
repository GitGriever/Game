/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines used for board MCBTSX1001
 * Note(s): possible defines select the used communication interface:
 *                   - UART interface (default)
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

#include <TSX1001.h>
#include "Serial.h"


/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
  uint32_t dlv;

  SystemCoreClockUpdate();
  dlv = ((SystemCoreClock / 16) / 9600);

  GPIO2->DATA[0].WORD |= (1UL << 16);    /* enable baud rate clock */

  UART->LCR =  UART_DLAB        |
               UART_1_STOP_BITS |
               UART_8_BITS;
  UART->DLL = ((dlv     ) & 0xFF);
  UART->DLM = ((dlv >> 8) & 0xFF);
  UART->LCR =  UART_1_STOP_BITS |
               UART_8_BITS;

}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int c) {

  while (!(UART->LSR & (1UL << 6)));
  UART->THR = (c & 0x1FF);

  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port
 *----------------------------------------------------------------------------*/
int SER_GetChar (void) {

  if (UART->LSR & (1UL << 0))
    return (UART->RBR & 0xFF);

  return (-1);
}
