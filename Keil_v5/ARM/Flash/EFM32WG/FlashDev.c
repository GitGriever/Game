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
 * Project:      Flash Device Description for Energy Micro EFM32G
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        /* FlashOS Structures */


/*
 * NOTE: Program Page Timeout is NOT time it takes to program a flash page !
 *       Timeout found by "try/fail" method plus ample margin,
 *       but never less than 100ms.
 */
struct FlashDevice const FlashDevice =
{
  FLASH_DRV_VERS,            /* Driver Version, do not modify!   */
  "EFM32 Wonder Gecko",      /* Device Name                      */
  ONCHIP,                    /* Flash Type                       */
  0x00000000,                /* Flash Start Address              */
  0x00040000,                /* Flash Size                       */
  (16*1024),                 /* Programming Page (buffer) Size   */
  0,                         /* Reserved, must be 0              */
  0xFF,                      /* Content of Erased Memory cell    */
  260,                       /* Program Page Timeout 260 mSec    */
  200,                       /* Erase Sector Timeout 200 mSec    */
  {
    {
      2048,                  /* Sector Size                      */
      0x00000000             /* Address of first sector          */
    },
    {
      SECTOR_END
    }
  }
};
