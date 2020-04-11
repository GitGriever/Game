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
 * Project:      Flash Device Description for Samsung S3FN429 Device Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        // FlashOS Structures

#ifdef S3FN429_PFLASH 
struct FlashDevice const FlashDevice  =  {
  FLASH_DRV_VERS,                       // Driver Version, do not modify!
  "S3FN429 32K Flash",                  // Device Name
  ONCHIP,                               // Device Type
  0x0,                                  // Device Start Address
  0x8000,                               // Device Size in Bytes (32 kB)
  1024,                                 // Programming Page Size
  0,                                    // Reserved, must be 0
  0xFF,                                 // Initial Content of Erased Memory
  500,                                  // Program Page Timeout 500  mSec
  5000,                                 // Erase Sector Timeout 5000 mSec
  // Specify Size and Address of Sectors
  0x2000, 0x0000,                       // Sector Size  8KB (4 sectors)
  SECTOR_END
};
#endif

#ifdef S3FN429_SOPT 
struct FlashDevice const FlashDevice  =  {
  FLASH_DRV_VERS,                       // Driver Version, do not modify!
  "S3FN429 Smart Option",               // Device Name
  ONCHIP,                               // Device Type
  0x1FFFFFC0,                           // Device Start Address
  0x8,                                  // Device Size in Bytes (8 Bytes)
  8,                                    // Programming Page Size
  0,                                    // Reserved, must be 0
  0xFF,                                 // Initial Content of Erased Memory
  1000,                                 // Program Page Timeout max. 30 uSec
  3000,                                 // Erase Sector Timeout max. 28 mSec
  // Specify Size and Address of Sectors
  0x8,  0x0000,                         // Sector Size  8 Bytes (1 Sector)
  SECTOR_END
};
#endif
