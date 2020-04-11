/**************************************************************************//**
 * @file     system_S3FM02G.c
 * @brief    CMSIS Cortex-M3 Device Peripheral Access Layer Source File
 *           for the Samsung S3FM02G Device Series
 * @version  V0.01
 * @date     02. February 2011
 *
 * @note
 * Copyright (C) 2011 ARM Limited. All rights reserved.
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
#include "S3FM02G.h"

/*
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
*/

/*--------------------- Clock Configuration ----------------------------------
//
// <e> Clock Configuration
//   <h> System Clock Configuration
//     <o1.0..2> CM_MR1.SYSCLK[2:0]: SYSCLK Source Selection
//           <i> SYSCLK[1:0] fields reset value depends on the POCCS[1:0] field in smart option configuration status register
//                     < 0=>   ESCLK
//                     < 1=>   EMCLK
//                     < 2=>   ISCLK
//                     < 3=>   IMCLK
//                     < 4=>   PLLCLK
//     <o2.0..2> CM_SCDR.SDIV[2:0]: SYSCLK DIVIDER REGISTER
//           <i> This field selects the division ratio for SYSCLK.
//                     < 0=>   Division ratio 1
//                     < 1=>   Division ratio 2
//                     < 2=>   Division ratio 3
//                     < 3=>   Division ratio 4
//                     < 4=>   Division ratio 5
//                     < 5=>   Division ratio 6
//                     < 6=>   Division ratio 7
//                     < 7=>   Division ratio 8
//   </h>
//
//   <h> PLL Configuration
//     <o3.0..10> CM_PSTR.PST[10:0]: CM PLL Stabilization Time Register
//           <i> PST value = (PLL stabilization time / (EMCLK period ´ 256))
//           <i> Typically, PLL stabilization time must be longer than 300 us.
//                     <0-32768>
//     <o4.0..7>  CM_PDPR.PLLMUL[7:0]: PLL Multiplier
//                     <0-255>
//                     <i> M Value
//     <o4.8..13> CM_PDPR.PLLPRE[5:0]: PLL Pre-divider
//                     <0-63>
//                     <i> P Value
//     <o4.16..17> CM_PDPR.PLLPOST[1:0]: PLL Post-scaler
//                     <0-3>
//                     <i> S Value
//     <o4.19..21>  CM_PDPR.PLLCUS[2:0]: Charge Pump Current Selection
//                     <0-3>
//                     <i> Charge Pump Current Selection
//   </h>
//
//   <h> Peripheral Clock (PCLK) Divider Configuration
//     <o5.0..3> CM_PCDR.PDIV[3:0]: 
//           <i> HCLK divider for PCLK
//                     < 0=>   Division ratio 1 (PCLK = HCLK) 
//                     < 1=>   Division ratio 2 (PCLK = HCLK/2)
//                     < 2=>   Division ratio 3 (PCLK = HCLK/4)
//                     < 3=>   Division ratio 3 (PCLK = HCLK/4)
//                     < 4=>   Division ratio 5 (PCLK = HCLK/8)
//                     < 5=>   Division ratio 5 (PCLK = HCLK/8)
//                     < 6=>   Division ratio 5 (PCLK = HCLK/8)
//                     < 7=>   Division ratio 5 (PCLK = HCLK/8)
//                     < 8=>   Division ratio 5 (PCLK = HCLK/16)
//                     < 9=>   Division ratio 5 (PCLK = HCLK/16)
//                     < 10=>  Division ratio 5 (PCLK = HCLK/16)
//                     < 11=>  Division ratio 5 (PCLK = HCLK/16)
//                     < 12=>  Division ratio 5 (PCLK = HCLK/16)
//                     < 13=>  Division ratio 5 (PCLK = HCLK/16)
//                     < 14=>  Division ratio 5 (PCLK = HCLK/16)
//                     < 15=>  Division ratio 5 (PCLK = HCLK/16)
//   </h>
// </e>
*/
#define CLOCK_SETUP           1
#define CM_MR1_SYSCLK_Val     0x00000004
#define CM_SCDR_SDIV_Val      0x00000000
#define CM_PSTRPST_Val        0x0000000A
#define CM_PDPR_Val           0x00080643
#define CM_PCDR_Val           0x00000000

