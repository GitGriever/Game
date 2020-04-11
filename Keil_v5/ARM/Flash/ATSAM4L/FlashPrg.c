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
 * Project:      Flash Programming Functions for ATSAM4L
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"                          /* FlashOS Structures */

typedef volatile unsigned long    vu32;
typedef          unsigned long     u32;
typedef          unsigned char     u8;

/* System Control Interface register */
typedef struct {
  vu32 IER;             /* offset: 0x000 ( /W) Interrupt Enable Register */
  vu32 IDR;             /* offset: 0x004 ( /W) Interrupt Disable Register */
  vu32 IMR;             /* offset: 0x008 (R/ ) Interrupt Mask Register */
  vu32 ISR;             /* offset: 0x00C (R/ ) Interrupt Status Register */
  vu32 ICR;             /* offset: 0x010 ( /W) Interrupt Clear Register */
  vu32 PCLKSR;          /* offset: 0x014 (R/ ) Power and Clocks Status Register */
  vu32 UNLOCK;          /* offset: 0x018 ( /W) Unlock Register */
  vu32 CSCR;            /* offset: 0x01C (R/W) Chip Specific Configuration Register */
  vu32 OSCCTRL0;        /* offset: 0x020 (R/W) Oscillator Control Register */
  vu32 PLL0;            /* offset: 0x024 (R/W) PLL0 Control Register */
  vu32 DFLL0CONF;       /* offset: 0x028 (R/W) DFLL0 Config Register */
  vu32 DFLL0VAL;        /* offset: 0x02C (R/W) DFLL Value Register */
  vu32 DFLL0MUL;        /* offset: 0x030 (R/W) DFLL0 Multiplier Register */
  vu32 DFLL0STEP;       /* offset: 0x034 (R/W) DFLL0 Step Register */
  vu32 DFLL0SSG;        /* offset: 0x038 (R/W) DFLL0 Spread Spectrum Generator Control Register */
  vu32 DFLL0RATIO;      /* offset: 0x03C (R/ ) DFLL0 Ratio Register */
  vu32 DFLL0SYNC;       /* offset: 0x040 ( /W) DFLL0 Synchronization Register */
  vu32 RCCR;            /* offset: 0x044 (R/W) System RC Oscillator Calibration Register */
  vu32 RCFASTCFG;       /* offset: 0x048 (R/W) 4/8/12 MHz RC Oscillator Configuration Register */
  vu32 RCFASTSR;        /* offset: 0x04C (R/ ) 4/8/12 MHz RC Oscillator Status Register */
  vu32 RC80MCR;         /* offset: 0x050 (R/W) 80 MHz RC Oscillator Register */
} SCIF_TypeDef;

/* SCIF System Control Interface register definitions */
#define SCIF_UNLOCK_KEY       (0xAAUL   << 24)   /* KEY: Unlock Key */
#define SCIF_RC80MCR_EN       (0x01UL   <<  0)   /* EN: Oscillator Enable */


/* Power Management register */
typedef struct {
  vu32 MCCTRL;          /* offset: 0x000 (R/W) Main Clock Control */
  vu32 CPUSEL;          /* offset: 0x004 (R/W) CPU Clock Select */
  vu32 Reserved1[1];
  vu32 PBASEL;          /* offset: 0x00C (R/W) PBA Clock Select */
  vu32 PBBSEL;          /* offset: 0x010 (R/W) PBB Clock Select */
  vu32 PBCSEL;          /* offset: 0x014 (R/W) PBC Clock Select */
  vu32 PBDSEL;          /* offset: 0x018 (R/W) PBD Clock Select */
  vu32 Reserved2[1];
  vu32 CPUMASK;         /* offset: 0x020 (R/W) CPU Mask */
  vu32 HSBMASK;         /* offset: 0x024 (R/W) HSB Mask */
  vu32 PBAMASK;         /* offset: 0x028 (R/W) PBA Mask */
  vu32 PBBMASK;         /* offset: 0x02C (R/W) PBB Mask */
  vu32 PBCMASK;         /* offset: 0x030 (R/W) PBC Mask */
  vu32 PBDMASK;         /* offset: 0x034 (R/W) PBD Mask */
  vu32 Reserved3[2];
  vu32 PBADIVMASK;      /* offset: 0x040 (R/W) PBA Divided Mask */
  vu32 Reserved4[4];
  vu32 CFDCTRL;         /* offset: 0x054 (R/W) Clock Failure Detector Control */
  vu32 UNLOCK;          /* offset: 0x058 ( /W) Unlock Register */
  vu32 Reserved5[25];
  vu32 IER;             /* offset: 0x0C0 ( /W) Interrupt Enable Register */
  vu32 IDR;             /* offset: 0x0C4 ( /W) Interrupt Disable Register */
  vu32 IMR;             /* offset: 0x0C8 (R/ ) Interrupt Mask Register */
  vu32 ISR;             /* offset: 0x0CC (R/ ) Interrupt Status Register */
  vu32 ICR;             /* offset: 0x0D0 ( /W) Interrupt Clear Register */
  vu32 SR;              /* offset: 0x0D4 (R/ ) Status Register */
} PM_TypeDef;


