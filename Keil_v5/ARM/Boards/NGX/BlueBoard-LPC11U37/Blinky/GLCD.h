/*----------------------------------------------------------------------------
 * Name:    GLCD.h
 * Purpose: Low level Graphic LCD functions prototypes and defines
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

#ifndef _GLCD_H
#define _GLCD_H

extern void GLCD_Init           (void);
extern void GLCD_Clear          (void);
extern void GLCD_DrawChar       (unsigned int x, unsigned int y, unsigned char *c);
extern void GLCD_DisplayChar    (unsigned int ln, unsigned int pos, unsigned char  c);
extern void GLCD_DisplayString  (unsigned int ln, unsigned int pos, unsigned char *s);
extern void GLCD_ClearLn        (unsigned int ln);
extern void GLCD_Bargraph       (unsigned int ln, unsigned int y, unsigned int w, unsigned int val);

#endif /* _GLCD_H */
