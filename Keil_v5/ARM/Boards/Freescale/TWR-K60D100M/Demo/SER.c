/*-----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): Possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *                        - UART3 interface  (default)
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/
#include <MK60D10.H>
#include "Ser.h"

#ifdef __DBG_ITM
volatile int32_t ITM_RxBuffer;          /* CMSIS Debug Input                  */
#endif


/*-----------------------------------------------------------------------------
 *       SER_Init:  Initialize Serial Interface
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifdef __DBG_ITM
  ITM_RxBuffer = ITM_RXBUFFER_EMPTY;

#else
  /* Configure UART3 as ALT3 function on pins PCT16 and PTC17                 */
  /* Settings: 115200 baud @ 48MHz, 8 data bits, 1 stop bit, no flow control  */

  SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;   /* Enable UART 3 gate clocking        */
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;   /* Enable PORTA gate clocking         */ 

  PORTC->PCR[16] = (3UL <<  8);         /* Pin mux configured as ALT3         */
  PORTC->PCR[17] = (3UL <<  8);         /* Pin mux configured as ALT3         */

  UART3->BDH =  0;
  UART3->BDL = 26;                      /* SBR = 26                           */

  UART3->C1  =  0; 
  UART3->C4  =  1;                      /* BRFA = 1                           */

  UART3->C2 = UART_C2_RE_MASK |         /* Receiver enable                    */
              UART_C2_TE_MASK;          /* Transmitter enable                 */
  UART3->S2 = 0;
  UART3->C3 = 0;

  UART3->RWFIFO = UART_RWFIFO_RXWATER(1);
  UART3->TWFIFO = UART_TWFIFO_TXWATER(0);
#endif
}


/*-----------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to the Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_PutChar (int32_t ch) {
#ifdef __DBG_ITM
  int i;
  ITM_SendChar (ch & 0xFF);
  for (i = 10000; i; i--);
#else
  while (!(UART3->S1 & UART_S1_TDRE_MASK));
  UART3->D = (ch & 0xFF);
#endif  
  return (ch & 0xFF);
}


/*-----------------------------------------------------------------------------
 *       SER_GetChar:  Read a character from the Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_GetChar (void) {
#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return ITM_ReceiveChar();
#else
  if (UART3->S1 & UART_S1_RDRF_MASK) {
    return (UART3->D);
  }
#endif
  return (-1);
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
