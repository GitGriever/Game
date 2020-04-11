/*-----------------------------------------------------------------------------
 * Name:    JOY.c
 * Purpose: Low level joystick functions
 * Note(s):
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f10x.h>                  /* STM32F10x Definitions              */
#include "JOY.h"


/*-----------------------------------------------------------------------------
 *  JOY_Init:  Initialize joystick
 *----------------------------------------------------------------------------*/
void JOY_Init (void) {

  /* Configure the GPIO for Joystick                                          */
  RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;   /* Enable GPIOD clock                 */
  GPIOD->CRH   &= 0x00000FFF;
  GPIOD->CRH   |= 0x44444000;
}


/*-----------------------------------------------------------------------------
 *  JOY_GetKeys:  Get joystick direction keys state
 *
 * Parameters: (none)
 * Return:     joystick directions bitmask
 *----------------------------------------------------------------------------*/
uint32_t JOY_GetKeys (void) {
  uint32_t pd, val = 0;

  pd = GPIOD->IDR;
  if ((pd & (1 << 15)) == 0) {
    val |= JOY_LEFT;
  }
  if ((pd & (1 << 13)) == 0) {
    val |= JOY_RIGHT;
  }
  if ((pd & (1 << 12)) == 0) {
    val |= JOY_UP;
  }
  if ((pd & (1 << 14)) == 0) {
    val |= JOY_DOWN;
  }
  if ((pd & (1 << 11)) == 0) {
    val |= JOY_CENTER;
  }
  return (val);
}
