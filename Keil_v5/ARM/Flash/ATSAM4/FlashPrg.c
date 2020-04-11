/* -----------------------------------------------------------------------------
 * Copyright (c) 2014 - 2015 ARM Ltd.
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
 * $Date:        19. May 2015
 * $Revision:    V1.02
 *  
 * Project:      Flash Programming Functions for ATSAM4
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.01
 *    Updated GPNVM bit handling
 *    On SAM4C32C device the set GPNVM bit failed for the 1. command.
 *    The clear bit always worked for 1. command.
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"                          /* FlashOS Structures */

typedef volatile unsigned long    vu32;
typedef          unsigned long     u32;
typedef          unsigned char     u8;

/* Power Management Controller register */
typedef struct {
  vu32 PMC_SCER;
  vu32 PMC_SCDR;
  vu32 PMC_SCSR;
  vu32 Reserved1[1];
  vu32 PMC_PCER;
  vu32 PMC_PCDR;
  vu32 PMC_PCSR;
  vu32 Reserved2[1];
  vu32 CKGR_MOR;
  vu32 CKGR_MCFR;
  vu32 CKGR_PLLAR;
  vu32 CKGR_PLLBR;
  vu32 PMC_MCKR;
  vu32 Reserved3[1];
  vu32 PMC_USB;
  vu32 Reserved4[1];
  vu32 PMC_PCK[3];
  vu32 Reserved5[5];
  vu32 PMC_IER;
  vu32 PMC_IDR;
  vu32 PMC_SR;
  vu32 PMC_IMR;
  vu32 PMC_FSMR;
  vu32 PMC_FSPR;
  vu32 PMC_FOCR;
  vu32 Reserved6[26];
  vu32 PMC_WPMR;
  vu32 PMC_WPSR;
} PMC_TypeDef;

/* PMC clock generator main oscillatoer register definitions */
#define PMC_CKGR_MOSCXTEN     (0x01   <<  0)     /* MOSCXTEN: Main Crystal Oscillator Enable             */
#define PMC_CKGR_MOSCRCEN     (0x01   <<  3)     /* MOSCRCEN: Main On-Chip RC Oscillator Enable          */
#define PMC_CKGR_MOSCRCF      (0x07   <<  4)     /* MOSCRCF:  Main On-Chip RC Oscillator Freq. Selection */
#define PMC_CKGR_MOSCRCF_12   (0x02   <<  4)     /* MOSCRCF:  The Fast RC Oscillator Freq. is at 12 MHz  */
#define PMC_CKGR_MOSCXTST     (0x0F   <<  8)     /* MOSCXTST: Main Crystal Oscillator Start-up Time (15) */
#define PMC_CKGR_CKGR_KEY     (0x37   << 16)     /* KEY: Password                                        */
#define PMC_CKGR_MOSCSEL      (0x01   << 24)     /* MOSCSEL: Main Oscillator Selection                   */

/* PMC master clock register definitions */
#define PMC_MCKR_CSS          (0x03   <<  0)     /* CSS: Master Clock Source Selection */
#define PMC_MCKR_CSS_MAIN_CLK (0x01   <<  0)     /* Main Clock is selected             */
#define PMC_MCKR_PRES         (0x07   <<  4)     /* PRES: Processor Clock Prescaler    */
#define PMC_MCKR_PRES_1       (0x01   <<  4)     /* PRES: Processor Clock Prescaler    */

/* PMC status register definitions */
#define PMC_SR_MOSCXTS        (0x01   <<  0)     /* MOSCXTS:  Main XTAL Oscillator Status       */
#define PMC_SR_MCKRDY         (0x01   <<  3)     /* MCKRDY:   Master Clock  Status              */
#define PMC_SR_MOSCSELS       (0x01   << 16)     /* MOSCSELS: Main Oscillator Selection Status  */
#define PMC_SR_MOSCRCS        (0x01   << 17)     /* MOSCRCS:  Main On-Chip RC Oscillator Status */

/* PMC write protection register definitions */
#define PMC_WPMR_WPEN         (0x01     <<  0)   /* WPEN: Write Protect Enable */
#define PMC_WPMR_WPKEY        (0x504D43 <<  8)   /* WPKEY: Write Protect KEY   */

