/*----------------------------------------------------------------------------
 * Name:    KBD.h
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
 * Copyright (c) 2009-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef __KBD_H
#define __KBD_H


#define KBD_MASK        0x03
#define KBD_PB0         0x01         /* Push Button 0 */
#define KBD_PB1         0x02         /* Push Button 1 */

extern uint32_t KBD_val;

extern void     KBD_Init(void);
extern uint32_t KBD_Get (void);

#endif
