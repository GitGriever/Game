/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: MCBTMPM395 Low level serial functions
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2010 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "TMPM395.h"                             /* TMPM395 definitions       */
#include "Serial.h"

/*SC0 Pins */
#define SC0_TXD        (1UL << 0)                /* PE.0 = TXD                */
#define SC0_RXD        (1UL << 1)                /* PE.1 = RXD                */
#define SC0_PINS       (SC0_RXD | SC0_TXD)

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_init (void) {
   
  TSB_PE->IE     &= ~SC0_TXD;                    /* Disable Input  PE0        */
  TSB_PE->CR     |=  SC0_TXD;                    /* Enable  Output PE0        */
  TSB_PE->IE     |=  SC0_RXD;                    /* Enable  Input  PE1        */
  TSB_PE->CR     &= ~SC0_RXD;                    /* Disable Output PE1        */
  TSB_PE->FR1    |=  SC0_PINS;                   /* PE0..1 used for SC0 TX,RX */  
  TSB_PE->FR2    |=  0;

  TSB_SC0->EN    =  (1UL << 0);                  /* enable SC0                */
                                                 /* 115200 @ 20MHz (fPeriph/2)*/
  TSB_SC0->BRADD =   ( 5UL << 0);                /* K = 5                     */ 
  TSB_SC0->BRCR  =  (( 1UL << 6) |               /* set BR0ADDE               */
                      ( 0UL << 4) |              /* select T1 clock           */
                      ( 2UL << 0) );             /* N =  2                    */ 
  TSB_SC0->CR    =     0;                        /* No Parity                 */
  TSB_SC0->MOD0  =  (( 1UL << 0) |               /* select Baudrate Generator */
                      ( 2UL << 2) );             /* 8bit data                 */ 
  TSB_SC0->MOD2  =     0;                        /* 1 Stop bit                */
  TSB_SC0->MOD1  =   ( 1UL << 4);                /* Enable TX                 */
  TSB_SC0->MOD0 |=   ( 1UL << 5);                /* Enable RX                 */
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_putChar (int c) {

  while ((TSB_SC0->MOD2 & 0xA0) != 0x80);
  TSB_SC0->BUF = (unsigned char)c;

  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER_getChar (void) {

  while (!(TSB_SC0->MOD2 & 0x40));

  return (TSB_SC0->BUF);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (non blocking read)
 *----------------------------------------------------------------------------*/
int SER_getChar_nb (void) {

  if (TSB_SC0->MOD2 & 0x40)
    return (TSB_SC0->BUF);
  else
    return 0;
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
void SER_putString (unsigned char *s) {

  while (*s != 0) {
    SER_putChar(*s++);
  }
}
