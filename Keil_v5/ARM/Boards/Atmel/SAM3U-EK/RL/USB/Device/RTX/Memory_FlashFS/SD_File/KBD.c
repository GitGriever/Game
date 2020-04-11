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

#include <SAM3U.h>                      /* SAM3U definitions                  */
#include "KBD.h"

#define NUM_KEYS  4                     /* Number of available keys           */

/* Keys for SAM3U-EK */
#define USR_RIGHT   (1 << 0)
#define USR_LEFT    (1 << 1)


/*-----------------------------------------------------------------------------
 *       KBD_Init:  Initialize keyboard/buttons
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void KBD_Init (void) {
  /* Enable clock and init GPIO inputs */
  PMC->PMC_WPMR  = 0x504D4300;               /* Disable write protect         */
  PMC->PMC_PCER0 = (1 << ID_PIOA);
  PMC->PMC_WPMR  = 0x504D4301;               /* Enable write protect          */
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
  uint32_t val = 0;

  if (PIOA->PIO_PDSR & PIO_PA19) {
    /* USR-RIGHT button */
    val |= USR_RIGHT;
  }
  if (PIOA->PIO_PDSR & PIO_PA18) {
    /* USR-LEFT button */
    val |= USR_LEFT;
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
