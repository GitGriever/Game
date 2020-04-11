/**************************************************************************//**
 * @file     system_SAM4C.c
 * @brief    CMSIS Cortex-M4 Device System Source File
 *           for Atmel SAM4S Device Series
 * @version  V1.01
 * @date     15. January 2014
 *
 * @note
 * Copyright (C) 2014 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

#include <stdint.h>
#include <sam4c.h>

/*
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
*/

/*--------------------- Watchdog Configuration -------------------------------*/
// <O0.0> Watchdog Disable
#define WDT_SETUP       0               // Reset values:


/*--------------------- Embedded Flash Controllers Configuration -------------*/
// <e0> Embedded Flash Controller  (EEFC)
//   <o1.8..11>  FWS: Flash Wait State <1-16><#-1>
//   <o1.16>     SCOD: Sequential Code Optimization
//                 <0=> Enabled
//                 <1=> Disabled
//   <o1.24>     FAM: Flash Access Mode
//                 <0=> 128-bit in read mode (enhance speed)
//                 <1=> 64-bit in read mode (enhance power consumption)
//   <o1.26>     CLOE: Code Loops Optimization
//                 <0=> Disabled
//                 <1=> Enabled
// </e0>
#define EEFC_SETUP      0               // Reset values:
#define EEFC_FMR_Val    0x00000300      // 0x00000000


/*--------------------- Slow Clock Configuration -----------------------------*/
// <e0> SLOW Clock Configuration
//   <o1.3>      XTALSEL: Crystal Oscillator Select
//                 <0=> 32KHz RC Oscillator
//                 <1=> 32KHz XTAL Oscillator
//   <o2.20>     OSCBYPASS: 32KHz Crystal Oscillator Bypass
// </e0>
#define SLC_SETUP       0               // Reset values:
#define SUPC_CR_Val     0x00000000      // WO register (NA)
#define SUPC_MR_Val     0x00000000      // 0x00005A00


