/*-----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: Low level LED functions
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

#include <LPC177x_8x.h>
#include "LED.h"
#include "I2C.h"

#define NUM_LEDS  8                     /* Number of LEDs                     */


#define I2S_ADDR  (uint8_t)(0x60)

/* PCA9532 registers */
#define PCA9532_LS2_REG      ((uint8_t)(0x08))
#define PCA9532_LS3_REG      ((uint8_t)(0x09))

/*-----------------------------------------------------------------------------
 *      LED_Init:  Initialize LEDs
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {
  uint8_t  val;
  val = 0x00;
  I2C_WrData (I2S_ADDR, PCA9532_LS2_REG, &val, 1);
  I2C_WrData (I2S_ADDR, PCA9532_LS3_REG, &val, 1);
}


/*-----------------------------------------------------------------------------
 *      LED_UnInit:  Uninitialize LEDs; pins are set to reset state
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_UnInit(void) {
  uint8_t  val;
  val = 0x00;
  I2C_WrData (I2S_ADDR, PCA9532_LS2_REG, &val, 1);
  I2C_WrData (I2S_ADDR, PCA9532_LS3_REG, &val, 1);
}

/*-----------------------------------------------------------------------------
 *      LED_On: Turns on requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {
  uint8_t  val;
  
  if (num % 2) {
    I2C_RdData (I2S_ADDR, PCA9532_LS3_REG, &val, 1);
    val |= (1 << (num - 1));
    I2C_WrData (I2S_ADDR, PCA9532_LS3_REG, &val, 1);
  }
  else {
    I2C_RdData (I2S_ADDR, PCA9532_LS2_REG, &val, 1);
    val |= (1 << num);
    I2C_WrData (I2S_ADDR, PCA9532_LS2_REG, &val, 1);
  }
}

/*-----------------------------------------------------------------------------
 *       LED_Off: Turns off requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Off (uint32_t num) {
  uint8_t  val;
  
  if (num % 2) {
    I2C_RdData (I2S_ADDR, PCA9532_LS3_REG, &val, 1);
    val &= ~(1 << (num - 1));
    I2C_WrData (I2S_ADDR, PCA9532_LS3_REG, &val, 1);
  }
  else {
    I2C_RdData (I2S_ADDR, PCA9532_LS2_REG, &val, 1);
    val &= ~(1 << num);
    I2C_WrData (I2S_ADDR, PCA9532_LS2_REG, &val, 1);
  }
}

/*-----------------------------------------------------------------------------
 *       LED_Val: Write value to LEDs
 *
 * Parameters:  val - value to be displayed on LEDs
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Val (uint32_t val) {
  int i;

  for (i = 0; i < NUM_LEDS; i++) {
    if (val & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}

/*-----------------------------------------------------------------------------
 *       LED_Num: Get number of available LEDs
 *
 * Parameters: (none)
 * Return:      number of available LEDs
 *----------------------------------------------------------------------------*/
uint32_t LED_Num (void) {
  return (NUM_LEDS);
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
