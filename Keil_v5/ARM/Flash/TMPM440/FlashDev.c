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
 * Project:      Flash Device Description for Toshiba TMPM365Fx Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        // FlashOS Structures


#ifdef TMPM440_1024
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "TMPM440 1024kB Flash",     // Device Name 
   ONCHIP,                     // Device Type
   0x00000000,                 // Device Start Address
   0x00100000,                 // Device Size (1024kB)
   256,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x002000, 0x000000,         // Sector Size   8kB (4 Sectors)
   0x004000, 0x008000,         // Sector Size  16kB (2 Sector) 
   0x008000, 0x010000,         // Sector Size  32kB (2 Sectors) 
   0x010000, 0x020000,         // Sector Size  64kB (6 Sectors) 
   0x002000, 0x080000,         // Sector Size   8kB (4 Sectors) 
   0x004000, 0x088000,         // Sector Size  16kB (2 Sectors) 
   0x008000, 0x090000,         // Sector Size  32kB (2 Sectors) 
   0x010000, 0x0A0000,         // Sector Size  64kB (6 Sectors) 
   SECTOR_END
};
#endif

#ifdef TMPM440_BE_1024
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "TMPM440 BE 1024kB Flash",  // Device Name 
   ONCHIP,                     // Device Type
   0x00000000,                 // Device Start Address
   0x00100000,                 // Device Size (1024kB)
   256,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x002000, 0x000000,         // Sector Size   8kB (4 Sectors)
   0x004000, 0x008000,         // Sector Size  16kB (2 Sector) 
   0x008000, 0x010000,         // Sector Size  32kB (2 Sectors) 
   0x010000, 0x020000,         // Sector Size  64kB (6 Sectors) 
   0x002000, 0x080000,         // Sector Size   8kB (4 Sectors) 
   0x004000, 0x088000,         // Sector Size  16kB (2 Sectors) 
   0x008000, 0x090000,         // Sector Size  32kB (2 Sectors) 
   0x010000, 0x0A0000,         // Sector Size  64kB (6 Sectors) 
   SECTOR_END
};
#endif

#ifdef TMPM440_768
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "TMPM440 768kB Flash",      // Device Name 
   ONCHIP,                     // Device Type
   0x00000000,                 // Device Start Address
   0x000C0000,                 // Device Size (768kB)
   256,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x002000, 0x000000,         // Sector Size   8kB (4 Sectors)
   0x004000, 0x008000,         // Sector Size  16kB (2 Sector) 
   0x008000, 0x010000,         // Sector Size  32kB (2 Sectors) 
   0x010000, 0x020000,         // Sector Size  64kB (6 Sectors) 
   0x002000, 0x080000,         // Sector Size   8kB (4 Sectors) 
   0x004000, 0x088000,         // Sector Size  16kB (2 Sectors) 
   0x008000, 0x090000,         // Sector Size  32kB (2 Sectors) 
   0x010000, 0x0A0000,         // Sector Size  64kB (2 Sectors) 
   SECTOR_END
};
#endif

#ifdef TMPM440_BE_768
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "TMPM440 BE 768kB Flash",   // Device Name 
   ONCHIP,                     // Device Type
   0x00000000,                 // Device Start Address
   0x000C0000,                 // Device Size (768kB)
   256,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x002000, 0x000000,         // Sector Size   8kB (4 Sectors)
   0x004000, 0x008000,         // Sector Size  16kB (2 Sector) 
   0x008000, 0x010000,         // Sector Size  32kB (2 Sectors) 
   0x010000, 0x020000,         // Sector Size  64kB (6 Sectors) 
   0x002000, 0x080000,         // Sector Size   8kB (4 Sectors) 
   0x004000, 0x088000,         // Sector Size  16kB (2 Sectors) 
   0x008000, 0x090000,         // Sector Size  32kB (2 Sectors) 
   0x010000, 0x0A0000,         // Sector Size  64kB (2 Sectors) 
   SECTOR_END
};
#endif
