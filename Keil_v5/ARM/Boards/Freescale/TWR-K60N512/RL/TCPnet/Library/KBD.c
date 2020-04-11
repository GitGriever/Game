/*-----------------------------------------------------------------------------
 * Name:    KBD.c
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
 * Copyright (c) 2004-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <MK60N512MD100.h>              /* MK60N512MD100 Definitions          */
#include "KBD.h"

#define NUM_KEYS  2                     /* Number of available keys           */

/* Keys for TWR-K60N512 */
#define SW6    1
#define SW7    2



/*-----------------------------------------------------------------------------
 *       KBD_Init:  Initialize keyboard/buttons
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void KBD_Init (void) {
  /* Enable clock and init GPIO inputs                                        */
  SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;   /* Enable Port A Clock                */
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;   /* Enable Port E Clock                */

  PTA->PDDR &= ~(1 << 19);              /* PTA19 input                        */
  PTE->PDDR &= ~(1 << 26);              /* PTE26 input                        */

  PORTA->PCR[19]  = PORT_PCR_MUX(1)  |  /* PTA19 is GPIO pin                  */
                    PORT_PCR_PS_MASK |  /* pull up on PTA19                   */
                    PORT_PCR_PE_MASK;

  PORTE->PCR[26]  = PORT_PCR_MUX(1)  |  /* PTE26 is GPIO pin                  */
                    PORT_PCR_PS_MASK |  /* pull up on PTA19                   */
                    PORT_PCR_PE_MASK;
}


/*-----------------------------------------------------------------------------
 *       KBD_UnInit:  Uninitialize keyboard/buttons
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void KBD_UnInit (void) {
  PORTA->PCR[19] = 0;                   /* PTA19 is at reset state            */
  PORTE->PCR[26] = 0;                   /* PTE26 is at reset state            */
}


/*-----------------------------------------------------------------------------
 *       KBD_GetKeys:  Get keyboard state
 *
 * Parameters: (none)
 * Return:      Keys bitmask
 *----------------------------------------------------------------------------*/
uint32_t KBD_GetKeys (void) {
  /* Read board keyboard inputs */
  uint32_t val = 0;

  if (!(PTA->PDIR & (1 << 19))) {
    /* SW6 */
    val |= SW6;
  }
  if (!(PTE->PDIR & (1 << 26))) {
    /* SW7 */
    val |= SW7;
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
 * End of file
 *----------------------------------------------------------------------------*/
