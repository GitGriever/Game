/*----------------------------------------------------------------------------
 * Name:    demo.c
 * Purpose: USB HID Demo
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008 - 2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <sam3u.h>

#include "type.h"

#include "usb.h"
#include "usbcfg_SAM3U.h"
#include "usbhw_SAM3U.h"

#include "demo.h"


U8 InReport;                                /* HID Input Report    */
                                            /*   Bit0..1: Buttons  */
                                            /*   Bit2..7: Reserved */

U8 OutReport;                               /* HID Out Report      */
                                            /*   Bit0..1: LEDs     */
                                            /*   Bit2..7: Reserved */


/*------------------------------------------------------------------------------
  Get HID Input Report -> InReport
 *------------------------------------------------------------------------------*/
void GetInReport (void) {
  uint32_t kbd_val;

  kbd_val = PIOA->PIO_PDSR;                           /* Read Pin Data          */
  InReport = 0x00;
  if ((kbd_val & PIO_PA18) == 0) InReport |= 0x01;    /* PB-LEFT  pressed == 0  */
  if ((kbd_val & PIO_PA19) == 0) InReport |= 0x02;    /* PB-RIGHT pressed == 0  */
}


/*------------------------------------------------------------------------------
  Set HID Output Report <- OutReport
 *------------------------------------------------------------------------------*/
void SetOutReport (void) {

  if (OutReport & 0x01)
    PIOB->PIO_CODR = PIO_PB0;                          /* Turn LED#1 On  */
  else
    PIOB->PIO_SODR = PIO_PB0;                          /* Turn LED#1 Off */

  if (OutReport & 0x02)
    PIOB->PIO_CODR = PIO_PB1;                          /* Turn LED#2 On  */
  else 
    PIOB->PIO_SODR = PIO_PB1;                          /* Turn LED#3 Off */
}


/*------------------------------------------------------------------------------
  Main Program
 *------------------------------------------------------------------------------*/
int main (void) {

  SystemCoreClockUpdate();

  /* Enable Clock for PIO */
  PMC->PMC_WPMR  = 0x504D4300;                         /* Disable write protect */
  PMC->PMC_PCER0 = (1 << ID_PIOA);                     /* Push Buttons */
  PMC->PMC_PCER0 = (1 << ID_PIOB);                     /* LEDs         */
  PMC->PMC_WPMR  = 0x504D4301;                         /* Enable write protect */

  PIOA->PIO_IDR  = 
  PIOA->PIO_ODR  = 
  PIOA->PIO_PER  = (PIO_PA19 | PIO_PA18);              /* Setup Pins PA18..PA19 for Buttons */

  PIOB->PIO_IDR  = 
  PIOB->PIO_OER  =
  PIOB->PIO_PER  = (PIO_PB1  | PIO_PB0 );              /* Setup Pins PB0..PB1 for LEDs      */
  PIOB->PIO_SODR = (PIO_PB1  | PIO_PB0 );              /* Turn LEDs Off                     */

  USB_Init();                                          /* USB Initialization */
  USB_Connect(__TRUE);                                 /* USB Connect        */

  while (1);                                           /* Loop forever       */
}
