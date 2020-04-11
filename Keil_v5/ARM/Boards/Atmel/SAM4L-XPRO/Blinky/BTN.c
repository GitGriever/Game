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

#include <SAM4L.h>


/*----------------------------------------------------------------------------
  initialize Push Button Pins  (PC24 used for SW0)
 *----------------------------------------------------------------------------*/
void BTN_Init(void) {

  GPIO->GPIO_PORT[2].GPIO_GPERS  = GPIO_PC24;
  GPIO->GPIO_PORT[2].GPIO_ODERC  = GPIO_PC24;
  GPIO->GPIO_PORT[2].GPIO_PUERS  = GPIO_PC24;
  GPIO->GPIO_PORT[2].GPIO_STERS  = GPIO_PC24;

}


/*----------------------------------------------------------------------------
  Get Push Button status
 *----------------------------------------------------------------------------*/
unsigned int BTN_Get (void) {

  return (~(GPIO->GPIO_PORT[2].GPIO_PVR) & (GPIO_PC24));
}
