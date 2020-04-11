;/*****************************************************************************/
;/* STM32W108_OPT.s: STM32W108 Flash Option Bytes                             */
;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2012 Keil Software. All rights reserved.                    */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/


;// <e> Flash Option Bytes
FLASH_OPT       EQU     1

;// <h> Flash Read Protection
;//     <i> Read protection is used to protect the software code stored in Flash memory
;//   <o0> Read Protection
;//          <0x5A=> Disabled 
;//          <0xFF=> Enabled
;// </h>
RDP            EQU     0x5A
nRDP           EQU     RDP:EOR:0xFF

;// <h> Flash Write Protection
;//   <o0.0> WRP0.0 Pages
;//   <o0.1> WRP0.1 Pages
;//   <o0.2> WRP0.2 Pages
;//   <o0.3> WRP0.3 Pages
;//   <o0.4> WRP0.4 Pages
;//   <o0.5> WRP0.5 Pages
;//   <o0.6> WRP0.6 Pages
;//   <o0.7> WRP0.7 Pages
;//   <o1.0> WRP1.0 Pages
;//   <o1.1> WRP1.1 Pages
;//   <o1.2> WRP1.2 Pages
;//   <o1.3> WRP1.3 Pages
;//   <o1.4> WRP1.4 Pages
;//   <o1.5> WRP1.5 Pages
;//   <o1.6> WRP1.6 Pages
;//   <o1.7> WRP1.7 Pages
;//   <o2.0> WRP2.0 Pages
;//   <o2.1> WRP2.1 Pages
;//   <o2.2> WRP2.2 Pages
;//   <o2.3> WRP2.3 Pages
;//   <o2.4> WRP2.4 Pages
;//   <o2.5> WRP2.5 Pages
;//   <o2.6> WRP2.6 Pages
;//   <o2.7> WRP2.7 Pages
;//   <o3.0> WRP3.0 Pages
;//   <o3.1> WRP3.1 Pages
;//   <o3.2> WRP3.2 Pages
;//   <o3.3> WRP3.3 Pages
;//   <o3.4> WRP3.4 Pages
;//   <o3.5> WRP3.5 Pages
;//   <o3.6> WRP3.6 Pages
;//   <o3.7> WRP3.7 Pages
;// </h>
nWRP0           EQU     0x00
nWRP1           EQU     0x00
nWRP2           EQU     0x00
nWRP3           EQU     0x00
WRP0            EQU     nWRP0:EOR:0xFF
WRP1            EQU     nWRP1:EOR:0xFF
WRP2            EQU     nWRP2:EOR:0xFF
WRP3            EQU     nWRP3:EOR:0xFF

;// </e>

RSVD            EQU     0xFF
nRSVD           EQU     RSVD;USER:EOR:0xFF



                IF      FLASH_OPT <> 0
                AREA    |.ARM.__AT_0x08040800|, CODE, READONLY
                DCB     RDP,   nRDP,   RSVD,  nRSVD
                DCB     RSVD,  nRSVD,  RSVD,  nRSVD
                DCB     WRP0,  nWRP0,  WRP1,  nWRP1
                DCB     WRP2,  nWRP2,  WRP3,  nWRP3
                ENDIF


                END
