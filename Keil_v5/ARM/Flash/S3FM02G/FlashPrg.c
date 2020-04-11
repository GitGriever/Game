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
 * Project:      Flash Programming Functions for Samsung S3FM02G device
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 


#include "..\FlashOS.H"        // FlashOS Structures
#include "Flash.h"
#define M8(adr)         (*((volatile unsigned char  *) (adr)))
#define M16(adr)        (*((volatile unsigned short *) (adr)))
#define M32(adr)        (*((volatile unsigned long  *) (adr)))

#define STACK_SIZE   (64*4)                 // Stack Size

#define CM_SDIVKEY      (0xACDCul << 16)    
#define CM_PDIVKEY      (0xA3C5ul << 16)   

#if defined (S3FM02G_SOPT)
  #define  SOPT_SIZE       (0x00000008)     
  #define  SOPT_ALGO_ADDR  (0x1FFFF400)
#endif
volatile unsigned int  status;               // Flash Controller Status      
         unsigned char initflag = 0;		 // Init Flag
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
  if (!initflag) {
    CM_PCSR0    = 0xFFFFFFFF;                           // All Peripheral Clock Enable.
#if defined (S3FM02G_DFLASH)
    CM_PCSR1    = 0x00000200;                           // DFC Clock Enable.
#endif
    WDT_OMR     = (0x234 << 4) | (0x37ul << 24);        // Disable watchdog 

    CM_CSR      = (1<<1);                               // IMCLK Enable.
    while( !(CM_SR & (1<<1)) ) ;                        // IMCLK Stable Check.
    CM_MR1      = (CM_MR1 & ~0x7) | 0x3;                // SYSCLK Select.(as IMCLK)
    CM_SCDR     = 0x0 | CM_SDIVKEY;                     // System Clock Divider set.
    CM_PCDR     = 0x1 | CM_PDIVKEY;                     // Peripheral Clock Divider set.

#if defined (S3FM02G_PFLASH) ||	defined (S3FM02G_SOPT)
    PFC_CEDR    = 0x1;                                  // PFC Clock Enable.
    PFC_SRR     = 0x1;                                  // PFC Software Reset.
    PFC_MR      |= ((1<<8)|(2<<4));                     // PFC Read 3 Wait Cycle.
#elif defined (S3FM02G_DFLASH)
    DFC_CEDR    = 0x1;                                  // DFC Clock Enable.
    DFC_SRR     = 0x1;                                  // DFC Software Reset.
    DFC_MR      = 0x3;                                  // DFC Read 3 Wait Cycle.
    DFC_CR      = DFC_DFEN;                             // Enable Data Flash
      
    while (!(DFC_CR & DFC_DFSTABLE));                   // Wait till Data Flash is stable 
#endif
    initflag = 1;
  }
  return (0);                                           // Success
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

/*- BlankCheck (...) -----------------------------------------------------------
 *
 *  Blank Check Checks if Memory is Blank
 *    Parameter:      adr:  Block Start Address
 *                    sz:   Block Size (in bytes)
 *                    pat:  Block Pattern
 *    Return Value:   0 - OK,  1 - Failed
 */
#if defined (S3FM02G_SOPT)

int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat) {


  return (1);                           // Always Force Erase
}
#endif
/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */
#ifdef S3FM02G_PFLASH
int EraseChip (void)
{

  PFC_KR = 0x5A5A5A5A;                                 // Key write.
  PFC_CR = (PFC_ChipErase | PFC_START);                // Command Start.

  do {
    status = PFC_RISR;
    if( status & (PFC_ERR2|PFC_ERR1|PFC_ERR0|PFC_END) ) {
      PFC_ICR = status;
      break;
    }
  } while ((status & (PFC_ERR2|PFC_ERR1|PFC_ERR0|PFC_END)));
  
  if(status & (PFC_ERR2|PFC_ERR1|PFC_ERR0))  return(1);

  return (0);                                          // Success
}
#elif defined (S3FM02G_SOPT)
int EraseChip (void)
{
  return (0);                                          // Success
}
#elif defined (S3FM02G_DFLASH)
int EraseChip (void)
{
  DFC_KR = 0xA5A5A5A5;                                 // Key write.
  DFC_CR = (DFC_ChipErase | DFC_DFEN | DFC_START);     // Command Start.

  do {
    status = DFC_RISR;
    if( status & (DFC_ERR2|DFC_ERR1|DFC_ERR0|DFC_END) ) {
      DFC_ICR = status;
      break;
    }
  } while ((status & (DFC_ERR2|DFC_ERR1|DFC_ERR0|DFC_END)));
  
  if(status & (DFC_ERR2|DFC_ERR1|DFC_ERR0))  return(1);

  return (0);                                          // Success
}
#endif
/*- EraseSector (...) ----------------------------------------------------------
 *
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */
#if defined (S3FM02G_PFLASH) ||	defined (S3FM02G_SOPT)
int EraseSector (unsigned long adr)
{

  PFC_KR = 0x5A5A5A5A;                       // Key write.
#if defined (S3FM02G_SOPT)
  adr &= ~0xFFFFF800;  
#endif
  PFC_AR = adr;
#if defined (S3FM02G_PFLASH)
  PFC_CR = (PFC_SectorErase | PFC_START);    // Command Start.
#elif defined (S3FM02G_SOPT)
  PFC_CR = (PFC_SmartErase | PFC_START);     // Command Start.
#endif

  do {
    status = PFC_RISR;
    if( status & (PFC_ERR2|PFC_ERR1|PFC_ERR0|PFC_END) ) {
      PFC_ICR = status;
      break;
    }
  } while (!(status & (PFC_ERR2|PFC_ERR1|PFC_ERR0|PFC_END)));
  
  if(status & (PFC_ERR2|PFC_ERR1|PFC_ERR0))  return(1);

  return (0);                                // Success
}
#elif defined (S3FM02G_DFLASH)
int EraseSector (unsigned long adr)
{
  volatile unsigned long limit = 10000;

  DFC_KR = 0xA5A5A5A5;                       // Key write.
  DFC_AR = adr;
  DFC_CR = (DFC_SectorErase | DFC_DFEN | DFC_START);

  do {
    status = DFC_RISR;
    if( status & (DFC_ERR2|DFC_ERR1|DFC_ERR0|DFC_END) ) {
      DFC_ICR = status;
      break;
    }
  } while (!(status & (DFC_ERR2|DFC_ERR1|DFC_ERR0|DFC_END)));
  
  if(status & (DFC_ERR2|DFC_ERR1|DFC_ERR0))  return(1);

  return (0);                                // Success
}
#endif

