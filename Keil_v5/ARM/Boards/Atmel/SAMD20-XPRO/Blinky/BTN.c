/*----------------------------------------------------------------------------
 * Name:    BTN.c
 * Purpose: low Level Push Button functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <samd20.h>


/*----------------------------------------------------------------------------
  initialize Push Button Pins
 *----------------------------------------------------------------------------*/
void BTN_Init(void) {

  /* Setup Pins for Push Button */
  PORT->Group[0].OUT.reg              |=  PORT_PA15;         /* Pull up       */
  PORT->Group[0].PINCFG[PIN_PA15].reg |=  ((1ul <<  1) |     /* Input Enable  */
                                           (1ul <<  2)  );   /* Pull Enable   */
  PORT->Group[0].DIRCLR.reg            =  PORT_PA15;
}


/*----------------------------------------------------------------------------
  Get Push Button status
 *----------------------------------------------------------------------------*/
unsigned int BTN_Get (void) {

  return (~(PORT->Group[0].IN.reg) & (PORT_PA15));
}
