;/*****************************************************************************/
;/* AT91SAM3xGPNVM.s: AT91SAM3x  General-purpose Non-volatile Memory Bits     */
;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2005-2010 Keil Software. All rights reserved.               */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/


;// <e> General-purpose Non-volatile Memory Bits
GPNVM       EQU     1

;//   <o0.0> Security bit
;//   <o0.1> Boot mode selection
;//          <0=> Boot from ROM <1=> Boot from Flash
;//   <o0.2> Boot Flash selection
;//          <0=> Boot from Flash0 <1=> Boot from Flash1
GPNVM_BITS      EQU     0x02

;// </e>


                IF      GPNVM <> 0
                AREA    |.ARM.__AT_0x1FFFFFF0|, CODE, READONLY
                DCB     GPNVM_BITS
                ENDIF


                END
