/*----------------------------------------------------------------------------
 * Name:    BTN.h
 * Purpose: low level Push Button definitions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2014 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef BTN_H
#define BTN_H

extern void     BTN_Initialize(void);
extern uint32_t BTN_Get      (void);

#endif