/*--------------------- Power Management Controller Configuration ------------*/
//
// <e0> Power Management Controller (PMC)
//   <h> System Clock Enable Register (PMC_SCER)
//     <o1.8>      PCK0: Programmable Clock 0 Output Enable
//     <o1.9>      PCK1: Programmable Clock 1 Output Enable
//     <o1.10>     PCK2: Programmable Clock 2 Output Enable
//   </h>
//
//   <h> Peripheral Clock Enable Register 0 (PMC_PCER0)
//     <o2.8>      PID8:  UART 0 Clock Enable
//     <o2.10>     PID10: Static Memory Controller 0 Clock Enable
//     <o2.11>     PID11: Parallel I/O Controller A Clock Enable
//     <o2.12>     PID12: Parallel I/O Controller B Clock Enable
//     <o2.14>     PID14: USART 0 Clock Enable
//     <o2.15>     PID15: USART 1 Clock Enable
//     <o2.16>     PID15: USART 2 Clock Enable
//     <o2.17>     PID15: USART 3 Clock Enable
//     <o2.18>     PID18: USART 4 Clock Enable
//     <o2.19>     PID19: Two-wire Interface 0 Clock Enable
//     <o2.20>     PID20: Two-wire Interface 1 Clock Enable
//     <o2.21>     PID21: Serial Peripheral Interface Clock Enable
//     <o2.23>     PID23: Timer/Counter 0 Clock Enable
//     <o2.24>     PID24: Timer/Counter 1 Clock Enable
//     <o2.25>     PID25: Timer/Counter 2 Clock Enable
//     <o2.26>     PID26: Timer/Counter 3 Clock Enable
//     <o2.27>     PID27: Timer/Counter 4 Clock Enable
//     <o2.28>     PID28: Timer/Counter 5 Clock Enable
//     <o2.29>     PID29: Analog-to-Digital Converter Clock Enable
//     <o2.30>     PID30: FPU signals (only on CM4P1 core) Clock Enable
//     <o2.31>     PID31: Interprocessor communication 0 Clock Enable
//   </h>
//
//   <h> Peripheral Clock Enable Register 1 (PMC_PCER1)
//     <o3.0>      PID32: Segment LCD Controller Clock Enable
//     <o3.1>      PID33: True Random Generator Clock Enable
//     <o3.2>      PID34: Integrity Check Module Clock Enable
//     <o3.3>      PID35: Classical Public Key Cryptography Controller Clock Enable
//     <o3.4>      PID36: Advanced Enhanced Standard Clock Enable
//     <o3.5>      PID37: Parallel I/O Controller C Clock Enable
//     <o3.6>      PID38: UART 1 Clock Enable
//     <o3.7>      PID39: Interprocessor communication 1 Clock Enable
//     <o3.8>      PID40: Serial Peripheral Interface 1 Clock Enable
//     <o3.9>      PID41: Pulse Width Modulation Clock Enable
//     <o3.10>     PID42: SRAM1, SRAM2 Clock Enable
//     <o3.11>     PID43: Static Memory Controller 1 Clock Enable
//   </h>
//
//   <h> Main Oscillator Register (CKGR_MOR)
//     <o4.0>      MOSCXTEN: Main Crystal Oscillator Enable
//     <o4.1>      MOSCXTBY: Main Crystal Oscillator Bypass
//     <o4.3>      MOSCRCEN: Main On-chip RC Oscillator Enable
//     <o4.4..6>   MOSCRCF: Main On-chip RC Oscillator Frequency Selection
//                   <0=> 4MHz <1=> 8 MHz <2=> 12 MHz <3=> Reserved
//     <o4.8..15>  MOSCXTST: Main Crystal Oscillator Startup Time <0-255>
//     <o4.24>     MOSCSEL: Main Oscillator Selection
//                   <0=> Main On-chip RC Oscillator <1=> Main Crystal Oscillator
//     <o4.25>     CFDEN: Clock Failure Detector Enable
//     <o4.26>     XT32KFME: Slow Crystal Oscillator Frequency Monitoring Enable
//   </h>
//
//   <h> Clock Generator Phase Locked Loop A Register (CKGR_PLLAR)
//                   <i> PLL A Freq = (Main CLOCK Freq) * (MULA + 1)
//                   <i> Example: XTAL = 32 KHz, MULA = 249  =>  PLLA =  8 MHz
//     <o5.0>      PLLAEN: PLL A enable
//     <o5.8..13>  PLLACOUNT: PLL A Counter <0-63>
//                   <i> Number of Slow Clocks before the LOCKA bit is set in
//                   <i> PMC_SR after CKGR_PLLAR is written
//     <o5.16..26> MULA: PLL A Multiplier <0-254>
//                   <i> 0          - The PLL A is deactivated
//                   <i> 200 .. 254 - The PLL A Clock frequency is the PLL a input
//                   <i>              frequency multiplied by MULA + 1
//   </h>
//
//   <h> Clock Generator Phase Locked Loop B Register (CKGR_PLLBR)
//                   <i> PLL B Freq = (Main CLOCK Freq / DIVB) * (MULB + 1)
//                   <i> Example: XTAL = 12 MHz, DIVB = 1, MULB = 14  =>  PLLB =  168 MHz
//     <o6.0..7>   DIVB: PLL Divider B <0-255>
//                   <i> 0        - Divider output is 0
//                   <i> 1        - Divider is bypassed
//                   <i> 2 .. 255 - Divider output is the Main Clock divided by DIVB
//     <o6.8..13>  PLLBCOUNT: PLL A Counter <0-63>
//                   <i> Number of Slow Clocks before the LOCKA bit is set in
//                   <i> PMC_SR after CKGR_PLLBR is written
//     <o6.16..26> MULB: PLL B Multiplier <0-62>
//                   <i> 0       - The PLL B is deactivated
//                   <i> 1 .. 62 - The PLL B Clock frequency is the PLL a input
//                   <i>           frequency multiplied by MULB + 1
//     <o6.29>     SRCB: Source for PLLB
//                   <0=> Main Clock <1=> PLL A Clock
//   </h>
//
//   <h> Master Clock Register (PMC_MCKR)
//     <o7.0..1>   CSS: Master Clock Selection
//                   <0=> Slow Clock
//                   <1=> Main Clock
//                   <2=> PLL A Clock
//                   <3=> PLL B Clock
//     <o7.4..6>   PRES: Master Clock Prescaler
//                   <0=> Clock        <1=> Clock / 2
//                   <2=> Clock / 4    <3=> Clock / 8
//                   <4=> Clock / 16   <5=> Clock / 32
//                   <6=> Clock / 64   <7=> Clock / 3
//     <o7.12>     PLLADIV2: PLLA Divisor by 2
//     <o7.13>     PLLBDIV2: PLLB Divisor by 2
//   </h>
//
//   <h> Programmable Clock Register 0 (PMC_PCK0)
//     <o9.0..2>   CSS: Master Clock Selection
//                   <0=> Slow Clock
//                   <1=> Main Clock
//                   <2=> PLL A Clock
//                   <3=> PLL B Clock
//                   <4=> Master Clock
//                   <5=> Reserved
//                   <6=> Reserved
//                   <7=> Reserved
//     <o9.4..6>   PRES: Programmable Clock Prescaler
//                   <0=> Clock        <1=> Clock / 2
//                   <2=> Clock / 4    <3=> Clock / 8
//                   <4=> Clock / 16   <5=> Clock / 32
//                   <6=> Clock / 64   <7=> Reserved
//   </h>
//
//   <h> Programmable Clock Register 1 (PMC_PCK1)
//     <o10.0..2>  CSS: Master Clock Selection
//                   <0=> Slow Clock
//                   <1=> Main Clock
//                   <2=> PLL A Clock
//                   <3=> PLL B Clock
//                   <4=> Master Clock
//                   <5=> Reserved
//                   <6=> Reserved
//                   <7=> Reserved
//     <o10.4..6>  PRES: Programmable Clock Prescaler
//                   <0=> Clock        <1=> Clock / 2
//                   <2=> Clock / 4    <3=> Clock / 8
//                   <4=> Clock / 16   <5=> Clock / 32
//                   <6=> Clock / 64   <7=> Reserved
//   </h>
//
//   <h> Programmable Clock Register 2 (PMC_PCK2)
//     <o11.0..2>  CSS: Master Clock Selection
//                   <0=> Slow Clock
//                   <1=> Main Clock
//                   <2=> PLL A Clock
//                   <3=> PLL B Clock
//                   <4=> Master Clock
//                   <5=> Reserved
//                   <6=> Reserved
//                   <7=> Reserved
//     <o11.4..6>  PRES: Programmable Clock Prescaler
//                   <0=> Clock        <1=> Clock / 2
//                   <2=> Clock / 4    <3=> Clock / 8
//                   <4=> Clock / 16   <5=> Clock / 32
//                   <6=> Clock / 64   <7=> Reserved
//   </h>
// </e0>
#define PMC_SETUP       0               // Reset values:
#define PMC_SCER_Val    0x00000000      // WO register (0x00000001)
#define PMC_PCER0_Val   0x80008100      // WO register (0x00000000)
#define PMC_PCER1_Val   0x000004A0      // WO register (0x00000000)
#define CKGR_MOR_Val    0x01003F29      // 0x00000008
#define CKGR_PLLAR_Val  0x00F90601      // 0x00003F00
#define CKGR_PLLBR_Val  0x00173F02      // 0x00003F00
#define PMC_MCKR_Val    0x00000003      // 0x00000001
#define FILL_0          0x00000000      // 0x00000000
#define PMC_PCK0_Val    0x00000001      // 0x00000000
#define PMC_PCK1_Val    0x00000004      // 0x00000000
#define PMC_PCK2_Val    0x00000000      // 0x00000000


