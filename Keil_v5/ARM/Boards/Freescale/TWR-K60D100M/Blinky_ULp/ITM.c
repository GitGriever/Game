/*----------------------------------------------------------------------------
 * Name:    ITM.c
 * Purpose: Low Level ITM Routines
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/
#include <MK60D10.H>

volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */



/*----------------------------------------------------------------------------
  Write character to ITM Port
 *----------------------------------------------------------------------------*/
int sendchar (int c) {

  ITM_SendChar(c);
  return (c);
}


/*----------------------------------------------------------------------------
  Read character from ITM Port   (blocking read)
 *----------------------------------------------------------------------------*/
int getkey (void) {

  while (ITM_CheckChar() != 1) __NOP();
  return (ITM_ReceiveChar());
}
