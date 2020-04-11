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

  /* Enable clock for GPIOD, GPIOG and AFIO                                   */
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPDEN | RCC_APB2ENR_IOPGEN;

  /* Set PG7,13,14,15 and PD3 as input pins                                   */
  GPIOG->CRH  &= ~0xFFF00000;
  GPIOG->CRH   |= 0x44400000;
  GPIOG->CRL  &= ~0xF0000000;
  GPIOG->CRL   |= 0x40000000;
  GPIOD->CRL  &= ~0x0000F000;
  GPIOD->CRL   |= 0x00004000;
}


/*-----------------------------------------------------------------------------
 *  JOY_GetKeys:  Get joystick direction keys state
 *
 * Parameters: (none)
 * Return:     joystick directions bitmask
 *----------------------------------------------------------------------------*/
uint32_t JOY_GetKeys (void) {
  uint32_t pg, pd, val = 0;

  pd = GPIOD->IDR;
  pg = GPIOG->IDR;
  if ((pg & (1 << 14)) == 0) {
    val |= JOY_LEFT;
  }
  if ((pg & (1 << 13)) == 0) {
    val |= JOY_RIGHT;
  }
  if ((pg & (1 << 15)) == 0) {
    val |= JOY_UP;
  }
  if ((pd & (1 <<  3)) == 0) {
    val |= JOY_DOWN;
  }
  if ((pg & (1 <<  7)) == 0) {
    val |= JOY_CENTER;
  }
  return (val);
}
