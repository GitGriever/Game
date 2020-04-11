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
 * Project:      Flash Programming Functions for Freescale MKExx
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        // FlashOS Structures

typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

/** WDOG - Peripheral register structure */
typedef struct WDOG_MemMap {
  uint16_t STCTRLH;                                /**< Watchdog Status and Control Register High, offset: 0x0 */
  uint16_t STCTRLL;                                /**< Watchdog Status and Control Register Low, offset: 0x2 */
  uint16_t TOVALH;                                 /**< Watchdog Time-out Value Register High, offset: 0x4 */
  uint16_t TOVALL;                                 /**< Watchdog Time-out Value Register Low, offset: 0x6 */
  uint16_t WINH;                                   /**< Watchdog Window Register High, offset: 0x8 */
  uint16_t WINL;                                   /**< Watchdog Window Register Low, offset: 0xA */
  uint16_t REFRESH;                                /**< Watchdog Refresh Register, offset: 0xC */
  uint16_t UNLOCK;                                 /**< Watchdog Unlock Register, offset: 0xE */
  uint16_t TMROUTH;                                /**< Watchdog Timer Output Register High, offset: 0x10 */
  uint16_t TMROUTL;                                /**< Watchdog Timer Output Register Low, offset: 0x12 */
  uint16_t RSTCNT;                                 /**< Watchdog Reset Count Register, offset: 0x14 */
  uint16_t PRESC;                                  /**< Watchdog Prescaler Register, offset: 0x16 */
} volatile * WDOG_MemMapPtr;
/** FTMRA - Peripheral register structure */
typedef struct FTMRA_MemMap {
  uint8_t FSEC;                                    /**< Flash Security Register, offset: 0x0 */
  uint8_t FCLKDIV;                                 /**< Flash Clock Divider Register, offset: 0x1 */
  uint8_t FECCRIX;                                 /**< Flash ECCR Index Register, offset: 0x2 */
  uint8_t FCCOBIX;                                 /**< Flash Common Command Object Index Register, offset: 0x3 */
  uint8_t FERCNFG;                                 /**< Flash Error Configuration Register, offset: 0x4 */
  uint8_t FCNFG;                                   /**< Flash Configuration Register, offset: 0x5 */
  uint8_t FERSTAT;                                 /**< Flash Error Status Register, offset: 0x6 */
  uint8_t FSTAT;                                   /**< Flash Status Register, offset: 0x7 */
  uint8_t DFPROT;                                  /**< D-Flash Protection Register, offset: 0x8 */
  uint8_t FPROT;                                   /**< P-Flash Protection Register, offset: 0x9 */
  uint8_t FCCOBLO;                                 /**< Flash Common Command Object Low Register, offset: 0xA */
  uint8_t FCCOBHI;                                 /**< Flash Common Command Object High Register, offset: 0xB */
  uint8_t RESERVED_0[2];
  uint8_t FECCRLO;                                 /**< Flash ECC Error Results Low Register, offset: 0xE */
  uint8_t FECCRHI;                                 /**< Flash ECC Error Results High Register, offset: 0xF */
  uint8_t RESERVED_1[1];
  uint8_t FOPT;                                    /**< Flash Option Register, offset: 0x11 */
} volatile *FTMRA_MemMapPtr;
/** Peripheral Map **/
#define FTMRA                           ((FTMRA_MemMapPtr)0x40039000u)
#define WDOG                            ((WDOG_MemMapPtr)0x4003A000u)


#define M16(adr) (*((volatile unsigned short *)(adr)))

#define WDOG_STCTRLH_WDOGEN_MASK                 0x1u

#define FTMRA_FCLKDIV_FDIVLD_MASK                0x80u
#define FTMRA_FSTAT_CCIF_MASK                    0x80u
#define FTMRA_FSTAT_ACCERR_MASK                  0x20u
#define FTMRA_FSTAT_FPVIOL_MASK                  0x10u
#define FTMRA_FSTAT_MGSTAT_MASK                  0x3u
#define FTMRA_ERROR                              (FTMRA_FSTAT_ACCERR_MASK | FTMRA_FSTAT_FPVIOL_MASK | FTMRA_FSTAT_MGSTAT_MASK)
#define FTMRA_FCCOB                              *((uint16_t *)(0x0a + 0x40039000))

