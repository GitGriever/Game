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
 * Project:      Flash Programming Functions for SiLabs SiM3x Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"

typedef volatile unsigned char    vu8;
typedef          unsigned char     u8;
typedef volatile unsigned short   vu16;
typedef          unsigned short    u16;
typedef volatile unsigned long    vu32;
typedef          unsigned long     u32;

#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))

// Peripheral Memory Map
#define CLK_BASE          0x4002D000
#define FLASH_BASE        0x4002E000
#define VMON_BASE         0x4002F000
#define WDT_BASE          0x40030000

#define CLK             ((CLK_TypeDef  *) CLK_BASE)
#define FLASH           ((FLASH_TypeDef*) FLASH_BASE)
#define VMON            ((VMON_TypeDef *) VMON_BASE)
#define WDT             ((WDT_TypeDef  *) WDT_BASE)

// Clock Control Registers
typedef struct {
  vu32 CONTROL;                           // offset 0x000 R/W Module Control Register
  vu32 RESERVED0[1];
  vu32 RESERVED1[1];
  vu32 RESERVED2[1];
  vu32 AHBCLKG;                           // offset 0x010 R/W AHB Clock Gate
  vu32 AHBCLKG_SET;                       // offset 0x014 R/W AHB Clock Gate Set   Bit
  vu32 AHBCLKG_CLR;                       // offset 0x018 R/W AHB Clock Gate Clear Bit
  vu32 RESERVED3[1];
  vu32 APBCLKG0;                          // offset 0x020 R/W APB Clock Gate 0
  vu32 APBCLKG0_SET;                      // offset 0x024 R/W APB Clock Gate 0 Set   Bit
  vu32 APBCLKG0_CLR;                      // offset 0x028 R/W APB Clock Gate 0 Clear Bit
  vu32 RESERVED4[1];
  vu32 APBCLKG1;                          // offset 0x030 R/W APB Clock Gate 1
  vu32 APBCLKG1_SET;                      // offset 0x034 R/W APB Clock Gate 1 Set   Bit
  vu32 APBCLKG1_CLR;                      // offset 0x038 R/W APB Clock Gate 1 Clear Bit
  vu32 RESERVED5[1];
  vu32 PM3CN;                             // offset 0x040 R/W Power Mode 3 Clock Control
} CLK_TypeDef;

// VMON Registers
typedef struct {
  vu32 CONTROL;                           // offset 0x000 R/W Module Configuration
  vu32 CONTROL_SET;                       // offset 0x004 R/W Module Configuration Set   Bit
  vu32 CONTROL_CLR;                       // offset 0x008 R/W Module Configuration Clear Bit
  vu32 RESERVED0[1];
} VMON_TypeDef;

// WDT Registers
typedef struct {
  vu32 CONTROL;                           // offset 0x000 R/W Module Control
  vu32 CONTROL_SET;                       // offset 0x004 R/W Module Control Set   Bit
  vu32 CONTROL_CLR;                       // offset 0x008 R/W Module Control Clear Bit
  vu32 RESERVED0[1];
  vu32 STATUS;                            // offset 0x010 R/W Module Status
  vu32 STATUS_SET;                        // offset 0x014 R/W Module Status Set   Bit
  vu32 STATUS_CLR;                        // offset 0x018 R/W Module Status Clear Bit
  vu32 RESERVED1[1];
  vu32 COMPARE;                           // offset 0x020 R/W Compare Values
  vu32 RESERVED2[3];
  vu32 WDTKEY;                            // offset 0x030 R/W Module Key
	} WDT_TypeDef;

// Flash Controller Registers
typedef struct {
  vu32 CONFIG;                            // offset 0x000 R/W Controller Configuration
  vu32 CONFIG_SET;                        // offset 0x000 R/W Controller Configuration Set   Bit
  vu32 CONFIG_CLR;                        // offset 0x000 R/W Controller Configuration Clear Bit
  vu32 RESERVED0[1];
  vu32 RESERVED1[4 * 9];
  vu32 WRADDR;                            // offset 0x0A0 R/W Flash Write Address
  vu32 RESERVED2[3];
  vu32 WRDATA;                            // offset 0x0B0 R/W Flash Write Data
  vu32 RESERVED3[3];
  vu32 KEY;                               // offset 0x0C0 R/W Flash Modification Key
  vu32 RESERVED4[3];
  vu32 TCONTROL;                          // offset 0x0D0 R/W Flash Timing Control
} FLASH_TypeDef;