/* Independent WATCHDOG register */
typedef struct {
  vu32 CR;
  vu32 MR;
  vu32 SR;
} WDT_TypeDef;

/* WDT mode register definitions */
#define WDT_MR_WDDIS          (0x01   << 15)     /* WDDIS: Watchdog Disable */

/* Flash Registers register */
typedef struct {
  vu32 FMR;
  vu32 FCR;
  vu32 FSR;
  vu32 FRR;
} EEFC_TypeDef;

/* EEFC command register definitions */
#define EEFC_FCR_FKEY         (0x5A   << 24)     /* Flash Writing Protection Key */
#define EEFC_FCR_FARG         (0xFFFF <<  8)     /* Flash Command Argument bits  */
#define EEFC_FCR_FCMD         (0xFF   <<  0)     /* Flash Command bits           */

/* EEFC command definitions */
#define EEFC_FCR_FCMD_GETD     0x00              /* Get Flash Descriptor                */
#define EEFC_FCR_FCMD_WP       0x01              /* Write Page                          */
#define EEFC_FCR_FCMD_WPL      0x02              /* Write Page and Lock                 */
#define EEFC_FCR_FCMD_EWP      0x03              /* Erase Page and Write Page           */
#define EEFC_FCR_FCMD_EWPL     0x04              /* Erase Page and Write Page then Lock */
#define EEFC_FCR_FCMD_EA       0x05              /* Erase All                           */
#define EEFC_FCR_FCMD_EPL      0x06              /* Erase Plane                         */
#define EEFC_FCR_FCMD_EPA      0x07              /* Erase Pages                         */
#define EEFC_FCR_FCMD_SLB      0x08              /* Set Lock Bit                        */
#define EEFC_FCR_FCMD_CLB      0x09              /* Clear Lock Bit                      */
#define EEFC_FCR_FCMD_GLB      0x0A              /* Get Lock Bit                        */
#define EEFC_FCR_FCMD_SGPB     0x0B              /* Set GPNVM Bit                       */
#define EEFC_FCR_FCMD_CGPB     0x0C              /* Clear GPNVM Bit                     */
#define EEFC_FCR_FCMD_GGPB     0x0D              /* Get GPNVM Bit                       */
#define EEFC_FCR_FCMD_STUI     0x0E              /* Start Read Unique Identifier        */
#define EEFC_FCR_FCMD_SPUI     0x0F              /* Stop Read Unique Identifier         */

/* EEFC status register definitions */
#define EEFC_FSR_FRDY         (0x01   <<  0)     /* Flash Ready Status         */
#define EEFC_FSR_FCMDE        (0x01   <<  1)     /* Flash Command Error Status */
#define EEFC_FSR_FLOCKE       (0x01   <<  2)     /* Flash Lock Error Status    */
#define EEFC_FSR_FLERR        (0x01   <<  3)     /* Flash Error Status         */

/* EEFC mode register */
#define EEFC_FMR_FAM          (0x01   << 24)     /* FAM: Flash Access mode     */
#define EEFC_FMR_FWS          (0x0F   <<  8)     /* FWS: Flash Wait State      */

/* GPNVM bits */
#define GPNVM_BIT0             0x00              /* GPNVM bit0: Security bit */
#define GPNVM_BIT1             0x01              /* GPNVM bit1: Boot mode selection  0 = Boot from ROM,    1 = Boot from Flash  */
#define GPNVM_BIT2             0x02              /* GPNVM bit2: Flash selection      0 = Boot from Flash0, 1 = Boot from Flash1 */


/* Peripheral Memory Map */
/* ---- SAM4C --------------------------------------------------------------------*/
#if defined ATSAM4C_256 || defined ATSAM4C_512 || defined ATSAM4C_1024
  #define PMC_BASE             0x400E0400        /* SAM4C                      */
  #define WDT_BASE             0x400E1450        /* SAM4C                      */
  #define EEFC0_BASE           0x400E0A00        /* SAM4C                      */
#endif

