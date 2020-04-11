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
#include <LPC15xx.h>                         /* LPC15xx definitions           */
#include "JOY.h"

#define  JOY_ALL   (JOY_UP | JOY_LEFT | JOY_DOWN | JOY_RIGHT | JOY_CENTER)


/*----------------------------------------------------------------------------
  initialize Joystick Pins (P2.0..3, P2.10)
 *----------------------------------------------------------------------------*/
void JOY_Init (void) {

  LPC_SYSCON->SYSAHBCLKCTRL0 |= (1UL << 16);   /* enable clock for GPIO2 port */

  /* configure GPIO as intput */
  LPC_GPIO_PORT->DIR[2]  &= ~(JOY_ALL);
}


/*----------------------------------------------------------------------------
  Get Joystick status
 *----------------------------------------------------------------------------*/
unsigned int JOY_Get (void) {

  return (LPC_GPIO_PORT->PIN[2] & JOY_ALL);
}
