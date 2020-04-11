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
 * Project:      Flash Device Description for Fujitsu MB9A130L/130LA/
 *               130N/A30N Series
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 
/***********************************************************************/
/*  This file is part of the uVision/ARM development tools             */
/*  Copyright (c) 2012 Keil - An ARM Company.                          */
/*  All rights reserved.                                               */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.C:  Device Description for                                */
/*               Fujitsu MB9A130L/ 130LA/ 130N/                        */
/*                           A30N                      Series          */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        // FlashOS Structures

#ifdef FLASH_MEM

#ifdef MB9AF13x_64
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MB9AF13x 64kB Flash",      // Device Name 
   ONCHIP,                     // Device Type
   0x00000000,                 // Device Start Address
   0x00010000,                 // Device Size in Bytes (64kB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec
   3000,                       // Erase Sector Timeout 3000 mSec                           SA2  (60KB)
//                                                                                         SA1  ( 4KB)    
// Specify Size and Address of Sectors                                                  <-------------->
   0x001000, 0x000000,         // Sector Size  4KB (1 Sector)                    bit 31                0
   0x00F000, 0x001000,         // Sector Size 60KB (1 Sector)
   SECTOR_END
};
#endif

#ifdef MB9AF13x_128
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MB9AF13x 128kB Flash",     // Device Name 
   ONCHIP,                     // Device Type
   0x00000000,                 // Device Start Address
   0x00020000,                 // Device Size in Bytes (64kB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec                            SA3  (64KB)
   3000,                       // Erase Sector Timeout 3000 mSec                           SA2  (60KB)
//                                                                                         SA1  ( 4KB)    
// Specify Size and Address of Sectors                                                  <-------------->
   0x001000, 0x000000,         // Sector Size  4KB (1 Sector)                    bit 31                0
   0x00F000, 0x001000,         // Sector Size 60KB (1 Sector)
   0x010000, 0x010000,         // Sector Size 64KB (1 Sector)
   SECTOR_END
};
#endif


#endif                         // end FLASH_MEM
