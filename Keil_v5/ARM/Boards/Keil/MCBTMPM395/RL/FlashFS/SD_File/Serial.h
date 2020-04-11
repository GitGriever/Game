/*----------------------------------------------------------------------------
 * Name:    Serial.h
 * Purpose: MCBTMPM395 Low level serial definitions
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

#ifndef __SERIAL_H
#define __SERIAL_H

extern void SER_init      (void);
extern int  SER_getChar   (void);
extern int  SER_getChar_nb(void);
extern int  SER_putChar   (int c);
extern void SER_putString (unsigned char *s);

#endif