#define FLASH_LOCKBYTE_ADDR   0x3FFFC     // address of the Lock Byte

// Flash Keys
#define FLASH_KEY_IUL         0xA5        // initial  unlock value
#define FLASH_KEY_SUL         0xF1        // single   unlock value
#define FLASH_KEY_MUL         0xF2        // multiple unlock value
#define FLASH_KEY_ML          0x5A        // multiple   lock value

// Flash Config Register definitions
#define FLASH_CONFIG_BUSYF    (1UL << 20)  // Flash Operation Busy Flag 
#define FLASH_CONFIG_BUFSTS   (1UL << 19)  // Flash Buffer Status   
#define FLASH_CONFIG_ERASEEN  (1UL << 18)  // Flash Page Erase Enable   
#define FLASH_CONFIG_SQWEN    (1UL << 16)  // Flash Write Sequence Enable



/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  /* disable Watchdog */
  WDT->WDTKEY = 0xA5;                      // Write Attention (ATTN)    Key
  WDT->WDTKEY = 0xDD;                      // Write Disable   (DISABLE) Key

  CLK->APBCLKG1_SET = (1UL <<  1);         // Enable the APB clock to the VMON0
  if ((VMON->CONTROL & (1UL << 31)) == 0) {
    return (1);                            // VDD Supply Monitor is disabled
  }

  /* Enable the APB clock to the Flash Controller Module
       SiM3U, SiM3C devices use bit 30 for Flash Controller Clock Enable
       SiM3L        devices use bit  0 for Flash Controller Clock Enable
     it does no harm to set both bits
   */
  CLK->APBCLKG0_SET = ((1UL << 30) | (1UL << 0));

  FLASH->CONFIG_CLR = 0x3;
  FLASH->CONFIG_SET = 0x1;                 // Set WaitStates to1 (require for > 20Mhz operation)

  return (0);
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {

  /* Disable the APB clock to the Flash Controller Module
       SiM3U, SiM3C devices use bit 30 for Flash Controller Clock Enable
       SiM3L        devices use bit  0 for Flash Controller Clock Enable
     it does no harm to reset both bits
   */
  /* Enable the APB clock to the Flash Controller Module
       SiM3U, SiM3C devices use bit 30 for Flash Controller Clock Enable
       SiM3L        devices use bit  0 for Flash Controller Clock Enable
     it does no harm to set both bits
   */
  CLK->APBCLKG0_CLR = ((1UL << 30) | (1UL << 0));

  return (0);
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {

  FLASH->CONFIG_SET = FLASH_CONFIG_ERASEEN;             // set Flash page erase enable 
  FLASH->KEY        = FLASH_KEY_IUL;                    // set initial unlock
  FLASH->KEY        = FLASH_KEY_SUL;                    // set single unlock

  FLASH->WRADDR     = adr;                              // Write page address
  FLASH->WRDATA     = 0x00;                             // erase page

  while (FLASH->CONFIG & FLASH_CONFIG_BUSYF) {
    ;
  }

  FLASH->CONFIG_CLR = FLASH_CONFIG_ERASEEN;             // clear Flash page erase enable 

  return (0);                                           // Done
}



/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {

  sz = (sz + 1) & ~1;                                   // Adjust size for HalfWords
  
  FLASH->CONFIG_CLR = FLASH_CONFIG_ERASEEN;             // clear Flash page erase enable 
  FLASH->CONFIG_SET = FLASH_CONFIG_SQWEN;               // enable Flash Write Sequence
  FLASH->KEY        = FLASH_KEY_IUL;                    // set initial unlock
  FLASH->KEY        = FLASH_KEY_MUL;                    // set multiple unlock

  FLASH->WRADDR     = adr;                              // Write 1st HalfWord address

  while (sz) {
    FLASH->WRDATA = *((u16 *)buf);                      // Program HalfWord
    while (FLASH->CONFIG & FLASH_CONFIG_BUFSTS);        // wait until buffer is available

    adr += 2;                                           // Go to next HalfWord
    buf += 2;
    sz  -= 2;

    if (adr == FLASH_LOCKBYTE_ADDR)
       break;                                           // do not write to Lock Byte
  }

  while (FLASH->CONFIG & FLASH_CONFIG_BUSYF) {
    ;
  }

  FLASH->KEY        = FLASH_KEY_ML;                     // set multiple lock
  FLASH->CONFIG_CLR = FLASH_CONFIG_SQWEN;               // disable Flash Write Sequence

  return (0);                                           // Done
}
