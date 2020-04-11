/*------------------------------------------------------------------------------
 *      RL-ARM
 *------------------------------------------------------------------------------
 *      Name:    Serial.c
 *      Purpose: Serial Input Output for Freescale K60
 *      Note(s): Possible defines to select the used communication interface:
 *                           - UART0 interface   (default)
 *------------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <MKL25Z4.H>
#include "Serial.h"


/*------------------------------------------------------------------------------
 *       SER_Init:  Initialize Serial Interface
 *----------------------------------------------------------------------------*/

void SER_Init (void) {

  /* Configure UART                                                           */
  /* Settings: 115200 baud @ 48MHz, 8 data bits, 1 stop bit, no flow control  */
  SIM->SOPT2 &=~SIM_SOPT2_UART0SRC_MASK ; /* MCGPLLCLK used as src            */
  SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1) |   /* MCGPLLCLK used as src            */
                SIM_SOPT2_PLLFLLSEL_MASK; /* Select MCGPLLCLK as clock        */

  SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;   /* Enable UART 1 gate clocking        */
  SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;   /* Enable PORTA gate clocking         */ 

  PORTA->PCR[14] = (3UL <<  8);         /* Pin mux configured as ALT3         */
  PORTA->PCR[15] = (3UL <<  8);         /* Pin mux configured as ALT3         */

  UART0->BDH =  0;
  UART0->BDL = 26;                      /* SBR = 26                           */

  UART0->C1  =  0; 
  UART0->C4  =  0x0F;                   /* OSR = 16                           */

  UART0->C2 = UART_C2_RE_MASK |         /* Receiver enable                    */
              UART_C2_TE_MASK;          /* Transmitter enable                 */
  UART0->S2 = 0;
  UART0->C3 = 0;
}


/*------------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to the Serial Port
 *----------------------------------------------------------------------------*/

int32_t SER_PutChar (int32_t ch) {

  while (!(UART0->S1 & UART_S1_TDRE_MASK));
  UART0->D = (ch & 0xFF);
  return (ch);
}


/*------------------------------------------------------------------------------
 *       SER_GetChar:  Read a character from the Serial Port (non-blocking)
 *----------------------------------------------------------------------------*/

int32_t SER_GetChar (void) {

  if (UART0->S1 & UART_S1_RDRF_MASK)
    return (UART0->D);
  return (-1);
}

/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
