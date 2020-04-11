/*-----------------------------------------------------------------------------
 * Name:    EE_24LC128.c
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
#include "EEPROM.h"
#include "I2C.h"

#define EE_I2C_ADDR     0x51            /* 24LC128 EEPROM I2C address         */

#define EE_MAX_ADDR    16384            /* Max memory locations available     */
#define EE_MAX_WRITE      64            /* Max bytes to write in one step     */

/* Prototypes */
uint32_t EE_WrBuf (uint16_t addr, uint8_t *buf, uint32_t cnt);
uint32_t EE_RdBuf (uint16_t addr, uint8_t *buf, uint32_t cnt);

/*-----------------------------------------------------------------------------
 *      EE_Init:  Initialize EEPROM
 *
 *  Parameter:  (none)
 *  Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t EE_Init (void) {
  uint8_t val;
  /* Read min and max address */
  if (EE_RdBuf (0x00, &val, 1) == 0) {
    if (EE_RdBuf (EE_MAX_ADDR-1, &val, 1) == 0) {
      return (0);
    }
  }
  return (1);
}

/*-----------------------------------------------------------------------------
 *      EE_GetSize:  Get EEPROM size in bytes
 *
 *  Parameter:  (none)
 *  Return:     EEPROM size in bytes
 *----------------------------------------------------------------------------*/
uint32_t EE_GetSize (void) {
  return (EE_MAX_ADDR);
}

/*-----------------------------------------------------------------------------
 *      EE_Read:  Read cnt bytes from specified address into buf
 *
 *  Parameter:  addr - Byte address
 *              buf  - Data buffer
 *              cnt  - Number of bytes to read
 *
 *  Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t EE_Read (uint32_t addr, uint8_t *buf, uint32_t cnt) {
  if (addr < EE_MAX_ADDR) {
    if (EE_RdBuf ((uint16_t)addr, buf, cnt) == 0) {
      return (0);
    }
  }
  return (1);
}


/*-----------------------------------------------------------------------------
 *      EE_Write:  Write cnt bytes from buf to the specified address
 *
 *  Parameter:  addr - Byte address
 *              buf  - Data buffer
 *              cnt  - Number of bytes to write
 *
 *  Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t EE_Write (uint32_t addr, uint8_t *buf, uint32_t cnt) {
  uint32_t num;

  if (addr < EE_MAX_ADDR) {
    while (cnt) {
      if (cnt > EE_MAX_WRITE) {
        num  = EE_MAX_WRITE;
        cnt -= EE_MAX_WRITE;
      }
      else {
        num = cnt;
        cnt = 0;
      }
      if (EE_WrBuf ((uint16_t)addr, buf, num)) {
        return (1);
      }
    }
    return (0);
  }
  return (1);
}


#define A_WR      0                     /* Master will write to the I2C       */
#define A_RD      1                     /* Master will read from the I2C      */

/*-----------------------------------------------------------------------------
 *      EE_WrBuf:
 *
 * Parameters: addr - EEPROM location address
 *             buf  - data buffer
 *             cnt  - number of bytes to read
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t EE_WrBuf (uint16_t addr, uint8_t *buf, uint32_t cnt) {
  uint8_t  a[2]={0,0};
  uint32_t err;

  a[0] = (uint8_t)(addr >> 8);
  a[1] = (uint8_t)(addr & 0xFF);

  err  = I2C_Start ();
  err |= I2C_Addr  (EE_I2C_ADDR, A_WR);
  err |= I2C_Write (a[0]);
  err |= I2C_Write (a[1]);
  
  while (!err && cnt--) {
  err |= I2C_Write (*buf++);
  }
  err |= I2C_Stop  ();
  return (err);
}


/*-----------------------------------------------------------------------------
 *      EE_RdBuf:
 *
 * Parameters: addr - EEPROM location address
 *             buf  - data buffer
 *             cnt  - number of bytes to read
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t EE_RdBuf (uint16_t addr, uint8_t *buf, uint32_t cnt) {
  uint8_t  a[2]={0,0};
  uint32_t err;

  a[0] = (uint8_t)(addr >> 8);
  a[1] = (uint8_t)(addr & 0xFF);

  err  = I2C_Start ();
  err |= I2C_Addr  (EE_I2C_ADDR, A_WR);
  err |= I2C_Write (a[0]);
  err |= I2C_Write (a[1]);
  err |= I2C_Start ();
  err |= I2C_Addr  (EE_I2C_ADDR, A_RD);
  
  while (!err && cnt--) {
  err |= I2C_Read ((cnt != 0), buf++);
  }
  err |= I2C_Stop  ();
  return (err);
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
