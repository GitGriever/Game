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
 * Project:      Flash Programming Functions for Samsung S3FN429 Device Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 
 
#include <stdint.h>
#include "..\FlashOS.H"        // FlashOS Structures
#include "flash.h"

#define M8(adr)         (*((volatile unsigned char  *) (adr)))
#define M16(adr)        (*((volatile unsigned short *) (adr)))
#define M32(adr)        (*((volatile unsigned long  *) (adr)))

#define STACK_SIZE  	64     /* Stack Size */

#define SOPT_SIZE       (8)

/************************* Flash Algorithm Functions ***************************/

/*- Init (...) -----------------------------------------------------------------
 *
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */
int Init (unsigned long adr, unsigned long clk, unsigned long fnc)
{
  uint32_t i = 8;

  /* Disable the watchdog timer */
  WDT0->OMR = 0x00002340;
	
  /* Enable target pripheral clock to access SFR */	
  CM0->PCSR = (0x1<<29); /* Enable IFC */
  
  /* Enable IFC controller */
  IFC0->CEDR |= IFC_CLKEN;
  
  if(((IFC0->SOCSR)& 0x1)==0x1)	/* System clock is IMCLK at reset */
  {
	/* High speed(over 20MHz) flash mode & Boot sector off */ 
    IFC0->MR |= IFC_FSMODE;
  }
  else /* System clock is EMCLK at reset */
  {
	/* Normal speed flash mode & Boot sector off */	
    IFC0->MR = 0;
  }
  /* SYSTEM clock divider from 8 to 1*/
  CM0->SCDR = 0xACDC0000;
  while(i--);

  return (0);                          // Success
}

/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */
int UnInit (unsigned long fnc)
{
  return (0);
}

/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */
int EraseChip (void)
{
  int32_t ret = 0;  
  
#if defined (S3FN429_PFLASH)
  /* chip erase */
  IFC0->ICR = IFC_END;                          // Clear 'END' flag 	
  IFC0->KR  = IFC_KEY;							// Unlock flash 
  IFC0->CR  = IFC_CMD(IFC_CE)|IFC_START;		// Start chip erase	
  while((IFC0->RISR & IFC_END) != IFC_END);     // Wait for erase to finish
	
  if (IFC0->RISR & (IFC_ERR0 | IFC_ERR1 | IFC_ERR2)) return(1);
	
  IFC0->ICR = IFC_END;							// Clear 'END' flag 	
	
#elif defined (S3FN429_SOPT)
  /* smart option erase */
  IFC0->ICR = IFC_END;                          // Clear 'END' flag 
  IFC0->KR  = IFC_KEY;							// Unlock flash
  IFC0->CR  = IFC_CMD(IFC_SOE)|IFC_START;		// Start smart option erase
  while((IFC0->RISR & IFC_END) != IFC_END);     // Wait for erase to finish
  
  if (IFC0->RISR & (IFC_ERR0 | IFC_ERR1 | IFC_ERR2)) return(1);
  
  IFC0->ICR = IFC_END;  						 // Clear 'END' flag  
#endif

  return (ret);                              	 // Success
}

/*- EraseSector (...) ----------------------------------------------------------
 *
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */
int EraseSector (unsigned long adr) {

  IFC0->ICR   = IFC_END;                    	// Clear 'END' flag 
  IFC0->KR    = IFC_KEY;						// Unlock flash
  IFC0->AR    = adr;							// Set the address of target sector
#if defined (S3FN429_PFLASH)
  IFC0->CR    = IFC_CMD(IFC_SE)|IFC_START;		// Start sector erase	
#elif defined (S3FN429_SOPT)
  IFC0->CR    = IFC_CMD(IFC_SOE)|IFC_START;		// Start smart option erase
#endif  
  while((IFC0->RISR & IFC_END) != IFC_END);   	// Wait for erase to finish
  
  if (IFC0->RISR & (IFC_ERR0 | IFC_ERR1 | IFC_ERR2)) return(1);
  
  IFC0->ICR = IFC_END;							// Clear 'END' flag 
  
  return (0);                                 	// Success
}

/*- BlankCheck (...) ----------------------------------------------------------
 *
 *  Blank Check in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    pat:  Pattern to compare
 *    Return Value:   0 - OK,  1 - Failed
 */
#if defined (S3FN429_SOPT)
int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat) {

  if (sz  != SOPT_SIZE) return(1);

  if (IFC0->SOCSR != 0xFFFFFFFF) return(1);
  if (IFC0->SOPSR != 0xFFFFFFFF) return(1);

  return (0);                      				// Done successfully
}

#endif

/*- ProgramPage (...) ----------------------------------------------------------
 *
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */
int ProgramPage (unsigned long adr, unsigned long size, unsigned char *buf) {

  if ((adr & 3) || (size & 3)) {          		// adr and size must be Word aligned
    return (1);
  }
  
  //IFC0->CEDR |= IFC_CLKEN;

#if defined (S3FN429_SOPT)
  adr = 0xC0;                             		// Change to SOPT address
#endif  
  while (size)
  { // Loop over the Words to be programmed
       
    IFC0->ICR = IFC_END;            			// Clear 'END' flag 
    IFC0->KR = IFC_KEY;							// Unlock flash
    IFC0->DR = *((unsigned long *)buf);			// Set the data to program
    IFC0->AR = (uint32_t) adr;					// Set target address to program the data
#if defined (S3FN429_PFLASH)
    IFC0->CR = IFC_CMD(IFC_NP)|IFC_START;		// Start noraml program		
#elif defined (S3FN429_SOPT)
    IFC0->CR = IFC_CMD(IFC_SOP)|IFC_START;		// Start smart option program
#endif
    while((IFC0->RISR & IFC_END) != IFC_END);	// Wait for write to finish
  
    if (IFC0->RISR & (IFC_ERR0 | IFC_ERR1 | IFC_ERR2)) return(1);
    
    IFC0->ICR = IFC_END;						// Clear 'END' flag 
    adr  += 4;
    buf  += 4;
    size -= 4;
  } 
  return (0);                                 	// Success
}

#if defined (S3FN429_SOPT)
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf) {
  volatile unsigned long * s_buffer;

  if (sz  != SOPT_SIZE) return(adr);
  s_buffer  = (volatile unsigned long *)buf;
  if ((*s_buffer) != IFC0->SOCSR) return(adr);
  s_buffer++;
  if ((*s_buffer) != IFC0->SOPSR) return(adr+4);

  return (adr+sz);                            	// Done successfully
}
#endif

