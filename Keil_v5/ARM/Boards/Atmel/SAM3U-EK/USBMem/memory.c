/*----------------------------------------------------------------------------
 * Name:    memory.c
 * Purpose: USB Memory Storage Demo
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <sam3u.h>

#include "type.h"

#include "usb.h"
#include "usbcfg_SAM3U.h"
#include "usbhw_SAM3U.h"
#include "mscuser.h"

#include "memory.h"




extern U8 Memory[MSC_MemorySize];                      /* MSC Memory in RAM */


/*------------------------------------------------------------------------------
  Switch on LEDs
 *------------------------------------------------------------------------------*/
void LED_On (unsigned int value) {

  PIOB->PIO_CODR = (value);                            /* Turn On  LED */
}


/*------------------------------------------------------------------------------
  Switch off LEDs
 *------------------------------------------------------------------------------*/
void LED_Off (unsigned int value) {

  PIOB->PIO_SODR = (value);                            /* Turn Off LED */
}


/*------------------------------------------------------------------------------
  Main Program
 *------------------------------------------------------------------------------*/
int main (void) {
  U32 n;

  SystemCoreClockUpdate();

  for (n = 0; n < MSC_ImageSize; n++) {                /* Copy Initial Disk Image */
    Memory[n] = DiskImage[n];                          /*   from Flash to RAM     */
  }

  /* Enable Clock for PIO */
  PMC->PMC_WPMR  = 0x504D4300;                         /* Disable write protect   */
  PMC->PMC_PCER0 = (1 << ID_PIOB);                     /* enable clock for LEDs   */
  PMC->PMC_WPMR  = 0x504D4301;                         /* Enable write protect    */

  PIOB->PIO_IDR  = 
  PIOB->PIO_OER  =
  PIOB->PIO_PER  = (PIO_PB1  | PIO_PB0 );              /* Setup Pins PB0..PB1 for LEDs */
  PIOB->PIO_SODR = (PIO_PB1  | PIO_PB0 );              /* Turn LEDs Off                */

  USB_Init();                                          /* USB Initialization      */
  USB_Connect(__TRUE);                                 /* USB Connect             */

  while (1);                                           /* Loop forever            */
}
