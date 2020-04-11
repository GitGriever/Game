/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <sam3u.h>



#if   defined FLASH_BANK && (FLASH_BANK == 0)
  #define LED     0x1
#elif defined FLASH_BANK && (FLASH_BANK == 1)
  #define LED     0x2
#else
  #define LED     0x3

#endif

/*----------------------------------------------------------------------------
  Function that delays
 *----------------------------------------------------------------------------*/
void wait (unsigned int n) {                        /* wait Function          */
  unsigned int i;

  for (i = 0; i < n; i++);
}

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {                                   /* Main Program           */

  SystemCoreClockUpdate();

  wait(0x50000); /* wait a while */

  PMC->PMC_WPMR = 0x504D4300;                       /* Disable write protect  */
  PMC->PMC_PCER0 = 0x00000800;                      /* enable clock for PIOB  */  

  PIOB->PIO_PER    =  LED;                          /* Setup Pin for used LED */
  PIOB->PIO_OER    =  LED;
  PIOB->PIO_PUDR   =  LED;

  while (1) {                                       /* Loop forever           */
    PIOB->PIO_CODR = LED;                           /* LED on                 */
    wait(0x50000);
    PIOB->PIO_SODR = LED;                           /* LED off                */
    wait(0x50000);             
  }
}
