/* -----------------------------------------------------------------------------
 * Copyright (c) 2004 - 2014 ARM Ltd.
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
 * $Date:        16. June 2014
 * $Revision:    V1.00
 *
 * Project:      Flash Device Description for
                 NXP LPC11xx/LPC11U6x/LPC11E6x13xx/LPC17xx Flash using IAP
 * --------------------------------------------------------------------------- */

#include "..\FlashOS.H"        // FlashOS Structures

// Memory Mapping Control
#if defined LPC1XXX  || defined LPC11U6X
  #define MEMMAP     (*((volatile unsigned char *) 0x40048000))

  //-----------------------------------------------------
  // undocumentad stuff from NXP for LPC122x
  #define FLASH_REG_BASE 0x50060000
  #define EPPCFG     ((volatile unsigned *)(FLASH_REG_BASE+0x040))
  #define EPPAA      ((volatile unsigned *)(FLASH_REG_BASE+0x044))
  #define EPPAB      ((volatile unsigned *)(FLASH_REG_BASE+0x048))
  #define TNVS       ((volatile unsigned *)(FLASH_REG_BASE+0x04C))
  #define TRCV       ((volatile unsigned *)(FLASH_REG_BASE+0x050))
  #define TERASE     ((volatile unsigned *)(FLASH_REG_BASE+0x054))
  #define TME        ((volatile unsigned *)(FLASH_REG_BASE+0x058))
  #define TNVH       ((volatile unsigned *)(FLASH_REG_BASE+0x05C))
  #define TNVH1      ((volatile unsigned *)(FLASH_REG_BASE+0x060))
  #define TPGS       ((volatile unsigned *)(FLASH_REG_BASE+0x064))
  #define TPROG      ((volatile unsigned *)(FLASH_REG_BASE+0x068))
  //-----------------------------------------------------
#endif

#ifdef LPC17XX
  #define MEMMAP     (*((volatile unsigned long *) 0x400FC040))
#endif

// Main Clock
#if defined LPC1XXX  || defined LPC11U6X
  #define MAINCLKSEL (*((volatile unsigned long *) 0x40048070))
  #define MAINCLKUEN (*((volatile unsigned long *) 0x40048074))
  #define MAINCLKDIV (*((volatile unsigned long *) 0x40048078))
#endif

// Phase Locked Loop (Main PLL)
#ifdef LPC17XX
  #define PLL0CON    (*((volatile unsigned long *) 0x400FC080))
  #define PLL0CFG    (*((volatile unsigned long *) 0x400FC084))
  #define PLL0STAT   (*((volatile unsigned long *) 0x400FC088))
  #define PLL0FEED   (*((volatile unsigned long *) 0x400FC08C))
  #define CCLKSEL    (*((volatile unsigned long *) 0x400FC104))
  #define CLKSRCSEL  (*((volatile unsigned long *) 0x400FC10C))
#endif


#define STACK_SIZE     64      // Stack Size

#define SET_VALID_CODE 1       // Set Valid User Code Signature

#ifdef LPC1XXX
  #ifdef FLASH_128
    #define END_SECTOR     31
  #endif
  #ifdef FLASH_96
    #define END_SECTOR     23
  #endif
  #ifdef FLASH_80
    #define END_SECTOR     19
  #endif
  #ifdef FLASH_64
    #define END_SECTOR     15
  #endif
  #ifdef FLASH_56
    #define END_SECTOR     13
  #endif
  #ifdef FLASH_48
    #define END_SECTOR     11
  #endif
  #ifdef FLASH_40
    #define END_SECTOR     9
  #endif
  #ifdef FLASH_32
    #define END_SECTOR     7
  #endif
  #ifdef FLASH_24
    #define END_SECTOR     5
  #endif
  #ifdef FLASH_16
    #define END_SECTOR     3
  #endif
  #ifdef FLASH_8
    #define END_SECTOR     1
  #endif
  #ifdef FLASH_4
    #define END_SECTOR     0
  #endif
#endif  // LPC1XXX

#ifdef LPC11U6X
  #ifdef FLASH_256
    #define END_SECTOR     28
  #endif
  #ifdef FLASH_128
    #define END_SECTOR     24
  #endif
#endif // LPC11U6X

#ifdef LPC17XX
  #ifdef FLASH_512
    #define END_SECTOR     29
  #endif
  #ifdef FLASH_256
    #define END_SECTOR     21
  #endif
  #ifdef FLASH_128
    #define END_SECTOR     17
  #endif
  #ifdef FLASH_64
    #define END_SECTOR     15
  #endif
  #ifdef FLASH_32
    #define END_SECTOR     7
  #endif
#endif  // LPC17XX

unsigned long CCLK;            // CCLK in kHz

struct sIAP {                  // IAP Structure
  unsigned long cmd;           // Command
  unsigned long par[4];        // Parameters
  unsigned long stat;          // Status
  unsigned long res[2];        // Result
} IAP;


/* IAP Call */
typedef void (*IAP_Entry) (unsigned long *cmd, unsigned long *stat);
#define IAP_Call ((IAP_Entry) 0x1FFF1FF1)


/*
 * Get Sector Number
 *    Parameter:      adr:  Sector Address
 *    Return Value:   Sector Number
 */

unsigned long GetSecNum (unsigned long adr) {
  unsigned long n;

#ifdef LPC1XXX
  n = adr >> 12;                               //  4kB Sector
#endif
#ifdef LPC11U6X
  n = adr >> 12;                               //  4kB Sector
  if (n >= 0x18) {
    n = 0x15 + (n >> 3);                       // 32kB Sector
  }
#endif
#ifdef LPC17XX
  n = adr >> 12;                               //  4kB Sector
  if (n >= 0x10) {
    n = 0x0E + (n >> 3);                       // 32kB Sector
  }
#endif

  return (n);                                  // Sector Number
}


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

