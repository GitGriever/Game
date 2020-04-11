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

#include "mss_gpio.h"
#include "BTN.h"

const uint32_t btn_mask[] = {1UL << 8, 1UL << 9, 1UL << 10, 1UL << 11, 1UL << 12};
#define  BTN_ALL   (btn_mask[0] | btn_mask[1] | btn_mask[2] | btn_mask[3] | btn_mask[4])


/*----------------------------------------------------------------------------
  initialize Push Button Pins
 *----------------------------------------------------------------------------*/
void BTN_Init (void) {

  MSS_GPIO_init();

  MSS_GPIO_config(MSS_GPIO_8, MSS_GPIO_INPUT_MODE);
  MSS_GPIO_config(MSS_GPIO_9, MSS_GPIO_INPUT_MODE);
  MSS_GPIO_config(MSS_GPIO_10, MSS_GPIO_INPUT_MODE);
  MSS_GPIO_config(MSS_GPIO_11, MSS_GPIO_INPUT_MODE);
  MSS_GPIO_config(MSS_GPIO_12, MSS_GPIO_INPUT_MODE);
}


/*----------------------------------------------------------------------------
  Get Push Button status
 *----------------------------------------------------------------------------*/
unsigned int BTN_Get (void) {

  return (~(MSS_GPIO_get_inputs()) & BTN_ALL);
}
