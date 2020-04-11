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
 * Copyright (c) 2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <TMPM369.H>

#define  BTN_ALL   0x0F


/*----------------------------------------------------------------------------
  initialize Push Button Pins
 *----------------------------------------------------------------------------*/
void BTN_Init (void) {

  TSB_PC->FR1 &= ~BTN_ALL;              /* Set PushButton pins func. as "Port"*/
  TSB_PC->PUP &= ~BTN_ALL;              /* Disable PushButton pins pull ups   */
  TSB_PC->IE  |=  BTN_ALL;              /* Enable  PushButton pins as inputs  */
}


/*----------------------------------------------------------------------------
  Get Push Button status
 *----------------------------------------------------------------------------*/
unsigned int BTN_Get (void) {

  return (~(TSB_PC->DATA) & BTN_ALL);
}
