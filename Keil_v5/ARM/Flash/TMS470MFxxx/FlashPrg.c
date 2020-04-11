/******************************************************************************/
/*  This file is part of the ARM Toolchain package                            */
/*  Copyright KEIL ELEKTRONIK GmbH 2003 - 2010                                */
/******************************************************************************/
/*                                                                            */
/*  FlashDev.C:  Flash Programming Functions for TMS570LS Device Series       */
/*                                                                            */
/******************************************************************************/

#include "..\FlashOS.H"                 // FlashOS Structures
#include "FlashAPI\f035.h"
#include "FlashAPI\flash470.h"

/*- Device specific configurations -------------------------------------------*/

/*--- TMS470MF Clock configuration Registers----------------------------------*/
// #define CSDIS                       (*(volatile UINT32 *)0xffffff30)
// #define CSDISSET                    (*(volatile UINT32 *)0xffffff34)
// #define CSDISCLR                    (*(volatile UINT32 *)0xffffff38)
// 
// #define GHVSRC                      (*(volatile UINT32 *)0xffffff48)
// #define CSVSTAT                     (*(volatile UINT32 *)0xffffff54)
// 
// #define PLLCTL1                     (*(volatile UINT32 *)0xffffff70)
// #define PLLCTL2                     (*(volatile UINT32 *)0xffffff74)
/*--- TMS470MF_BANK0_512KB configuration ----------------------------------------------*/
#ifdef TMS470MF_BANK0_512KB
 #define BANKS_NUM             2         // Number of all banks   in Flash
 #define SEC_NUM               10        // Number of all sectors in Flash
//#define PROT_KEYS_START       0xFFE0    // Start of Protect keys in Flash
//#define PROT_KEYS_SIZE        32        // Number of bytes for protection keys

 const UINT32 sec_addr[SEC_NUM] = {      // Sector start addresses
   0x000000,0x04000,0x8000,0x10000,0x20000,0x30000,0x40000,0x50000,0x60000,0x70000,
 };
 const UINT32 bank_end[BANKS_NUM] = { 0x7FFFF, 0x9FFFF};
                                       // Ending addresses of banks
 #define PROGRAM_FLASH

#endif
///*--- TMS470MF_BANK0_256KB_ECC configuration ----------------------------------------------*/
#ifdef TMS470MF_BANK0_256KB_ECC
 #define BANKS_NUM             2         // Number of all banks   in Flash
//                                        // Ending addresses of banks
const UINT32 bank_end[BANKS_NUM] = { 0x43FFFF, 0x44FFFF};
#endif

 /*--- TMS470MF_BANK0_384KB configuration ----------------------------------------------*/
#ifdef TMS470MF_BANK0_384KB
 #define BANKS_NUM             2         // Number of all banks   in Flash
 #define SEC_NUM               8         // Number of all sectors in Flash
//#define PROT_KEYS_START       0xFFE0    // Start of Protect keys in Flash
//#define PROT_KEYS_SIZE        32        // Number of bytes for protection keys

 const UINT32 sec_addr[SEC_NUM] = {      // Sector start addresses
   0x000000,0x04000,0x8000,0x10000,0x20000,0x30000,0x40000,0x50000,
 };
 const UINT32 bank_end[BANKS_NUM] = { 0x7FFFF, 0x9FFFF};
                                       // Ending addresses of banks
 #define PROGRAM_FLASH
#endif

///*--- TMS470MF_BANK0_192KB_ECC configuration ----------------------------------------------*/
#ifdef TMS470MF_BANK0_192KB_ECC
 #define BANKS_NUM             2         // Number of all banks   in Flash
//                                        // Ending addresses of banks
const UINT32 bank_end[BANKS_NUM] = { 0x43FFFF, 0x44FFFF};
#endif

 /*--- TMS470MF_BANK0_256KB configuration ----------------------------------------------*/
#ifdef TMS470MF_BANK0_256KB
 #define BANKS_NUM             2         // Number of all banks   in Flash
 #define SEC_NUM               6         // Number of all sectors in Flash
//#define PROT_KEYS_START       0xFFE0    // Start of Protect keys in Flash
//#define PROT_KEYS_SIZE        32        // Number of bytes for protection keys

 const UINT32 sec_addr[SEC_NUM] = {      // Sector start addresses
   0x000000,0x04000,0x8000,0x10000,0x20000,0x30000
 };
 const UINT32 bank_end[BANKS_NUM] = { 0x7FFFF, 0x9FFFF};
                                       // Ending addresses of banks
 #define PROGRAM_FLASH
