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
 * Project:      Flash Device Description for Cypress CY8C5xxxx Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        // FlashOS Structures


#ifdef CY8C5XXXX_FLASH
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "CY8C5xxx Flash",           // Device Name 
   ONCHIP,                     // Device Type
   0x00000000,                 // Device Start Address
   0x00040000,                 // Device Size (max 256kB)
   256,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec
   500,                        // Erase Sector Timeout 500 mSec

// Specify Size and Address of Sectors
   0x004000, 0x000000,         // Sector Size 16kB
   SECTOR_END
};
#endif

#ifdef CY8C5XXXX_CFG
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "CY8C5xxx Configuration",   // Device Name 
   ONCHIP,                     // Device Type
   0x80000000,                 // Device Start Address
   0x00008000,                 // Device Size (max 32kB)
   32,                         // Programming Page Size
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec
   500,                        // Erase Sector Timeout 500 mSec

// Specify Size and Address of Sectors
   0x000800, 0x000000,         // Sector Size 2kB
   SECTOR_END
};
#endif

#ifdef CY8C5XXXX_CFG_NVL
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "CY8C5xxx CFG NVL",         // Device Name 
   ONCHIP,                     // Device Type
   0x90000000,                 // Device Start Address
   0x00000004,                 // Device Size (4B)
   4,                          // Programming Page Size
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   1000,                       // Erase Sector Timeout 1000 mSec

// Specify Size and Address of Sectors
   0x000004, 0x000000,         // Sector Size 4
   SECTOR_END
};
#endif

#ifdef CY8C5XXXX_WO_NVL
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "CY8C5xxx WO NVL",          // Device Name 
   ONCHIP,                     // Device Type
   0x90100000,                 // Device Start Address
   0x00000004,                 // Device Size (4B)
   4,                          // Programming Page Size
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   1000,                       // Erase Sector Timeout 1000 mSec

// Specify Size and Address of Sectors
   0x000004, 0x000000,         // Sector Size 4
   SECTOR_END
};
#endif

#ifdef CY8C5XXXX_EEPROM
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "CY8C5xxx EEPROM",          // Device Name 
   ONCHIP,                     // Device Type
   0x90200000,                 // Device Start Address
   0x00000800,                 // Device Size (max 2kB)
   16,                         // Programming Page Size
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   300,                        // Program Page Timeout 300 mSec
   300,                        // Erase Sector Timeout 300 mSec

// Specify Size and Address of Sectors
   0x000400, 0x000000,         // Sector Size 1kB
   SECTOR_END
};
#endif

#ifdef CY8C5XXXX_PROTECT
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "CY8C5xxx Flash Protection",// Device Name 
   ONCHIP,                     // Device Type
   0x90400000,                 // Device Start Address
   0x00000100,                 // Device Size (max 256B)
   64,                         // Programming Page Size
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   1000,                       // Erase Sector Timeout 1000 mSec

// Specify Size and Address of Sectors
   0x000040, 0x000000,         // Sector Size 64B
   SECTOR_END
};
#endif
