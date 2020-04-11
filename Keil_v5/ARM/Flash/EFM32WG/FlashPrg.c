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
 * Project:      Flash Programming Functions for EFM32 Wonder Gecko device series
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        /* FlashOS Structures */
#include "em_device.h"
#include "em_msc.h"

/* Leopard/Wonder Gecko has 2K flash page size */
#define FLASH_PAGE_SIZE      2048
#define FLASH_PAGE_MASK      (~(FLASH_PAGE_SIZE-1))

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static msc_Return_TypeDef MscStatusWait( uint32_t mask, uint32_t value )
{
  uint32_t status;
  int timeOut = MSC_PROGRAM_TIMEOUT;

  while (1)
  {
    status = MSC->STATUS;

    if ( status &
         ( MSC_STATUS_LOCKED | MSC_STATUS_INVADDR | MSC_STATUS_WORDTIMEOUT ) )
    {
      MSC->WRITECTRL &= ~MSC_WRITECTRL_WREN;

      if ( status & MSC_STATUS_LOCKED )
        return mscReturnLocked;

      if ( status & MSC_STATUS_INVADDR )
        return mscReturnInvalidAddr;

      return mscReturnTimeOut;
    }

    if ( ( status & mask ) == value )
      return mscReturnOk;

    timeOut--;
    if ( timeOut == 0 )
      break;
  }
  return mscReturnTimeOut;
}

static msc_Return_TypeDef DoFlashCmd( uint32_t cmd )
{
  MSC->WRITECMD = cmd;
  return MscStatusWait( MSC_STATUS_BUSY, 0 );
}

static msc_Return_TypeDef PgmBurst( uint32_t addr, uint32_t *p, uint32_t cnt )
{
  msc_Return_TypeDef retVal;

  if ( (retVal = MscStatusWait( MSC_STATUS_BUSY, 0 )) != mscReturnOk )
    return retVal;

  MSC->ADDRB    = addr;
  MSC->WRITECMD = MSC_WRITECMD_LADDRIM;
  MSC->WDATA    = *p++;
  MSC->WRITECMD = MSC_WRITECMD_WRITEONCE;
  cnt          -= 4;

  while ( cnt )
  {
    if ( (retVal = MscStatusWait( MSC_STATUS_WDATAREADY, MSC_STATUS_WDATAREADY )) != mscReturnOk )
      return retVal;

    MSC->WDATA = *p++;
    cnt       -= 4;
  }
  return mscReturnOk;
}

/*****************************************************************************
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 ****************************************************************************/
int Init(unsigned long adr, unsigned long clk, unsigned long fnc)
{
  /* Unlock the MSC */
  MSC->LOCK = MSC_UNLOCK_CODE;

  return 0;
}


/*****************************************************************************
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 ****************************************************************************/
int UnInit(unsigned long fnc)
{
  /* Disable write in MSC */
  MSC->WRITECTRL &= ~MSC_WRITECTRL_WREN;
  return 0;
}


/*****************************************************************************
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 ****************************************************************************/
int EraseChip(void)
{
  msc_Return_TypeDef result;

  MSC->WRITECTRL |= MSC_WRITECTRL_WREN;
  MSC->MASSLOCK   = MSC_MASSLOCK_LOCKKEY_UNLOCK;

  result = DoFlashCmd( MSC_WRITECMD_ERASEMAIN0 );

  MSC->MASSLOCK   = 0;
  MSC->WRITECTRL &= ~MSC_WRITECTRL_WREN;

  return result == mscReturnOk ? 0 : 1;
}

/*****************************************************************************
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 ****************************************************************************/
int EraseSector(unsigned long adr)
{
  uint32_t            blank;
  msc_Return_TypeDef  result    = mscReturnOk;
  uint32_t            *p        = (uint32_t*)adr;
  uint32_t            pageSize  = FLASH_PAGE_SIZE;

  do
  {
    blank     = *p++;
    pageSize -= 4;
  } while ( pageSize && ( blank == 0xFFFFFFFF ) );

  if ( blank != 0xFFFFFFFF )
  {
    MSC->WRITECTRL |= MSC_WRITECTRL_WREN;
    MSC->ADDRB      = adr;
    MSC->WRITECMD   = MSC_WRITECMD_LADDRIM;
    result          = DoFlashCmd( MSC_WRITECMD_ERASEPAGE );
    MSC->WRITECTRL &= ~MSC_WRITECTRL_WREN;
  }

  return result == mscReturnOk ? 0 : 1;
}

/*****************************************************************************
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 ****************************************************************************/
int ProgramPage(unsigned long adr, unsigned long sz, unsigned char *buf)
{
  uint32_t burst;

  sz = (sz + 3) & ~3;                     /* Make sure we are modulo 4. */

  MSC->WRITECTRL |= MSC_WRITECTRL_WREN;

  while ( sz )
  {
    /* Max burst len is up to next flash page boundary. */
    burst = MIN( sz, ( ( adr + FLASH_PAGE_SIZE ) & FLASH_PAGE_MASK ) - adr );

    if ( PgmBurst( adr, (uint32_t*)buf, burst ) != mscReturnOk )
      return 1;

    buf += burst;
    adr += burst;
    sz  -= burst;
  }
  if ( MscStatusWait( MSC_STATUS_BUSY, 0 ) != mscReturnOk )
    return 1;

  MSC->WRITECTRL &= ~MSC_WRITECTRL_WREN;

  return 0;
}