/*--------------------- Program Flash Mode Configuration -------------------------------
// <e> Program Flash Mode Configuration
// <h> Program Flash Mode Configuration 
//   <o1.0> PF_MR.BACEN: Boot Area Erase Enable/Disable by Entire Program Memory Erase
//   <o1.4..5> PF_MR.WAIT[1:0]: Program Flash Access Wait Cycle
//        <i> NOTE) WAIT[1:0] must be set the value which meets this equation
//        <i>            (WAIT[1:0] + 1) * period of HCLK > 50ns
//        <i>             WAIT[1:0] is no effect when PBUFEN is 0.
//               <0=> 1 Wait Cycle during the Programming Flash Read Access
//               <1=> 2 Wait Cycle during the Programming Flash Read Access
//               <2=> 3 Wait Cycle during the Programming Flash Read Access
//               <3=> 4 Wait Cycle during the Programming Flash Read Access
//   <o1.8> PF_MR.PBUFEN: Pre-Fetch Buffer Enable
//        <i> NOTE) PBUFEN must be enabled when the period of HCLK is greater than 50ns
// </h>
// </e>
*/
#define PFLASH_SETUP          0x00000001
#define PF_MR_Val             0x00000110

/*--------------------- WatchDog Configuration -------------------------------
//
// <o0.0>  Watchdog disable 
*/
#define WDT_DISABLE           0x00000001

/*
//-------- <<< end of configuration section >>> ------------------------------
*/



/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/
    
/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define __IMCLK8MHZ    ( 8000000UL)         /* Internal Main Clock: 8 Mhz     */
#define __IMCLK16MHZ   (16000000UL)         /* Internal Main Clock: 16 Mhz    */
#define __IMCLK20MHZ   (20000000UL)         /* Internal Main Clock: 20 Mhz    */
#define __ESCLK        (   32768UL)         /* External Sub Clock : 32.768 Khz*/
#define __ISCLK        (   32768UL)         /* Internal Sub Clock : 32.768 Khz*/
#define __EMCLK        ( 4000000UL)         /* External Main Clock:  4 Mhz)   */



/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __EMCLK;  /*!< System Clock Frequency (Core Clock)*/


/**
 * Retrieve the system core clock
 *
 * @param  none
 * @return none
 *
 * @brief  retrieve system core clock from register settings.
 */
