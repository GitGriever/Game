/******************************************************************************/
/* LCD.h: LCD function prototypes and defines                                 */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2010 Keil Software. All rights reserved.                     */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#ifndef _LCD_H
#define _LCD_H


extern void LCD_Init            (void);
extern void LCD_Clear           (void);
extern void LCD_DisplayChar     (unsigned int pos, unsigned char  c);
extern void LCD_DisplayString   (unsigned int pos, unsigned char *s);

#endif /* _LCD_H */
