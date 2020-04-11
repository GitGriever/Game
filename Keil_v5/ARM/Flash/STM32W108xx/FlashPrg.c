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
 * Project:      Flash Programming Functions for ST Microelectronics STM32W108 Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        // FlashOS Structures

typedef volatile unsigned char    vu8;
typedef volatile unsigned short   vu16;
typedef volatile unsigned long    vu32;

#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))

// Peripheral Memory Map
#define FPEC_BASE         0x40004000
#define FLASH_BASE        0x40008000
#define FLASH_OPT_BASE    0x08040800            // start address of the option bytes 

#define FPEC            ((FPEC_TypeDef *) FPEC_BASE)
#define FLASH           ((FLASH_TypeDef*) FLASH_BASE)

// FPEC Registers
typedef struct {
  vu32 RESERVED0[11];
  vu32 CLKER;                                   // offset  0x02C
  vu32 CLKSR;                                   // offset  0x030
} FPEC_TypeDef;

// Flash Registers
typedef struct {
  vu32 ACR;                                     // offset  0x000
  vu32 KEYR;                                    // offset  0x004
  vu32 OPTKEYR;                                 // offset  0x008
  vu32 SR;                                      // offset  0x00C
  vu32 CR;                                      // offset  0x010
  vu32 AR;                                      // offset  0x014
  vu32 RESERVED0[1];
  vu32 OBR;                                     // offset  0x01C
  vu32 WRPR;                                    // offset  0x020
} FLASH_TypeDef;


// Flash Keys
#define RDPRT_KEY          0x00A5
#define FLASH_KEY1         0x45670123
#define FLASH_KEY2         0xCDEF89AB

// Flash Control Register definitions
#define FLASH_PG           (1UL << 0)
#define FLASH_PER          (1UL << 1)
#define FLASH_MER          (1UL << 2)
#define FLASH_OPTPG        (1UL << 4)
#define FLASH_OPTER        (1UL << 5)
#define FLASH_STRT         (1UL << 6)
#define FLASH_LOCK         (1UL << 7)
#define FLASH_OPTWRE       (1UL << 9)

// Flash Status Register definitions
#define FLASH_BSY          (1UL << 0)
#define FLASH_PGERR        (1UL << 2)
#define FLASH_WRPRTERR     (1UL << 4)
#define FLASH_EOP          (1UL << 5)


unsigned long base_adr;


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  // Unlock Flash    
  FLASH->KEYR  = FLASH_KEY1;
  FLASH->KEYR  = FLASH_KEY2;

  // Zero Wait State
  FLASH->ACR  = 0x00000000;
  
  FPEC->CLKER = (1UL << 0);                // enable FPEC_CLK

  return (0);
}
#endif

#ifdef FLASH_OPT
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  // Unlock Flash    
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;

  // Unlock Option Bytes
  FLASH->OPTKEYR = FLASH_KEY1;
  FLASH->OPTKEYR = FLASH_KEY2;

  // Zero Wait State
  FLASH->ACR  = 0x00000000;
  
  FPEC->CLKER = (1UL << 0);                // enable FPEC_CLK

  return (0);
}
#endif


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int UnInit (unsigned long fnc) {

  // Lock Flash
  FLASH->CR  |=  FLASH_LOCK;

  FPEC->CLKER = (0UL << 0);                // disable FPEC_CLK

  return (0);
}
#endif

#ifdef FLASH_OPT
int UnInit (unsigned long fnc) {

  // Lock Flash & Option Bytes
  FLASH->CR &= ~FLASH_OPTWRE;
  FLASH->CR |=  FLASH_LOCK;

  FPEC->CLKER = (0UL << 0);                // disable FPEC_CLK

  return (0);
}
#endif


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int EraseChip (void) {

  FLASH->CR  |=  FLASH_MER;                     // Mass Erase Enabled
  FLASH->CR  |=  FLASH_STRT;                    // Start Erase

  while (FLASH->SR  & FLASH_BSY) {
    __nop();                                    // do nothing
  }

  FLASH->CR  &= ~FLASH_MER;                     // Mass Erase Disabled

  return (0);                                   // Done
}
#endif