#endif

///*--- TMS470MF_BANK0_128KB_ECC configuration ----------------------------------------------*/
#ifdef TMS470MF_BANK0_128KB_ECC
 #define BANKS_NUM             2         // Number of all banks   in Flash
//                                        // Ending addresses of banks
const UINT32 bank_end[BANKS_NUM] = { 0x43FFFF, 0x44FFFF};
#endif


/*--- TMS470MF_BANK1_128KB configuration ----------------------------------------------*/
#ifdef TMS470MF_BANK1_128KB
 #define BANKS_NUM             2         // Number of all banks   in Flash
 #define SEC_NUM               5         // Number of all sectors in Flash
//#define PROT_KEYS_START       0xFFE0    // Start of Protect keys in Flash
//#define PROT_KEYS_SIZE        32        // Number of bytes for protection keys

 const UINT32 sec_addr[SEC_NUM] = {      // Sector start addresses
   0x80000,0x84000,0x88000,0x8C000,0x90000
 };
 const UINT32 bank_end[BANKS_NUM] = { 0x7FFFF, 0x9FFFF};
                                       // Ending addresses of banks
 #define PROGRAM_FLASH
#endif
///*--- TMS470MF_BANK1_64KB_ECC configuration ----------------------------------------------*/
#ifdef TMS470MF_BANK1_64KB_ECC
 #define BANKS_NUM             2         // Number of all banks   in Flash
//                                        // Ending addresses of banks
const UINT32 bank_end[BANKS_NUM] = { 0x43FFFF, 0x44FFFF};
#endif

/*--- TMS470MF_BANK1_64KB configuration ----------------------------------------------*/
#ifdef TMS470MF_BANK1_64KB
 #define BANKS_NUM             2         // Number of all banks   in Flash
 #define SEC_NUM               4         // Number of all sectors in Flash
//#define PROT_KEYS_START       0xFFE0    // Start of Protect keys in Flash
//#define PROT_KEYS_SIZE        32        // Number of bytes for protection keys

 const UINT32 sec_addr[SEC_NUM] = {      // Sector start addresses
   0x80000,0x84000,0x88000,0x8C000
 };
 const UINT32 bank_end[BANKS_NUM] = { 0x7FFFF, 0x9FFFF};
                                       // Ending addresses of banks
 #define PROGRAM_FLASH
#endif
///*--- TMS470MF_BANK1_32KB_ECC configuration ----------------------------------------------*/
#ifdef TMS470MF_BANK1_32KB_ECC
 #define BANKS_NUM             2         // Number of all banks   in Flash
//                                        // Ending addresses of banks
const UINT32 bank_end[BANKS_NUM] = { 0x43FFFF, 0x44FFFF};
#endif

/*----------------------------------------------------------------------------*/


// Flash status structure
FLASH_STATUS_ST flash_status;

// Flash Delay parameter
UINT32 Delay;


/** Auxiliary functions *******************************************************/

/*- GetBank (...) ------------------------------------------
 *
 *  Get Bank number in which the requested address is
 *    Parameter:      adr:  Data Address
 *    Return Value:   0..7 - Bank Index, 0xFF - Unexisting Bank
 */

int GetBank (unsigned long adr)  {
  int i;

  // Find out in which bank is given address
  for (i = 0; i < BANKS_NUM; i++) {
    if (adr < bank_end[i]) return (i);
  }

  return (0xFF);
}


/** Exported functions ********************************************************/