/*--------------------- Coprocessor Configuration ----------------------------*/
// <e0> Coprocessor Configuration
//   <h> System Clock Enable Register (PMC_SCER)
//     <o1.16>     CPCK: Coprocessor (second processor) Clocks Enable
//     <o1.17>     CPBMCK: Coprocessor Bus Master Clocks Enable
//   </h>
//
//   <h> Master Clock Register (PMC_MCKR)
//     <o2.16..18> CPCSS: Coprocessor Master Clock Source Selection
//                   <0=> Slow Clock
//                   <1=> Main Clock
//                   <2=> PLL A Clock
//                   <3=> PLL B Clock
//                   <4=> Master Clock
//     <o2.20..23> CPCSS: Master Clock Prescaler <0-15>
//   </h>
//
// </e0>
#define CM4P1_SETUP     0               // Reset values:
#define PMC_SCER_Val2   0x00030000      // WO register (0x00000001)
#define PMC_MCKR_Val2   0x00010000      // 0x00000001


/*--------------------- XTAL Setting -----------------------------------------*/
// <o0> System Oscillator (XTAL) Frequency [Hz] <3000000-20000000>
//          <i> XTAL frequency must be in the range of  3 MHz to  20 MHz
#define XTAL_CLK_Val          8000000


/*
//-------- <<< end of configuration section >>> ------------------------------
*/

