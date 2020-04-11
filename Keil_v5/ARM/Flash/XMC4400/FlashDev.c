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
 * Project:      Flash Device Description for Infineon XMC4400 Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        // FlashOS Structures

#ifdef FLASH_MEM

#ifdef XMC4400_512
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,                // Driver Version, do not modify!
   "XMC4400 512kB Flash",         // Device name
   ONCHIP,                        // Device Type
   0x0C000000,                    // Device Start Address
   0x00080000,                    // Device Size in Bytes (512kB)
   256,                           // Programming Page Size (Bytes)
   0,                             // Reserved, must be 0
   0x00,                          // Initial Content of Erased Memory
   100,                           // Program Page Timeout 100 mSec
   20000,                         // Erase Sector Timeout 20000 mSec

// Specify Size and Address of Sectors
   0x04000, 0x000000,             // Sector Size  16kB (8 Sectors)
   0x20000, 0x020000,             // Sector Size 128kB (1 Sector )
   0x40000, 0x040000,             // Sector Size 256kB (1 Sectors)
   SECTOR_END                 
};
#endif

#ifdef XMC4400c_512
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,                // Driver Version, do not modify!
   "XMC4400 512kB cached Flash",  // Device name
   ONCHIP,                        // Device Type
   0x08000000,                    // Device Start Address
   0x00080000,                    // Device Size in Bytes (512kB)
   256,                           // Programming Page Size (Bytes)
   0,                             // Reserved, must be 0
   0x00,                          // Initial Content of Erased Memory
   100,                           // Program Page Timeout 100 mSec
   20000,                         // Erase Sector Timeout 20000 mSec

// Specify Size and Address of Sectors
   0x04000, 0x000000,             // Sector Size  16kB (8 Sectors)
   0x20000, 0x020000,             // Sector Size 128kB (1 Sector )
   0x40000, 0x040000,             // Sector Size 256kB (1 Sectors)
   SECTOR_END                 
};
#endif

#ifdef XMC4400_256
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,                // Driver Version, do not modify!
   "XMC4400 256kB Flash",         // Device name
   ONCHIP,                        // Device Type
   0x0C000000,                    // Device Start Address
   0x00040000,                    // Device Size in Bytes (256kB)
   256,                           // Programming Page Size (Bytes)
   0,                             // Reserved, must be 0
   0x00,                          // Initial Content of Erased Memory
   100,                           // Program Page Timeout 100 mSec
   20000,                         // Erase Sector Timeout 20000 mSec

// Specify Size and Address of Sectors
   0x04000, 0x000000,             // Sector Size  16kB (8 Sectors)
   0x20000, 0x020000,             // Sector Size 128kB (1 Sector )
   SECTOR_END                 
};
#endif

#ifdef XMC4400c_256
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,                // Driver Version, do not modify!
   "XMC4400 256kB cached Flash",  // Device name
   ONCHIP,                        // Device Type
   0x08000000,                    // Device Start Address
   0x00040000,                    // Device Size in Bytes (256kB)
   256,                           // Programming Page Size (Bytes)
   0,                             // Reserved, must be 0
   0x00,                          // Initial Content of Erased Memory
   100,                           // Program Page Timeout 100 mSec
   20000,                         // Erase Sector Timeout 20000 mSec

// Specify Size and Address of Sectors
   0x04000, 0x000000,             // Sector Size  16kB (8 Sectors)
   0x20000, 0x020000,             // Sector Size 128kB (1 Sector )
   SECTOR_END                 
};
#endif

#endif