#if defined (S3FM02G_PFLASH) ||	defined (S3FM02G_SOPT)
/*- ProgramPage (...) ----------------------------------------------------------
 *
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */


int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf)
{
  unsigned long *ptr;
  
  ptr = (unsigned long *)buf;
  
  if ((adr & 3) || (sz & 3)) {                  // Address and Size must be Word aligned
    return (1);
  }
#if defined (S3FM02G_SOPT)
  adr &= ~0xFFFFF800;  
#endif
  while (sz) {                                  // Loop over the Words to be programmed
    PFC_KR = 0x5A5A5A5A;
    PFC_AR = adr;
    PFC_DR = *ptr;
#if defined (S3FM02G_PFLASH)
    PFC_CR = (PFC_Program | PFC_START);         // Command Start.
#elif defined (S3FM02G_SOPT)
    PFC_CR = (PFC_SmartProgram | PFC_START);    // Command Start.
#endif
    do {
      status = PFC_RISR;
      if(status & (PFC_END)) {
        PFC_ICR = status;
        break;
      }
    } while (!(status & (PFC_ERR2|PFC_ERR1|PFC_ERR0|PFC_END)));
  
    if(status & (PFC_ERR2|PFC_ERR1|PFC_ERR0))  return(1);

    adr += 4; sz  -= 4; ptr ++;
  }

  return (0);                           // Success
}
#elif defined (S3FM02G_DFLASH)
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf)
{
  unsigned long *ptr;
  
  
  ptr = (unsigned long *)buf;
  
  if ((adr & 3) || (sz & 3)) {                             // Address and Size must be Word aligned
    return (1);
  }
  
  while (sz) {                                             // Loop over the Words to be programmed
    DFC_KR = 0xA5A5A5A5;
    DFC_AR = adr;
    DFC_DR = *ptr;
    DFC_CR = (DFC_WordProgram | DFC_DFEN | DFC_START);     // Command Start.
  
    do {
      status = DFC_RISR;
      if(status & (DFC_ERR2|DFC_ERR1|DFC_ERR0|DFC_END)) {
        DFC_ICR = status;
        break;
      }
    } while (!(status & (DFC_ERR2|DFC_ERR1|DFC_ERR0|DFC_END)));
  
    if(status & (DFC_ERR2|DFC_ERR1|DFC_ERR0))  return(1);
  
    adr += 4; sz  -= 4; ptr ++;
  }
  return (0);                                              // Success
}

#endif

/*- Verify (...) ---------------------------------------------------------------
 *
 *  Verify Flash Contents
 *    Parameter:      adr:  Start Address
 *                    sz:   Size (in bytes)
 *                    buf:  Data
 *    Return Value:   (adr+sz) - OK, Failed Address
 */
#if defined (S3FM02G_SOPT)
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf) {
  volatile unsigned long * s_buffer;

  if (sz  != SOPT_SIZE)      return(adr);
  if (adr != SOPT_ALGO_ADDR) return(adr);


  s_buffer  = (volatile unsigned long *)buf;
  if ((*s_buffer) != PFC_SO_CSR) return(adr);
  s_buffer++;
  if ((*s_buffer) != PFC_SO_PSR) return(adr+4);

  return (adr+sz);                      // Done successfully
}

#endif