/*----------------------------------------------------------------------------
  Check the register settings
 *----------------------------------------------------------------------------*/
#define CHECK_RANGE(val, min, max)                ((val < min) || (val > max))
#define CHECK_RSVD(val, mask)                     (val & mask)

/* Embedded Flash Controllers Configuration ----------------------------------*/
#if (CHECK_RSVD((EEFC_FMR_Val),  ~0x05010F01))
   #error "EEFC_FMR: Invalid values of reserved bits!"
#endif

/* Slow Clock Configuration --------------------------------------------------*/
#if (CHECK_RSVD((SUPC_CR_Val),  ~0x00000008))
   #error "SUPC_CR: Only XTALSEL allowed!"
#endif

#if (CHECK_RSVD((SUPC_MR_Val),  ~0x00100000))
   #error "SUPC_MR: Only OSCBYPASS allowed!"
#endif

/* Power Management Controller Configuration ---------------------------------*/
#if (CHECK_RSVD((PMC_SCER_Val),   ~0x00000700))
   #error "PMC_SCER: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PMC_PCER0_Val),   ~0xFFFFFF00))
   #error "PMC_PCER0: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PMC_PCER1_Val),   ~0x00000FFF))
   #error "PMC_PCER1: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((CKGR_MOR_Val),   ~0x0700FF7F))
   #error "CKGR_MOR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((CKGR_PLLAR_Val), ~0x00FF3F01))
   #error "CKGR_PLLAR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((CKGR_PLLBR_Val), ~0x203F3FFF))
   #error "CKGR_PLLBR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PMC_MCKR_Val),   ~0x00003073))
   #error "PMC_MCKR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PMC_PCK0_Val),   ~0x00000077))
   #error "PMC_PCK0: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PMC_PCK1_Val),   ~0x00000077))
   #error "PMC_PCK1: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PMC_PCK2_Val),   ~0x00000077))
   #error "PMC_PCK2: Invalid values of reserved bits!"
#endif

/* Coprocessor Configuration -------------------------------------------------*/
#if (CHECK_RSVD((PMC_SCER_Val2),   ~0x00030000))
   #error "PMC_SCER: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PMC_MCKR_Val2),   ~0x00010000))
   #error "PMC_MCKR: Invalid values of reserved bits!"
#endif



/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/

