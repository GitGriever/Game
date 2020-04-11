/*----------------------------------------------------------------------------
 * Name:    LED.h
 * Purpose: LED driver header file
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2010 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef __LED_H
#define __LED_H

extern void LED_Init(void);
extern void LED_On  (int led);
extern void LED_Off (int led);
extern void LED_Out (int val);

#endif