void SystemCoreClockUpdate (void) {
  /* Determine clock frequency according to clock register values             */
  switch(S3FM_CM->MR1 & 0x7) {
    case 0U:                        /* If SYSCLK is External Sub Clock(32.768 Khz)   */
      SystemCoreClock = __ESCLK; 
      break;    
    case 1U:                        /* If SYSCLK is External Main Clock(4 ~ 8 Mhz)   */ 
      SystemCoreClock = __EMCLK; 
      break;    
    case 2U:                        /* If SYSCLK is Internal Sub Clock(32.768 Khz)   */
      SystemCoreClock = __ISCLK; 
      break;    
    case 3U:                        /* If SYSCLK is Internal Main Clock(8/16/20 Mhz) */
      switch( (S3FM_PFC->SO_CSR >> 6) & 3U ) {
        case 0U:                    /* Not Supported */
          break;
        case 1U:                    /* 8 Mhz  */
          SystemCoreClock = __IMCLK8MHZ; 
          break;     
        case 2U:                    /* 16 Mhz */
          SystemCoreClock = __IMCLK16MHZ; 
          break;     
        case 3U:                    /* 20 Mhz */ 
          SystemCoreClock = __IMCLK20MHZ; 
          break;     
      }
      break;
    case 4U:                       /* If SYSCLK is PLL Output Clock(4 ~ 75 Mhz)     */
      SystemCoreClock = (((S3FM_CM->PDPR & 0xFF)+8) * __EMCLK) / \
                        ((((S3FM_CM->PDPR >> 8) & 0x3F)+2) *   \
                        (1<<((S3FM_CM->PDPR >> 16) & 0x3)));
      break;
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
void SystemInit (void) {
  
#if (WDT_DISABLE)                                           /* Watchdog Disable */
  S3FM_WDT->OMR         = 0x2340;
#endif

#if (PFLASH_SETUP)                                          /* Program Flash Serup */
  S3FM_PFC->CEDR        = 0x1;                              /* Program Flash Clock Enable      */
  S3FM_PFC->MR          = PF_MR_Val;                        /* Program Flash Mode Setting      */
#endif

#if (CLOCK_SETUP)                                           /* Clock Setup */
  switch(CM_MR1_SYSCLK_Val & 0x7) {
    case 0U:                                                /* External Sub Clock(32.768 Khz) */
      S3FM_CM->CSR      = CM_MR1_SYSCLK_Val & 0x07;         /* ESCLK Enable */
      while(!(S3FM_CM->SR & 0x04)) ;                        /* ESCLK stable check  */
      S3FM_CM->MR1      = (S3FM_CM->MR1 & ~0x7) | 0x00;     /* Set SYSCLK as ESCLK */
      break;
    case 1U:                                                /* External Main Clock(4 ~ 8 Mhz) */
      S3FM_CM->CSR      = CM_MR1_SYSCLK_Val & 0x07;         /* EMCLK Enable */
      while(!(S3FM_CM->SR & 0x01)) ;                        /* EMCLK stable check */
      S3FM_CM->MR1      = (S3FM_CM->MR1 & ~0x7) | 0x01;     /* Set SYSCLK as EMCLK */
      break;
    case 2U:                                                /* Internal Sub Clock(32.768 Khz) */
      S3FM_CM->CSR      = CM_MR1_SYSCLK_Val & 0x07;         /* ISCLK Enable */
      while(!(S3FM_CM->SR & 0x08)) ;                        /* ISCLK stable check */
      S3FM_CM->MR1      = (S3FM_CM->MR1 & ~0x7) | 0x02;     /* Set SYSCLK as ISCLK */
      break;
    case 3U:                                                /* Internal Main Clock(8/16/20 Mhz) */
      S3FM_CM->CSR      = CM_MR1_SYSCLK_Val & 0x07;         /* IMCLK Enable */
      while(!(S3FM_CM->SR & 0x02) ) ;                       /* IMCLK stable check */
      S3FM_CM->MR1      = (S3FM_CM->MR1 & ~0x7) | 0x03;     /* Set SYSCLK as IMCLK */
      break;
    case 4U:                                                /* PLL Clock(Max 75Mhz) */
      S3FM_CM->CSR      = 0x01;                             /* EMCLK Enable */
      while(!(S3FM_CM->SR & 0x01)) ;                        /* EMCLK stable check */

      /* PLL Initialization (Start) */
      S3FM_CM->PSTR	    = (0x59C1ul << 16) | \
                           CM_PSTRPST_Val;                  /* Set PSTR(PLL Stabilization Time Register) value */
      S3FM_CM->PDPR	    = (0xC1ul << 24) | CM_PDPR_Val;     /* Set PDPR(PLL Divider Parameters Register) value */
      S3FM_CM->CSR      = 0x80;                             /* PLL Enable */
      while(!(S3FM_CM->SR & 0x80)) ;                        /* PLL stable check */
      /* PLL Initialization (End) */
      
      S3FM_CM->MR1      = (S3FM_CM->MR1 & ~0x7) | 0x04;     /* Set SYSCLK as PLLCLK */
      break;
  }

  S3FM_CM->SCDR         = (0xACDCul << 16) | \
                           CM_SCDR_SDIV_Val;                /* Set SCDR(System Clock Divider Register) value */
  S3FM_CM->PCDR         = (0xA3C5ul << 16) | \
                           CM_PCDR_Val;                     /* Set PCDR(Peripheral Clock Divider Register) value */
#endif
}
