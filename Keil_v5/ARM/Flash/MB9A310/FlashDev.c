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
 * Project:      Flash Device Description for Fujitsu MB9A310K/110K Series
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        // FlashOS Structures

#ifdef FLASH_MEM

#ifdef MB9A310_64
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MB9A310 64kB Flash",       // Device Name
   ONCHIP,                     // Device Type
   0x00000000,                 // Device Start Address
   0x00010000,                 // Device Size in Bytes (64kB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec                       SA9  (16KB)  | SA8  (16KB)
   3000,                       // Erase Sector Timeout 3000 mSec                      SA7  ( 8KB)  | SA6  ( 8KB)
//                                                                                    SA5  ( 8KB)  | SA4  ( 8KB)
// Specify Size and Address of Sectors                                             <---------------|-------------->
   0x004000, 0x000000,         // Sector Size  16KB (2 Sectors)              bit 63              32 31             0
   0x008000, 0x008000,         // Sector Size  32KB (1 Sectors)                     +7  +6  +5  +4  +3  +2  +1  +0
   SECTOR_END
};
#endif

#ifdef MB9A310_128
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MB9A310 128kB Flash",      // Device Name
   ONCHIP,                     // Device Type
   0x00000000,                 // Device Start Address
   0x00020000,                 // Device Size in Bytes (64kB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec                       SA9  (48KB)  | SA8  (48KB)
   3000,                       // Erase Sector Timeout 3000 mSec                      SA7  ( 8KB)  | SA6  ( 8KB)
//                                                                                    SA5  ( 8KB)  | SA4  ( 8KB)
// Specify Size and Address of Sectors                                             <---------------|-------------->
   0x004000, 0x000000,         // Sector Size  16KB (2 Sectors)              bit 63              32 31             0
   0x018000, 0x008000,         // Sector Size  96KB (1 Sectors)                     +7  +6  +5  +4  +3  +2  +1  +0
   SECTOR_END
};
#endif


#endif                         // end FLASH_MEM