/* ---- SAM4C32, SAM4CM32 (same as SAM4SD) ---------------------------------------*/
#if defined ATSAM4C32_2048
  #define PMC_BASE             0x400E0400        /* SAM4C                      */
  #define WDT_BASE             0x400E1450        /* SAM4C                      */
  #define EEFC0_BASE           0x400E0A00        /* SAM4C                      */
  #define EEFC1_BASE           0x400E0C00        /* SAM4C32, SAM4CM32          */

  #if defined ATSAM4C32_2048
    #define FLASH_SIZE         0x00200000        /* 2048 kB  (2 * 1024 kB) */
  #endif
#endif

/* ---- SAM4E --------------------------------------------------------------------*/
#if defined ATSAM4E_512 || defined ATSAM4E_1024
  #define PMC_BASE             0x400E0400        /* SAM4E                      */
  #define WDT_BASE             0x400E1850        /* SAM4E                      */
  #define EEFC0_BASE           0x400E0A00        /* SAM4E                      */
#endif

/* ---- SAM4N - (also used for SAMG51/SAMG53)-------------------------------------*/
#if defined ATSAM4N_256 || defined ATSAM4N_512 || defined ATSAM4N_1024
  #define PMC_BASE             0x400E0400        /* SAM4N                      */
  #define WDT_BASE             0x400E1450        /* SAM4N                      */
  #define EEFC0_BASE           0x400E0A00        /* SAM4N                      */
#endif

/* ---- SAM4S --------------------------------------------------------------------*/
#if defined ATSAM4S_128 || defined ATSAM4S_256 || defined ATSAM4S_512 || defined ATSAM4S_1024
  #define PMC_BASE             0x400E0400        /* SAM4S                      */
  #define WDT_BASE             0x400E1450        /* SAM4S                      */
  #define EEFC0_BASE           0x400E0A00        /* SAM4S                      */
#endif

/* ---- SAM4SD -------------------------------------------------------------------*/
#if defined ATSAM4SD_1024 || defined ATSAM4SD_2048
  #define PMC_BASE             0x400E0400        /* SAM4S                      */
  #define WDT_BASE             0x400E1450        /* SAM4S                      */
  #define EEFC0_BASE           0x400E0A00        /* SAM4S                      */
  #define EEFC1_BASE           0x400E0C00        /* SAM4SD                     */

  #if defined ATSAM4SD_1024
    #define FLASH_SIZE         0x00100000        /* 1024 kB  (2 *  512 kB) */
  #endif
  #if defined ATSAM4SD_2048
    #define FLASH_SIZE         0x00200000        /* 2048 kB  (2 * 1024 kB) */
  #endif
#endif

/* ---- SAM4SP -------------------------------------------------------------------*/
#if defined ATSAM4SP_2048
  #define PMC_BASE             0x400E0400        /* SAM4P                      */
  #define WDT_BASE             0x400E1450        /* SAM4P                      */
  #define EEFC0_BASE           0x400E0A00        /* SAM4P                      */
  #define EEFC1_BASE           0x400E0C00        /* SAM4SP                     */

  #if defined ATSAM4SP_2048
    #define FLASH_SIZE         0x00200000        /* 2048 kB  (2 * 1024 kB) */
  #endif
#endif


#define PMC            (( PMC_TypeDef *)   PMC_BASE)
#define WDT            (( WDT_TypeDef *)   WDT_BASE)
#define EEFC0          ((EEFC_TypeDef *) EEFC0_BASE)
#define EEFC1          ((EEFC_TypeDef *) EEFC1_BASE)

#define FLASH_PAGE_SIZE        512               /* Page size is 512 Byte for all devices */



unsigned long base_adr;        /* Base Address  */


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  /* store Flash Start address */
  base_adr = adr;

  /* set Flash Waite State to maximum */
  EEFC0->FMR = EEFC_FMR_FWS;
#if defined EEFC1_BASE
  EEFC1->FMR = EEFC_FMR_FWS;
