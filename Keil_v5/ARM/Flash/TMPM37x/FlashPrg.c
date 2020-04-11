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
 * Project:      Flash Programming Functions for Toshiba TMPM37x Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        // FlashOS Structures

#define M8(adr)  (*((volatile unsigned char  *)(adr)))
#define M16(adr) (*((volatile unsigned short *)(adr)))
#define M32(adr) (*((volatile unsigned long  *)(adr)))

// Watchdog Timer Registers
#define WDMOD    (*((volatile unsigned char  *)0x40040000))
#define WDCR     (*((volatile unsigned char  *)0x40040004))

// Flash Control/Status Register
#define FLCSF0   (*((volatile unsigned long  *)0x41FFF020))

#define FLCS_RDY  0x00000001

#if defined TMPM37x_64
  #define PAGE_SIZE        128      // Page Size

  #define MACRO_ADD1(add) (0x000550)  
  #define MACRO_ADD2(add) (0x000AA0)  
#endif

#if defined TMPM37x_128 || defined TMPM37x_256
  #define PAGE_SIZE        256      // Page Size

  #define MACRO_ADD1(add) (0x005400)  
  #define MACRO_ADD2(add) (0x00AA00)  
#endif

#if defined TMPM37x_512
  #define PAGE_SIZE        512      // Page Size

  #define MACRO_ADD1(add) (0x005400)  
  #define MACRO_ADD2(add) (0x00AA00)  
#endif

unsigned long base_adr;


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
  return (1);                                  // Always Force Erase
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {

  // Start Chip Erase Command
  M32(base_adr + MACRO_ADD1(adr)) = 0xAA;
  M32(base_adr + MACRO_ADD2(adr)) = 0x55;
  M32(base_adr + MACRO_ADD1(adr)) = 0x80;
  M32(base_adr + MACRO_ADD1(adr)) = 0xAA;
  M32(base_adr + MACRO_ADD2(adr)) = 0x55;
  M32(base_adr + MACRO_ADD1(adr)) = 0x10;

  while ((FLCSF0 & FLCS_RDY) != FLCS_RDY);     // Wait until completed

  return (0);                                  // Finished without Errors
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {

  // Start Block Erase Command
  M32(base_adr + MACRO_ADD1(adr)) = 0xAA;
  M32(base_adr + MACRO_ADD2(adr)) = 0x55;
  M32(base_adr + MACRO_ADD1(adr)) = 0x80;
  M32(base_adr + MACRO_ADD1(adr)) = 0xAA;
  M32(base_adr + MACRO_ADD2(adr)) = 0x55;
  M32(adr) = 0x30;

  while ((FLCSF0 & FLCS_RDY) != FLCS_RDY);     // Wait until completed

  return (0);                                  // Finished without Errors
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  int i;

  // Start Page Programming Command
  M32(base_adr + MACRO_ADD1(adr)) = 0xAA;
  M32(base_adr + MACRO_ADD2(adr)) = 0x55;
  M32(base_adr + MACRO_ADD1(adr)) = 0xA0;

  // Write Page Data
  for (i = 0; i < (PAGE_SIZE/4); i++) {
    M32(adr) = *((unsigned long *)buf);

    buf += 4;
  }

  while ((FLCSF0 & FLCS_RDY) != FLCS_RDY);     // Wait until completed

  return (0);                                  // Finished without Errors
}
