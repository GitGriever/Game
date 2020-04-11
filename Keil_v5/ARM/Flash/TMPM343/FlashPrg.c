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
 * Project:      Flash Programming Functions for Toshiba TMPM343 Flashes
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        // FlashOS Structures

#define M8(adr)  (*((volatile unsigned char  *)(adr)))
#define M16(adr) (*((volatile unsigned short *)(adr)))
#define M32(adr) (*((volatile unsigned long  *)(adr)))

#define STACK_SIZE     64      // Stack Size
#define PAGE_SIZE    4096      // Page Size for TMPM343 but data is written in junks of 4 Words


// Watchdog Timer Registers
#define WDMOD            (*((volatile unsigned long  *)0x400F2000ul))
#define WDCR             (*((volatile unsigned long  *)0x400F2004ul))

// Protect Status Register 0
#define FCPSR0           (*((volatile unsigned long  *)0x5DFF0020ul))
#define FCPSR0_RDY       (1ul << 0)

// Area Select Register
#define FCAREASEL        (*((volatile unsigned long  *)0x5DFF0140ul))
#define FCAREASEL_AREA0  (7ul << 0)
#define FCAREASEL_AREA1  (7ul << 4)


unsigned long base_adr;
unsigned long area_adr;
unsigned long area_sel;

/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  // Disable Watchdog
  WDMOD = 0x00;
  WDCR  = 0xB1;

  base_adr = adr;

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


/*  Blank Check Block in Flash Memory
 *    Parameter:      adr:  Block Start Address
 *                    sz:   Block Size (in bytes)
 *                    pat:  Block Pattern
 *    Return Value:   0 - OK,  1 - Failed
 */

int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat) {
  return (1);                                    // Always Force Erase
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {
  
  area_adr = 0x5E000000ul;
  area_sel = (FCAREASEL_AREA1 | FCAREASEL_AREA0);
  
  FCAREASEL = area_sel;                          // Specify the target area of Flash memory
  while (FCAREASEL != area_sel);  

  // Start Chip Erase Command
  M8(area_adr | 0x5400) = 0xAA;
  M8(area_adr | 0xAA00) = 0x55;
  M8(area_adr | 0x5400) = 0x80;
  M8(area_adr | 0x5400) = 0xAA;
  M8(area_adr | 0xAA00) = 0x55;
  M8(area_adr | 0x5400) = 0x10;

  while ((FCPSR0 & FCPSR0_RDY) != FCPSR0_RDY);   // Wait until completed

  FCAREASEL = 0;                                 // Reset the target area of Flash memory
  while (FCAREASEL != 0);  

  return (0);                                    // Finished without Errors
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {

  area_adr = 0x5E000000ul + (adr & 0x00080000ul);
  area_sel = (adr & 0x00080000ul) ? FCAREASEL_AREA1 : FCAREASEL_AREA0;

  FCAREASEL = area_sel;                          // Specify the target area of Flash memory
  while (FCAREASEL != area_sel);  

  // Start Block Erase Command
  M8(area_adr | 0x5400) = 0xAA;
  M8(area_adr | 0xAA00) = 0x55;
  M8(area_adr | 0x5400) = 0x80;
  M8(area_adr | 0x5400) = 0xAA;
  M8(area_adr | 0xAA00) = 0x55;
  M8(area_adr | adr   ) = 0x30;

  while ((FCPSR0 & FCPSR0_RDY) != FCPSR0_RDY);   // Wait until completed

  FCAREASEL = 0;                                 // Reset the target area of Flash memory
  while (FCAREASEL != 0);  

  return (0);                                    // Finished without Errors
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  int len = sz;
  
  area_adr = 0x5E000000 + (adr & 0x00080000);
  area_sel = (adr & 0x00080000) ? FCAREASEL_AREA1 : FCAREASEL_AREA0;

  FCAREASEL = area_sel;                          // Specify the target area of Flash memory
  while (FCAREASEL != area_sel);  

  while (len > 0) {
    // Start Page Programming Command
    M8(area_adr | 0x5400) = 0xAA;
    M8(area_adr | 0xAA00) = 0x55;
    M8(area_adr | 0x5400) = 0xA0;

    // Write Data in junks of 4 * 4 words
    M32(area_adr | adr) = *((unsigned long *)(buf +  0));
    M32(area_adr | adr) = *((unsigned long *)(buf +  4));
    M32(area_adr | adr) = *((unsigned long *)(buf +  8));
    M32(area_adr | adr) = *((unsigned long *)(buf + 12));
    adr += 16;
    buf += 16;
    len -= 16;

    while ((FCPSR0 & FCPSR0_RDY) != FCPSR0_RDY); // Wait until completed
  }

  FCAREASEL = 0;                                 // Reset the target area of Flash memory
  while (FCAREASEL != 0);  

  return (0);                                    // Finished without Errors
}
