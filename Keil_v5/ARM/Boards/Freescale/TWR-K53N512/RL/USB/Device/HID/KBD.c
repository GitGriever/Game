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
 * Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <MK53N512ZCMD100.h>
#include "KBD.h"

#define NUM_KEYS  2                     /* Number of available keys           */

#define SW1  1
#define SW2  2



/*-----------------------------------------------------------------------------
 *       KBD_Init:  Initialize keyboard/buttons
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void KBD_Init (void) {
  /* Enable clock and init GPIO inputs                                        */
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;   /* Enable Port C Clock                */

  PTC->PDDR &= ~((1 << 5) | (1 << 13)); /* PTC5 and PTC13 inputs              */

  PORTC->PCR[5]  = PORT_PCR_MUX(1)   |  /* PTC5 is GPIO pin                   */
                   PORT_PCR_PS_MASK  |  /* pull up on PTC1                    */
                   PORT_PCR_PE_MASK;

  PORTC->PCR[13] = PORT_PCR_MUX(1)   |  /* PTC13 is GPIO pin                   */
                   PORT_PCR_PS_MASK  |  /* pull up on PTC2                    */
                   PORT_PCR_PE_MASK;
}


/*-----------------------------------------------------------------------------
 *       KBD_UnInit:  Uninitialize keyboard/buttons
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void KBD_UnInit (void) {
  PORTC->PCR[5] = 0;                    /* PTC1 is at reset state             */
  PORTC->PCR[13] = 0;                   /* PTC2 is at reset state             */
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

  if (!(PTC->PDIR & (1 << 5))) {
    /* SW1 */
    val |= SW1;
  }
  if (!(PTC->PDIR & (1 << 13))) {
    /* SW2 */
    val |= SW2;
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