#endif

  /* disable Watchdog */
  WDT->MR   = WDT_MR_WDDIS;

  /* disable PMC write protection */
  PMC->PMC_WPMR = PMC_WPMR_WPKEY;

  /* before we change the clocksetup we switch Master Clock Source
     to MAIN_CLK and set prescaler to one
   */
  PMC->PMC_MCKR = (PMC->PMC_MCKR & ~PMC_MCKR_CSS) | PMC_MCKR_CSS_MAIN_CLK;
  while (!(PMC->PMC_SR & PMC_SR_MCKRDY));     /* Wait for MCKRDY                   */

  PMC->PMC_MCKR = (PMC->PMC_MCKR & ~PMC_MCKR_PRES);
  while (!(PMC->PMC_SR & PMC_SR_MCKRDY));     /* Wait for MCKRDY                   */

  /* we use the internal fast oscillator.
     in the long term all the algorithms should be changed that the internal
     oscillator instead the external oscillator is used for all SAM4 families */

  PMC->CKGR_MOR = (PMC->CKGR_MOR                   ) | PMC_CKGR_CKGR_KEY | PMC_CKGR_MOSCRCEN;
  while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));    /* Wait for MOSCRCS                   */

  PMC->CKGR_MOR = (PMC->CKGR_MOR & ~PMC_CKGR_MOSCRCF) | PMC_CKGR_CKGR_KEY | PMC_CKGR_MOSCRCF_12 ;

  PMC->CKGR_MOR = (PMC->CKGR_MOR & ~PMC_CKGR_MOSCSEL) | PMC_CKGR_CKGR_KEY;
  while (!(PMC->PMC_SR & PMC_SR_MOSCSELS));   /* Wait for MOSCSELS                  */

#if 0
  /* Enable the Main Oscillator:
     SCK Period = 1/32768 = 30.51 us
     Start-up Time = 8 * 15 / SCK = 120 * 30.51us = 3.6612 ms */
  PMC->CKGR_MOR = (PMC_CKGR_MOSCSEL | PMC_CKGR_CKGR_KEY | PMC_CKGR_MOSCXTST | PMC_CKGR_MOSCXTEN);

  /* Wait the Start-up Time */
  while(!(PMC->PMC_SR & PMC_SR_MOSCXTS));
#endif

  /* Select Main Clock (CSS field) */
  PMC->PMC_MCKR = (PMC->PMC_MCKR & ~PMC_MCKR_CSS) | PMC_MCKR_CSS_MAIN_CLK;

  /* Wait for Clock ready */
  while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

  /* Select Main Clock */
  PMC->PMC_MCKR = PMC_MCKR_CSS_MAIN_CLK;

  /* Wait for Clock ready */
  while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

  /* enable PMC write protection */
  PMC->PMC_WPMR = PMC_WPMR_WPKEY | PMC_WPMR_WPEN;

  return (0);
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {
#ifdef FLASH_MEM
  u32           frr;
  u32           fcr;

  /* after programming Flash  set GPNVMBit to boot from Flash */
  if (fnc == 2) {

    /* read current GPNVM bits */
    EEFC0->FCR = EEFC_FCR_FKEY | EEFC_FCR_FCMD_GGPB;
    while (!(EEFC0->FSR & EEFC_FSR_FRDY));
    frr = EEFC0->FRR;

    /* configure GPNVM bit #1 :  1 = Boot from Flash  */
    if ((frr & (1 << GPNVM_BIT1)) == 0) {
      fcr = EEFC_FCR_FKEY | (GPNVM_BIT1 << 8) | EEFC_FCR_FCMD_SGPB;

      EEFC0->FCR = fcr;                           /* Write the Command */
      while (!(EEFC0->FSR & EEFC_FSR_FRDY));      /* Wait until the end of Command */
      if ((EEFC0->FSR & EEFC_FSR_FLERR) == EEFC_FSR_FLERR) {
        EEFC0->FCR = fcr;                         /* reWrite the Command */
        while (!(EEFC0->FSR & EEFC_FSR_FRDY));    /* Wait until the end of Command */
      }
    }

//#if defined FLASH_BANK_SELECTION
    /* configure GPNVM bit #2 : Flash selection      0 = Boot from Flash 0 */
    if ((frr & (1 << GPNVM_BIT2)) != 0) {
      fcr = EEFC_FCR_FKEY | (GPNVM_BIT2 << 8) | EEFC_FCR_FCMD_CGPB;

      EEFC0->FCR = fcr;                           /* Write the Command */
      while (!(EEFC0->FSR & EEFC_FSR_FRDY));      /* Wait until the end of Command */
      if ((EEFC0->FSR & EEFC_FSR_FLERR) == EEFC_FSR_FLERR) {
        EEFC0->FCR = fcr;                         /* reWrite the Command */
        while (!(EEFC0->FSR & EEFC_FSR_FRDY));    /* Wait until the end of Command */
      }
    }
//#endif

  }
#endif

  return (0);
}


