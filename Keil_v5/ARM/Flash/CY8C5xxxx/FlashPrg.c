/* -----------------------------------------------------------------------------
 * Copyright (c) 2014 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty. 
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software. Permission is granted to anyone to use this 
 * software for any purpose, including commercial applications, and to alter 
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not 
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be 
 *    appreciated but is not required. 
 * 
 * 2. Altered source versions must be plainly marked as such, and must not be 
 *    misrepresented as being the original software. 
 * 
 * 3. This notice may not be removed or altered from any source distribution.
 *   
 *
 * $Date:        15. April 2014
 * $Revision:    V1.00
 *  
 * Project:      Flash Programming Functions for Cypress CY8C5xxxx Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"         // FlashOS Structures


#define STACK_SIZE     64       // Stack Size

// CY8C5xxx Registers
#define FASTCLK_IMO_CR  (*((volatile unsigned char *) 0x40004200))
#define PM_ACT_CFG0     (*((volatile unsigned char *) 0x400043A0))
#define PM_ACT_CFG12    (*((volatile unsigned char *) 0x400043AC))
#define SPC_CPU_DATA    (*((volatile unsigned char *) 0x40004720))
#define SPC_DMA_DATA    (*((volatile unsigned char *) 0x40004721))
#define SPC_SR          (*((volatile unsigned char *) 0x40004722))
#define DEVICE_ID       (*((volatile unsigned long *) 0x4008001C))

// SPC Command Codes
#define LOAD_BYTE           0x00
#define LOAD_MULTI_BYTES    0x01
#define LOAD_ROW            0x02
#define READ_BYTE           0x03
#define READ_MULTI_BYTES    0x04
#define WRITE_ROW           0x05
#define WRITE_NVL           0x06
#define ERASE_SECTOR        0x08
#define ERASE_ALL           0x09
#define READ_HIDDEN_ROW     0x0A
#define PROTECT             0x0B
#define GET_CHECKSUM        0x0C
#define GET_TEMPERATURE     0x0E
#define KEY1                0xB6
#define KEY2                0xD3

// Flash Array ID Codes
#define ID_ALL_FLASH        0x3F
#define ID_EEPROM           0x40
#define ID_CFG_NVL          0x80
#define ID_WO_NVL           0xF8

// SPC Status Codes
#define OK                  0x00
#define INVALID_ARRAY       0x01
#define INVALID_KEY         0x02
#define ARRAY_ASLEEP        0x03
#define ACCESS_FAILURE      0x04
#define INVALID_N_VALUE     0x05
#define TEST_MODE_FAILURE   0x06
#define PROGRAM_FAILURE     0x07
#define ERASE_FAILURE       0x08
#define PROTECTION_FAILURE  0x09
#define INVALID_ADDRESS     0x0A
#define INVALID_COMMAND     0x0B
#define INVALID_ROW         0x0C
#define INVALID_STATE       0x0F

// SPC Status Register
#define DATA_READY          (1<<0)
#define SPC_IDLE            (1<<1)
#define STATUS_CODE_POS     2
#define STATUS_CODE_MASK    0x3F


// EEPROM Addresses
#define EEPROM_BASE         0x40008000
#define EEPROM_IMAGE        0x90200000


#define PSOC5LP_ID          0x0E123069

#ifdef  CY8C5XXXX_CFG_NVL
#define ID_NVL              ID_CFG_NVL
#else
#define ID_NVL              ID_WO_NVL
#endif


       unsigned char PSoC5LP;


#if (defined (CY8C5XXXX_FLASH) || defined (CY8C5XXXX_CFG))
static unsigned char CFG_NVL3;
#endif


#if (defined (CY8C5XXXX_FLASH) || defined (CY8C5XXXX_CFG) || defined(CY8C5XXXX_PROTECT) || defined (CY8C5XXXX_EEPROM))
// Measured Temperature
static unsigned char TemperatureSign;
static unsigned char TemperatureMagnitude;
#endif


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  PM_ACT_CFG0    = 0xBF;                        // Enable CPU & Flash & SPC
  PM_ACT_CFG12  |= 0x10;                        // Enable EEPROM
  FASTCLK_IMO_CR = 0x02;                        // 24MHz Normal mode

  if ((DEVICE_ID & 0x0FFFFFFF) == PSOC5LP_ID) {
    PSoC5LP = 1;
  } else {
    PSoC5LP = 0;
  }

#if (defined (CY8C5XXXX_FLASH) || defined (CY8C5XXXX_CFG))
  // Read CFG NVL Byte 3
  SPC_CPU_DATA = KEY1;
  SPC_CPU_DATA = KEY2 + READ_BYTE;
  SPC_CPU_DATA = READ_BYTE;
  SPC_CPU_DATA = ID_CFG_NVL;
  SPC_CPU_DATA = 3;                             // Byte index

  while (!(SPC_SR & DATA_READY));               // Wait until Data ready

  if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
    return (1);                                 // Failure
  }

  CFG_NVL3 = SPC_CPU_DATA;                      // Store CFG NVL Byte 3

  while (!(SPC_SR & SPC_IDLE));                 // Wait until idle

  if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
    return (1);                                 // Failure
  }
#endif

#if (defined (CY8C5XXXX_FLASH) || defined (CY8C5XXXX_CFG) || defined(CY8C5XXXX_PROTECT) || defined (CY8C5XXXX_EEPROM))
  if (fnc == 2) {
    if (PSoC5LP) {
      // Measure Temparature for Programming
      SPC_CPU_DATA = KEY1;
      SPC_CPU_DATA = KEY2 + GET_TEMPERATURE;
      SPC_CPU_DATA = GET_TEMPERATURE;
      SPC_CPU_DATA = 3;                         // Number of samples

      while (!(SPC_SR & DATA_READY));           // Wait until Data ready

      if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
        return (1);                             // Failure
      }

      TemperatureSign = SPC_CPU_DATA;           // Store Temperature Sign

      while (!(SPC_SR & DATA_READY));           // Wait until Data ready

      TemperatureMagnitude = SPC_CPU_DATA;      // Store Temperature Magnitude

      while (!(SPC_SR & SPC_IDLE));             // Wait until idle

      if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
        return (1);                             // Failure
      }
    }
    else {
      TemperatureSign      = 1;                 // Temperature Sign (+)
      TemperatureMagnitude = 25;                // Temparature Magnitude (25C)
    }
  }
#endif

  return (0);
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {
  return (0);
}


/*
 *  Blank Check Checks if Memory is Blank
 *    Parameter:      adr:  Block Start Address
 *                    sz:   Block Size (in bytes)
 *                    pat:  Block Pattern
 *    Return Value:   0 - OK,  1 - Failed
 */