#if (PMC_SETUP)

  #if (SUPC_CR_Val & SUPC_CR_XTALSEL)
    #define __SLOW_CLK     (CHIP_FREQ_XTAL_32K)
  #else
    #define __SLOW_CLK     (CHIP_FREQ_SLCK_RC)
  #endif

  /* Determine clock frequency according to clock register values */
  #if   ((PMC_MCKR_Val & PMC_MCKR_CSS_Msk) == PMC_MCKR_CSS_SLOW_CLK)  /* Slow Clock is selected   */
    #define __CORE_CLK_PRE     (__SLOW_CLK)

  #elif ((PMC_MCKR_Val & PMC_MCKR_CSS_Msk) == PMC_MCKR_CSS_MAIN_CLK)  /* Main Clock is selected   */
    #if (CKGR_MOR_Val & CKGR_MOR_MOSCSEL)
      #if ((CKGR_MOR_Val & CKGR_MOR_MOSCXTEN) == 0)
        #error "CKGR_MOR: Main Crystal Oscillator selected but not enabled!"
      #endif
      #define __CORE_CLK_PRE     (XTAL_CLK_Val)
    #else
      #if   ((CKGR_MOR_Val & CKGR_MOR_MOSCRCF_Msk) == CKGR_MOR_MOSCRCF_4_MHz)
          #define __CORE_CLK_PRE     (CHIP_FREQ_MAINCK_RC_4MHZ)
      #elif ((CKGR_MOR_Val & CKGR_MOR_MOSCRCF_Msk) == CKGR_MOR_MOSCRCF_8_MHz)
          #define __CORE_CLK_PRE     (CHIP_FREQ_MAINCK_RC_8MHZ)
      #elif ((CKGR_MOR_Val & CKGR_MOR_MOSCRCF_Msk) == CKGR_MOR_MOSCRCF_12_MHz)
          #define __CORE_CLK_PRE     (CHIP_FREQ_MAINCK_RC_12MHZ)
      #else
        #error "CKGR_MOR: Value not valid!"
      #endif
    #endif

  #elif ((PMC_MCKR_Val & PMC_MCKR_CSS_Msk) == PMC_MCKR_CSS_PLLA_CLK)  /* PLLA Clock is selected   */
    #define __PLLA_CLK       (__SLOW_CLK)
    #define __PLLA_MUL       (((CKGR_PLLAR_Val & CKGR_PLLAR_MULA_Msk) >> CKGR_PLLAR_MULA_Pos) + 1)
    #define __PLLA_DIV2      ((PMC_MCKR_Val & PMC_MCKR_PLLADIV2) >> 12)

    #define __CORE_CLK_PRE   ((__PLLA_CLK * __PLLA_MUL) >> __PLLA_DIV2)

  #elif ((PMC_MCKR_Val & PMC_MCKR_CSS_Msk) == PMC_MCKR_CSS_PLLB_CLK)  /* PLLB Clock is selected   */
    #if ((CKGR_PLLBR_Val & CKGR_PLLBR_SRCB) == CKGR_PLLBR_SRCB)
      #define __PLLA_CLK       (__SLOW_CLK)
      #define __PLLA_MUL       (((CKGR_PLLAR_Val & CKGR_PLLAR_MULA_Msk) >> CKGR_PLLAR_MULA_Pos) + 1)
      /* PMC_MCKR.PLLADIV2 has no effect when CKGR_PLLBR.SRCB is set! */

      #define __PLLB_CLK       (__PLLA_CLK * __PLLA_MUL)
    #else
      #if (CKGR_MOR_Val & CKGR_MOR_MOSCSEL)
        #if ((CKGR_MOR_Val & CKGR_MOR_MOSCXTEN) == 0)
          #error "CKGR_MOR: Main Crystal Oscillator selected but not enabled!"
        #endif
        #define __PLLB_CLK       (XTAL_CLK_Val)
      #else
        #if   ((CKGR_MOR_Val & CKGR_MOR_MOSCRCF_Msk) == CKGR_MOR_MOSCRCF_4_MHz)
          #define __PLLB_CLK       ( CHIP_FREQ_MAINCK_RC_4MHZ)
        #elif ((CKGR_MOR_Val & CKGR_MOR_MOSCRCF_Msk) == CKGR_MOR_MOSCRCF_8_MHz)
              #define __PLLB_CLK       (CHIP_FREQ_MAINCK_RC_8MHZ)
        #elif ((CKGR_MOR_Val & CKGR_MOR_MOSCRCF_Msk) == CKGR_MOR_MOSCRCF_12_MHz)
              #define __PLLB_CLK       (CHIP_FREQ_MAINCK_RC_12MHZ)
        #else
          #error "CKGR_MOR: Value not valid!"
        #endif
      #endif
    #endif

    #define __PLLB_MUL       (((CKGR_PLLBR_Val & CKGR_PLLBR_MULB_Msk) >> CKGR_PLLBR_MULB_Pos) + 1)
    #define __PLLB_DIV       (((CKGR_PLLBR_Val & CKGR_PLLBR_DIVB_Msk) >> CKGR_PLLBR_DIVB_Pos)    )
    #define __PLLB_DIV2      ((PMC_MCKR_Val & PMC_MCKR_PLLBDIV2) >> 13)
    #define __CORE_CLK_PRE   ((__PLLB_CLK * __PLLB_MUL / __PLLB_DIV) >> __PLLB_DIV2)

  #else                              /* Reserved                             */
    #define __CORE_CLK_PRE   (0UL)
  #endif

  #if ((PMC_MCKR_Val & PMC_MCKR_PRES_Msk) == PMC_MCKR_PRES_CLK_3)
    #define __CORE_CLK   (__CORE_CLK_PRE / 3)
  #else
    #define __CORE_CLK   (__CORE_CLK_PRE >> ((PMC_MCKR_Val & PMC_MCKR_PRES_Msk) >> PMC_MCKR_PRES_Pos))
  #endif

#else // if (PMC_SETUP)
    #define __CORE_CLK   (CHIP_FREQ_MAINCK_RC_4MHZ)
#endif

#if (__CORE_CLK > 120000000UL)
  #error "Core Clock > 120MHz!"
#endif


/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __CORE_CLK;       /*!< System Core Clock Frequency */


