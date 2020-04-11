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
 * Project:      Flash Device Description for ATSAM4L
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"         // FlashOS Structures


/* ---- SAM4L ----------------------------------------------------------*/
#ifdef FLASH_MEM

#ifdef ATSAM4L_128
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,              // Driver Version, do not modify!
   "ATSAM4L 128kB Flash",       // Device Name
   ONCHIP,                      // Device Type
   0x00000000,                  // Device Start Address
   0x00020000,                  // Device Size in Bytes (128kB)
   512,                         // Programming Page Size
   0,                           // Reserved, must be 0
   0xFF,                        // Initial Content of Erased Memory
   300,                         // Program Page Timeout 300 mSec
   3000,                        // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x4000, 0x000000,            // Sector Size 16KB (8 Sectors(Regions); 1 Sector  = PAGE_SIZE * 32)
   SECTOR_END
};
#endif

#ifdef ATSAM4L_256
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,              // Driver Version, do not modify!
   "ATSAM4L 256kB Flash",       // Device Name
   ONCHIP,                      // Device Type
   0x00000000,                  // Device Start Address
   0x00040000,                  // Device Size in Bytes (256kB)
   512,                         // Programming Page Size
   0,                           // Reserved, must be 0
   0xFF,                        // Initial Content of Erased Memory
   300,                         // Program Page Timeout 300 mSec
   3000,                        // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x4000, 0x000000,            // Sector Size 16KB (16 Sectors(Regions); 1 Sector  = PAGE_SIZE * 32)
   SECTOR_END
};
#endif


#ifdef ATSAM4L_512
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,              // Driver Version, do not modify!
   "ATSAM4L 512kB Flash",       // Device Name
   ONCHIP,                      // Device Type
   0x00000000,                  // Device Start Address
   0x00080000,                  // Device Size in Bytes (256kB)
   512,                         // Programming Page Size
   0,                           // Reserved, must be 0
   0xFF,                        // Initial Content of Erased Memory
   300,                         // Program Page Timeout 300 mSec
   3000,                        // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x4000, 0x000000,            // Sector Size 16KB (16 Sectors(Regions); 1 Sector  = PAGE_SIZE * 32)
   SECTOR_END
};
#endif
#endif /* FLASH_MEM */

#ifdef FLASH_GPNVM

#endif /* FLASH_GPNVM */
