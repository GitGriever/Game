/*-----------------------------------------------------------------------------
 * Name:    KBD_LPC177x_8x.c
 * Purpose: Low level keyboard functions
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

#include <LPC177x_8x.h>                 /* LPC177x/8x definitions             */
#include "KBD.h"
#include "I2C.h"

#define NUM_KEYS  5                     /* Number of available keys           */

/* Keys for EA LPC1788-32                                                     */
#define SW2             0x01
#define SW3             0x02
#define SW4             0x04
#define SW5             0x08
#define SW6             0x10

#define  IOE_I2C_ADDR   0x60            /* IO Expander 7-bit I2C address      */

/* Register addresses */
#define INPUT0          0x00

/* Prototypes */
static uint32_t IOE_RdReg (uint8_t reg, uint8_t *val);


/*-----------------------------------------------------------------------------
 *       KBD_Init:  Initialize keyboard/buttons
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void KBD_Init (void) {
  /* Init P2.10 as GPIO for SW6 */
  LPC_IOCON->P2_10 = 0;                 /* Push-pull, no pull-up/down         */
}


/*-----------------------------------------------------------------------------
 *       KBD_UnInit:  Uninitialize keyboard/buttons
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void KBD_UnInit (void) {
}


/*-----------------------------------------------------------------------------
 *       KBD_GetKeys:  Get keyboard state
 *
 * Parameters: (none)
 * Return:      Keys bitmask
 *----------------------------------------------------------------------------*/
uint32_t KBD_GetKeys (void) {
  /* Read board keyboard inputs */
  uint8_t  inp = 0;
  uint32_t val = 0;

  if (IOE_RdReg (INPUT0, &inp) == 0) {
    if (!(inp & (1 << 0))) {
      val |= SW2;
    }
    if (!(inp & (1 << 1))) {
      val |= SW3;
    }
    if (!(inp & (1 << 2))) {
      val |= SW4;
    }
    if (!(inp & (1 << 3))) {
      val |= SW5;
    }
  }
  if (!(LPC_GPIO2->PIN & (1 << 10))) {
    val |= SW6;
  }

  return (val);
}


/*-----------------------------------------------------------------------------
 *       KBD_Num:  Get number of available keys
 *
 * Parameters: (none)
 * Return:      number of keys
 *----------------------------------------------------------------------------*/
uint32_t KBD_Num (void) {
  return (NUM_KEYS);
}


/*-----------------------------------------------------------------------------
 *       IOE_RdReg:  Read IO Expander register
 *
 * Parameters:  reg - register to read
 *              val - variable where value will be stored
 *
 * Return:      0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
static uint32_t IOE_RdReg (uint8_t reg, uint8_t *val) {
  return I2C_RdData (IOE_I2C_ADDR, reg, val, 1);
}


/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
