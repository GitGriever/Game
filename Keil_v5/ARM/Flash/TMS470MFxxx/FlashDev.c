/******************************************************************************/
/*  This file is part of the ARM Toolchain package                            */
/*  Copyright KEIL ELEKTRONIK GmbH 2003 - 2012                                */
/******************************************************************************/
/*                                                                            */
/*  FlashDev.C:  Device Description for TMS470MS Device Series Flash          */
/*                                                                            */
/******************************************************************************/

#include "..\FlashOS.H"                 // FlashOS Structures



#ifdef  TMS470MF_BANK0_512KB
struct FlashDevice const FlashDevice  =  {
  FLASH_DRV_VERS,                       // Driver Version, do not modify!
  "TMS470MF Bank0 512KB Flash",         // Device Name
  ONCHIP,                               // Device Type
  0x00000,                              // Device Start Address
  0x80000,                              // Device Size in Bytes (512 KB)
  4096,                                 // Programming Page Size
  0,                                    // Reserved, must be 0
  0xFF,                                 // Initial Content of Erased Memory
  6000,                                 // Program Page Timeout 6000 mSec
  8000,                                 // Erase Sector Timeout 8000 mSec

  // Specify Size and Address of Sectors
  0x4000,  0x000000,                    // Sector Size  16kB ( 2 Sectors)
  0x8000,  0x008000,                    // Sector Size  32kB ( 1 Sectors)
  0x10000, 0x010000,                    // Sector Size  64kB ( 7 Sectors)
  SECTOR_END
};
#endif

#ifdef  TMS470MF_BANK0_256KB_ECC
struct FlashDevice const FlashDevice  =  {
  FLASH_DRV_VERS,                       // Driver Version, do not modify!
  "TMS470MF Bank0 256KB ECC",           // Device Name
  ONCHIP,                               // Device Type
  0x400000,                             // Device Start Address
  0x40000,                              // Device Size in Bytes (640 KB)
  4096,                                 // Programming Page Size
  0,                                    // Reserved, must be 0
  0xFF,                                 // Initial Content of Erased Memory
  6000,                                 // Program Page Timeout 6000 mSec
  8000,                                 // Erase Sector Timeout 8000 mSec

  // Specify Size and Address of Sectors: Dummy. Not used
  0x2000,  0x000000,                    // Sector Size  8kB  ( 2 Sectors)
  0x4000,  0x004000,                    // Sector Size  16kB ( 1 Sectors)
  0x8000,  0x008000,                    // Sector Size  32kB ( 7 Sectors)
  SECTOR_END
};
#endif

#ifdef  TMS470MF_BANK0_384KB
struct FlashDevice const FlashDevice  =  {
  FLASH_DRV_VERS,                       // Driver Version, do not modify!
  "TMS470MF Bank0 384KB Flash",         // Device Name
  ONCHIP,                               // Device Type
  0x00000,                              // Device Start Address
  0x60000,                              // Device Size in Bytes (384 KB)
  4096,                                 // Programming Page Size
  0,                                    // Reserved, must be 0
  0xFF,                                 // Initial Content of Erased Memory
  6000,                                 // Program Page Timeout 6000 mSec
  8000,                                 // Erase Sector Timeout 8000 mSec

  // Specify Size and Address of Sectors
  0x4000,  0x000000,                    // Sector Size  16kB ( 2 Sectors)
  0x8000,  0x008000,                    // Sector Size  32kB ( 1 Sectors)
  0x10000, 0x010000,                    // Sector Size  64kB ( 5 Sectors)
  SECTOR_END
};
#endif

#ifdef  TMS470MF_BANK0_192KB_ECC
struct FlashDevice const FlashDevice  =  {
  FLASH_DRV_VERS,                       // Driver Version, do not modify!
  "TMS470MF Bank0 192KB ECC",           // Device Name
  ONCHIP,                               // Device Type
  0x400000,                             // Device Start Address
  0x30000,                              // Device Size in Bytes (640 KB)
  4096,                                 // Programming Page Size
  0,                                    // Reserved, must be 0
  0xFF,                                 // Initial Content of Erased Memory
  6000,                                 // Program Page Timeout 6000 mSec
  8000,                                 // Erase Sector Timeout 8000 mSec

  // Specify Size and Address of Sectors: Dummy. Not used
  0x2000,  0x000000,                    // Sector Size   8kB ( 2 Sectors)
  0x4000,  0x004000,                    // Sector Size  16kB ( 1 Sectors)
  0x8000,  0x008000,                    // Sector Size  32kB ( 5 Sectors)
  SECTOR_END
};
#endif

#ifdef  TMS470MF_BANK0_256KB
struct FlashDevice const FlashDevice  =  {
  FLASH_DRV_VERS,                       // Driver Version, do not modify!
  "TMS470MF Bank0 256KB Flash",         // Device Name
  ONCHIP,                               // Device Type
  0x00000,                              // Device Start Address
  0x40000,                              // Device Size in Bytes (192 KB)
  4096,                                 // Programming Page Size
  0,                                    // Reserved, must be 0
  0xFF,                                 // Initial Content of Erased Memory
  6000,                                 // Program Page Timeout 6000 mSec
  8000,                                 // Erase Sector Timeout 8000 mSec