/* PM Power Manager register definitions */
#define PM_UNLOCK_KEY         (0xAAUL   << 24)   /* KEY: Unlock Key */
#define PM_CPUSEL_CPUDIV      (0x01UL   << 07)   /* CPUDIV: use devider; main clock divided by 2 pow (CPUSEL+1) */
#define PM_SR_CKRDY           (0x01UL   << 05)   /* CKRDY: clocks as indicated in the CKSEL register */
#define PM_MCCTRL_MCSEL_RC80M (0x04UL   <<  0)   /* MCSEL: Main Clock Select (80MHz RC oscillator) */

/* Watchdog Timer register */
typedef struct {
  vu32 CTRL;
  vu32 CLR;
  vu32 SR;
} WDT_TypeDef;

/* WDT Watchdog Timer register definitions */
#define WDT_CTRL_EN          (0x01   <<  0)     /* EN: Watchdog Enable/Disable */

/* Flash Registers register */
typedef struct {
  vu32 FCR;             /* offset: 0x000 (R/W) Flash Control Register */
  vu32 FCMD;            /* offset: 0x004 (R/W) Flash Command Register */
  vu32 FSR;             /* offset: 0x008 (R/W) Flash Status Register */
  vu32 FPR;             /* offset: 0x00C (R/W) Flash Parameter Register */
  vu32 FVR;             /* offset: 0x010 (R/W) Flash Version Register */
  vu32 FGPFRHI;         /* offset: 0x014 (R/W) Flash General Purpose Fuse Register Hi */
  vu32 FGPFRLO;         /* offset: 0x018 (R/W) Flash General Purpose Fuse Register Lo */
} FLASHCALW_TypeDef;

/* FLASHCALW command register definitions */
#define FLASHCALW_FCR_FKEY         (0xA5UL   << 24)   /* Flash Write protection key */
#define FLASHCALW_FCR_PAGEN        (0xFFFFUL <<  8)   /* Flash Command Page number*/
#define FLASHCALW_FCR_FCMD         (0xFFUL   <<  0)   /* Flash Command bits */

/* FLASHCALW command definitions */
#define FLASHCALW_FCR_FCMD_NOP      0x00              /* No operation */
#define FLASHCALW_FCR_FCMD_WP       0x01              /* Write Page */
#define FLASHCALW_FCR_FCMD_EP       0x02              /* Erase Page */
#define FLASHCALW_FCR_FCMD_CPB      0x03              /* Clear Page Buffer */
#define FLASHCALW_FCR_FCMD_LP       0x04              /* Lock region containing given Page */
#define FLASHCALW_FCR_FCMD_UP       0x05              /* Unlock region containing given Page */
#define FLASHCALW_FCR_FCMD_EA       0x06              /* Erase All */
#define FLASHCALW_FCR_FCMD_WGPB     0x07              /* Write General-Purpose Fuse Bit */
#define FLASHCALW_FCR_FCMD_EGPB     0x08              /* Erase General-Purpose Fuse Bit */
#define FLASHCALW_FCR_FCMD_SSB      0x09              /* Set Security Fuses */
#define FLASHCALW_FCR_FCMD_PGPFB    0x0A              /* Program GP Fuse Byte */
#define FLASHCALW_FCR_FCMD_EAGPF    0x0B              /* Erase All GPFuses */
#define FLASHCALW_FCR_FCMD_QPR      0x0C              /* Quick Page Read */
#define FLASHCALW_FCR_FCMD_WUP      0x0D              /* Write User Page */
#define FLASHCALW_FCR_FCMD_EUP      0x0E              /* Erase User Page */
#define FLASHCALW_FCR_FCMD_QPRUP    0x0F              /* Quick Page Read User Page */
#define FLASHCALW_FCR_FCMD_HSEN     0x10              /* High Speed Mode Enable */
#define FLASHCALW_FCR_FCMD_HSDIS    0x11              /* High Speed Mode Disable */

/* FLASHCALW status register definitions */
#define FLASHCALW_FSR_FRDY         (0x01UL   <<  0)   /* Flash Ready Status */
#define FLASHCALW_FSR_LOCKE        (0x01UL   <<  2)   /* Lock Error Status */
#define FLASHCALW_FSR_PROGE        (0x01UL   <<  3)   /* Programming Error Status */

/* FLASHCALW mode register */
#define FLASHCALW_FMR_FAM          (0x01UL   << 24)   /* FAM: Flash Access mode */
#define FLASHCALW_FCR_FWS          (0x01UL   <<  6)   /* FWS: Flash Wait State */