/*
 *  Blank Check Checks if Memory is Blank
 *    Parameter:      adr:  Block Start Address
 *                    sz:   Block Size (in bytes)
 *                    pat:  Block Pattern
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_GPNVM
int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat) {

  /* for GPNVM bits blankCheck is always ok */
  return (0);
}
#endif


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {
#ifdef FLASH_MEM

  /* erase all command
     memory covered by the Flash Programming Algorithm is erased */

  EEFC0->FCR  = EEFC_FCR_FKEY | EEFC_FCR_FCMD_EA;
  while (!(EEFC0->FSR  & EEFC_FSR_FRDY));

#if defined ATSAM4SD_1024 || defined ATSAM4SD_2048 || defined ATSAM4SP_2048 || defined ATSAM4C32_2048
  EEFC1->FCR  = EEFC_FCR_FKEY | EEFC_FCR_FCMD_EA;
  while (!(EEFC1->FSR  & EEFC_FSR_FRDY));
#endif

#endif

  return (0);
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {
  u32           page;
  EEFC_TypeDef *EEFCx;

  /* Note:
           For SAM3S16, SAM4S Erase and write Command (EWP) does not work on all Flash pages.
           Only for pages in the first 2 Small Sector, 16K Bytes.
           We select a "sector size" of 8K which works for Unlock and Erase Page (16 pages)
   */

  /* select EEFC, calculate page */
#if defined ATSAM4S_128 || defined ATSAM4S_256  || defined ATSAM4S_512 || defined ATSAM4S_1024 || \
    defined ATSAM4E_512 || defined ATSAM4E_1024 || \
    defined ATSAM4C_256 || defined ATSAM4C_512  || defined ATSAM4C_1024 || \
    defined ATSAM4N_256 || defined ATSAM4N_512  || defined ATSAM4N_1024
    EEFCx =  EEFC0;
    page  = (adr - (base_adr                   )) / FLASH_PAGE_SIZE;
#endif

#if defined ATSAM4SD_1024 || defined ATSAM4SD_2048 || defined ATSAM4SP_2048 || defined ATSAM4C32_2048
  if (adr < (base_adr + (FLASH_SIZE / 2))) {
    EEFCx =  EEFC0;
    page  = (adr - (base_adr                   )) / FLASH_PAGE_SIZE;
  }
  else {
    EEFCx =  EEFC1;
    page  = (adr - (base_adr + (FLASH_SIZE / 2))) / FLASH_PAGE_SIZE;
  }
#endif

  /* FARG[1:0] Number of pages to be erased with EPA command */
  page = ((page & ~3UL) | 2);

  /* unlock page command */
  EEFCx->FCR = EEFC_FCR_FKEY | EEFC_FCR_FCMD_CLB | (EEFC_FCR_FARG & (page << 8));
  while (!(EEFCx->FSR & EEFC_FSR_FRDY));

  /* Erase page command (Erase page on 16 pages) */
  EEFCx->FCR = EEFC_FCR_FKEY | EEFC_FCR_FCMD_EPA | (EEFC_FCR_FARG & (page << 8));
  while (!(EEFCx->FSR & EEFC_FSR_FRDY));

  /* check for errors */
  if (EEFCx->FSR & (EEFC_FSR_FLERR |EEFC_FSR_FCMDE | EEFC_FSR_FLOCKE))
    return (1);

  return (0);
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */
#ifdef FLASH_MEM
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  u32           page;
  u32          *Flash;
  EEFC_TypeDef *EEFCx;

  /* select EEFC, calculate page */
#if defined ATSAM4S_128 || defined ATSAM4S_256  || defined ATSAM4S_512 || defined ATSAM4S_1024 || \
    defined ATSAM4E_512 || defined ATSAM4E_1024 || \
    defined ATSAM4C_256 || defined ATSAM4C_512  || defined ATSAM4C_1024 || \
    defined ATSAM4N_256 || defined ATSAM4N_512  || defined ATSAM4N_1024
    EEFCx =  EEFC0;
    page  = (adr - (base_adr                   )) / FLASH_PAGE_SIZE;
#endif

#if defined ATSAM4SD_1024 || defined ATSAM4SD_2048 || defined ATSAM4SP_2048 || defined ATSAM4C32_2048
  if (adr < (base_adr + (FLASH_SIZE / 2))) {
    EEFCx =  EEFC0;
    page  = (adr - (base_adr                   )) / FLASH_PAGE_SIZE;
  }
  else {
    EEFCx =  EEFC1;
    page  = (adr - (base_adr + (FLASH_SIZE / 2))) / FLASH_PAGE_SIZE;
  }
#endif

  /* set write pointer to Flash address */
  Flash = (unsigned long *)adr;

  /* copy data to write buffer */
  for (sz = (sz + 3) & ~3; sz; sz -= 4, buf += 4) {
    *Flash++ = *((unsigned long *)buf);
  }

  /* start programming command */
  EEFCx->FCR = EEFC_FCR_FKEY | EEFC_FCR_FCMD_WP | (EEFC_FCR_FARG & (page << 8));
  while (!(EEFCx->FSR & EEFC_FSR_FRDY));

  /* check for errors */
  if (EEFCx->FSR & (EEFC_FSR_FLERR |EEFC_FSR_FCMDE | EEFC_FSR_FLOCKE))
    return (1);

  return (0);
}
#endif

#ifdef FLASH_GPNVM
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  u32           frr;
  u32           fcr;
  u8            gpnvm;

  gpnvm = *buf;

  /* read current GPNVM bits */
  EEFC0->FCR = EEFC_FCR_FKEY | EEFC_FCR_FCMD_GGPB;
  while (!(EEFC0->FSR & EEFC_FSR_FRDY));
  frr = EEFC0->FRR;

  /* configure GPNVM bit #0 : Security bit */
  if ((gpnvm & (1 << GPNVM_BIT0)) != (frr & (1 << GPNVM_BIT0))) {
    if (gpnvm & (1 << GPNVM_BIT0))
      fcr = EEFC_FCR_FKEY | (GPNVM_BIT0 << 8) | EEFC_FCR_FCMD_SGPB;
    else
      fcr = EEFC_FCR_FKEY | (GPNVM_BIT0 << 8) | EEFC_FCR_FCMD_CGPB;

    EEFC0->FCR = fcr;                           /* Write the Command */
    while (!(EEFC0->FSR & EEFC_FSR_FRDY));      /* Wait until the end of Command */
    if ((EEFC0->FSR & EEFC_FSR_FLERR) == EEFC_FSR_FLERR) {
      EEFC0->FCR = fcr;                         /* Write the Command */
      while (!(EEFC0->FSR & EEFC_FSR_FRDY));    /* Wait until the end of Command */
    }
  }

  /* configure GPNVM bit #1 : Boot mode selection  0 = Boot from ROM,    1 = Boot from Flash  */
  if ((gpnvm & (1 << GPNVM_BIT1)) != (frr & (1 << GPNVM_BIT1))) {
    if (gpnvm & (1 << GPNVM_BIT1))
      fcr = EEFC_FCR_FKEY | (GPNVM_BIT1 << 8) | EEFC_FCR_FCMD_SGPB;
    else
      fcr = EEFC_FCR_FKEY | (GPNVM_BIT1 << 8) | EEFC_FCR_FCMD_CGPB;

    EEFC0->FCR = fcr;                           /* Write the Command */
    while (!(EEFC0->FSR & EEFC_FSR_FRDY));      /* Wait until the end of Command */
    if ((EEFC0->FSR & EEFC_FSR_FLERR) == EEFC_FSR_FLERR) {
      EEFC0->FCR = fcr;                         /* Write the Command */
      while (!(EEFC0->FSR & EEFC_FSR_FRDY));    /* Wait until the end of Command */
    }
  }

  /* configure GPNVM bit #2 : Flash selection      0 = Boot from Flash0, 1 = Boot from Flash1 */
  if ((gpnvm & (1 << GPNVM_BIT2)) != (frr & (1 << GPNVM_BIT2))) {
    if (gpnvm & (1 << GPNVM_BIT2))
      fcr = EEFC_FCR_FKEY | (GPNVM_BIT2 << 8) | EEFC_FCR_FCMD_SGPB;
    else
      fcr = EEFC_FCR_FKEY | (GPNVM_BIT2 << 8) | EEFC_FCR_FCMD_CGPB;

    
    EEFC0->FCR = fcr;                           /* Write the Command */
    while (!(EEFC0->FSR & EEFC_FSR_FRDY));      /* Wait until the end of Command */
    if ((EEFC0->FSR & EEFC_FSR_FLERR) == EEFC_FSR_FLERR) {
      EEFC0->FCR = fcr;                         /* Write the Command */
      while (!(EEFC0->FSR & EEFC_FSR_FRDY));    /* Wait until the end of Command */
    }
}

  return (0);
}
#endif