#define BUS_CLK         (20971520/2)

#define FTFx_SSD_FSTAT_FPVIOL (8)
/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */



int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {
  
  WDOG->UNLOCK = 0xC520;                               /* Write 0xC520 to the unlock register */
  WDOG->UNLOCK = 0xD928;                               /* Followed by 0xD928 to complete the unlock */
  WDOG->STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;          /* Clear the WDOGEN bit to disable the watchdog */
  
  if (!(FTMRA->FCLKDIV & FTMRA_FCLKDIV_FDIVLD_MASK)) { /* if FCLKDIV have not write since the last reset, initialize the flash and write FCLKDIV */
    
    uint32_t uiFlashClock;
    uint8_t  ucFlashDiv;
    ucFlashDiv = BUS_CLK/1000000 - 1;
    uiFlashClock = BUS_CLK/(ucFlashDiv + 1);
    /*
    * insure flash clock greater than 0.8M and less than 1.05M
    */
    if( uiFlashClock < 800000 )
    {
      FTMRA->FCLKDIV = ucFlashDiv - 1;
    }
    else if( uiFlashClock > 1050000 )
    {
      FTMRA->FCLKDIV = ucFlashDiv + 1;
    }
    else
    {
      FTMRA->FCLKDIV = ucFlashDiv;
    }
  }

  return (0);
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {

  /* Add your Code */
  return (0);                                  // Finished without Errors
}

/*  
 *  Verify Flash Contents
 *    Parameter:      adr:  Start Address
 *                    sz:   Size (in bytes)
 *                    buf:  Data
 *    Return Value:   (adr+sz) - OK, Failed Address
 */
#ifdef DFLASH
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf) {
  int i;
  unsigned long b_adr = adr;
  for (i = 0; i < ((sz+1)/2); i++)  {
    if (M16(b_adr) != (*(volatile unsigned short *)buf)) return(0);
    buf   += 2;
    b_adr += 2;
  }
  return (adr+sz);                                  // Finished without Errors
}
#endif