/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
  /* Determine clock frequency according to clock register values */
  switch (PMC->PMC_MCKR & (uint32_t)PMC_MCKR_CSS_Msk)
  {
    case PMC_MCKR_CSS_SLOW_CLK:  /* Slow clock */
      if (SUPC->SUPC_SR & SUPC_SR_OSCSEL)
      {
        SystemCoreClock = CHIP_FREQ_XTAL_32K;
      }
      else
      {
        SystemCoreClock = CHIP_FREQ_SLCK_RC;
      }
    break;

    case PMC_MCKR_CSS_MAIN_CLK:  /* Main clock */
      if (PMC->CKGR_MOR & CKGR_MOR_MOSCSEL)
      {
        SystemCoreClock = XTAL_CLK_Val;
      }
      else
      {
        SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;

        switch (PMC->CKGR_MOR & CKGR_MOR_MOSCRCF_Msk)
        {
          case CKGR_MOR_MOSCRCF_4_MHz:
            SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;
          break;

          case CKGR_MOR_MOSCRCF_8_MHz:
            SystemCoreClock = CHIP_FREQ_MAINCK_RC_8MHZ;
          break;

          case CKGR_MOR_MOSCRCF_12_MHz:
            SystemCoreClock = CHIP_FREQ_MAINCK_RC_12MHZ;
          break;

          default:
            // Ooops
          break;
        }
      }
    break;

    case PMC_MCKR_CSS_PLLA_CLK:  /* PLLA clock */
      if (SUPC->SUPC_SR & SUPC_SR_OSCSEL)
      {
        SystemCoreClock = CHIP_FREQ_XTAL_32K;
      }
      else
      {
        SystemCoreClock = (CHIP_FREQ_SLCK_RC);
      }

      SystemCoreClock  *= (((PMC->CKGR_PLLAR & CKGR_PLLAR_MULA_Msk) >> CKGR_PLLAR_MULA_Pos) + 1U);
      SystemCoreClock >>=  ((PMC->PMC_MCKR   & PMC_MCKR_PLLADIV2  ) >> 12                 );     // PMC_MCKR.PLLADIV2
    break;

    case PMC_MCKR_CSS_PLLB_CLK:  /* PLLB clock */
      /* status of SRCB is reflected in bit 27 */
      if ((PMC->CKGR_PLLBR & (1ul << 27)) == (1ul << 27))
//    if ((PMC->CKGR_PLLBR & CKGR_PLLBR_SRCB) == CKGR_PLLBR_SRCB)  // This does not work on Chip  AU-ES!
      {
        if (SUPC->SUPC_SR & SUPC_SR_OSCSEL)
        {
          SystemCoreClock = CHIP_FREQ_XTAL_32K;
        }
        else
        {
          SystemCoreClock = (CHIP_FREQ_SLCK_RC);
        }

        SystemCoreClock  *= (((PMC->CKGR_PLLAR & CKGR_PLLAR_MULA_Msk) >> CKGR_PLLAR_MULA_Pos) + 1U);
        /* PMC_MCKR.PLLADIV2 has no effect when CKGR_PLLBR.SRCB is set! */
      }
      else
      {
        if (PMC->CKGR_MOR & CKGR_MOR_MOSCSEL)
        {
          SystemCoreClock = XTAL_CLK_Val;
        }
        else
        {
          SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;

          switch (PMC->CKGR_MOR & CKGR_MOR_MOSCRCF_Msk)
          {
            case CKGR_MOR_MOSCRCF_4_MHz:
              SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;
            break;

            case CKGR_MOR_MOSCRCF_8_MHz:
              SystemCoreClock = CHIP_FREQ_MAINCK_RC_8MHZ;
            break;

            case CKGR_MOR_MOSCRCF_12_MHz:
              SystemCoreClock = CHIP_FREQ_MAINCK_RC_12MHZ;
            break;

            default:
            break;
          }
        }
      }

      SystemCoreClock  *= (((PMC->CKGR_PLLBR & CKGR_PLLBR_MULB_Msk) >> CKGR_PLLBR_MULB_Pos) + 1U);
      SystemCoreClock  /= (((PMC->CKGR_PLLBR & CKGR_PLLBR_DIVB_Msk) >> CKGR_PLLBR_DIVB_Pos)     );
      SystemCoreClock >>=  ((PMC->PMC_MCKR   & PMC_MCKR_PLLBDIV2  ) >> 13);     // PMC_MCKR.PLLBDIV2
    break;

    default:
      // Ooops
    break;
  }

  if ((PMC->PMC_MCKR & PMC_MCKR_PRES_Msk) == PMC_MCKR_PRES_CLK_3)
  {
    SystemCoreClock  /= 3U;
  }
  else
  {
    SystemCoreClock >>= ((PMC->PMC_MCKR & PMC_MCKR_PRES_Msk) >> PMC_MCKR_PRES_Pos);
  }
}


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{

#if (__FPU_USED == 1)
  /* enable FPU if available and used */
  SCB->CPACR |= ((3UL << 10*2) |             /* set CP10 Full Access               */
                 (3UL << 11*2)  );           /* set CP11 Full Access               */
#endif

#if (EEFC_SETUP == 1)                        /* Embedded Flash Controller Setup    */
  EFC->EEFC_FMR = EEFC_FMR_Val;
#endif

#if (WDT_SETUP == 1)                          /* Watchdog Setup                    */
  WDT->WDT_MR = WDT_MR_WDDIS;
#endif

#if (SLC_SETUP == 1)                          /* Slow Clock Setup                  */
  PMC->PMC_WPMR = 0x52544300;                 /* Disable write protect             */

  SUPC->SUPC_CR = ((SUPC->SUPC_CR & ~SUPC_CR_XTALSEL) |
                   (SUPC_CR_Val   &  SUPC_CR_XTALSEL) |
                    SUPC_CR_KEY_PASSWD                 );

  SUPC->SUPC_MR = ((SUPC->SUPC_MR & ~SUPC_MR_OSCBYPASS) |
                   (SUPC_MR_Val   &  SUPC_MR_OSCBYPASS) |
                    SUPC_MR_KEY_PASSWD                   );

  PMC->PMC_WPMR = 0x52544301;                 /* Enable write protect              */
#endif

  PMC->PMC_WPMR = 0x504D4300;                 /* Disable write protect             */

#if (PMC_SETUP == 1)                          /* Power Management Controller Setup */
  /* before we change the clocksetup we switch Master Clock Source
     to MAIN_CLK and set prescaler to one
   */
  PMC->PMC_MCKR = (PMC->PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
  while (!(PMC->PMC_SR & PMC_SR_MCKRDY));     /* Wait for MCKRDY                   */

	PMC->PMC_MCKR = (PMC->PMC_MCKR & ~ PMC_MCKR_PRES_Msk) | PMC_MCKR_PRES_CLK_1;
  while (!(PMC->PMC_SR & PMC_SR_MCKRDY));     /* Wait for MCKRDY                   */

#if (CKGR_MOR_Val & (CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN))    /* If MOSCRCEN/MOSCXTEN set  */
  PMC->CKGR_MOR  = ((PMC->CKGR_MOR &  CKGR_MOR_MOSCSEL)    |    /* Keep the current MOSCSEL  */
                    (CKGR_MOR_Val  & ~CKGR_MOR_MOSCSEL)    |    /* Set value except MOSCSEL  */
                    (CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN)|    /* and enable bothe OSCs     */
                    (CKGR_MOR_KEY_PASSWD)                   );
#if (CKGR_MOR_Val & CKGR_MOR_MOSCRCEN)
  while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));   /* Wait for MOSCRCS                   */
