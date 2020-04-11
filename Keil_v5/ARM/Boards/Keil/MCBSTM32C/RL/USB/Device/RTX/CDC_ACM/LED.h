/*----------------------------------------------------------------------------
 * Name:    LED.h
 * Purpose: LED definitions
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef __LED_H
#define __LED_H

extern void     LED_Initialize   (void);
extern void     LED_Uninitialize (void);
extern void     LED_On           (uint32_t idx);
extern void     LED_Off          (uint32_t idx);
extern void     LED_Value        (uint32_t val);
extern uint32_t LED_GetNumber    (void);

#endif /* __LED_H */
