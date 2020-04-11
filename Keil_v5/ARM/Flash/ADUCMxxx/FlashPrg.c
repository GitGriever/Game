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
 * Project:      Flash Programming Functions for Analog Devices ADuCRF101, ADuCM360
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 


#include "..\FlashOS.H"        // FlashOS Structures
#include <ADI\ADuCRF101.H>        

/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) 
{
   // Try to turn off the watchdog while we are programming the flash
  pADI_WDT->T3CON    = 0x0;

  // Write 1 to unprotect all the sections of flash. Note that this register
  // is read-only if the write protection in flash is programmed.
  pADI_FEE->FEEPROL = 0xFFFF;
  pADI_FEE->FEEPROH = 0xFFFF;

  // Enable flash writing, disable any flash related interrupts
  pADI_FEE->FEECON0  = FEECON0_WREN; 
  return (0);
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) 
{
  // Disable flash writing
  pADI_FEE->FEECON0  = 0x0;

  return (0);
}

/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */
int EraseChip (void) 
{
   unsigned short int   FEESTACpy;

   // Clear any status from any previous command
   (void)pADI_FEE->FEESTA;

   // Enable flash writing
   pADI_FEE->FEECON0  = FEECON0_WREN;

   // Write the user keys
   pADI_FEE->FEEKEY = FEEKEY_VALUE_USERKEY1;
   pADI_FEE->FEEKEY = FEEKEY_VALUE_USERKEY2;

   // Issue the command to erase the chip
   pADI_FEE->FEECMD = FEECMD_CMD_MASSERASE;

   // Wait for the command to complete
   do
      {
      FEESTACpy = pADI_FEE->FEESTA;
      }
    while((FEESTACpy & FEESTA_CMDDONE) == 0x0);

    // Check result of operation
    if((FEESTACpy & FEESTA_CMDRES_MSK) != FEESTA_CMDRES_SUCCESS)
        return (1);                             // Error
    else
        return (0);                                   // Done
}

/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) 
{

   unsigned short int   FEESTACpy;

   // Clear any status from any previous command
   (void)pADI_FEE->FEESTA;

   // Enable flash writing
   pADI_FEE->FEECON0  = FEECON0_WREN;

   // Write the user keys
   pADI_FEE->FEEKEY = FEEKEY_VALUE_USERKEY1;
   pADI_FEE->FEEKEY = FEEKEY_VALUE_USERKEY2;

   // Write the page to be erased to the register
   pADI_FEE->FEEADR0L = adr & 0xFFFF;
   pADI_FEE->FEEADR0H = (adr >> 16) & 0xFFFF;

   // Issue the command to erase the page
   pADI_FEE->FEECMD = FEECMD_CMD_ERASEPAGE;

   // Wait for the command to complete
   do
      {
      FEESTACpy = pADI_FEE->FEESTA;
      }
    while((FEESTACpy & FEESTA_CMDDONE) == 0x0);

    // Check result of operation
    if((FEESTACpy & FEESTA_CMDRES_MSK) != FEESTA_CMDRES_SUCCESS)
        return (1);                             // Error
    else
        return (0);                                   // Done
}

/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long count, unsigned char *buf) 
{
   unsigned short int   FEESTACpy;
   unsigned long * dst = (unsigned long *)adr;
   unsigned long * src = (unsigned long *)buf;

   // Write the user keys
   pADI_FEE->FEEKEY = FEEKEY_VALUE_USERKEY1;
   pADI_FEE->FEEKEY = FEEKEY_VALUE_USERKEY2;

   // Assumptions:
   // adr aligned to a word boundary
   // count is a multiple of 4
   // Clear any status from any previous command
   (void)pADI_FEE->FEESTA;

   // The main block of data is written using 32 bit writes
   for ( ; count; count -= 0x4)
       {
       if (count < 4)
           {
           // Do a 32 bit write with 0xFF in the missing bytes
           unsigned long ulTmpData = 0xFFFFFFFF;
           unsigned char * pTmp     = (unsigned char *)&ulTmpData;
           switch (count)
               {
               case 0x3:
                   pTmp[2] = *(((unsigned char *)src)+ 0x2);
               case 0x2:
                   pTmp[1] = *(((unsigned char *)src)+ 0x1);
               case 0x1:
                   pTmp[0] = *(((unsigned char *)src)+ 0x0);
                   break;
               default:
                   break;
               }
           *dst++ = ulTmpData;
           count = 0x4; // 4 bytes written, also allows the loop to terminate without overflow
           }
       else
           // Perform the write
           *dst++ = *src++;

      // Wait for write to be completed
       do
          {FEESTACpy = pADI_FEE->FEESTA;}
        while((FEESTACpy & FEESTA_WRDONE) == 0x0);
      }
 
   return (0);                                   // Done
}

