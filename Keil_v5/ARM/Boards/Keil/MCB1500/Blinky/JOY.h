/*----------------------------------------------------------------------------
 * Name:    JOY.h
 * Purpose: low level Joystick definitions
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

#ifndef __JOY_H
#define __JOY_H

#define JOY_UP        (1UL <<  0)
#define JOY_LEFT      (1UL <<  1)
#define JOY_DOWN      (1UL <<  2)
#define JOY_RIGHT     (1UL <<  3)
#define JOY_CENTER    (1UL << 10)

extern void         JOY_Init(void);
extern unsigned int JOY_Get (void);

#endif