#endif
#if (CKGR_MOR_Val & CKGR_MOR_MOSCXTEN)
  while (!(PMC->PMC_SR & PMC_SR_MOSCXTS));   /* Wait for MOSCXTS                   */
#endif
  PMC->CKGR_MOR  =  ((CKGR_MOR_Val      ) |  /* set the desired selection          */
                     (CKGR_MOR_KEY_PASSWD) );
  while (!(PMC->PMC_SR & PMC_SR_MOSCSELS));  /* Wait for MOSCSELS                  */
#endif

  /* write PLLBDIV2, PLLADIV2 */
  PMC->PMC_MCKR = ((PMC->PMC_MCKR & ~(PMC_MCKR_PLLADIV2 | PMC_MCKR_PLLBDIV2)) |
                   (PMC_MCKR_Val  &  (PMC_MCKR_PLLADIV2 | PMC_MCKR_PLLBDIV2))  );
  while (!(PMC->PMC_SR & PMC_SR_MCKRDY));    /* Wait for MCKRDY                    */

#if (CKGR_PLLAR_Val & CKGR_PLLAR_MULA_Msk)   /* If PLL is activated                */
  PMC->CKGR_PLLAR = CKGR_PLLAR_Val;
  while (!(PMC->PMC_SR & PMC_SR_LOCKA));     /* Wait for LOCKA                     */
