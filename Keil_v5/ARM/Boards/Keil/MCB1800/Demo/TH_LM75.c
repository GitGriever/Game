/*-----------------------------------------------------------------------------
 * Name:    TH_LM75.c
 * Purpose: LM75 Thermometer routines
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

#include <stdint.h>
#include "TH_LM75.h"
#include "I2C.h"

#define TH_I2C_ADDR     0x48            /* LM 75 Thermometer I2C address      */


/* Register addresses */
#define REG_TEMP        0x00
#define REG_CONF        0x01
#define REG_THYS        0x02
#define REG_TOS         0x03


/*-----------------------------------------------------------------------------
 *      TH_Init:  Initialize thermometer
 *
 *  Parameter:  (none)
 *  Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t TH_Init (void) {
  uint8_t val[2];
  uint32_t err;

  /* Set default configuration */
  val[0] = 0;
  err    = I2C_WrData (TH_I2C_ADDR, REG_CONF, val, 1);
  
  /* Set Hysteresis */
  val[1] = 75;
  val[0] =  0;
  err   |= I2C_WrData (TH_I2C_ADDR, REG_THYS, val, 2);

  /* Set Tos */
  val[1] = 80;
  val[0] =  0;
  err   |= I2C_WrData (TH_I2C_ADDR, REG_TOS, val, 2);

  return (err) ? (1) : (0);
}


/*-----------------------------------------------------------------------------
 *      TH_SetMode:  Set thermometer mode
 *
 *  Parameter:  mode - LM 75 thermometer mode
 *
 *  Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t TH_SetMode (TH_MODE mode) {
  uint8_t val;

  if (I2C_RdData (TH_I2C_ADDR, REG_CONF, &val, 1) == 0) {
    if (mode == MODE_COMP) {
      val &= ~(1 << 1);
    }
    else {
      val |= (1 << 1);
    }
    if (I2C_RdData (TH_I2C_ADDR, REG_CONF, &val, 1) == 0) {
      return (0);
    }
  }
  return (1);
}


/*-----------------------------------------------------------------------------
 *      TH_GetTemp:  Get temperature data from thermometer
 *
 *  Parameter:  (none)
 *  Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint16_t TH_GetTemp (TH_DATA *ThDat) {
  uint8_t val[2];

  if (I2C_RdData (TH_I2C_ADDR, REG_TEMP, val, 2) == 0) {
    ThDat->Temp = (val[0] << 8) | (val[1] & 0x80) ;
    return (0);
  }
  return (1);
}


/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
