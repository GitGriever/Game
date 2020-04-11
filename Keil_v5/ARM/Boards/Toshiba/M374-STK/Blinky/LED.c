/*----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: Low Level LED functions
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "TMPM374.h"                       /* TMPM374 definitions             */
#include "LED.h"                           /* LED     definitions             */

#define LED_2    (1UL << 7)
#define LED_3    (1UL << 4)
#define LED_4    (1UL << 2)
#define LED_5    (1UL << 0)
const unsigned long led_mask[] = { LED_5, LED_4, LED_3, LED_2 };

/* SC1 Pins */
#define SC1_MISO       (1UL << 6)          /* PA.6 = MISO                     */
#define SC1_MOSI       (1UL << 5)          /* PA.5 = MOSI                     */
#define SC1_CLK        (1UL << 4)          /* PA.4 = CLK                      */
#define SC1_ALE        (1UL << 0)          /* PF.0 =                          */
#define SC1_PINS       (SC1_MISO | SC1_MOSI | SC1_CLK)


/*----------------------------------------------------------------------------
 *  Initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_init (void) {

  TSB_PG->CR   |=  (LED_5 | LED_4 | LED_3);/* Set LED pins to outputs         */
  TSB_PG->FR1  &= ~(LED_5 | LED_4 | LED_3);/* Set LED pins function as "Port" */
  TSB_PG->IE   &= ~(LED_5 | LED_4 | LED_3);/* Disable LED pins input          */
  TSB_PG->DATA &= ~(LED_5 | LED_4 | LED_3);/* Turn all LEDs initially off     */


  /* ALE is GPIO, output set to LOW. */
  TSB_PF->CR   |=  SC1_ALE;                /* PF.0 is output                  */
  TSB_PF->IE   &= ~SC1_ALE;
  TSB_PF->DATA &= ~SC1_ALE;                /* set PF.0 LOW (ALE inactive)     */
  TSB_PF->FR1  &= ~SC1_ALE;                /* PF.0 ALE (used as GPIO)         */

  /* SCK, MISO, MOSI are SSP pins. */
  TSB_PA->IE   |=  SC1_MISO;               /* Enable  Input  PA.6             */
  TSB_PA->CR   &= ~SC1_MISO;               /* Disable Output PA.5             */
  TSB_PA->IE   &= ~(SC1_MOSI | SC1_CLK);   /* Disable Input  PA.4..5          */
  TSB_PA->CR   |=  (SC1_MOSI | SC1_CLK);   /* Enable  Output PA.4..5          */
  TSB_PA->FR1  |=  SC1_PINS;               /* PA.4..6 CLK, MOSI, MISO         */

  TSB_SC1->EN   =   ( 1UL << 0);           /* enable SC1                      */
  TSB_SC1->BRCR =  (( 1UL << 4) |          /* select T4 clock                 */
                     (10UL << 0) );        /* N =  10                         */ 
  TSB_SC1->CR   =  (( 0UL << 0) |          /* select baud reate gen. clock    */
                     ( 0UL << 1) );        /* Data TX @ rising edges of SCLK0 */
  TSB_SC1->MOD0 =  (( 0UL << 2) );         /* I/O interface mode              */ 
  TSB_SC1->MOD1 =  (( 2UL << 1) |          /* 2 SCLK  intervall time          */
                     ( 2UL << 5) |         /* Half duplex(TX)                 */
                     ( 1UL << 4) );        /* Enable TX                       */
  TSB_SC1->MOD2 =   ( 1UL << 3);           /* TX direction MSB first          */

  TSB_SC1->BUF = 0x00;                     /* Turn     LED initially off      */
  while ((TSB_SC1->MOD2 & 0xA0) != 0x80);
  TSB_PF->DATA   |=  SC1_ALE; 
  TSB_PF->DATA   &= ~SC1_ALE; 
}


/*----------------------------------------------------------------------------
 *  Switch LED On
 *----------------------------------------------------------------------------*/
void LED_on (uint32_t led) {

  if (led < LED_NUM) {
    if ( led != 3) { 
      TSB_PG->DATA |=  led_mask[led];      /* Turn On  GPIO LED               */
    }
    else {
      TSB_SC1->BUF = led_mask[led];        /* Turn On  SPI  LED               */
      while ((TSB_SC1->MOD2 & 0xA0) != 0x80);
      TSB_PF->DATA   |=  SC1_ALE;          /* set PF.0 HIGH (ALE active)      */
      TSB_PF->DATA   &= ~SC1_ALE;          /* set PF.0 LOW  (ALE inactive)    */
    }
  }
}

/*----------------------------------------------------------------------------
 *  Switch Led Off
 *----------------------------------------------------------------------------*/
void LED_off (uint32_t led) {

  if (led < LED_NUM) {
    if ( led != 3) { 
      TSB_PG->DATA &= ~led_mask[led];      /* Turn Off GPIO LED               */
    }
    else {
      TSB_SC1->BUF = 0x00;                 /* Turn Off  SPI  LED              */
      while ((TSB_SC1->MOD2 & 0xA0) != 0x80);
      TSB_PF->DATA   |=  SC1_ALE;          /* set PF.0 HIGH (ALE active)      */
      TSB_PF->DATA   &= ~SC1_ALE;          /* set PF.0 LOW  (ALE inactive)    */
    }
  }
}

/*----------------------------------------------------------------------------
 *  Output Value to LEDs
 *----------------------------------------------------------------------------*/
void LED_out(uint32_t value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (value & (1<<i)) {
      LED_on (i);
    } else {
      LED_off(i);
    }
  }
}
