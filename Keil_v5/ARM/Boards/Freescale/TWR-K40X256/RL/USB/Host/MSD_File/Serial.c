/*------------------------------------------------------------------------------
 *      RL-ARM
 *------------------------------------------------------------------------------
 *      Name:    Serial.c
 *      Purpose: Serial Input Output for Freescale K40
 *      Note(s): Possible defines to select the used communication interface:
 *               __DBG_ITM   - ITM SWO interface (UART is used by default)
 *                           - UART3 interface   (default)
 *------------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <MK40N512MD100.H>
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
  SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;   /* Enable UART 3 gate clocking        */
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;   /* Enable PORTC gate clocking         */ 

  PORTC->PCR[17] = (3UL <<  8);         /* Pin mux configured as ALT3         */
  PORTC->PCR[16] = (3UL <<  8);         /* Pin mux configured as ALT3         */

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


/*------------------------------------------------------------------------------
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
  if (UART3->S1 & UART_S1_RDRF_MASK) 
    return (UART3->D);
#endif
  return (-1);
}

/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