int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat) {
#ifdef CY8C5XXXX_EEPROM
#ifdef EEPROM_ERASE
  unsigned char *eeprom;

  eeprom = (unsigned char *)((adr - EEPROM_IMAGE) + EEPROM_BASE);

  while (sz--) {
    if (*eeprom++ != pat) {                     // Compare Data
      return (1);
    }
  }
#endif
  return (0);
#else
  return (1);                                   // Force Erase
#endif
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifndef CY8C5XXXX_EEPROM
int EraseChip (void) {

  // Erase All
  SPC_CPU_DATA = KEY1;
  SPC_CPU_DATA = KEY2 + ERASE_ALL;
  SPC_CPU_DATA = ERASE_ALL;

  while (!(SPC_SR & SPC_IDLE));                 // Wait until idle

  if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
    return (1);                                 // Failure
  }

  return (0);                                   // Finished without Errors
}
#endif


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

#if (defined (CY8C5XXXX_FLASH) || defined (CY8C5XXXX_CFG))
int EraseSector (unsigned long adr) {
  unsigned char array;
  unsigned char sector;

  array  = (unsigned char)((adr >> 16) & 0x1F);
  sector = (unsigned char) (adr >>  8+6);

  // Erase Sector
  SPC_CPU_DATA = KEY1;
  SPC_CPU_DATA = KEY2 + ERASE_SECTOR;
  SPC_CPU_DATA = ERASE_SECTOR;
  SPC_CPU_DATA = array;
  SPC_CPU_DATA = sector;

  while (!(SPC_SR & SPC_IDLE));                 // Wait until idle

  if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
    return (1);                                 // Failure
  }

  return (0);                                   // Finished without Errors
}
#endif

