/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2012 Keil - An ARM Company. All rights reserved.     */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.C:  Flash Programming Functions adapted                   */
/*               for SONix SN32F700 Flash                              */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        // FlashOS Structures
#include "F700_FlashCtrl.h"

typedef volatile unsigned long    vu32;
typedef          unsigned long     u32;

#define M32(adr) (*((vu32 *) (adr)))

// Peripheral Memory Map
#define WDT_BASE        0x40010000
#define FMC_BASE        0x40062000
#define SYS0_BASE       0x40060000

#define WDT             ((WDT_TypeDef*)   WDT_BASE)
#define FMC             ((FMC_TypeDef*)   FMC_BASE)
#define SYS0            ((SYS0_TypeDef*) SYS0_BASE)


// Watchdog Registers
typedef struct {
  vu32 CFG;                       // Offset 0x00  Watchdog Configuration register
  vu32 CLKSOURCE;                 // Offset 0x04  Watchdog Clock Source  register
  vu32 TC;                        // Offset 0x08  Watchdog Timer Constant register
  vu32 FEED;                      // Offset 0x0C  Watchdog Feed register
} WDT_TypeDef;

// Flash Memory Controller Registers
typedef struct {
  vu32 RESERVED0;                 // Offset 0x00
  vu32 STATUS;                    // Offset 0x04  Flash Status register
  vu32 CTRL;                      // Offset 0x08  Flash Control register
  vu32 DATA;                      // Offset 0x0C  Flash Data register
  vu32 ADDR;                      // Offset 0x10  Flash Address register
  vu32 CHKSUM;                    // Offset 0x14  Flash Checksum register
} FMC_TypeDef;

typedef struct
{	
  vu32 ANBCTRL;                   // Offset 0x00
  vu32 PLLCTRL;                   // Offset 0x04
  vu32 CSST;                      // Offset 0x08
  vu32 CLKCFG;                    // Offset 0x0C
  vu32 AHBCP;                     // Offset 0x10
  vu32 RSTST;                     // Offset 0x14
  vu32 LVDCTRL;                   // Offset 0x18
  vu32 EXRSTCTRL;                 // Offset 0x1C
  vu32 SWDCTRL;                   // Offset 0x20
  vu32 IVTM;                      // Offset 0x24
  vu32 RESERVED[2];
  vu32 SR;                        // Offset 0x30
} SYS0_TypeDef;


// Flash Control Register definitions
#define FLASH_PG                ((unsigned int)0x00000001)
#define FLASH_PER               ((unsigned int)0x00000002)
#define FLASH_STARTE            ((unsigned int)0x00000040)
#define FLASH_CHK               ((unsigned int)0x00000080)

// Flash Status Register definitions
#define FLASH_BSY               ((unsigned int)0x00000001)
#define FLASH_PGERR             ((unsigned int)0x00000004)
#define FLASH_EOP               ((unsigned int)0x00000020)

#define FLASH_ERR               (FLASH_PGERR)

/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  SYS0->IVTM = 1;                              // MAP to USER ROM
  WDT->CFG &= ~(1UL << 0);                     // disbale watchdog

	Flash_Ctrl1();

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
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {
	
	FMC->STATUS  &= ~(FLASH_ERR); 	                       // Reset Error Flags

	if(Flash_Ctrl2(adr) == 1)
		return (1);	
	
	FMC->CTRL    = FLASH_PER;                             // Page Erase Enabled 
	FMC->ADDR    = adr;                                   // Page Address	
	
	FMC->CTRL   |= FLASH_STARTE;                          // Start Erase

	while (FMC->STATUS & FLASH_BSY) {
		__nop();
	}

	if (FMC->STATUS & FLASH_ERR) {                        // Check for Errors
		return (1);                                         // Failed
	}

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

	if(Flash_Ctrl2(adr) == 1)
		return (1);
	
	sz = (sz + 3) & ~3;                                   // Adjust size for Words
		
	FMC->STATUS  &= ~(FLASH_ERR);                         // Reset Error Flags

	while (sz) {

		FMC->CTRL  =  FLASH_PG;                             // Programming Enabled
		FMC->ADDR  =  adr;
		FMC->DATA  = *((u32 *)buf);

		__nop();__nop();__nop();__nop();__nop();__nop();    // does not work without! reason ??
			
		while (FMC->STATUS & FLASH_BSY) {
			__nop();
		}

		if (FMC->STATUS & FLASH_ERR) {                      // Check for Errors
			return (1);                                       // Failed
		}

		adr += 4;                                           // Go to next Word
		buf += 4;
		sz  -= 4;
	}
	return (0);                                           // Done
}
