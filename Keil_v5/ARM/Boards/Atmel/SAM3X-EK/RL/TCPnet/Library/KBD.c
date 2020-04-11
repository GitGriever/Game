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
 * Copyright (c) 2004-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <sam3xa.h>
#include "KBD.h"

#define NUM_KEYS  2                     /* Number of available keys           */

/* Keys for SAM3X-EK */
#define RIGHT_CLICK   0x01
#define LEFT_CLICK    0x02


/* PMC Write Protect Mode Keys */
#define PMC_WPEN_KEY        0x504D4301
#define PMC_WPDIS_KEY       0x504D4300

/* PIO Write Protect Mode Keys */
#define PIO_WPEN_KEY        0x50494F01
#define PIO_WPDIS_KEY       0x50494F00

/*-----------------------------------------------------------------------------
 *       KBD_Init:  Initialize keyboard/buttons
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void KBD_Init (void) {
  PMC->PMC_WPMR  = PMC_WPDIS_KEY;       /* Disable PMC write protection       */
  PMC->PMC_PCER0 = (1UL << ID_PIOB) |   /* Enable clock for PIOB              */
                   (1UL << ID_PIOE) ;   /* Enable clock for PIOE              */
  PMC->PMC_WPMR  = PMC_WPEN_KEY;        /* Enable PMC write protection        */
  
  PIOB->PIO_WPMR = PIO_WPDIS_KEY;       /* Disable PIO write protection       */
  PIOB->PIO_PER  = PIO_PB23;
  PIOB->PIO_PUER = PIO_PB23;
  PIOB->PIO_ODR  = PIO_PB23;
  PIOB->PIO_WPMR = PIO_WPEN_KEY;        /* Enable PIO write protection        */
  
  PIOE->PIO_WPMR = PIO_WPDIS_KEY;       /* Disable PIO write protection       */
  PIOE->PIO_PER  = PIO_PE7;
  PIOE->PIO_PUER = PIO_PE7;
  PIOE->PIO_ODR  = PIO_PE7;
  PIOE->PIO_WPMR = PIO_WPEN_KEY;        /* Enable PIO write protection        */
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

  if ((PIOB->PIO_PDSR & PIO_PB23) == 0) {
    /* Right click - PB23 */
    val |= RIGHT_CLICK;
  }
  if ((PIOE->PIO_PDSR & PIO_PE7) == 0) {
    /* Left click - PE7 */
    val |= LEFT_CLICK;
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