#ifdef CY8C5XXXX_EEPROM
int EraseSector (unsigned long adr) {
#ifdef EEPROM_ERASE
  unsigned char sector;

  sector = (unsigned char)(adr >> 4+6);

  // Erase Sector
  SPC_CPU_DATA = KEY1;
  SPC_CPU_DATA = KEY2 + ERASE_SECTOR;
  SPC_CPU_DATA = ERASE_SECTOR;
  SPC_CPU_DATA = ID_EEPROM;
  SPC_CPU_DATA = sector;

  while (!(SPC_SR & SPC_IDLE));                 // Wait until idle

  if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
    return (1);                                 // Failure
  }
#endif
  return (0);                                   // Finished without Errors
}
#endif

#if (defined (CY8C5XXXX_PROTECT) || defined (CY8C5XXXX_CFG_NVL) || defined (CY8C5XXXX_WO_NVL))
int EraseSector (unsigned long adr) {
  return (0);
}
#endif


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

#if (defined (CY8C5XXXX_FLASH) || defined (CY8C5XXXX_CFG))
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  unsigned char array;
  unsigned char row;

  if (sz > 288) return (1);                     // Failure

  if (CFG_NVL3 & (1 << 3)) {                    // ECC Mode ?
#ifdef CY8C5XXXX_FLASH
    if (sz > 256) sz = 256;
#else
    return (0);                                 // Skip programming
#endif
  }

  array = (unsigned char)((adr >> 16) & 0x1F);
  row   = (unsigned char) (adr >>  8);

  // Load Row
  SPC_CPU_DATA = KEY1;
  SPC_CPU_DATA = KEY2 + LOAD_ROW;
  SPC_CPU_DATA = LOAD_ROW;
  SPC_CPU_DATA = array;
  while (sz--) {
    SPC_CPU_DATA = *buf++;                      // Load Data
  }

  while (!(SPC_SR & SPC_IDLE));                 // Wait until idle

  if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
    return (1);                                 // Failure
  }

  // Write Row
  SPC_CPU_DATA = KEY1;
  SPC_CPU_DATA = KEY2 + WRITE_ROW;
  SPC_CPU_DATA = WRITE_ROW;
  SPC_CPU_DATA = array;
  SPC_CPU_DATA = 0;
  SPC_CPU_DATA = row;
  SPC_CPU_DATA = TemperatureSign;               // Measured Temperature Sign
  SPC_CPU_DATA = TemperatureMagnitude;          // Measured Temparature Magnitude

  while (!(SPC_SR & SPC_IDLE));                 // Wait until idle

  if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
    return (1);                                 // Failure
  }

  return (0);                                   // Finished without Errors
}
#endif

#ifdef CY8C5XXXX_EEPROM
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  unsigned char row;
  unsigned long n;
#ifndef EEPROM_ERASE
  unsigned char *eeprom;
#endif

  if (sz > 16) return (1);                      // Failure

#ifndef EEPROM_ERASE
  eeprom = (unsigned char *)((adr - EEPROM_IMAGE) + EEPROM_BASE);

  for (n = 0; n < sz; n++) {
    if (buf[n] != *eeprom++) break;
  }
  if (n == sz) return (0);                      // Skip programming
#endif

  row = (unsigned char)(adr >> 4);

  // Load Row
  SPC_CPU_DATA = KEY1;
  SPC_CPU_DATA = KEY2 + LOAD_ROW;
  SPC_CPU_DATA = LOAD_ROW;
  SPC_CPU_DATA = ID_EEPROM;

  n = 16 - sz;
  while (sz--) {
    SPC_CPU_DATA = *buf++;                      // Load Data
  }
  while (n--) {
    SPC_CPU_DATA = 0;
  }

  while (!(SPC_SR & SPC_IDLE));                 // Wait until idle

  if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
    return (1);                                 // Failure
  }

  // Write Row
  SPC_CPU_DATA = KEY1;
  SPC_CPU_DATA = KEY2 + WRITE_ROW;
  SPC_CPU_DATA = WRITE_ROW;
  SPC_CPU_DATA = ID_EEPROM;
  SPC_CPU_DATA = 0;
  SPC_CPU_DATA = row;
  SPC_CPU_DATA = TemperatureSign;               // Measured Temperature Sign
  SPC_CPU_DATA = TemperatureMagnitude;          // Measured Temparature Magnitude

  while (!(SPC_SR & SPC_IDLE));                 // Wait until idle

  if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
    return (1);                                 // Failure
  }

  return (0);                                   // Finished without Errors
}
#endif

