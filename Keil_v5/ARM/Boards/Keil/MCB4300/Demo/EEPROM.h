/*-----------------------------------------------------------------------------
 * Name:    EEPROM.h
 * Purpose: EEPROM definitions
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef __EEPROM_H
#define __EEPROM_H

/* EEPROM function prototypes */
extern uint32_t EE_Init    (void);
extern uint32_t EE_GetSize (void);

extern uint32_t EE_Read   (uint32_t addr, uint8_t *buf, uint32_t cnt);
extern uint32_t EE_Write  (uint32_t addr, uint8_t *buf, uint32_t cnt);

#endif /* __EEPROM_H */