/*
 *  Verify Flash Contents
 *    Parameter:      adr:  Start Address
 *                    sz:   Size (in bytes)
 *                    buf:  Data
 *    Return Value:   (adr+sz) - OK, Failed Address
 */

#ifdef FLASH_GPNVM
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf) {
  u32           frr;
  u8            gpnvm;

  gpnvm = *buf;

  /* read current GPNVM bits */
  EEFC0->FCR = EEFC_FCR_FKEY | EEFC_FCR_FCMD_GGPB;
  while (!(EEFC0->FSR & EEFC_FSR_FRDY));
  frr = EEFC0->FRR;

  /* check GPNVM bit #0 : Security bit */
  if ((gpnvm & (1 << GPNVM_BIT0)) != (frr & (1 << GPNVM_BIT0))) {
    return (adr + GPNVM_BIT0);
  }

  /* check GPNVM bit #1 : Boot mode selection */
  if ((gpnvm & (1 << GPNVM_BIT1)) != (frr & (1 << GPNVM_BIT1))) {
    return (adr + GPNVM_BIT1);
  }

  /* check GPNVM bit #2 : Flash selection */
  if ((gpnvm & (1 << GPNVM_BIT2)) != (frr & (1 << GPNVM_BIT2))) {
    return (adr + GPNVM_BIT2);
}

  return (adr + sz);
}
#endif