#ifdef CY8C5XXXX_PROTECT
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  unsigned char array;
  unsigned long n;

  if (sz > 64) return (1);                      // Failure

  array = (unsigned char)((adr >> 6) & 0x1F);

  // Load Row
  SPC_CPU_DATA = KEY1;
  SPC_CPU_DATA = KEY2 + LOAD_ROW;
  SPC_CPU_DATA = LOAD_ROW;
  SPC_CPU_DATA = array;

  n = 288 - sz;
  while (sz--) {
    SPC_CPU_DATA = *buf++;                      // Load Data
  }
  while (n--) {
    SPC_CPU_DATA = 0;
  }

  while (!(SPC_SR & SPC_IDLE));                 // Wait until idle

  if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
    return (1);                                 // Failure
  }

  // Write Row
  SPC_CPU_DATA = KEY1;
  SPC_CPU_DATA = KEY2 + PROTECT;
  SPC_CPU_DATA = PROTECT;
  SPC_CPU_DATA = array;
  SPC_CPU_DATA = 0;
  if (PSoC5LP) {
    SPC_CPU_DATA = TemperatureSign;             // Measured Temperature Sign
    SPC_CPU_DATA = TemperatureMagnitude;        // Measured Temparature Magnitude
  }

  while (!(SPC_SR & SPC_IDLE));                 // Wait until idle

  if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
    return (1);                                 // Failure
  }

  return (0);                                   // Finished without Errors
}
#endif

#if (defined (CY8C5XXXX_CFG_NVL) || defined (CY8C5XXXX_WO_NVL))
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  unsigned char nvl[4];
  unsigned long n;

  if (sz > 4) return (1);                       // Failure

  // Read current NVL
  for (n = 0; n < 4; n++) {

    // Read NVL Byte
    SPC_CPU_DATA = KEY1;
    SPC_CPU_DATA = KEY2 + READ_BYTE;
    SPC_CPU_DATA = READ_BYTE;
    SPC_CPU_DATA = ID_NVL;
    SPC_CPU_DATA = (unsigned char)n;

    while (!(SPC_SR & DATA_READY));             // Wait until Data ready

    if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
      return (1);                               // Failure
    }

    nvl[n] = SPC_CPU_DATA;

    while (!(SPC_SR & SPC_IDLE));               // Wait until idle

    if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
      return (1);                               // Failure
    }
  }

  // Check if new NVL is different
  for (n = 0; n < 4; n++) {
    if (nvl[n] != buf[n]) break;
  }
  if (n == 4) return (0);                       // NVL is up to date

  // Load new NVL
  for (n = 0; n < 4; n++) {

    // Load NVL Byte
    SPC_CPU_DATA = KEY1;
    SPC_CPU_DATA = KEY2 + LOAD_BYTE;
    SPC_CPU_DATA = LOAD_BYTE;
    SPC_CPU_DATA = ID_NVL;
    SPC_CPU_DATA = (unsigned char)n;
    SPC_CPU_DATA = *buf++;

    while (!(SPC_SR & SPC_IDLE));               // Wait until idle

    if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
      return (1);                               // Failure
    }
  }

  // Write new NVL
  SPC_CPU_DATA = KEY1;
  SPC_CPU_DATA = KEY2 + WRITE_NVL;
  SPC_CPU_DATA = WRITE_NVL;
  SPC_CPU_DATA = ID_NVL;

  while (!(SPC_SR & SPC_IDLE));                 // Wait until idle

  if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
    return (1);                                 // Failure
  }

  return (0);                                   // Finished without Errors
}
#endif


/*
 *  Verify Flash Contents
 *    Parameter:      adr:  Start Address
 *                    sz:   Size (in bytes)
 *                    buf:  Data
 *    Return Value:   (adr+sz) - OK, Failed Address
 */

