/*------------------------------------------------------------------------------
 *      RL-ARM
 *------------------------------------------------------------------------------
 *      Name:    Serial.c
 *      Purpose: Serial Input Output for Infineon XMC4500
 *------------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2012 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "XMC4500.h"                         /* XMC4500 definitions           */
#include "Serial.h"

volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */

/*------------------------------------------------------------------------------
 *       SER_Init:  Initialize Serial Interface
 *----------------------------------------------------------------------------*/

void SER_Init (void) {
}


/*------------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to the Serial Port
 *----------------------------------------------------------------------------*/

int32_t SER_PutChar (int32_t ch) {
  int i;
  ITM_SendChar (ch & 0xFF);
  for (i = 5000; i; i--);
  return (ch & 0xFF);
}


/*------------------------------------------------------------------------------
 *       SER_GetChar:  Read a character from the Serial Port (non-blocking)
 *----------------------------------------------------------------------------*/

int32_t SER_GetChar (void) {

  if (ITM_CheckChar()) 
    return (ITM_ReceiveChar());
  return (-1);
}

/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