  // Specify Size and Address of Sectors
  0x4000,  0x000000,                    // Sector Size  16kB ( 2 Sectors)
  0x8000,  0x008000,                    // Sector Size  32kB ( 1 Sectors)
  0x10000, 0x010000,                    // Sector Size  64kB ( 3 Sectors)
  SECTOR_END
};
#endif

#ifdef  TMS470MF_BANK0_128KB_ECC
struct FlashDevice const FlashDevice  =  {
  FLASH_DRV_VERS,                       // Driver Version, do not modify!
  "TMS470MF Bank0 128KB ECC",           // Device Name
  ONCHIP,                               // Device Type
  0x400000,                             // Device Start Address
  0x20000,                              // Device Size in Bytes (128 KB)
  4096,                                 // Programming Page Size
  0,                                    // Reserved, must be 0
  0xFF,                                 // Initial Content of Erased Memory
  6000,                                 // Program Page Timeout 6000 mSec
  8000,                                 // Erase Sector Timeout 8000 mSec

  // Specify Size and Address of Sectors: Dummy. Not used !!
  0x2000,  0x000000,                    // Sector Size   8kB ( 2 Sectors)
  0x4000,  0x004000,                    // Sector Size  16kB ( 1 Sectors)
  0x8000,  0x008000,                    // Sector Size  32kB ( 3 Sectors)
  SECTOR_END
};
#endif

#ifdef  TMS470MF_BANK1_128KB
struct FlashDevice const FlashDevice  =  {
  FLASH_DRV_VERS,                       // Driver Version, do not modify!
  "TMS470MF Bank1 128KB Flash",         // Device Name
  ONCHIP,                               // Device Type
  0x80000,                              // Device Start Address
  0x20000,                              // Device Size in Bytes (128 KB)
  4096,                                 // Programming Page Size
  0,                                    // Reserved, must be 0
  0xFF,                                 // Initial Content of Erased Memory
  6000,                                 // Program Page Timeout 6000 mSec
  8000,                                 // Erase Sector Timeout 8000 mSec

  // Specify Size and Address of Sectors
  0x4000,  0x000000,                    // Sector Size  16kB ( 4 Sectors)
  0x10000, 0x010000,                    // Sector Size  64kB ( 1 Sectors)
  SECTOR_END
};
#endif

#ifdef  TMS470MF_BANK1_64KB_ECC
struct FlashDevice const FlashDevice  =  {
  FLASH_DRV_VERS,                       // Driver Version, do not modify!
  "TMS470MF Bank1 64KB ECC",            // Device Name
  ONCHIP,                               // Device Type
  0x440000,                             // Device Start Address
  0x10000,                              // Device Size in Bytes (64 KB)
  4096,                                 // Programming Page Size
  0,                                    // Reserved, must be 0
  0xFF,                                 // Initial Content of Erased Memory
  6000,                                 // Program Page Timeout 6000 mSec
  8000,                                 // Erase Sector Timeout 8000 mSec

  // Specify Size and Address of Sectors: Dummy. Not used !!
  0x2000,  0x000000,                    // Sector Size   8kB ( 4 Sectors)
  0x8000,  0x008000,                    // Sector Size  32kB ( 1 Sectors)
  SECTOR_END
};
#endif

#ifdef  TMS470MF_BANK1_64KB
struct FlashDevice const FlashDevice  =  {
  FLASH_DRV_VERS,                       // Driver Version, do not modify!
  "TMS470MF Bank1 64KB Flash",          // Device Name
  ONCHIP,                               // Device Type
  0x80000,                              // Device Start Address
  0x10000,                              // Device Size in Bytes (64 KB)
  4096,                                 // Programming Page Size
  0,                                    // Reserved, must be 0
  0xFF,                                 // Initial Content of Erased Memory
  6000,                                 // Program Page Timeout 6000 mSec
  8000,                                 // Erase Sector Timeout 8000 mSec

  // Specify Size and Address of Sectors
  0x4000,  0x000000,                    // Sector Size  16kB ( 4 Sectors)
  SECTOR_END
};
#endif

#ifdef  TMS470MF_BANK1_32KB_ECC
struct FlashDevice const FlashDevice  =  {
  FLASH_DRV_VERS,                       // Driver Version, do not modify!
  "TMS470MF Bank1 32KB ECC",            // Device Name
  ONCHIP,                               // Device Type
  0x440000,                             // Device Start Address
  0x8000,                               // Device Size in Bytes (32 KB)
  4096,                                 // Programming Page Size
  0,                                    // Reserved, must be 0
  0xFF,                                 // Initial Content of Erased Memory
  6000,                                 // Program Page Timeout 6000 mSec
  8000,                                 // Erase Sector Timeout 8000 mSec

  // Specify Size and Address of Sectors: Dummy. Not used !!
  0x2000,  0x000000,                    // Sector Size  8kB ( 4 Sectors)
  SECTOR_END
};
#endif

