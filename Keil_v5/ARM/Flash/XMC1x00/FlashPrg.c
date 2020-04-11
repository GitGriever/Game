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
 * Project:      Flash Programming Functions for Infineon XMC1x00 Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        // FlashOS Structures

/* Constants definitions */
#define ROM_FUNCTION_TABLE_START (0x00000100)                           // Start address of the ROM function table

#define _NvmErase                (ROM_FUNCTION_TABLE_START + 0x00)      // Pointer to Erase Flash Page routine
#define _NvmProgVerify           (ROM_FUNCTION_TABLE_START + 0x04)      // Pointer to Erase, Program & Verify Flash Page routine


/* Types definitions */
typedef enum tagNVMStatus
{
    NVM_PASS                              = (signed long) 0x00010000,   // the function succeeded
    NVM_E_FAIL                            = (signed long) 0x80010001,   // generic error code
    NVM_E_SRC_AREA_EXCCEED                = (signed long) 0x80010003,   // source data not in RAM
    NVM_E_SRC_ALIGNMENT                   = (signed long) 0x80010004,   // source data is not 4 byte aligned
    NVM_E_NVM_FAIL                        = (signed long) 0x80010005,   // NVM module cannot be physically accessed
    NVM_E_VERIFY                          = (signed long) 0x80010006,   // verification of written page not successful
    NVM_E_DST_AREA_EXCEED                 = (signed long) 0x80010009,   // destination data is not (completely) located in NVM
    NVM_E_DST_ALIGNMENT                   = (signed long) 0x80010010,   // destination data is not properly aligned
} NVM_STATUS;


/* Macros definitions */

/******************************************************************************************************************************
Description: TIMM1 Flash can be erased with granularity of one page = 16 blocks of 16 Bytes = 256 Bytes using this routine.

•    Input parameters:
– logical address of the Flash Page to be erased, must be page aligned and in NVM address range

•    Return status:
– OK (NVM_PASS)
– invalid address (NVM_E_DST_ALIGNMENT or NVM_E_DST_AREA_EXCEED)
– operation failed (error during low level NVM programming driver: NVM_E_FAIL, NVM_E_VERIFY, NVM_E_NVM_FAIL)

•    Prototype:
     NVM_STATUS TIMM_NvmErasePage(unsigned long* pageAddr)
********************************************************************************************************************************/
#define TIMM_NvmErasePage (*((NVM_STATUS (**) (unsigned long * )) _NvmErase))

/******************************************************************************************************************************
Description: This procedure performs erase (skipped if not necessary), program and verify of selected Flash page.

•    Input parameter:
– logical address of the target Flash Page, must be page aligned and in NVM address range
– Address in SRAM where the data starts, must be 4-byte aligned

•    Return status:
–  OK (NVM_PASS)
–  invalid addresses (NVM_E_DST_ALIGNMENT, NVM_E_SRC_ALIGNMENT, NVM_E_DST_AREA_EXCEED, NVM_E_SRC_AREA_EXCCEED)
–  operation failed	(error during low level NVM programming driver: NVM_E_FAIL, NVM_E_VERIFY, NVM_E_NVM_FAIL)

•    Prototype:
  NVM_STATUS TIMM_NvmProgVerify(unsigned long* srcAddr, unsigned long* dstAddr)
********************************************************************************************************************************/
#define TIMM_NvmProgVerify (*((NVM_STATUS (**) (unsigned long * , unsigned long * )) _NvmProgVerify))

NVM_STATUS status = NVM_E_FAIL;


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  return (0);
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {

  return (0);
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */
int EraseSector (unsigned long adr) {
  unsigned long* pageAddr = (unsigned long*)adr;
  unsigned int   count;                                // 16 pages = 1 sector

  for (count = 0; count < 16; count++) {
    status = TIMM_NvmErasePage(pageAddr);
    if (status != NVM_PASS) {                          // Check for error
      return (1);                                      // Failed
    }
    pageAddr += 0x100 / 4;                             // next page
  }
  return 0;
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  unsigned long* srcAddr = (unsigned long *)buf;
  unsigned long* dstAddr = (unsigned long *)adr;

  status = TIMM_NvmProgVerify(srcAddr, dstAddr);
  if (status != NVM_PASS) {                            // Check for error
    return (1);                                        // Failed
  }

  return (0);
}

