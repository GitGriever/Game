/*-----------------------------------------------------------------------------
 * Name:    TH_LM75.h
 * Purpose: LM75 Thermometer definitions
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

#ifndef __TH_LM75_H
#define __TH_LM75_H

/* Thermometer modes */
typedef enum {
  MODE_COMP = 0,                        /* Comparator mode                    */
  MODE_INT                              /* Interrupt mode                     */
} TH_MODE;

/* Thermometer temperature */
typedef struct {
  uint16_t Temp;
} TH_DATA;

/* Thermometer function prototypes */
extern uint32_t TH_Init    (void);
extern uint16_t TH_GetTemp (TH_DATA *ThDat);
extern uint32_t TH_SetMode (TH_MODE  Mode);

#endif /* __TH_LM75_H */
