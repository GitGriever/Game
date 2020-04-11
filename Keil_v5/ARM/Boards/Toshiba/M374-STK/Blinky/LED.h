/*----------------------------------------------------------------------------
 * Name:    LED.h
 * Purpose: Low level LED definitions
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

#ifndef __LED_H
#define __LED_H

#define LED_NUM     4                   /* Number of user LEDs                */

extern void LED_init(void);
extern void LED_on  (uint32_t led);
extern void LED_off (uint32_t led);
extern void LED_out (uint32_t val);

#endif