/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {
  // Clear error flags
  FTMRA->FSTAT = 0x30;
  
  // Write index to specify the command code to be loaded
  FTMRA->FCCOBIX = 0;
  // Write command code and memory address bits[17:16]	
  FTMRA->FCCOBHI = 0x09;// Erase Block command
#ifdef DFLASH
  FTMRA->FCCOBLO = (uint8_t) (0x80); // Bit7 = 1 for DFlash	
#else
  FTMRA->FCCOBLO = (uint8_t) (0x00); // Bit7 = 0 for PFlash	
#endif  
  // Launch the command
  FTMRA->FSTAT = 0x80;
  
  // Wait till command is completed
  while (!(FTMRA->FSTAT & FTMRA_FSTAT_CCIF_MASK))
    ;
  if (FTMRA->FSTAT & FTMRA_FSTAT_FPVIOL_MASK)  return(FTFx_SSD_FSTAT_FPVIOL);
  if (FTMRA->FSTAT & FTMRA_ERROR)              return(1);
  return (0);
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {
  // Clear error flags
  FTMRA->FSTAT = 0x30;
  
#ifdef DFLASH
  // Write index to specify the command code to be loaded
  FTMRA->FCCOBIX = 0;
  // Write command code and memory address bits[17:16]	
  FTMRA->FCCOBHI = 0x12;                            // D-Flash Sector Erase command
  FTMRA->FCCOBLO = (uint8_t) (0);                   // Addr[22:16] for D-Flash is always 0
  // Fill sector address
  FTMRA->FCCOBIX = 1;
  FTMRA_FCCOB = (uint16_t) (adr & 0xFFFF);
#else
  // Write index to specify the command code to be loaded
  FTMRA->FCCOBIX = 0;
  // Write command code and memory address bits[23:16]	
  FTMRA->FCCOBHI = 0x0A;                            // P-Flash Sector Erase command
  FTMRA->FCCOBLO = (uint8_t)((adr >> 16)&0x007f);   // memory address bits[17:16]
  // Write index to specify the lower byte memory address bits[15:0] to be loaded
  FTMRA->FCCOBIX = 0x1;
  // Write the lower byte memory address bits[15:0]
  FTMRA_FCCOB = (uint16_t)adr;

#endif
  // Launch the command
  FTMRA->FSTAT = 0x80;
  
  // Wait till command is completed
  while (!(FTMRA->FSTAT & FTMRA_FSTAT_CCIF_MASK))
    ;
  if (FTMRA->FSTAT & FTMRA_FSTAT_FPVIOL_MASK)  return(FTFx_SSD_FSTAT_FPVIOL);
  if (FTMRA->FSTAT & FTMRA_ERROR)              return(1);
  return (0);
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */
#ifdef DFLASH

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  int  i;
  /* Add your Code */
  for (i = 0; i < ((sz+3)/4); i++)  {
    // Clear error flags
    FTMRA->FSTAT = 0x30;
    
    // Write index to specify the command code to be loaded
    FTMRA->FCCOBIX = 0;
    // Write command code and memory address bits[17:16]	
    FTMRA->FCCOBHI = 0x11;// program D-FLASH command
    FTMRA->FCCOBLO = (uint8_t) 0;// Addr[22:16] always 0
    // Write index to specify the lower byte memory address bits[15:0] to be loaded
    FTMRA->FCCOBIX = 0x1;
    // Write the lower byte memory address bits[15:0]
    FTMRA_FCCOB = (uint16_t) (adr & 0xFFFF);
    // Write index to specify the word0 (MSB word) to be programmed
    FTMRA->FCCOBIX = 0x2;
    // Write the word
    FTMRA_FCCOB = (*(unsigned long *)buf) & 0xFFFF;
    // Write index to specify the word1 (LSB word) to be programmed
    FTMRA->FCCOBIX = 0x3;
    // Write the word
    FTMRA_FCCOB = ((*(unsigned long *)buf) >> 16) & 0xFFFF;
    
    // Launch the command
    FTMRA->FSTAT = 0x80;
    // Wait till command is completed
    while (!(FTMRA->FSTAT & FTMRA_FSTAT_CCIF_MASK))
      ;
    // Check error status
    if (FTMRA->FSTAT & FTMRA_ERROR) return(1);

    buf += 4;
    adr += 4;
  }
  return (0);                                  // Finished without Errors
}
#else
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  int  i;

  for (i = 0; i < ((sz+7)/8); i++)  {
    // Clear error flags
    FTMRA->FSTAT = 0x30;
    
    // Write index to specify the command code to be loaded
    FTMRA->FCCOBIX = 0;
    // Write command code and memory address bits[17:16]	
    FTMRA->FCCOBHI = 0x06;// program P-FLASH command
    FTMRA->FCCOBLO = (uint8_t) ((adr >> 16));// Addr[22:16] always 0
    // Write index to specify the lower byte memory address bits[15:0] to be loaded
    FTMRA->FCCOBIX = 0x1;
    // Write the lower byte memory address bits[15:0]
    FTMRA_FCCOB = (uint16_t) (adr & 0xFFFF);
    // Write index to specify the word0 (MSB word) to be programmed
    FTMRA->FCCOBIX = 0x2;
    // Write the word0
    FTMRA_FCCOB = *((unsigned short *)buf);
    // Write index to specify the word1 (LSB word) to be programmed
    FTMRA->FCCOBIX = 0x3;
    // Write the word1
    FTMRA_FCCOB = *((unsigned short *)buf + 1);
    FTMRA->FCCOBIX = 0x4;
    // Write the word2
    FTMRA_FCCOB = *((unsigned short *)buf + 2);
    FTMRA->FCCOBIX = 0x5;
    // Write the word3
    FTMRA_FCCOB = *((unsigned short *)buf + 3);
    
    // Launch the command
    FTMRA->FSTAT = 0x80;
    // Wait till command is completed
    while (!(FTMRA->FSTAT & FTMRA_FSTAT_CCIF_MASK))
     ;
    // Check error status
    if (FTMRA->FSTAT & FTMRA_ERROR) return(1);
    buf += 8;
    adr += 8;

  }
  return (0);                                  // Finished without Errors
}
#endif
