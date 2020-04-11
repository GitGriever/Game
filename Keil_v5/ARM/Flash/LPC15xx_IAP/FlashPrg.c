/***********************************************************************/
/*  This file is part of the uVision/ARM development tools             */
/*  Copyright (c) 2003-2013 Keil - An ARM Company.                     */
/*  All rights reserved.                                               */
/***********************************************************************/
/*                                                                     */
/*  FlashPrg.c:  Flash Programming Functions adapted for               */
/*               NXP LPC15xx Flash using IAP                           */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        // FlashOS Structures

#ifdef LPC15XX
// Memory Mapping Control
  #define MEMMAP       (*((volatile unsigned char *) 0x40074000))

// Main Clock
  #define MAINCLKSELA  (*((volatile unsigned long *) 0x40074080))
  #define MAINCLKSELB  (*((volatile unsigned long *) 0x40074084))
  #define SYSAHBCLKDIV (*((volatile unsigned long *) 0x400740C0))
#endif


#define SET_VALID_CODE 1       // Set Valid User Code Signature

#ifdef LPC15XX
  #ifdef FLASH_256
    #define END_SECTOR     63
  #endif
  #ifdef FLASH_128
    #define END_SECTOR     31
  #endif
  #ifdef FLASH_64
    #define END_SECTOR     15
  #endif
#endif  // LPC15XX


unsigned long CCLK;            // CCLK in kHz

struct sIAP {                  // IAP Structure
  unsigned long cmd;           // Command
  unsigned long par[4];        // Parameters
  unsigned long stat;          // Status
  unsigned long res[2];        // Result
} IAP;


/* IAP Call */
typedef void (*IAP_Entry) (unsigned long *cmd, unsigned long *stat);
#define IAP_Call ((IAP_Entry) 0x03000205)


/*
 * Get Sector Number
 *    Parameter:      adr:  Sector Address
 *    Return Value:   Sector Number
 */

unsigned long GetSecNum (unsigned long adr) {
  unsigned long n;

#ifdef LPC15XX
  n = adr >> 12;                               //  4kB Sector
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

#ifdef LPC15XX
  CCLK       = 12000;                          // 12MHz Internal RC Oscillator

  /* Switch Main clk source A to IRC and Main clk source B to Main clk A out */
  MAINCLKSELA = 0;
  MAINCLKSELB = 0;
  SYSAHBCLKDIV = 1;                            // Set Main Clock divider to 1

  MEMMAP     = 0x02;                           // User Flash Mode

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
  IAP.par[2] = 1024;                           // Fixed Page Size
  IAP.par[3] = CCLK;                           // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (1);                    // Command Failed

  return (0);                                  // Finished without Errors
}
