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
 * Project:      Flash Programming Functions for Toshiba TMPM365Fx Flash
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
#define PAGE_SIZE     256      // Page Size for TMPM365FYXBG


// Watchdog Timer Registers
#define WDMOD    (*((volatile unsigned long  *)0x400F2000))
#define WDCR     (*((volatile unsigned long  *)0x400F2004))

// Flash Control/Status Register
#define FLCSF    (*((volatile unsigned long  *)0x41FFF020))

#define FLCS_RDY  0x00000001


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
  M32(base_adr + 0x5400) = 0xAA;
  M32(base_adr + 0xAA00) = 0x55;
  M32(base_adr + 0x5400) = 0x80;
  M32(base_adr + 0x5400) = 0xAA;
  M32(base_adr + 0xAA00) = 0x55;
  M32(base_adr + 0x5400) = 0x10;

  while ((FLCSF & FLCS_RDY) != FLCS_RDY);      // Wait until completed

  return (0);                                  // Finished without Errors
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {

  // Start Block Erase Command
  M32(base_adr + 0x5400) = 0xAA;
  M32(base_adr + 0xAA00) = 0x55;
  M32(base_adr + 0x5400) = 0x80;
  M32(base_adr + 0x5400) = 0xAA;
  M32(base_adr + 0xAA00) = 0x55;
  M32(adr) = 0x30;

  while ((FLCSF & FLCS_RDY) != FLCS_RDY);      // Wait until completed

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
  M32(base_adr + 0x5400) = 0xAA;
  M32(base_adr + 0xAA00) = 0x55;
  M32(base_adr + 0x5400) = 0xA0;

  // Write Page Data
  for (i = 0; i < (PAGE_SIZE/4); i++) {
    M32(adr) = *((unsigned long *)buf);
    buf += 4;
  }

  while ((FLCSF & FLCS_RDY) != FLCS_RDY);      // Wait until completed

  return (0);                                  // Finished without Errors
}