#if (defined (CY8C5XXXX_FLASH) || defined (CY8C5XXXX_CFG))
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf) {
  unsigned long n;

#ifdef CY8C5XXXX_FLASH
  if (sz > 256) return (adr);                   // Failure
#else
  if (sz > 32)  return (adr);                   // Failure
#endif

  // Read Flash
  SPC_CPU_DATA = KEY1;
  SPC_CPU_DATA = KEY2 + READ_MULTI_BYTES;
  SPC_CPU_DATA = READ_MULTI_BYTES;
#ifdef CY8C5XXXX_FLASH
  SPC_CPU_DATA = (unsigned char)(adr >> 16);    // Array
#else
  SPC_CPU_DATA = (unsigned char)(adr >> 13);    // Array
#endif
  SPC_CPU_DATA = (unsigned char)(adr >> 24);    // High Address
  SPC_CPU_DATA = (unsigned char)(adr >>  8);    // Mid Address
  SPC_CPU_DATA = (unsigned char)(adr >>  0);    // Low Address
  SPC_CPU_DATA = (unsigned char)(sz-1);         // Number of bytes minus one

  while (!(SPC_SR & DATA_READY));               // Wait until Data ready

  if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
    return (adr);                               // Failure
  }

  n = sz;
  while (sz--) {
    n--;
    while (!(SPC_SR & DATA_READY));             // Wait until Data ready
    if (SPC_CPU_DATA != *buf++) {               // Compare Data
      break;
    }
    adr++;
  }
  while (n--) {
    while (!(SPC_SR & DATA_READY));             // Wait until Data ready
    SPC_CPU_DATA;
  }

  while (!(SPC_SR & SPC_IDLE));                 // Wait until idle

  return (adr);
}
#endif

#ifdef CY8C5XXXX_EEPROM
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf) {
  unsigned char *eeprom;

  eeprom = (unsigned char *)((adr - EEPROM_IMAGE) + EEPROM_BASE);

  while (sz--) {
    if (*eeprom++ != *buf++) {                  // Compare Data
      break;
    }
    adr++;
  }

  return (adr);
}
#endif

#ifdef CY8C5XXXX_PROTECT
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf) {
  unsigned char array;
  unsigned long n;

  if (sz > 64) return (adr);                    // Failure

  array = (unsigned char)((adr >> 6) & 0x1F);

  // Read Flash
  SPC_CPU_DATA = KEY1;
  SPC_CPU_DATA = KEY2 + READ_HIDDEN_ROW;
  SPC_CPU_DATA = READ_HIDDEN_ROW;
  SPC_CPU_DATA = array;
  SPC_CPU_DATA = 0;

  while (!(SPC_SR & DATA_READY));               // Wait until Data ready

  if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
    return (adr);                               // Failure
  }

  n = 256;
  while (sz--) {
    n--;
    while (!(SPC_SR & DATA_READY));             // Wait until Data ready
    if (SPC_CPU_DATA != *buf++) {               // Compare Data
      break;
    }
    adr++;
  }
  while (n--) {
    while (!(SPC_SR & DATA_READY));             // Wait until Data ready
    SPC_CPU_DATA;
  }

  while (!(SPC_SR & SPC_IDLE));                 // Wait until idle

  return (adr);
}
#endif

#if (defined (CY8C5XXXX_CFG_NVL) || defined (CY8C5XXXX_WO_NVL))
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf) {
  unsigned char nvl;
  unsigned long n;

  if (sz != 4) return (adr);                    // Failure

  // Read NVL and Verify
  for (n = 0; n < 4; n++) {

    // Read NVL Byte
    SPC_CPU_DATA = KEY1;
    SPC_CPU_DATA = KEY2 + READ_BYTE;
    SPC_CPU_DATA = READ_BYTE;
    SPC_CPU_DATA = ID_NVL;
    SPC_CPU_DATA = (unsigned char)n;

    while (!(SPC_SR & DATA_READY));             // Wait until Data ready

    if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
      return (adr);                             // Failure
    }

    nvl = SPC_CPU_DATA;

    while (!(SPC_SR & SPC_IDLE));               // Wait until idle

    if (((SPC_SR >> STATUS_CODE_POS) & STATUS_CODE_MASK) != OK) {
      return (adr);                             // Failure
    }

    if (nvl != *buf++) break;
    adr++;
  }

  return (adr);
}
#endif
