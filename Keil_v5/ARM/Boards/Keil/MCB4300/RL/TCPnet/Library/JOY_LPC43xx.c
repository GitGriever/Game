/*-----------------------------------------------------------------------------
 * Name:    JOY.c
 * Purpose: Low level joystick functions
 * Note(s):
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

#include <LPC43xx.h>                    /* LPC43xx Definitions                */
#include "JOY.h"

/* Clock Control Unit register bits */
#define CCU_CLK_CFG_RUN   (1 << 0)
#define CCU_CLK_CFG_AUTO  (1 << 1)
#define CCU_CLK_STAT_RUN  (1 << 0)

/*-----------------------------------------------------------------------------
 *       JOY_Init:  Initialize joystick
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void JOY_Init (void) {

  /* Enable clock and init GPIO inputs */
  LPC_CCU1->CLK_M4_GPIO_CFG = CCU_CLK_CFG_AUTO | CCU_CLK_CFG_RUN;
  while (!(LPC_CCU1->CLK_M4_GPIO_STAT & CCU_CLK_STAT_RUN));

  LPC_SCU->SFSPC_9  = (1 << 6) | 4;
  LPC_SCU->SFSPC_11 = (1 << 6) | 4;
  LPC_SCU->SFSPC_12 = (1 << 6) | 4;
  LPC_SCU->SFSPC_13 = (1 << 6) | 4;
  LPC_SCU->SFSPC_14 = (1 << 6) | 4;

  LPC_GPIO_PORT->DIR[6]  &= ~(1 << 8 );
  LPC_GPIO_PORT->DIR[6]  &= ~(1 << 10);
  LPC_GPIO_PORT->DIR[6]  &= ~(1 << 11);
  LPC_GPIO_PORT->DIR[6]  &= ~(1 << 12);
  LPC_GPIO_PORT->DIR[6]  &= ~(1 << 13);
}


/*-----------------------------------------------------------------------------
 *       JOY_UnInit:  Uninitialize joystick
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void JOY_UnInit (void) {
  LPC_SCU->SFSPC_9   =  0;
  LPC_SCU->SFSPC_11  =  0;
  LPC_SCU->SFSPC_12  =  0;
  LPC_SCU->SFSPC_13  =  0;
  LPC_SCU->SFSPC_14  =  0;
}


/*-----------------------------------------------------------------------------
 *       JOY_GetKeys:  Get joystick direction keys state
 *
 * Parameters: (none)
 * Return:     joystick directions bitmask
 *----------------------------------------------------------------------------*/
uint32_t JOY_GetKeys (void) {
  /* Read board keyboard inputs                                               */
  uint32_t val = 0;

  if (!(LPC_GPIO_PORT->PIN[6] & (1 << 8 ))) {
    val |= JOY_CENTER;
  }
  if (!(LPC_GPIO_PORT->PIN[6] & (1 << 10))) {
    val |= JOY_UP;
  }
  if (!(LPC_GPIO_PORT->PIN[6] & (1 << 11))) {
    val |= JOY_DOWN;
  }
  if (!(LPC_GPIO_PORT->PIN[6] & (1 << 12))) {
    val |= JOY_LEFT;
  }
  if (!(LPC_GPIO_PORT->PIN[6] & (1 << 13))) {
    val |= JOY_RIGHT; 
  }

  return (val);
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