#if defined LPC1XXX  || defined LPC11U6X
  CCLK       = 12000;                          // 12MHz Internal RC Oscillator

  MAINCLKSEL = 0;                              // Select Internal RC Oscillator
  MAINCLKUEN = 1;                              // Update Main Clock Source
  MAINCLKUEN = 0;                              // Toggle Update Register
  MAINCLKUEN = 1;
//  while (!(MAINCLKUEN & 1));                   // Wait until updated
  MAINCLKDIV = 1;                              // Set Main Clock divider to 1

  MEMMAP     = 0x02;                           // User Flash Mode
#endif

#ifdef LPC17XX
  IAP.stat =  0;                               // Note: Some Bootloader versions don't set the status if this command is executed
  IAP.cmd  = 54;                               // Read Part ID
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

  switch ((IAP.res[0] >> 24) & 0xFF) {
    case 0x25:
                                               // Part ID LPC1759 = 0x25113737
                                               // Part ID LPC1758 = 0x25013F37
                                               // Part ID LPC1756 = 0x25011723
                                               // Part ID LPC1754 = 0x25011722
                                               // Part ID LPC1752 = 0x25001121
                                               // Part ID LPC1751 = 0x25001118 / 0x25001110
    case 0x26:
                                               // Part ID LPC1769 = 0x26113F37
                                               // Part ID LPC1768 = 0x26013F37
                                               // Part ID LPC1767 = 0x26012837
                                               // Part ID LPC1766 = 0x26013F33
                                               // Part ID LPC1765 = 0x26013733
                                               // Part ID LPC1764 = 0x26011922
      CCLK  =  4000;                           //  4MHz Internal RC Oscillator
      break;
    case 0x27:
                                               // Part ID LPC1778 = 0x27193F47
                                               // Part ID LPC1777 = 0x27193747
                                               // Part ID LPC1776 = 0x27191F43
                                               // Part ID LPC1774 = 0x27011132
                                               // Part ID LPC1772 = 0x27011121
    case 0x20:  // found out during test
    case 0x28:
                                               // Part ID LPC1788 = 0x281D3F47
                                               // Part ID LPC1787 = 0x281D3747
                                               // Part ID LPC1786 = 0x281D1F43
                                               // Part ID LPC1785 = 0x281D1743
    case 0x48:                                 // Part ID LPC4088 = 0x481D3F47
    case 0x47:                                 // Part ID LPC4078 = 0x47193F47
                                               // Part ID LPC4076 = 0x47191F43
                                               // Part ID LPC4074 = 0x47011132
      CCLK  = 12000;                           // 12MHz Internal RC Oscillator
      break;
    default:
      CCLK  =  4000;                           //  4MHz Internal RC Oscillator
  }

  CLKSRCSEL = 0x00;                            // sysclk = IRC

  PLL0CON  = 0x00;                             // Disable PLL (use Oscillator)
  PLL0FEED = 0xAA;                             // Feed Sequence Part #1
  PLL0FEED = 0x55;                             // Feed Sequence Part #2

  switch ((IAP.res[0] >> 24) & 0xFF) {
    case 0x27:                                 // LPC177x
    case 0x20:                                 // LPC178x
    case 0x28:                                 // LPC178x
    case 0x48:                                 // LPC407x
    case 0x47:                                 // LPC408x
      CCLKSEL  = 0x01;                         // use Sysclk devided by 1 for CPU
      break;
    default:
      CCLKSEL  = 0x00;                         // CPU clk divider is 1
  }

  MEMMAP   = 0x01;                             // User Flash Mode
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
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {

  IAP.cmd    = 50;                             // Prepare Sector for Erase
  IAP.par[0] = 0;                              // Start Sector
  IAP.par[1] = END_SECTOR;                     // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

  IAP.cmd    = 52;                             // Erase Sector
  IAP.par[0] = 0;                              // Start Sector
  IAP.par[1] = END_SECTOR;                     // End Sector
  IAP.par[2] = CCLK;                           // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

  return (0);                                  // Finished without Errors
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {
  unsigned long n;

  n = GetSecNum(adr);                          // Get Sector Number

  IAP.cmd    = 50;                             // Prepare Sector for Erase
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

  IAP.cmd    = 52;                             // Erase Sector
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP.par[2] = CCLK;                           // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

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
  unsigned long n;

#if SET_VALID_CODE != 0                        // Set valid User Code Signature
  if (adr == 0) {                              // Check for Vector Table
    n = *((unsigned long *)(buf + 0x00)) +
        *((unsigned long *)(buf + 0x04)) +
        *((unsigned long *)(buf + 0x08)) +
        *((unsigned long *)(buf + 0x0C)) +
        *((unsigned long *)(buf + 0x10)) +
        *((unsigned long *)(buf + 0x14)) +
        *((unsigned long *)(buf + 0x18));
    *((unsigned long *)(buf + 0x1C)) = 0 - n;  // Signature at Reserved Vector
  }
#endif

  n = GetSecNum(adr);                          // Get Sector Number

  IAP.cmd    = 50;                             // Prepare Sector for Write
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

  IAP.cmd    = 51;                             // Copy RAM to Flash
  IAP.par[0] = adr;                            // Destination Flash Address
  IAP.par[1] = (unsigned long)buf;             // Source RAM Address
#if defined LPC1XXX && defined FLASH_4
  IAP.par[2] = 256;                            // Fixed Page Size
#else
  IAP.par[2] = 1024;                           // Fixed Page Size
#endif
  IAP.par[3] = CCLK;                           // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

  return (0);                                  // Finished without Errors
}