#endif

#if (CKGR_PLLBR_Val & CKGR_PLLBR_MULB_Msk)   /* If PLL is activated                */
  PMC->CKGR_PLLBR = CKGR_PLLBR_Val;
  while (!(PMC->PMC_SR & PMC_SR_LOCKB));     /* Wait for LOCKB                     */
#endif

  if ((PMC_MCKR_Val & PMC_MCKR_CSS_Msk) >= 2) {
    /* Write PRES field only                                                       */
    PMC->PMC_MCKR = ((PMC->PMC_MCKR & ~PMC_MCKR_PRES_Msk) |
                     (PMC_MCKR_Val  &  PMC_MCKR_PRES_Msk)  );
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));    /* Wait for MCKRDY                  */

    /* Write CSS field only                                                        */
    PMC->PMC_MCKR = ((PMC->PMC_MCKR & ~PMC_MCKR_CSS_Msk) |
                     (PMC_MCKR_Val  &  PMC_MCKR_CSS_Msk)  );
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));    /* Wait for MCKRDY                  */
  }
  else {
    /* Write CSS field only                                                        */
    PMC->PMC_MCKR = ((PMC->PMC_MCKR & ~PMC_MCKR_CSS_Msk) |
                     (PMC_MCKR_Val  &  PMC_MCKR_CSS_Msk)  );
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));    /* Wait for MCKRDY                  */

    /* Write PRES field only                                                       */
    PMC->PMC_MCKR = ((PMC->PMC_MCKR & ~PMC_MCKR_PRES_Msk) |
                     (PMC_MCKR_Val  &  PMC_MCKR_PRES_Msk)  );
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));    /* Wait for MCKRDY                  */
  }

  PMC->PMC_SCER  = PMC_SCER_Val;
  PMC->PMC_PCER0 = PMC_PCER0_Val;
  PMC->PMC_PCER1 = PMC_PCER1_Val;

#if (PMC_SCER_Val & PMC_SCER_PCK0)
  PMC->PMC_PCK[0] = PMC_PCK0_Val;            /* Write PCK0                         */
  while (!(PMC->PMC_SR & PMC_SR_PCKRDY0));   /* Wait for PCKRDY0                   */
#endif
#if (PMC_SCER_Val & PMC_SCER_PCK1)
  PMC->PMC_PCK[1] = PMC_PCK1_Val;            /* Write PCK1                         */
  while (!(PMC->PMC_SR & PMC_SR_PCKRDY1));   /* Wait for PCKRDY1                   */
#endif
#if (PMC_SCER_Val & PMC_SCER_PCK2)
  PMC->PMC_PCK[2] = PMC_PCK2_Val;            /* Write PCK2                         */
  while (!(PMC->PMC_SR & PMC_SR_PCKRDY2));   /* Wait for PCKRDY2                   */
#endif

#endif  // end PMC_SETUP

#if (CM4P1_SETUP == 1)                       /* Coprocessor Setup                  */
   REG_PMC_SCER = ((PMC_SCER_Val2 & PMC_SCER_CPBMCK) |       /* write CPBMCK       */
                   (PMC_SCER_CPKEY_PASSWD          )  );

   REG_PMC_SCER = ((PMC_SCER_Val2 & PMC_SCER_CPCK) |         /* write CPCK         */
                   (PMC_SCER_CPKEY_PASSWD        )  );

   PMC->PMC_MCKR = ((PMC->PMC_MCKR & ~PMC_MCKR_CPPRES_Msk) |  /* write CPPRES      */
                    (PMC_MCKR_Val2 &  PMC_MCKR_CPPRES_Msk)  );

   PMC->PMC_MCKR = ((PMC->PMC_MCKR & ~PMC_MCKR_CPCSS_Msk) |   /* write CPCSS       */
                    (PMC_MCKR_Val2 &  PMC_MCKR_CPCSS_Msk)  );

   PMC->PMC_PCER1 = (1ul << 10);                 /* enable SRAM 1/2 in any case    */

   RSTC->RSTC_CPMR = (RSTC_CPMR_CPEREN       |   /* enable Coprocessor Peripherals */
                      RSTC_CPMR_CPKEY(0x5Aul)  );

#endif

  PMC->PMC_WPMR = 0x504D4301;                /* Enable write protect               */
}
