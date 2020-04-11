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
 * Project:      Flash Device Description for ST Microelectronics STM32L15x Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        // FlashOS Structures

#ifdef FLASH_MEMORY

#ifdef STM32L15x_128
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "STM32L15x Low Power Flash", // Device Name (128kB/64kB/32kB)
   ONCHIP,                     // Device Type
   0x08000000,                 // Device Start Address
   0x00020000,                 // Device Size in Bytes (128kB)
   256,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   500,                        // Program Page Timeout 500 mSec
   500,                        // Erase Sector Timeout 500 mSec

// Specify Size and Address of Sectors
   0x0100, 0x000000,           // Sector Size 256B (512 Sectors)
   SECTOR_END
};
#endif // STM32L15x_128

#endif // FLASH_MEMORY


#ifdef FLASH_EEPROM
#ifdef STM32L15x_EEPROM
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "STM32L15x Data EEPROM",    // Device Name (4kB)
   ONCHIP,                     // Device Type
   0x08080000,                 // Device Start Address
   0x00001000,                 // Device Size in Bytes (4B)
   256,                        // Programming Page Size (256B)
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   500,                        // Program Page Timeout 500 mSec
   500,                        // Erase Sector Timeout 500 mSec

// Specify Size and Address of Sectors
   0x100, 0x000000,            // Sector Size 256B (16 Sectors)
   SECTOR_END
};
#endif // STM32L15x_EEPROM
#endif // FLASH_EEPROM

#ifdef FLASH_OPTION
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "STM32L15x Flash Options",  // Device Name
   ONCHIP,                     // Device Type
   0x1FF80000,                 // Device Start Address
   0x00000010,                 // Device Size in Bytes (16)
   16,                         // Programming Page Size
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   3000,                       // Program Page Timeout 3 Sec
   3000,                       // Erase Sector Timeout 3 Sec

// Specify Size and Address of Sectors
   0x0010, 0x000000,           // Sector Size 16B
   SECTOR_END
};
#endif // FLASH_OPTION 
