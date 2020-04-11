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
 * Project:      Flash Programming Functions for Freescale MKE04 Device Flash
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
/** FTMRE - Register Layout Typedef */
typedef struct {
  volatile uint8_t RESERVED_0[1];
  volatile uint8_t FCCOBIX;                            /**< Flash CCOB Index Register, offset: 0x1 */
  volatile uint8_t FSEC;                               /**< Flash Security Register, offset: 0x2 */
  volatile uint8_t FCLKDIV;                            /**< Flash Clock Divider Register, offset: 0x3 */
  volatile uint8_t RESERVED_1[1];
  volatile uint8_t FSTAT;                              /**< Flash Status Register, offset: 0x5 */
  volatile uint8_t RESERVED_2[1];
  volatile uint8_t FCNFG;                              /**< Flash Configuration Register, offset: 0x7 */
  volatile uint8_t FCCOBLO;                            /**< Flash Common Command Object Register: Low, offset: 0x8 */
  volatile uint8_t FCCOBHI;                            /**< Flash Common Command Object Register:High, offset: 0x9 */
  volatile uint8_t RESERVED_3[1];
  volatile uint8_t FPROT;                              /**< Flash Protection Register, offset: 0xB */
  volatile uint8_t RESERVED_4[3];
  volatile uint8_t FOPT;                               /**< Flash Option Register, offset: 0xF */
} * FTMRE_MemMapPtr;
/** Peripheral Map **/
#define FTMRE                           ((FTMRE_MemMapPtr)0x40020000u)
#define WDOG                            ((WDOG_MemMapPtr)0x40052000u)

#define M8(adr)  (*((volatile unsigned char *)(adr)))
#define M16(adr) (*((volatile unsigned short *)(adr)))

#define WDOG_CS1_EN_MASK                         0x80u
//#define WDOG_WIN                                 *((uint16_t *)(0x06 + 0x40052000u))