// --- Test --- Test --- Test --- Test ---
#ifdef FLASH_TEST
void rdEFD (unsigned int *efd) {                   // read embedded Flash Descriport

  EEFC0->FCR = EEFC_FCR_FKEY | EEFC_FCR_FCMD_GETD;
  while (!(EEFC0->FSR & EEFC_FSR_FRDY));
  *efd++ = EEFC0->FRR;                             // Flash Interface Description
  *efd++ = EEFC0->FRR;                             // Flash size in bytes
  *efd++ = EEFC0->FRR;                             // Page size in bytes
  *efd++ = EEFC0->FRR;                             // Number of planes
  *efd++ = EEFC0->FRR;                             // Number of bytes in the first plane
  *efd++ = EEFC0->FRR;                             // Number of lock bits
  *efd++ = EEFC0->FRR;                             // Number of bytes in the 1st lock region
  *efd++ = EEFC0->FRR;                             // Number of bytes in the 2nd lock region
  *efd++ = EEFC0->FRR;                             // Number of bytes in the 3rd lock region
}

void rdLB (unsigned int *lb) {                     // read Lock Bits

  EEFC0->FCR = EEFC_FCR_FKEY | EEFC_FCR_FCMD_GLB;
  while (!(EEFC0->FSR & EEFC_FSR_FRDY));
  *lb++ = EEFC0->FRR;                              // Lock Bits  0..31
  *lb++ = EEFC0->FRR;                              // Lock Bits 32..63
  *lb++ = EEFC0->FRR;                              // Lock Bits 64..95
}

void rdGPNVM (u32 *gpnvm) {

  /* read current GPNVM bits */
  EEFC0->FCR = EEFC_FCR_FKEY | EEFC_FCR_FCMD_GGPB;
  while (!(EEFC0->FSR & EEFC_FSR_FRDY));
  *gpnvm = EEFC0->FRR;
}
#endif
