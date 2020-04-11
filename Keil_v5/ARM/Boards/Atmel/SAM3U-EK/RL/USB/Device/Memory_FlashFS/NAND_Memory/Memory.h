/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    NAND_Memory.h
 *      Purpose: File manipulation example definitions
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

/* LED Definitions */
#define LED_MSK         0x00000003  /* PB0..PB1 */
#define LED_RD          0x00000000  /* not used */
#define LED_WR          0x00000000  /* not used */
#define LED_CFG         0x00000001  /* PB0      */
#define LED_SUSP        0x00000002  /* PB1      */

extern void LED_On  (unsigned int value);
extern void LED_Off (unsigned int value);

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
