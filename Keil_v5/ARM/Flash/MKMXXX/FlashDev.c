/***********************************************************************/
/*  This file is part of the uVision/ARM development tools             */
/*  Copyright (c) 2003-2013 Keil Software. All rights reserved.        */
/***********************************************************************/
/*                                                                     */
/* FlashDev.c    Freescale MKMxxx Series Internal Flash                */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        // FlashOS Structures


#ifdef MKMP64_1KB
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,               // Driver Version, do not modify!
   "MKMxx 64KB P-Flash(1KB Sec)",// Device Name 
   ONCHIP,                       // Device Type
   0x00000000,                   // Device Start Address
   0x00010000,                   // Device Size (64kB)
   256,                          // Programming Page Size
   0,                            // Reserved, must be 0
   0xFF,                         // Initial Content of Erased Memory
   1000,                         // Program Page Timeout 1000 mSec
   3000,                         // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000400, 0x000000,           // Sector Size  1kB (64 Sectors)
   SECTOR_END
};
#endif

#ifdef MKMP128_1KB
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,               // Driver Version, do not modify!
   "MKMxx 128KB P-Flash(1KB Sec)",// Device Name 
   ONCHIP,                       // Device Type
   0x00000000,                   // Device Start Address
   0x00020000,                   // Device Size (128kB)
   256,                          // Programming Page Size
   0,                            // Reserved, must be 0
   0xFF,                         // Initial Content of Erased Memory
   1000,                         // Program Page Timeout 1000 mSec
   3000,                         // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x000400, 0x000000,           // Sector Size  1kB (128 Sectors)
   SECTOR_END
};
#endif

#ifdef MKMPIFR                 //Program Flash IFR
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MKMxx P-Flash IFR",        // Device Name 
   ONCHIP,                     // Device Type
   0x0FFFFFC0,                 // Device Start Address
   0x00000040,                 // Device Size (64 Byte)
   64,                         // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec
// Specify Size and Address of Sectors
   0x000040, 0x000000,         // Sector Size  64 Byte (1 Sector)
   SECTOR_END
};
#endif

///#ifdef MKDIFR_50MHZ  //Data Flash IFR
///struct FlashDevice const FlashDevice  =  {
///   FLASH_DRV_VERS,             // Driver Version, do not modify!
///   "MKXX 50Mhz Data Flash IFR",// Device Name 
///   ONCHIP,                     // Device Type
///   0x13FFFFFC,                 // Device Start Address
///   0x00000004,                 // Device Size (4 Byte)
///   4,                          // Programming Page Size
///   0,                          // Reserved, must be 0
///   0xFF,                       // Initial Content of Erased Memory
///   1000,                       // Program Page Timeout 1000 mSec
///   3000,                       // Erase Sector Timeout 3000 mSec
///// Specify Size and Address of Sectors
///   0x000004, 0x000000,         // Sector Size  4 Byte (1 Sector)
///   SECTOR_END
///};
///#endif
