;/*****************************************************************************/
;/* S3FM02G_SOPT.S: S3FM02G Smart Option Bytes                              */
;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2005-2011 Keil Software. All rights reserved.               */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/


;// <e> Smart Option Bytes
SMART_OPT       EQU     1
;// <h> SO_CSR (Smart Option Configuration Status Register)
;//   <o0.0..1> POCCS  
;//     <i> Power-On System Clock Source Selection Field
;//          <0=> ESCLK <1=> EMCLK <2=> ISCLK <3=> IMCLK
;//   <o0.6..7> IMSEL  
;//     <i> Internal Main OSC Frequency Selection Field
;//          <1=> 8MHz <2=> 16MHz <3=> 20MHz
;//   <o0.12..15> BTDIV  
;//     <i> Basic timer divider selection bit in the reset time
;//          <3=> 1 <4=> 2 <5=> 4 <6=> 8 <7=> 16 <8=> 32 <9=> 64 <10=> 128 <11=> 256 <12=> 512 <13=> 1024 <14=> 2048 <15=> 4096
;// </h>
SO_CSR            EQU     0xFFFF
nSO_CSR           EQU     SO_CSR:OR:0xFFFF0F3C   ;Keep reserved Bits

;// <h> SO_PSR (Smart Option Protection Status Register)
;     <i> WARNING: If nJTAGP field is configured as "JTAG Protection is enabled",
;     <i> access via the debugger is blocked !!!!!!!
;//   <o0.8> nJTAGP  
;//     <i> JTAG Protection Status Flag
;//          <0=> JTAG Protection is enabled <1=> JTAG Protection is disabled
;//   <o0.17> nHWP  
;//     <i> Hardware Protection Status Flag
;//          <0=> Hardware Protection is enabled <1=> Hardware Protection is disabled
;//   <o0.27> nRDP  
;//     <i> Read Protection Status Flag
;//          <0=> Serial Read Protection is enabled <1=> Serial Read Protection is disabled
;//   <o0.4> HWPA0  
;//     <i> Hardware Protection Area 0
;//          <0=> Protected <1=> Not Protected
;//   <o0.5> HWPA1  
;//     <i> Hardware Protection Area 1
;//          <0=> Protected <1=> Not Protected
;//   <o0.6> HWPA2  
;//     <i> Hardware Protection Area 2
;//          <0=> Protected <1=> Not Protected
;//   <o0.7> HWPA3  
;//     <i> Hardware Protection Area 3
;//          <0=> Protected <1=> Not Protected
;//   <o0.12> HWPA4  
;//     <i> Hardware Protection Area 4
;//          <0=> Protected <1=> Not Protected
;//   <o0.13> HWPA5  
;//     <i> Hardware Protection Area 5
;//          <0=> Protected <1=> Not Protected
;//   <o0.14> HWPA6  
;//     <i> Hardware Protection Area 6
;//          <0=> Protected <1=> Not Protected
;//   <o0.15> HWPA7  
;//     <i> Hardware Protection Area 7
;//          <0=> Protected <1=> Not Protected
;//   <o0.20> HWPA8  
;//     <i> Hardware Protection Area 8
;//          <0=> Protected <1=> Not Protected
;//   <o0.21> HWPA9  
;//     <i> Hardware Protection Area 9
;//          <0=> Protected <1=> Not Protected
;//   <o0.22> HWPA10  
;//     <i> Hardware Protection Area 10
;//          <0=> Protected <1=> Not Protected
;//   <o0.23> HWPA11  
;//     <i> Hardware Protection Area 11
;//          <0=> Protected <1=> Not Protected

;// </h>
SO_PSR            EQU     0xFFFFFFFF
nSO_PSR           EQU     SO_PSR:OR:0xF70D0E0F     ;Keep reserved Bits
;// </e>


                IF      SMART_OPT <> 0
                AREA    |.ARM.__AT_0x1FFFF400|, CODE, READONLY
                DCD     nSO_CSR,  nSO_PSR
                ENDIF


                END