/* Peripheral Memory Map */
/* ---- SAM4L --------------------------------------------------------------------*/
#if defined ATSAM4L_512 || ATSAM4L_256 || defined ATSAM4L_128
  #define FLASHCALW_BASE       0x400A0000        /* SAM4L                      */
  #define PM_BASE              0x400E0000        /* SAM4L                      */
  #define SCIF_BASE            0x400E0800        /* SAM4L                      */
  #define WDT_BASE             0x400F0C00        /* SAM4L                      */
#endif


#define PM             ((       PM_TypeDef *)   PM_BASE)
#define SCIF           ((     SCIF_TypeDef *)   SCIF_BASE)
#define WDT            ((      WDT_TypeDef *)   WDT_BASE)
#define FLASHCALW      ((FLASHCALW_TypeDef *)   FLASHCALW_BASE)

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

  /* disable Watchdog */
  WDT->CTRL = ((WDT->CTRL | (0x55UL << 24)) & ~(WDT_CTRL_EN));
  WDT->CTRL = ((WDT->CTRL | (0xAAUL << 24)) & ~(WDT_CTRL_EN));

  /* set Flash Waite State to maximum */
//  FLASHCALW->FCR = FLASHCALW_FCR_FWS;

  /* enable 80 MHz RC Oscillator */
  SCIF->UNLOCK = SCIF_UNLOCK_KEY | ((u32)&SCIF->RC80MCR - (u32)SCIF_BASE);
  SCIF->RC80MCR = SCIF_RC80MCR_EN;
  while((SCIF->RC80MCR & SCIF_RC80MCR_EN) != SCIF_RC80MCR_EN);

  PM->UNLOCK = PM_UNLOCK_KEY | ((u32)&PM->CPUSEL - (u32)PM_BASE);
  PM->CPUSEL = PM_CPUSEL_CPUDIV | 2;     /* 80 MHz / 8 */
  while((PM->SR & PM_SR_CKRDY) != PM_SR_CKRDY);

  PM->UNLOCK = PM_UNLOCK_KEY | ((u32)&PM->MCCTRL - (u32)PM_BASE);
  PM->MCCTRL = PM_MCCTRL_MCSEL_RC80M;
  while((PM->SR & PM_SR_CKRDY) != PM_SR_CKRDY);

  /* store Flash Start address */
  base_adr = adr;

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
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

#if 0
/*
   We do not use the 'Erase All' command because also RAM (where the code is running) content is erased.
   Therefore the 'Erase Flash' command in uVision fails everytime.
 */
int EraseChip (void) {

  /* erase all command */
  FLASHCALW->FCMD  = FLASHCALW_FCR_FKEY | FLASHCALW_FCR_FCMD_EA;
  while (!(FLASHCALW->FSR  & FLASHCALW_FSR_FRDY));

  return (0);
}
#endif


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {
  u32           page;
  u32           i;

  /* calculate page */
  page  = (adr - (base_adr)) / FLASH_PAGE_SIZE;

  /* unlock region containing given page command */
  FLASHCALW->FCMD = FLASHCALW_FCR_FKEY | FLASHCALW_FCR_FCMD_UP | (FLASHCALW_FCR_PAGEN & (page << 8));
  while (!(FLASHCALW->FSR & FLASHCALW_FSR_FRDY));

  /* erase all pages (32) in the given region */
  for (i = page;  i < (page + 32); i++) {
    /* Erase page command */
    FLASHCALW->FCMD = FLASHCALW_FCR_FKEY | FLASHCALW_FCR_FCMD_EP | (FLASHCALW_FCR_PAGEN & (i << 8));
    while (!(FLASHCALW->FSR & FLASHCALW_FSR_FRDY));

    /* check for errors */
    if (FLASHCALW->FSR & (FLASHCALW_FSR_PROGE | FLASHCALW_FSR_LOCKE))
      return (1);
  }

  return (0);
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  u32           page;
  u32          *Flash;

  /* calculate page */
    page  = (adr - (base_adr)) / FLASH_PAGE_SIZE;

  /* Clear Page Buffer */
  FLASHCALW->FCMD = FLASHCALW_FCR_FKEY | FLASHCALW_FCR_FCMD_CPB;
  while (!(FLASHCALW->FSR & FLASHCALW_FSR_FRDY));

  /* set write pointer to Flash address */
  Flash = (unsigned long *)adr;

  /* copy data to write buffer (boundry are double words) */
  for (sz = (sz + 7) & ~7; sz; sz -= 4, buf += 4) {
    *Flash++ = *((unsigned long *)buf);
  }

  /* start programming command */
  FLASHCALW->FCMD = FLASHCALW_FCR_FKEY | FLASHCALW_FCR_FCMD_WP | (FLASHCALW_FCR_PAGEN & (page << 8));
  while (!(FLASHCALW->FSR & FLASHCALW_FSR_FRDY));

  /* check for errors */
  if (FLASHCALW->FSR & (FLASHCALW_FSR_PROGE | FLASHCALW_FSR_LOCKE))
    return (1);

  return (0);
}
