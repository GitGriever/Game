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
 * $Date:        18. November 2014
 * $Revision:    V1.00
 *  
 * Project:      Flash Programming Functions for ST STM32L0xx Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        // FlashOS Structures

typedef volatile unsigned char  vu8;
typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;

#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))

// Peripheral Memory Map
#define IWDG_BASE       0x40003000
#define FLASH_BASE      0x40022000

#define IWDG            ((IWDG_TypeDef *) IWDG_BASE)
#define FLASH           ((FLASH_TypeDef*) FLASH_BASE)

// Independent WATCHDOG
typedef struct {
  vu32 KR;                                      // offset  0x000 Key register (IWDG_KR)
  vu32 PR;                                      // offset  0x004 Prescaler register (IWDG_PR)
  vu32 RLR;                                     // offset  0x008 Reload register (IWDG_RLR)
  vu32 SR;                                      // offset  0x00C Status register (IWDG_SR)
} IWDG_TypeDef;

// Flash Registers
typedef struct {
  vu32 ACR;                                     // offset  0x000 Flash access control register (FLASH_ACR)
  vu32 PECR;                                    // offset  0x004 Flash program erase control register (FLASH_PECR)
  vu32 PDKEYR;                                  // offset  0x008 Flash power down key register (FLASH_PDKEYR)
  vu32 PEKEYR;                                  // offset  0x00C Flash program erase key register (FLASH_PEKEYR)
  vu32 PRGKEYR;                                 // offset  0x010 Flash program memory key register (FLASH_PRGKEYR)
  vu32 OPTKEYR;                                 // offset  0x014 Flash option key register (FLASH_OPTKEYR)
  vu32 SR;                                      // offset  0x018 Flash status register (FLASH_SR)
  vu32 OPTR;                                    // offset  0x01C Option byte register (FLASH_OBR)
  vu32 WRPRT;                                   // offset  0x020 Flash write protection register (FLASH_WRPR)
} FLASH_TypeDef;


// Flash Keys
#define FLASH_PEKEY1           (0x89ABCDEFul)
#define FLASH_PEKEY2           (0x02030405ul)
#define FLASH_PRGKEY1          (0x8C9DAEBFul)
#define FLASH_PRGKEY2          (0x13141516ul)
#define FLASH_OPTKEY1          (0xFBEAD9C8ul)
#define FLASH_OPTKEY2          (0x24252627ul)

// Flash program erase control register (FLASH_PECR) definitions
#define FLASH_PELOCK           (0x00000001ul)			// FLASH_PECR and data memory lock 
#define FLASH_PRGLOCK          (0x00000002ul)			// Program memory lock
#define FLASH_OPTLOCK          (0x00000004ul)			// Option bytes block lock
#define FLASH_PROG             (0x00000008ul)			// Program memory selection
#define FLASH_DATA             (0x00000010ul)			// Data memory selection
#define FLASH_OPT              (0x00000020ul)			// Option Bytes memory selection
#define FLASH_FIX              (0x00000100ul)			// Fixed time data write for Byte, Half Word and Word programming
#define FLASH_ERASE            (0x00000200ul)			// Page or Double Word erase mode
#define FLASH_FPRG             (0x00000400ul)			// Half Page/Double Word programming mode
#define FLASH_GBHF_ER          (0x00000800ul)			// Global Half Erase mode

// Flash status register (FLASH_SR) definitions
#define FLASH_BSY              (0x00000001ul)     // Write/erase operations in progress  
#define FLASH_EOP              (0x00000002ul)			// End of operation
#define FLASH_ENDHV            (0x00000004ul)			// End of high voltage
#define FLASH_WRPERR           (0x00000100ul)			// Write protected error
#define FLASH_PGAERR           (0x00000200ul)			// Programming alignment error
#define FLASH_SIZERR           (0x00000400ul)			// Size error
#define FLASH_OPTVERR          (0x00000800ul)			// Option validity error

#define FLASH_ERRs         (FLASH_PGAERR | FLASH_WRPERR | FLASH_SIZERR | FLASH_OPTVERR)

// Option byte register (FLASH_OBR) definitions
#define FLASH_IWDG_SW       (0x00100000ul)			// Software IWDG or Hardware IWDG selected

/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEMORY
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

      FLASH->SR |= FLASH_ERRs;                  // clear error flags

      // Unlock PECR Register    
      FLASH->PEKEYR = FLASH_PEKEY1;
      FLASH->PEKEYR = FLASH_PEKEY2;

      // Unlock Program Matrix    
      FLASH->PRGKEYR = FLASH_PRGKEY1;
      FLASH->PRGKEYR = FLASH_PRGKEY2;  


  return (0);
}
#endif  // FLASH_MEMORY

/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEMORY
int UnInit (unsigned long fnc) {

  switch (fnc) {
    case 1:
    case 2:
      // Lock PECR register and program matrix
      FLASH->PECR |= FLASH_PRGLOCK;             // Program memory lock
      FLASH->PECR |= FLASH_PELOCK;              // FLASH_PECR and data memory lock
    break;
  }

      return (0);
}
#endif  // FLASH_MEMORY

/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEMORY
int EraseSector (unsigned long adr) {

  FLASH->PECR |= FLASH_ERASE;                   // Page or Double Word Erase enabled
  FLASH->PECR |= FLASH_PROG;                    // Program memory selected
     
  M32(adr) = 0x00000000;                        // write '0' to the first address to erase page

  while (FLASH->SR & FLASH_BSY) {
    IWDG->KR = 0xAAAA;                          // Reload IWDG
  }

  FLASH->PECR &= ~FLASH_ERASE;                  // Page or Double Word Erase disabled
  FLASH->PECR &= ~FLASH_PROG;                   // Program memory deselected   

  return (0);                                   // Done
}
#endif  // FLASH_MEMORY

/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEMORY
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  unsigned long  cnt;
  unsigned long i;

  sz = (sz + 63) & ~63;                        // adjust programming size

  for (i = 0; i < (sz / 64); i++) {
    FLASH->PECR |= FLASH_FPRG;                 // Half Page programming mode enabled
    FLASH->PECR |= FLASH_PROG;                 // Program memory selected
                                               
    cnt = 64;                                  
    while (cnt ) {                             
       M32(adr) = *((unsigned long *)buf);     // Program Word
       adr += 4;                               
       buf += 4;                               
       cnt -= 4;                               
    }                                          
                                               
    while (FLASH->SR & FLASH_BSY) {            
      IWDG->KR = 0xAAAA;                       // Reload IWDG
    }                                          
                                               
    if (FLASH->SR & (FLASH_ERRs)) {            // Check for Errors
      FLASH->SR |= FLASH_ERRs;                 // clear error flags
      return (1);                              // Failed
    }                                          
                                               
    FLASH->PECR &= ~FLASH_FPRG;                // Half Page programming mode disabled
    FLASH->PECR &= ~FLASH_PROG;                // Program memory deselected   
  }                                            
                                               
  return (0);                                   // Done
}
#endif  // FLASH_MEMORY

