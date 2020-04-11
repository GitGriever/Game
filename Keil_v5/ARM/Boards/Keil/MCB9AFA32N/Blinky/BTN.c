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

#include <mb9aa30n.h>

/* Buttons : P34..P37*/
const uint32_t btn_mask[] = {1UL << 4, 1UL << 5, 1UL << 6, 1UL << 7};
#define  BTN_ALL   (btn_mask[0] | btn_mask[1] | btn_mask[2] | btn_mask[3])


/*----------------------------------------------------------------------------
  initialize Push Button Pins
 *----------------------------------------------------------------------------*/
void BTN_Init (void) {

  FM3_GPIO->PFR3  &= ~(BTN_ALL);        /* BTNs on port 3 defined as GPIO     */
  FM3_GPIO->DDR3  &= ~(BTN_ALL);        /* BTNs on port 3 defined as Input    */
  FM3_GPIO->PCR3  &= ~(BTN_ALL);        /* BTNs on port 3 do not use pull-up  */
}


/*----------------------------------------------------------------------------
  Get Push Button status
 *----------------------------------------------------------------------------*/
unsigned int BTN_Get (void) {

  return (~(FM3_GPIO->PDIR3) & BTN_ALL);
}
