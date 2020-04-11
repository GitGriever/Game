/***********************************************************************/
/*  This file is part of the uVision/ARM development tools             */
/*  Copyright (c) 2003-2013 Keil Software. All rights reserved.        */
/***********************************************************************/
/*                                                                     */
/* FlashDev.c    Freescale MKVXX  Flash                        */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        // FlashOS Structures


#ifdef MKV_P32_1KB_SEC
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,                  // Driver Version, do not modify!
  "MKVXX 32KB P-Flash(1KB Sector)", // Device Name 
   ONCHIP,                          // Device Type
   0x00000000,                      // Device Start Address
   0x00008000,                      // Device Size (32kB)
   256,                             // Programming Page Size
   0,                               // Reserved, must be 0
   0xFF,                            // Initial Content of Erased Memory
   1000,                            // Program Page Timeout 1000 mSec
   3000,                            // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000400, 0x000000,              // Sector Size  1kB (32 Sectors)
   SECTOR_END
};
#endif


#ifdef MKV_P16_1KB_SEC
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,                  // Driver Version, do not modify!
  "MKVXX 16KB P-Flash(1KB Sector)", // Device Name 
   ONCHIP,                          // Device Type
   0x00000000,                      // Device Start Address
   0x00004000,                      // Device Size (16kB)
   256,                             // Programming Page Size
   0,                               // Reserved, must be 0
   0xFF,                            // Initial Content of Erased Memory
   1000,                            // Program Page Timeout 1000 mSec
   3000,                            // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000400, 0x000000,              // Sector Size  1kB (16 Sectors)
   SECTOR_END
};
#endif

#ifdef MKV_PIFR_75MHZ
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,                  // Driver Version, do not modify!
   "MKVXX 75Mhz PIFR",              // Device Name 
   ONCHIP,                          // Device Type
   0x0FFFFFC0,                      // Device Start Address
   0x00000040,                      // Device Size (64 Byte)
   64,                              // Programming Page Size
   0,                               // Reserved, must be 0
   0xFF,                            // Initial Content of Erased Memory
   1000,                            // Program Page Timeout 1000 mSec
   3000,                            // Erase Sector Timeout 3000 mSec
// Specify Size and Address of Sectors
   0x000040, 0x000000,              // Sector Size  64 Byte (1 Sector)
   SECTOR_END
};
#endif
