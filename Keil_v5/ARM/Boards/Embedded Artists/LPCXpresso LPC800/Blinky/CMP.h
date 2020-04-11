/*----------------------------------------------------------------------------
 * Name:    CMP.h
 * Purpose: low level Comparator definitions
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

#ifndef __CMP_H
#define __CMP_H

#define CMP_LAD_MAX       (0x1F)
#define CMP_LAD_MID      ((CMP_LAD_MAX + 1) / 2)

extern uint16_t CMP_last;

extern void     CMP_Init    (void);
extern uint16_t CMP_GetLad  (void);

#endif
