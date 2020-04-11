/*----------------------------------------------------------------------------
 * Name:    JOY.c
 * Purpose: low Level Joystick functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "TMPM369.h"                    /* Toshiba TMPM369 definitions        */

#define  JOY_ALL   0x1F


/*----------------------------------------------------------------------------
  initialize Joystick Pins
 *----------------------------------------------------------------------------*/
void JOY_Init (void) {

  TSB_PD->FR1 &= ~JOY_ALL;              /* Set Joystick pins func. as "Port"  */
  TSB_PD->PUP &= ~JOY_ALL;              /* Disable Joystick pins pull ups     */
  TSB_PD->IE  |=  JOY_ALL;              /* Enable  Joystick pins as inputs    */
}


/*----------------------------------------------------------------------------
  Get Joystick status
 *----------------------------------------------------------------------------*/
unsigned int JOY_Get (void) {

  return (~(TSB_PD->DATA) & JOY_ALL);
}