#ifdef FLASH_OPT
int EraseChip (void) {

  FLASH->CR |=  FLASH_OPTER;                    // Option Byte Erase Enabled 
  FLASH->CR |=  FLASH_STRT;                     // Start Erase

  while (FLASH->SR & FLASH_BSY) {
    __nop();                                    // do nothing
  }

  FLASH->CR &= ~FLASH_OPTER;                    // Option Byte Erase Disabled 

  // Unprotect Flash

  FLASH->CR |=  FLASH_OPTPG;                    // Option Byte Programming Enabled

  M16(FLASH_OPT_BASE) = RDPRT_KEY;              // Program Half Word: RDPRT Key
  while (FLASH->SR & FLASH_BSY) {
    __nop();                                    // do nothing
  }

  FLASH->CR &= ~FLASH_OPTPG;                    // Option Byte Programming Disabled

  // Check for Errors
  if (FLASH->SR & (FLASH_PGERR | FLASH_WRPRTERR)) {
    FLASH->SR |= FLASH_PGERR | FLASH_WRPRTERR;
    return (1);                                 // Failed
  }

  return (0);                                   // Done
}
#endif


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int EraseSector (unsigned long adr) {

  FLASH->CR  |=  FLASH_PER;                     // Page Erase Enabled 
  FLASH->AR   =  adr;                           // Page Address
  FLASH->CR  |=  FLASH_STRT;                    // Start Erase

  while (FLASH->SR  & FLASH_BSY) {
    __nop();                                    // do nothing
  }

  FLASH->CR  &= ~FLASH_PER;                     // Page Erase Disabled 

  return (0);                                   // Done
}
#endif

#ifdef FLASH_OPT
int EraseSector (unsigned long adr) {

  FLASH->CR |=  FLASH_OPTER;                    // Option Byte Erase Enabled 
  FLASH->CR |=  FLASH_STRT;                     // Start Erase

  while (FLASH->SR & FLASH_BSY) {
    __nop();                                    // do nothing
  }

  FLASH->CR &= ~FLASH_OPTER;                    // Option Byte Erase Disabled 

  return (0);                                   // Done
}
#endif


/*  
 *  Blank Check Checks if Memory is Blank
 *    Parameter:      adr:  Block Start Address
 *                    sz:   Block Size (in bytes)
 *                    pat:  Block Pattern
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_OPT
int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat) {
  return (1);                                   // Always Force Erase
}
#endif


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {

  sz = (sz + 1) & ~1;                           // Adjust size for Half Words
  
  while (sz) {

    FLASH->CR  |=  FLASH_PG;                    // Programming Enabled

    M16(adr) = *((unsigned short *)buf);        // Program Half Word
    while (FLASH->SR & FLASH_BSY) {
      __nop();                                  // do nothing
    }

    FLASH->CR  &= ~FLASH_PG;                    // Programming Disabled

    // Check for Errors
    if (FLASH->SR  & (FLASH_PGERR | FLASH_WRPRTERR)) {
      FLASH->SR  |= FLASH_PGERR | FLASH_WRPRTERR;
      return (1);                               // Failed
    }

    // Go to next Half Word
    adr += 2;
    buf += 2;
    sz  -= 2;
  }

  return (0);                                   // Done
}
#endif

#ifdef FLASH_OPT
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {

  sz = (sz + 1) & ~1;                           // Adjust size for Half Words
  
  while (sz) {

    FLASH->CR |=  FLASH_OPTPG;                  // Option Byte Programming Enabled

    M16(adr) = *((unsigned short *)buf);        // Program Half Word
    while (FLASH->SR & FLASH_BSY) {
      __nop();                                  // do nothing
    }

    FLASH->CR &= ~FLASH_OPTPG;                  // Option Byte Programming Disabled

    // Check for Errors
    if (FLASH->SR & (FLASH_PGERR | FLASH_WRPRTERR)) {
      FLASH->SR |= FLASH_PGERR | FLASH_WRPRTERR;
      return (1);                               // Failed
    }

    if (adr == (FLASH_OPT_BASE + 0)) {
      // skip reserved option bytes
      adr += 8;
      buf += 8;
      sz  -= 8;
    } else {
      // Go to next Half Word
      adr += 2;
      buf += 2;
      sz  -= 2;
    }
   
  }

  return (0);                                   // Done
}
#endif


/*
 *
 *  Verify Flash Contents
 *    Parameter:      adr:  Start Address
 *                    sz:   Size (in bytes)
 *                    buf:  Data
 *    Return Value:   (adr+sz) - OK, Failed Address
 */

#ifdef FLASH_OPT
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf) {

  while (sz) {

    if (M8(adr) != *buf) {
      return (adr);                             // Verification Failed
    }

    if (adr == (FLASH_OPT_BASE + 1)) {
      // skip reserved option bytes
      adr += 7;
      buf += 7;
      sz  -= 7;
    } else {
      // Go to next Byte
      adr += 1;
      buf += 1;
      sz  -= 1;
    }
  }

  return (adr+sz);                              // Done successfully
}
#endif
