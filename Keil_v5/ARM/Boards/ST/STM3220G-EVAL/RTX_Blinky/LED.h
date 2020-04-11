/******************************************************************************/
/* LED.h: Low Level LED definitions                                        */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2010 Keil - An ARM Company. All rights reserved.             */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#ifndef __LED_H
#define __LED_H

/* LED Definitions */
#define LED_NUM     4                        /* Number of user LEDs          */

extern void LED_init(void);
extern void LED_on  (int led);
extern void LED_off (int led);
extern void LED_out (int led);

#endif
