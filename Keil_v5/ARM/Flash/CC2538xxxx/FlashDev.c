/***********************************************************************/
/*  This file is part of the uVision/ARM development tools             */
/*  Copyright (c) 2006-2013 Keil - An ARM Company.                     */
/*  All rights reserved.                                               */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.C:  Flash Programming Functions adapted for               */
/*               TI CC2538xx Flash                                     */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        // FlashOS Structures

#ifdef CC2538_512KB
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "CC2538xx 512 KB",          // Device Name (512kB)
   ONCHIP,                     // Device Type
   0x00200000,                 // Device Start Address
   0x00080000,                 // Device Size in Bytes (256kB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   3000,                       // Program Page Timeout 3000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x0800, 0x000000,           // Sector Size 2kB (256 Sectors)
   SECTOR_END
};
#endif

#ifdef CC2538_256KB
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "CC2538xx 256 KB",          // Device Name (256kB)
   ONCHIP,                     // Device Type
   0x00200000,                 // Device Start Address
   0x00040000,                 // Device Size in Bytes (256kB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   3000,                       // Program Page Timeout 3000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x0800, 0x000000,           // Sector Size 2kB (128 Sectors)
   SECTOR_END
};
#endif

#ifdef CC2538_128KB
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "CC2538xx 128 KB",          // Device Name (128kB)
   ONCHIP,                     // Device Type
   0x00200000,                 // Device Start Address
   0x00020000,                 // Device Size in Bytes (128kB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   3000,                       // Program Page Timeout 3000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x0800, 0x000000,           // Sector Size 2kB (64 Sectors)
   SECTOR_END
};
#endif
