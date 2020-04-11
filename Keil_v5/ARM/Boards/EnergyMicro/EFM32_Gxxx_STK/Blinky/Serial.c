/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
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

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "Serial.h"

#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */
#endif


/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifndef __DBG_ITM
  ;
#endif
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_PutChar (int32_t ch) {

#ifdef __DBG_ITM
  ITM_SendChar(ch);
#else
  ;
#endif
  return (ch);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (non-blocking)
 *----------------------------------------------------------------------------*/
int32_t SER_GetChar (void) {

#ifdef __DBG_ITM
  if (ITM_CheckChar()) { 
    return (ITM_ReceiveChar());
  }
#else
  ;
#endif
  return (-1);
}
