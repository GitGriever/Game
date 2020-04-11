;/**************************************************************************//**
; * @file     startup_TMS470Mx.s
; * @brief    CMSIS Cortex-M3 Device Peripheral Access Layer Source File
; *           for the TI TMS470Mx Device Series
; * @version  V0.01
; * @date     16. August 2012
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; * @note
; * Copyright (C) 2012 ARM Limited. All rights reserved.
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


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     tms470_nmi                ;  0:  
                DCD     0                         ;  1:  Reserved
                DCD     esmLowInterrupt           ;  2:  
                DCD     sysSWInterrupt            ;  3:  
                DCD     rtiCompare0Interrupt      ;  4:  
                DCD     rtiCompare1Interrupt      ;  5:  
                DCD     rtiCompare2Interrupt      ;  6:  
                DCD     rtiCompare3Interrupt      ;  7:  
                DCD     rtiOverflow0Interrupt     ;  8:  
                DCD     rtiOverflow1Interrupt     ;  9:  
                DCD     0                         ; 10:  Reserved
                DCD     gioHighLevelInterrupt     ; 11:  
                DCD     gioLowLevelInterrupt      ; 12:  
                DCD     hetHighLevelInterrupt     ; 13:  
                DCD     hetLowLevelInterrupt      ; 14:  
                DCD     spi1LowLevelInterrupt     ; 15:  
                DCD     spi1HighLevelInterrupt    ; 16:  
                DCD     0                         ; 17:  Reserved
                DCD     sci2HighLevelInterrupt    ; 18:  
                DCD     sci2LowLevelInterrupt     ; 19:  
                DCD     sci1HighLevelInterrupt    ; 20:  
                DCD     sci1LowLevelInterrupt     ; 21:  
                DCD     can1HighLevelInterrupt    ; 22:  
                DCD     can1LowLevelInterrupt     ; 23:  
                DCD     adcGroup0Interrupt        ; 24:  
                DCD     adcGroup1Interrupt        ; 25:  
                DCD     adcGroup2Interrupt        ; 26:  
                DCD     spi2LowLevelInterrupt     ; 27:  
                DCD     spi2HighLevelInterrupt    ; 28:  
                DCD     can2HighLevelInterrupt    ; 29:  
                DCD     can2LowLevelInterrupt     ; 30:  
                DCD     0                         ; 31:  Reserved
                DCD     spi3LowLevelInterrupt     ; 32:  
                DCD     spi3HighLevelInterrupt    ; 33:  
                DCD     0                         ; 34:  Reserved
                DCD     0                         ; 35:  Reserved
                DCD     0                         ; 36:  Reserved
                DCD     0                         ; 37:  Reserved
                DCD     0                         ; 38:  Reserved
                DCD     0                         ; 39:  Reserved
                DCD     0                         ; 40:  Reserved
                DCD     0                         ; 41:  Reserved
                DCD     0                         ; 42:  Reserved
                DCD     0                         ; 43:  Reserved
                DCD     0                         ; 44:  Reserved
                DCD     0                         ; 45:  Reserved
                DCD     0                         ; 46:  Reserved
                DCD     0                         ; 47:  Reserved


                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  tms470_nmi               [WEAK]
                EXPORT  esmLowInterrupt          [WEAK]
                EXPORT  sysSWInterrupt           [WEAK]
                EXPORT  rtiCompare0Interrupt     [WEAK]
                EXPORT  rtiCompare1Interrupt     [WEAK]
                EXPORT  rtiCompare2Interrupt     [WEAK]
                EXPORT  rtiCompare3Interrupt     [WEAK]
                EXPORT  rtiOverflow0Interrupt    [WEAK]
                EXPORT  rtiOverflow1Interrupt    [WEAK]
                EXPORT  gioHighLevelInterrupt    [WEAK]
                EXPORT  gioLowLevelInterrupt     [WEAK]
                EXPORT  hetHighLevelInterrupt    [WEAK]
                EXPORT  hetLowLevelInterrupt     [WEAK]
                EXPORT  spi1LowLevelInterrupt    [WEAK]
                EXPORT  spi1HighLevelInterrupt   [WEAK]
                EXPORT  sci2HighLevelInterrupt   [WEAK]
                EXPORT  sci2LowLevelInterrupt    [WEAK]
                EXPORT  sci1HighLevelInterrupt   [WEAK]
                EXPORT  sci1LowLevelInterrupt    [WEAK]
                EXPORT  can1HighLevelInterrupt   [WEAK]
                EXPORT  can1LowLevelInterrupt    [WEAK]
                EXPORT  adcGroup0Interrupt       [WEAK]
                EXPORT  adcGroup1Interrupt       [WEAK]
                EXPORT  adcGroup2Interrupt       [WEAK]
                EXPORT  spi2LowLevelInterrupt    [WEAK]
                EXPORT  spi2HighLevelInterrupt   [WEAK]
                EXPORT  can2HighLevelInterrupt   [WEAK]
                EXPORT  can2LowLevelInterrupt    [WEAK]
                EXPORT  spi3LowLevelInterrupt    [WEAK]
                EXPORT  spi3HighLevelInterrupt   [WEAK]

tms470_nmi
esmLowInterrupt
sysSWInterrupt
rtiCompare0Interrupt
rtiCompare1Interrupt
rtiCompare2Interrupt
rtiCompare3Interrupt
rtiOverflow0Interrupt
rtiOverflow1Interrupt
gioHighLevelInterrupt
gioLowLevelInterrupt
hetHighLevelInterrupt
hetLowLevelInterrupt
spi1LowLevelInterrupt
spi1HighLevelInterrupt
sci2HighLevelInterrupt
sci2LowLevelInterrupt
sci1HighLevelInterrupt
sci1LowLevelInterrupt
can1HighLevelInterrupt
can1LowLevelInterrupt
adcGroup0Interrupt
adcGroup1Interrupt
adcGroup2Interrupt
spi2LowLevelInterrupt
spi2HighLevelInterrupt
can2HighLevelInterrupt
can2LowLevelInterrupt
spi3LowLevelInterrupt
spi3HighLevelInterrupt
                B       .
                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