#define FTMRE_FCLKDIV_FDIVLD_MASK                0x80u
#define FTMRE_FSTAT_CCIF_MASK                    0x80u
#define FTMRE_FSTAT_ACCERR_MASK                  0x20u
#define FTMRE_FSTAT_FPVIOL_MASK                  0x10u
#define FTMRE_FSTAT_MGSTAT_MASK                  0x3u
#define FTMRE_ERROR                              (FTMRE_FSTAT_ACCERR_MASK | FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_MGSTAT_MASK)
//#define FTMRE_FCCOB                              *((volatile uint16_t *)(0x08 + 0x40020000))

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

  FTMRE->FCLKDIV = 0x10;
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
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {
  // Clear error flags
  FTMRE->FSTAT = 0x30;
  
  // Write index to specify the command code to be loaded
  FTMRE->FCCOBIX = 0;
  FTMRE->FCCOBHI = 0x09;// erase FLASH block command
  // Write command code and memory address bits[17:16]	
  FTMRE->FCCOBLO = (uint8_t) (0x00); 

  FTMRE->FCCOBIX = 0x1;
  // Write the lower byte memory address bits[15:0]
  //FTMRH_FCCOB = (uint16_t)adr;
  FTMRE->FCCOBHI = (uint8_t)(0);
  FTMRE->FCCOBLO = (uint8_t)(0);

  // Launch the command
  FTMRE->FSTAT = 0x80;
  
  // Wait till command is completed
  while (!(FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK))
    ;
  if (FTMRE->FSTAT & FTMRE_FSTAT_FPVIOL_MASK)  return(FTFx_SSD_FSTAT_FPVIOL);
  if (FTMRE->FSTAT & FTMRE_ERROR)              return(1);

  /* Write 4 bytes*/
  // Write index to specify the command code to be loaded
  FTMRE->FCCOBIX = 0x0;
  // Write command code and memory address bits[17:16]	
  FTMRE->FCCOBHI = 0x06;// program FLASH command
  FTMRE->FCCOBLO = 0;
  /*Set destination address*/
  FTMRE->FCCOBIX = 0x1;    
  FTMRE->FCCOBHI = 0x04; // flash configuration field address
  FTMRE->FCCOBLO = 0x0C;    
  /*data*/
  FTMRE->FCCOBIX = 0x2;       
  FTMRE->FCCOBHI = 0xFF;
  FTMRE->FCCOBLO = 0xFF;

  FTMRE->FCCOBIX = 0x3;       
  FTMRE->FCCOBHI = 0xFF;  
  FTMRE->FCCOBLO = 0xFE;      
  // Launch the command
  FTMRE->FSTAT = 0x80;

  // Wait till command is completed
  while (!(FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK))
    ;
  if (FTMRE->FSTAT & FTMRE_ERROR) return(1);

  return (0);
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {
  // Clear error flags
  FTMRE->FSTAT = 0x30;
  
  // Write index to specify the command code to be loaded
  FTMRE->FCCOBIX = 0;
  // Write command code and memory address bits[23:16]	
  FTMRE->FCCOBHI = 0x0A;                            // Flash Sector Erase command

  FTMRE->FCCOBLO = (uint8_t)((adr >> 16)&0x007f);   // memory address bits[17:16]
  // Write index to specify the lower byte memory address bits[15:0] to be loaded
  FTMRE->FCCOBIX = 0x1;
  // Write the lower byte memory address bits[15:0]
  //FTMRH_FCCOB = (uint16_t)adr;
  FTMRE->FCCOBHI = (uint8_t)(adr >>  8);
  FTMRE->FCCOBLO = (uint8_t)(adr);

  // Launch the command
  FTMRE->FSTAT = 0x80;
  
  // Wait till command is completed
  while (!(FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK))
    ;
  if (FTMRE->FSTAT & FTMRE_FSTAT_FPVIOL_MASK)  return(FTFx_SSD_FSTAT_FPVIOL);
  if (FTMRE->FSTAT & FTMRE_ERROR)              return(1);
  return (0);
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  int  i;

  for (i = 0; i < ((sz+7)/8); i++)  {
    // Clear error flags
    FTMRE->FSTAT = 0x30;
    
    // Write index to specify the command code to be loaded
    FTMRE->FCCOBIX = 0;
    // Write command code and memory address bits[17:16]	
    FTMRE->FCCOBHI = 0x06;// program P-FLASH command
    FTMRE->FCCOBLO = (uint8_t) ((adr >> 16));// Addr[22:16] always 0
    // Write index to specify the lower byte memory address bits[15:0] to be loaded
    FTMRE->FCCOBIX = 0x1;
    // Write the lower byte memory address bits[15:0]
    FTMRE->FCCOBHI = (uint8_t)(adr >>  8);
    FTMRE->FCCOBLO = (uint8_t)(adr);

    // Write index to specify the word0 (MSB word) to be programmed
    FTMRE->FCCOBIX = 0x2;
    // Write the word0
    FTMRE->FCCOBHI =  buf[1];
    FTMRE->FCCOBLO =  buf[0];
    // Write index to specify the word1 (LSB word) to be programmed
    FTMRE->FCCOBIX = 0x3;
    // Write the word1
    FTMRE->FCCOBHI =  buf[3];
    FTMRE->FCCOBLO =  buf[2];
    
    // Write the word2
		FTMRE->FCCOBIX = 0x4;
    FTMRE->FCCOBHI =  buf[5];
    FTMRE->FCCOBLO =  buf[4];

    // Write the word3
    FTMRE->FCCOBIX = 0x5;
    FTMRE->FCCOBHI =  buf[7];
    FTMRE->FCCOBLO =  buf[6];
    
    // Launch the command
    FTMRE->FSTAT = 0x80;
    // Wait till command is completed
    while (!(FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK))
     ;
    // Check error status
    if (FTMRE->FSTAT & FTMRE_ERROR) return(1);
    buf += 8;
    adr += 8;

  }
  return (0);                                  // Finished without Errors
}
