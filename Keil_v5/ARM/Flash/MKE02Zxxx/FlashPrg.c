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
 * Project:      Flash Programming Functions for Freescale MKE02 Device Flash
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
  volatile uint8_t CS1;                                /**< Watchdog Control and Status Register 1, offset: 0x0 */
  volatile uint8_t CS2;                                /**< Watchdog Control and Status Register 2, offset: 0x1 */
  volatile uint8_t CNTH;                               /**< Watchdog Counter Register: High, offset: 0x2 */
  volatile uint8_t CNTL;                               /**< Watchdog Counter Register: Low, offset: 0x3 */
  volatile uint8_t TOVALH;                             /**< Watchdog Timeout Value Register: High, offset: 0x4 */
  volatile uint8_t TOVALL;                             /**< Watchdog Timeout Value Register: Low, offset: 0x5 */
  volatile uint8_t WINH;                               /**< Watchdog Window Register: High, offset: 0x6 */
  volatile uint8_t WINL;                               /**< Watchdog Window Register: Low, offset: 0x7 */
} * WDOG_MemMapPtr;
/** FTMRH - Peripheral register structure */
typedef struct FTMRA_MemMap {
  volatile uint8_t FCLKDIV;                            /**< Flash Clock Divider Register, offset: 0x0 */
  volatile uint8_t FSEC;                               /**< Flash Security Register, offset: 0x1 */
  volatile uint8_t FCCOBIX;                            /**< Flash CCOB Index Register, offset: 0x2 */
  volatile uint8_t RESERVED_0[1];
  volatile uint8_t FCNFG;                              /**< Flash Configuration Register, offset: 0x4 */
  volatile uint8_t FERCNFG;                            /**< Flash Error Configuration Register, offset: 0x5 */
  volatile uint8_t FSTAT;                              /**< Flash Status Register, offset: 0x6 */
  volatile uint8_t FERSTAT;                            /**< Flash Error Status Register, offset: 0x7 */
  volatile uint8_t FPROT;                              /**< Flash Protection Register, offset: 0x8 */
  volatile uint8_t EEPROT;                             /**< EEPROM Protection Register, offset: 0x9 */
  volatile uint8_t FCCOBHI;                            /**< Flash Common Command Object Register:High, offset: 0xA */
  volatile uint8_t FCCOBLO;                            /**< Flash Common Command Object Register: Low, offset: 0xB */
  volatile uint8_t FOPT;                               /**< Flash Option Register, offset: 0xC */
} *FTMRH_MemMapPtr;
/** Peripheral Map **/
#define FTMRH                           ((FTMRH_MemMapPtr)0x40020000u)
#define WDOG                            ((WDOG_MemMapPtr)0x40052000u)

#define M8(adr)  (*((volatile unsigned char *)(adr)))
#define M16(adr) (*((volatile unsigned short *)(adr)))

#define WDOG_CS1_EN_MASK                         0x80u
//#define WDOG_WIN                                 *((uint16_t *)(0x06 + 0x40052000u))

#define FTMRH_FCLKDIV_FDIVLD_MASK                0x80u
#define FTMRH_FSTAT_CCIF_MASK                    0x80u
#define FTMRH_FSTAT_ACCERR_MASK                  0x20u
#define FTMRH_FSTAT_FPVIOL_MASK                  0x10u
#define FTMRH_FSTAT_MGSTAT_MASK                  0x3u
#define FTMRH_ERROR                              (FTMRH_FSTAT_ACCERR_MASK | FTMRH_FSTAT_FPVIOL_MASK | FTMRH_FSTAT_MGSTAT_MASK)
#define FTMRH_FCCOB                              *((volatile uint16_t *)(0x0a + 0x40020000))

#define MCM_PLACR                                *((volatile uint32_t *)(0xC + 0xF0003000))

#define WDOG_TOVAL                               *((volatile uint16_t *)(&WDOG->TOVALH))
#define WDOG_CNT                                 *((volatile uint16_t *)(&WDOG->CNTH))
  
#define FTFx_SSD_FSTAT_FPVIOL                   (8)
/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */



int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  WDOG_CNT   = 0x20C5; 
  WDOG_CNT   = 0x28D9;
  WDOG_TOVAL = 0xFFFF;

  WDOG->WINH = 0xFF;
  WDOG->WINL = 0xFF;

  WDOG->CS2 = 0x01;
  WDOG->CS1 = 0x20;          /* WDOGA = 1 to allow reconfigure watchdog at any time by executing an unlock sequence */

  FTMRH->FCLKDIV = 0x10;
  MCM_PLACR      = 0x1BC00;  /* Disable flash cache */
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
#ifdef EEPROM
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf) {
  int i;
  unsigned long b_adr = adr;
  for (i = 0; i < sz; i++)  {
    if (M8(b_adr) != (*(volatile unsigned char *)buf)) return(0);
    buf   += 1;
    b_adr += 1;
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
  FTMRH->FSTAT = 0x30;
  
  // Write index to specify the command code to be loaded
  FTMRH->FCCOBIX = 0;
  FTMRH->FCCOBHI = 0x09;// erase FLASH block command
  // Write command code and memory address bits[17:16]	
#ifdef EEPROM
  FTMRH->FCCOBLO = (uint8_t) (0x80); // Bit7 = 1 for EEPROM	
#else
  FTMRH->FCCOBLO = (uint8_t) (0x00); // Bit7 = 0 for PFlash	
#endif  

  FTMRH->FCCOBIX = 0x1;
  // Write the lower byte memory address bits[15:0]
  //FTMRH_FCCOB = (uint16_t)adr;
  FTMRH->FCCOBHI = (uint8_t)(0);
  FTMRH->FCCOBLO = (uint8_t)(0);

  // Launch the command
  FTMRH->FSTAT = 0x80;
  
  // Wait till command is completed
  while (!(FTMRH->FSTAT & FTMRH_FSTAT_CCIF_MASK))
    ;
  if (FTMRH->FSTAT & FTMRH_FSTAT_FPVIOL_MASK)  return(FTFx_SSD_FSTAT_FPVIOL);
  if (FTMRH->FSTAT & FTMRH_ERROR)              return(1);

#ifndef EEPROM
  /* Write 4 bytes*/
  // Write index to specify the command code to be loaded
  FTMRH->FCCOBIX = 0x0;
  // Write command code and memory address bits[17:16]	
  FTMRH->FCCOBHI = 0x06;// program FLASH command
  FTMRH->FCCOBLO = 0;
  /*Set destination address*/
  FTMRH->FCCOBIX = 0x1;    
  FTMRH->FCCOBHI = 0x04; // flash configuration field address
  FTMRH->FCCOBLO = 0x0C;    
  /*data*/
  FTMRH->FCCOBIX = 0x2;       
  FTMRH->FCCOBHI = 0xFF;
  FTMRH->FCCOBLO = 0xFF;

  FTMRH->FCCOBIX = 0x3;       
  FTMRH->FCCOBHI = 0xFF;  
  FTMRH->FCCOBLO = 0xFE;      
  // Launch the command
  FTMRH->FSTAT = 0x80;

  // Wait till command is completed
  while (!(FTMRH->FSTAT & FTMRH_FSTAT_CCIF_MASK))
    ;
  if (FTMRH->FSTAT & FTMRH_ERROR) return(1);
#endif

  return (0);
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {
  // Clear error flags
  FTMRH->FSTAT = 0x30;
  
  // Write index to specify the command code to be loaded
  FTMRH->FCCOBIX = 0;
  // Write command code and memory address bits[23:16]	
#ifdef EEPROM
  FTMRH->FCCOBHI = 0x12;                            // EEPROM Sector Erase command 
#else
  FTMRH->FCCOBHI = 0x0A;                            // Flash Sector Erase command
#endif
  FTMRH->FCCOBLO = (uint8_t)((adr >> 16)&0x007f);   // memory address bits[17:16]
  // Write index to specify the lower byte memory address bits[15:0] to be loaded
  FTMRH->FCCOBIX = 0x1;
  // Write the lower byte memory address bits[15:0]
  //FTMRH_FCCOB = (uint16_t)adr;
  FTMRH->FCCOBHI = (uint8_t)(adr >>  8);
  FTMRH->FCCOBLO = (uint8_t)(adr);

  // Launch the command
  FTMRH->FSTAT = 0x80;
  
  // Wait till command is completed
  while (!(FTMRH->FSTAT & FTMRH_FSTAT_CCIF_MASK))
    ;
  if (FTMRH->FSTAT & FTMRH_FSTAT_FPVIOL_MASK)  return(FTFx_SSD_FSTAT_FPVIOL);
  if (FTMRH->FSTAT & FTMRH_ERROR)              return(1);

  return (0);
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */
#ifdef EEPROM
//
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  int  i;

  for (i = 0; i < ((sz+1)/2); i++)  {
    // Clear error flags
    FTMRH->FSTAT = 0x30;
    
    // Write index to specify the command code to be loaded
    FTMRH->FCCOBIX = 0;
    // Write command code and memory address bits[17:16]	
    FTMRH->FCCOBHI = 0x11;// program EEPROM command
    FTMRH->FCCOBLO = (uint8_t) ((adr >> 16));// Addr[22:16] always 0
    // Write index to specify the lower byte memory address bits[15:0] to be loaded
    FTMRH->FCCOBIX = 0x1;
    // Write the lower byte memory address bits[15:0]
    FTMRH->FCCOBHI = (uint8_t)(adr >>  8);
    FTMRH->FCCOBLO = (uint8_t)(adr);

    // Write index to specify the word0 (MSB word) to be programmed
    FTMRH->FCCOBIX = 0x2;
    FTMRH->FCCOBLO =  buf[0];
    // Write index to specify the word1 (LSB word) to be programmed
    FTMRH->FCCOBIX = 0x3;
    FTMRH->FCCOBLO =  buf[1];
    
    // Launch the command
    FTMRH->FSTAT = 0x80;
    // Wait till command is completed
    while (!(FTMRH->FSTAT & FTMRH_FSTAT_CCIF_MASK))
     ;
    // Check error status
    if (FTMRH->FSTAT & FTMRH_ERROR) return(1);
    buf += 2;
    adr += 2;

  }
  return (0);                                  // Finished without Errors
}
#else
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  int  i;

  for (i = 0; i < ((sz+7)/8); i++)  {
    // Clear error flags
    FTMRH->FSTAT = 0x30;
    
    // Write index to specify the command code to be loaded
    FTMRH->FCCOBIX = 0;
    // Write command code and memory address bits[17:16]	
    FTMRH->FCCOBHI = 0x06;// program P-FLASH command
    FTMRH->FCCOBLO = (uint8_t) ((adr >> 16));// Addr[22:16] always 0
    // Write index to specify the lower byte memory address bits[15:0] to be loaded
    FTMRH->FCCOBIX = 0x1;
    // Write the lower byte memory address bits[15:0]
    FTMRH->FCCOBHI = (uint8_t)(adr >>  8);
    FTMRH->FCCOBLO = (uint8_t)(adr);

    // Write index to specify the word0 (MSB word) to be programmed
    FTMRH->FCCOBIX = 0x2;
    // Write the word0
    FTMRH->FCCOBHI =  buf[1];
    FTMRH->FCCOBLO =  buf[0];
    // Write index to specify the word1 (LSB word) to be programmed
    FTMRH->FCCOBIX = 0x3;
    // Write the word1
    FTMRH->FCCOBHI =  buf[3];
    FTMRH->FCCOBLO =  buf[2];
    
    // Write the word2
		FTMRH->FCCOBIX = 0x4;
    FTMRH->FCCOBHI =  buf[5];
    FTMRH->FCCOBLO =  buf[4];

    // Write the word3
    FTMRH->FCCOBIX = 0x5;
    FTMRH->FCCOBHI =  buf[7];
    FTMRH->FCCOBLO =  buf[6];
    
    // Launch the command
    FTMRH->FSTAT = 0x80;
    // Wait till command is completed
    while (!(FTMRH->FSTAT & FTMRH_FSTAT_CCIF_MASK))
     ;
    // Check error status
    if (FTMRH->FSTAT & FTMRH_ERROR) return(1);
    buf += 8;
    adr += 8;

  }
  return (0);                                  // Finished without Errors
}
#endif