/*- Init (...) -----------------------------------------------------------------
 *
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {
#ifdef PROGRAM_FLASH
  int i;
#endif
  Delay = (537*(clk >> 10)) >> 20;   
  
  switch (fnc)  {
    case 1:                             // Erase   Init
//      CSDIS    = 0x0c; /* set clock sources 2,3,6,7 to disabled on wakeup */
//      GHVSRC   = 0;    /* select clock source 0 (OSCIN) */
//      CSDISSET = 0x2;  /* set clock source 1 disable */
//      
//      /* add some wait states */
//      __nop(); 
//      __nop();
//      PLLCTL1               =  0x00000000U 
//                            |  0x20000000U 
//                            | (1U - 1U << 24U) 
//                            |  0x00000000U 
//                            | (4U - 1U << 16U) 
//                            | (100U  - 1U << 8U);
//      PLLCTL2               = 0x00000000U
//                            | (255U << 22U)
//                            | (0U << 12U)
//                            | (5U - 1U << 9U)
//                            |  61U;
//      /* add some wait states */
//      __nop();
//      __nop();
//      __nop();
//      __nop();
//      
//      CSDISCLR = 0x2;               /* switch PLL on -> GCLK is still OSCIN */
//      while ((CSVSTAT & 0x2)==0) 
//      {
//      } /* wait for clock valid */
//      GHVSRC = 1;                   /* enable PLL */
//      /* add some wait states */
//      __nop();
//      __nop();

#ifdef PROGRAM_FLASH
      for (i = 0; i < SEC_NUM; i++) {
        if (Flash_Compact_B ( (UINT32 *) sec_addr[i],
                              (FLASH_CORE) GetBank(sec_addr[i]),
                              (FLASH_SECT) 0,
                              Delay,
                              (FLASH_ARRAY_ST) 0xFFF87000,
                              (FLASH_STATUS_ST *) &flash_status) == 0 )
          return (1);                   // Compaction Failed
      }
      break;
#else
      return(0);
#endif
    case 2:                             // Program Init
      break;
    case 3:                             // Verify  Init
      break;
  }

  return (0);
}


/*- UnInit (...) ---------------------------------------------------------------
 *
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {
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

int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat) {
#ifdef PROGRAM_FLASH
  if (Flash_Blank_B ( (UINT32 *) adr,
                      sz>>2,            // Size in 32-bit words
                      (FLASH_CORE) GetBank(adr),
                      (FLASH_ARRAY_ST) 0xFFF87000,
                      (FLASH_STATUS_ST *) &flash_status) == 0 )
  {
    flash_status.stat1 = 0x00000000;
    return(1);                           // Always Force Erase
  }
  else {
    flash_status.stat1 = 0x00000000;
    return(0);
  }
#else
  return(0);
#endif
}


/*- EraseSector (...) ----------------------------------------------------------
 *
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {
#ifdef PROGRAM_FLASH

  if (Flash_Erase_B ( (UINT32 *) adr,
                      0,
                      (FLASH_CORE) GetBank(adr),
                      (FLASH_SECT) 0,
                      Delay,
                      (FLASH_ARRAY_ST) 0xFFF87000,
                      (FLASH_STATUS_ST *) &flash_status) == 0 )
    return (1);                         // Sector Erase Failed
#endif
  return (0);                           // Done successfully
}


/*- ProgramPage (...) ----------------------------------------------------------
 *
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size (in bytes)
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {

  // Disable writing of the protection keys
//#ifndef WRITE_PROT_KEYS
//  if ((adr < (PROT_KEYS_START+PROT_KEYS_SIZE) && ((adr+sz) > PROT_KEYS_START)))
//    sz -= (adr+sz-PROT_KEYS_START);
//
//  if (sz == 0) return (1);              // Writing of keys attempted
//#endif

  if (Flash_Prog_B  ( (UINT32 *) adr,
                      (UINT32 *) buf,
                      sz>>2,            // Size in 32-bit words
                      (FLASH_CORE) GetBank(adr),
                      Delay,
                      (FLASH_ARRAY_ST) 0xFFF87000,
                      (FLASH_STATUS_ST *) &flash_status) == 0 )
    return (1);                         // Programming Page Failed

  return (0);                           // Done successfully
}


/*- Verify (...) ---------------------------------------------------------------
 *
 *  Verify Flash Contents
 *    Parameter:      adr:  Start Address
 *                    sz:   Size (in bytes)
 *                    buf:  Data
 *    Return Value:   (adr+sz) - OK, Failed Address
 */

unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf) {

  if (Flash_Verify_B ((UINT32 *) adr,
                      (UINT32 *) buf,
                      sz>>2,            // Size in 32-bit words
                      (FLASH_CORE) GetBank(adr),
                      (FLASH_ARRAY_ST) 0xFFF87000,
                      (FLASH_STATUS_ST *) &flash_status) == 0 )
    return (flash_status.stat1);        // Verification Failed (return address)

  return (adr+sz);                      // Done successfully
}
