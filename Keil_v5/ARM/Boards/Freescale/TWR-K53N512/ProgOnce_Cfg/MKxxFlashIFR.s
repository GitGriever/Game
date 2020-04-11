;/**************************************************************************//**
; * @file     MKXXFlashIFR.s
; * @brief    Program Once Memory (Program Flash IFR)
; *           Freescale 'MKXX' Device Series 
; * @version  V1.01
; * @date     04. Oct 2010
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; * @note
; * Copyright (C) 2010 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M 
; * processor based microcontrollers.  This file can be freely distributed 
; * within development tools that are supporting such ARM based processors. 
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/

;  <h>  Program Once Memory (Program Flash IFR)
;    <i>  Nonvolatile memory information memory in Program Flash area
;    <o0>  Program Once Field 0  <0x00-0xFF>
;    <o1>  Program Once Field 1  <0x00-0xFF>
;    <o2>  Program Once Field 2  <0x00-0xFF>
;    <o3>  Program Once Field 3  <0x00-0xFF>
;    <o4>  Program Once Field 4  <0x00-0xFF>
;    <o5>  Program Once Field 5  <0x00-0xFF>
;    <o6>  Program Once Field 6  <0x00-0xFF>
;    <o7>  Program Once Field 7  <0x00-0xFF>
;    <o8>  Program Once Field 8  <0x00-0xFF>
;    <o9>  Program Once Field 9  <0x00-0xFF>
;    <o10> Program Once Field 10 <0x00-0xFF>
;    <o11> Program Once Field 11 <0x00-0xFF>
;    <o12> Program Once Field 12 <0x00-0xFF>
;    <o13> Program Once Field 13 <0x00-0xFF>
;    <o14> Program Once Field 14 <0x00-0xFF>
;    <o15> Program Once Field 15 <0x00-0xFF>
;    <o13> Program Once Field 13 <0x00-0xFF>
;    <o14> Program Once Field 14 <0x00-0xFF>
;    <o15> Program Once Field 15 <0x00-0xFF>
;    <o16> Program Once Field 16 <0x00-0xFF>
;    <o17> Program Once Field 17 <0x00-0xFF>
;    <o18> Program Once Field 18 <0x00-0xFF>
;    <o19> Program Once Field 19 <0x00-0xFF>
;    <o20> Program Once Field 20 <0x00-0xFF>
;    <o21> Program Once Field 21 <0x00-0xFF>
;    <o22> Program Once Field 22 <0x00-0xFF>
;    <o23> Program Once Field 23 <0x00-0xFF>
;    <o24> Program Once Field 24 <0x00-0xFF>
;    <o25> Program Once Field 25 <0x00-0xFF>
;    <o26> Program Once Field 26 <0x00-0xFF>
;    <o27> Program Once Field 27 <0x00-0xFF>
;    <o28> Program Once Field 28 <0x00-0xFF>
;    <o29> Program Once Field 29 <0x00-0xFF>
;    <o30> Program Once Field 30 <0x00-0xFF>
;    <o31> Program Once Field 31 <0x00-0xFF>
;    <o32> Program Once Field 32 <0x00-0xFF>
;    <o33> Program Once Field 33 <0x00-0xFF>
;    <o34> Program Once Field 34 <0x00-0xFF>
;    <o35> Program Once Field 35 <0x00-0xFF>
;    <o36> Program Once Field 36 <0x00-0xFF>
;    <o37> Program Once Field 37 <0x00-0xFF>
;    <o38> Program Once Field 38 <0x00-0xFF>
;    <o39> Program Once Field 39 <0x00-0xFF>
;    <o40> Program Once Field 40 <0x00-0xFF>
;    <o41> Program Once Field 41 <0x00-0xFF>
;    <o42> Program Once Field 42 <0x00-0xFF>
;    <o43> Program Once Field 43 <0x00-0xFF>
;    <o44> Program Once Field 44 <0x00-0xFF>
;    <o45> Program Once Field 45 <0x00-0xFF>
;    <o46> Program Once Field 46 <0x00-0xFF>
;    <o47> Program Once Field 47 <0x00-0xFF>
;    <o48> Program Once Field 48 <0x00-0xFF>
;    <o49> Program Once Field 49 <0x00-0xFF>
;    <o50> Program Once Field 50 <0x00-0xFF>
;    <o51> Program Once Field 51 <0x00-0xFF>
;    <o52> Program Once Field 52 <0x00-0xFF>
;    <o53> Program Once Field 53 <0x00-0xFF>
;    <o54> Program Once Field 54 <0x00-0xFF>
;    <o55> Program Once Field 55 <0x00-0xFF>
;    <o56> Program Once Field 56 <0x00-0xFF>
;    <o57> Program Once Field 57 <0x00-0xFF>
;    <o58> Program Once Field 58 <0x00-0xFF>
;    <o59> Program Once Field 59 <0x00-0xFF>
;    <o60> Program Once Field 60 <0x00-0xFF>
;    <o61> Program Once Field 61 <0x00-0xFF>
;    <o62> Program Once Field 62 <0x00-0xFF>
;    <o63> Program Once Field 63 <0x00-0xFF>
;    <o64> Program Once Field 64 <0x00-0xFF>
POF0            EQU     0xFF
POF1            EQU     0xFF
POF2            EQU     0xFF
POF3            EQU     0xFF
POF4            EQU     0xFF
POF5            EQU     0xFF
POF6            EQU     0xFF
POF7            EQU     0xFF
POF8            EQU     0xFF
POF9            EQU     0xFF
POF10           EQU     0xFF
POF11           EQU     0xFF
POF12           EQU     0xFF
POF13           EQU     0xFF
POF14           EQU     0xFF
POF15           EQU     0xFF
POF16           EQU     0xFF
POF17           EQU     0xFF
POF18           EQU     0xFF
POF19           EQU     0xFF
POF20           EQU     0xFF
POF21           EQU     0xFF
POF22           EQU     0xFF
POF23           EQU     0xFF
POF24           EQU     0xFF
POF25           EQU     0xFF
POF26           EQU     0xFF
POF27           EQU     0xFF
POF28           EQU     0xFF
POF29           EQU     0xFF
POF30           EQU     0xFF
POF31           EQU     0xFF
POF32           EQU     0xFF
POF33           EQU     0xFF
POF34           EQU     0xFF
POF35           EQU     0xFF
POF36           EQU     0xFF
POF37           EQU     0xFF
POF38           EQU     0xFF
POF39           EQU     0xFF
POF40           EQU     0xFF
POF41           EQU     0xFF
POF42           EQU     0xFF
POF43           EQU     0xFF
POF44           EQU     0xFF
POF45           EQU     0xFF
POF46           EQU     0xFF
POF47           EQU     0xFF
POF48           EQU     0xFF
POF49           EQU     0xFF
POF50           EQU     0xFF
POF51           EQU     0xFF
POF52           EQU     0xFF
POF53           EQU     0xFF
POF54           EQU     0xFF
POF55           EQU     0xFF
POF56           EQU     0xFF
POF57           EQU     0xFF
POF58           EQU     0xFF
POF59           EQU     0xFF
POF60           EQU     0xFF
POF61           EQU     0xFF
POF62           EQU     0xFF
POF63           EQU     0xFF
POF64           EQU     0xFF
;  </h>

                AREA    |.ARM.__AT_0x0FFFFFC0|, CODE, READONLY
                DCB     POF0,  POF1,  POF2,  POF3 
                DCB     POF4,  POF5,  POF6,  POF7 
                DCB     POF8,  POF9,  POF10, POF11 
                DCB     POF12, POF13, POF14, POF15 
                DCB     POF16, POF17, POF18, POF19 
                DCB     POF20, POF21, POF22, POF23 
                DCB     POF24, POF25, POF26, POF27 
                DCB     POF28, POF29, POF30, POF31 
                DCB     POF32, POF33, POF34, POF35 
                DCB     POF36, POF37, POF38, POF39 
                DCB     POF40, POF41, POF42, POF43 
                DCB     POF44, POF45, POF46, POF47
                DCB     POF48, POF49, POF50, POF51
                DCB     POF52, POF53, POF54, POF55
                DCB     POF56, POF57, POF58, POF59
                DCB     POF60, POF61, POF62, POF63 


                END
