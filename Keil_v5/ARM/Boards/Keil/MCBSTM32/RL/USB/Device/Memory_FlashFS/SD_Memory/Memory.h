/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    Memory.h
 *      Purpose: USB Memory Storage Demo Definitions
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

/* LED Definitions */
#define LED_MSK         0x0000FF00  /* PB8..15 */
#define LED_RD          0x00000100  /* PB8 */
#define LED_WR          0x00000200  /* PB9 */
#define LED_CFG         0x00000400  /* PB10 */
#define LED_SUSP        0x00000800  /* PB11 */

extern void LED_On  (unsigned int value);
extern void LED_Off (unsigned int value);
