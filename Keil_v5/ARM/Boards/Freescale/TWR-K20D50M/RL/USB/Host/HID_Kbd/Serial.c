/*------------------------------------------------------------------------------
 *      RL-ARM
 *------------------------------------------------------------------------------
 *      Name:    Serial.c
 *      Purpose: Serial Input Output for Freescale K60
 *      Note(s): Possible defines to select the used communication interface:
 *               __DBG_ITM   - ITM SWO interface (USART is used by default)
 *                           - UART1 interface  (default)
 *------------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <MK20D5.H>
#include "Serial.h"

#ifdef __DBG_ITM
volatile int32_t ITM_RxBuffer;
#endif


/*------------------------------------------------------------------------------
 *       SER_Init:  Initialize Serial Interface
 *----------------------------------------------------------------------------*/

void SER_Init (void) {

#ifdef __DBG_ITM
  ITM_RxBuffer = ITM_RXBUFFER_EMPTY;
#else
  /* Configure UART                                                           */
  /* Settings: 115200 baud @ 48MHz, 8 data bits, 1 stop bit, no flow control  */

  SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;   /* Enable UART 1 gate clocking        */
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;   /* Enable PORTE gate clocking         */ 

  PORTE->PCR[0] = (3UL <<  8);          /* Pin mux configured as ALT3         */
  PORTE->PCR[1] = (3UL <<  8);          /* Pin mux configured as ALT3         */

  UART1->BDH =  0;
  UART1->BDL = 26;                      /* SBR = 26                           */

  UART1->C1  =  0; 
  UART1->C4  =  1;                      /* BRFA = 1                           */

  UART1->C2 = UART_C2_RE_MASK |         /* Receiver enable                    */
              UART_C2_TE_MASK;          /* Transmitter enable                 */
  UART1->S2 = 0;
  UART1->C3 = 0;

  UART1->RWFIFO = UART_RWFIFO_RXWATER(1);
  UART1->TWFIFO = UART_TWFIFO_TXWATER(0);
#endif
}


/*------------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to the Serial Port
 *----------------------------------------------------------------------------*/

int32_t SER_PutChar (int32_t ch) {

#ifdef __DBG_ITM
  int i;
  ITM_SendChar (ch & 0xFF);
  for (i = 10000; i; i--);
#else
  while (!(UART1->S1 & UART_S1_TDRE_MASK));
  UART1->D = (ch & 0xFF);
#endif
  return (ch);
}


/*------------------------------------------------------------------------------
 *       SER_GetChar:  Read a character from the Serial Port (non-blocking)
 *----------------------------------------------------------------------------*/

int32_t SER_GetChar (void) {

#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return (ITM_ReceiveChar());
#else
  if (UART1->S1 & UART_S1_RDRF_MASK) 
    return (UART1->D);
#endif
  return (-1);
}

/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
