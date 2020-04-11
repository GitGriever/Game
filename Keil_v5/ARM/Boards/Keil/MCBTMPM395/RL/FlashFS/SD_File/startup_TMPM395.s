;/**************************************************************************//**
; * @file     startup_TMPM395.s
; * @brief    CMSIS Cortex-M3 Core Device Startup File for the
; *           Toshiba 'TMPM395' Device Series 
; * @version  V1.00
; * @date     09. June 2010
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


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000800

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
                DCD     INT0_IRQHandler           ;  0: Interrupt pin (PJ0)
                DCD     INT1_IRQHandler           ;  1: Interrupt pin (PJ1)
                DCD     INT2_IRQHandler           ;  2: Interrupt pin (PJ2)
                DCD     INT3_IRQHandler           ;  3: Interrupt pin (PJ3)
                DCD     INT4_IRQHandler           ;  4: Interrupt pin (PJ4)
                DCD     INT5_IRQHandler           ;  5: Interrupt pin (PJ5)
                DCD     INTRX0_IRQHandler         ;  6: Serial reception (channel.0)
                DCD     INTTX0_IRQHandler         ;  7: Serial transmit (channel.0)
                DCD     INTRX1_IRQHandler         ;  8: Serial reception (channel.1)
                DCD     INTTX1_IRQHandler         ;  9: Serial transmit (channel.1)
                DCD     I2CINT0_IRQHandler        ; 10: Serial bus interface 0
                DCD     INTSBI1_IRQHandler        ; 11: Serial bus interface 1
                DCD     INTCECRX_IRQHandler       ; 12: CEC reception
                DCD     INTCECTX_IRQHandler       ; 13: CEC transmission
                DCD     INTRMCRX0_IRQHandler      ; 14: Remote control signal reception (channel.0)
                DCD     INTADHP_IRQHandler        ; 15: Highest priority AD conversion complete interrupt
                DCD     INTADM0_IRQHandler        ; 16: AD conversion monitoring function interrupt 0
                DCD     INTADM1_IRQHandler        ; 17: AD conversion monitoring function interrupt 1
                DCD     INTTB0_IRQHandler         ; 18: 16bit TMRB match detection 0
                DCD     INTTB1_IRQHandler         ; 19: 16bit TMRB match detection 1
                DCD     INTTB2_IRQHandler         ; 20: 16bit TMRB match detection 2
                DCD     INTTB3_IRQHandler         ; 21: 16bit TMRB match detection 3
                DCD     INTTB4_IRQHandler         ; 22: 16bit TMRB match detection 4
                DCD     INTTB5_IRQHandler         ; 23: 16bit TMRB match detection 5
                DCD     INTTB6_IRQHandler         ; 24: 16bit TMRB match detection 6
                DCD     INTRTC_IRQHandler         ; 25: Real time clock timer
                DCD     INTCAP00_IRQHandler       ; 26: 16bit TMRB input capture 00
                DCD     INTCAP01_IRQHandler       ; 27: 16bit TMRB input capture 01
                DCD     INTCAP10_IRQHandler       ; 28: 16bit TMRB input capture 10
                DCD     INTCAP11_IRQHandler       ; 29: 16bit TMRB input capture 11
                DCD     INTCAP50_IRQHandler       ; 30: 16bit TMRB input capture 50
                DCD     INTCAP51_IRQHandler       ; 31: 16bit TMRB input capture 51
                DCD     INTCAP60_IRQHandler       ; 32: 16bit TMRB input capture 60
                DCD     INTCAP61_IRQHandler       ; 33: 16bit TMRB Input capture 61
                DCD     INT6_IRQHandler           ; 34: Interrupt pin (PJ6/39pin)
                DCD     INT7_IRQHandler           ; 35: Interrupt pin (PJ7/58pin)
                DCD     INTRX2_IRQHandler         ; 36: Serial reception (channel.2)
                DCD     INTTX2_IRQHandler         ; 37: Serial transmission (channel.2)
                DCD     INTLVD_IRQHandler         ; 38: Low voltage detection
                DCD     INTRMCRX1_IRQHandler      ; 39: Remote control signal reception (channel.1)
                DCD     INTTB7_IRQHandler         ; 40: 16bit TMRB match detection 7
                DCD     INTTB8_IRQHandler         ; 41: 16bit TMRB match detection 8
                DCD     INTPHT_IRQHandler         ; 42: 16bit TMRB (two phase pulse input counter)
                DCD     INTCAP20_IRQHandler       ; 43: 16bit TMRB input capture 20
                DCD     INTCAP21_IRQHandler       ; 44: 16bit TMRB input capture 21
                DCD     INTCAP30_IRQHandler       ; 45: 16bit TMRB input capture 30
                DCD     INTCAP31_IRQHandler       ; 46: 16bit TMRB input capture 31
                DCD     INTCAP40_IRQHandler       ; 47: 16bit TMRB input capture 40
                DCD     INTCAP41_IRQHandler       ; 48: 16bit TMRB input capture 41
                DCD     INTAD_IRQHandler          ; 49: A/D conversion completion
                DCD     INT8_IRQHandler           ; 50: Interrupt pin (PK3)
                DCD     INT9_IRQHandler           ; 51: Interrupt pin (PK4)
                DCD     INT10_IRQHandler          ; 52: Interrupt pin (PK5)
                DCD     INTSPI0_IRQHandler        ; 53: SPI serial interface (channel.0)
                DCD     INTSPI1_IRQHandler        ; 54: SPI serial interface (channel.1)
                DCD     INTSPI2_IRQHandler        ; 55: SPI serial interface (channel.2)
                DCD     INTSPI3_IRQHandler        ; 56: SPI serial interface (channel.3)


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

                EXPORT  INT0_IRQHandler           [WEAK]
                EXPORT  INT1_IRQHandler           [WEAK]
                EXPORT  INT2_IRQHandler           [WEAK]
                EXPORT  INT3_IRQHandler           [WEAK]
                EXPORT  INT4_IRQHandler           [WEAK]
                EXPORT  INT5_IRQHandler           [WEAK]
                EXPORT  INTRX0_IRQHandler         [WEAK]
                EXPORT  INTTX0_IRQHandler         [WEAK]
                EXPORT  INTRX1_IRQHandler         [WEAK]
                EXPORT  INTTX1_IRQHandler         [WEAK]
                EXPORT  I2CINT0_IRQHandler        [WEAK]
                EXPORT  INTSBI1_IRQHandler        [WEAK]
                EXPORT  INTCECRX_IRQHandler       [WEAK]
                EXPORT  INTCECTX_IRQHandler       [WEAK]
                EXPORT  INTRMCRX0_IRQHandler      [WEAK]
                EXPORT  INTADHP_IRQHandler        [WEAK]
                EXPORT  INTADM0_IRQHandler        [WEAK]
                EXPORT  INTADM1_IRQHandler        [WEAK]
                EXPORT  INTTB0_IRQHandler         [WEAK]
                EXPORT  INTTB1_IRQHandler         [WEAK]
                EXPORT  INTTB2_IRQHandler         [WEAK]
                EXPORT  INTTB3_IRQHandler         [WEAK]
                EXPORT  INTTB4_IRQHandler         [WEAK]
                EXPORT  INTTB5_IRQHandler         [WEAK]
                EXPORT  INTTB6_IRQHandler         [WEAK]
                EXPORT  INTRTC_IRQHandler         [WEAK]
                EXPORT  INTCAP00_IRQHandler       [WEAK]
                EXPORT  INTCAP01_IRQHandler       [WEAK]
                EXPORT  INTCAP10_IRQHandler       [WEAK]
                EXPORT  INTCAP11_IRQHandler       [WEAK]
                EXPORT  INTCAP50_IRQHandler       [WEAK]
                EXPORT  INTCAP51_IRQHandler       [WEAK]
                EXPORT  INTCAP60_IRQHandler       [WEAK]
                EXPORT  INTCAP61_IRQHandler       [WEAK]
                EXPORT  INT6_IRQHandler           [WEAK]
                EXPORT  INT7_IRQHandler           [WEAK]
                EXPORT  INTRX2_IRQHandler         [WEAK]
                EXPORT  INTTX2_IRQHandler         [WEAK]
                EXPORT  INTLVD_IRQHandler         [WEAK]
                EXPORT  INTRMCRX1_IRQHandler      [WEAK]
                EXPORT  INTTB7_IRQHandler         [WEAK]
                EXPORT  INTTB8_IRQHandler         [WEAK]
                EXPORT  INTPHT_IRQHandler         [WEAK]
                EXPORT  INTCAP20_IRQHandler       [WEAK]
                EXPORT  INTCAP21_IRQHandler       [WEAK]
                EXPORT  INTCAP30_IRQHandler       [WEAK]
                EXPORT  INTCAP31_IRQHandler       [WEAK]
                EXPORT  INTCAP40_IRQHandler       [WEAK]
                EXPORT  INTCAP41_IRQHandler       [WEAK]
                EXPORT  INTAD_IRQHandler          [WEAK]
                EXPORT  INT8_IRQHandler           [WEAK]
                EXPORT  INT9_IRQHandler           [WEAK]
                EXPORT  INT10_IRQHandler          [WEAK]
                EXPORT  INTSPI0_IRQHandler        [WEAK]
                EXPORT  INTSPI1_IRQHandler        [WEAK]
                EXPORT  INTSPI2_IRQHandler        [WEAK]
                EXPORT  INTSPI3_IRQHandler        [WEAK]

INT0_IRQHandler
INT1_IRQHandler
INT2_IRQHandler
INT3_IRQHandler
INT4_IRQHandler
INT5_IRQHandler
INTRX0_IRQHandler
INTTX0_IRQHandler
INTRX1_IRQHandler
INTTX1_IRQHandler
I2CINT0_IRQHandler
INTSBI1_IRQHandler
INTCECRX_IRQHandler
INTCECTX_IRQHandler
INTRMCRX0_IRQHandler
INTADHP_IRQHandler
INTADM0_IRQHandler
INTADM1_IRQHandler
INTTB0_IRQHandler
INTTB1_IRQHandler
INTTB2_IRQHandler
INTTB3_IRQHandler
INTTB4_IRQHandler
INTTB5_IRQHandler
INTTB6_IRQHandler
INTRTC_IRQHandler
INTCAP00_IRQHandler
INTCAP01_IRQHandler
INTCAP10_IRQHandler
INTCAP11_IRQHandler
INTCAP50_IRQHandler
INTCAP51_IRQHandler
INTCAP60_IRQHandler
INTCAP61_IRQHandler
INT6_IRQHandler
INT7_IRQHandler
INTRX2_IRQHandler
INTTX2_IRQHandler
INTLVD_IRQHandler
INTRMCRX1_IRQHandler
INTTB7_IRQHandler
INTTB8_IRQHandler
INTPHT_IRQHandler
INTCAP20_IRQHandler
INTCAP21_IRQHandler
INTCAP30_IRQHandler
INTCAP31_IRQHandler
INTCAP40_IRQHandler
INTCAP41_IRQHandler
INTAD_IRQHandler
INT8_IRQHandler
INT9_IRQHandler
INT10_IRQHandler
INTSPI0_IRQHandler
INTSPI1_IRQHandler
INTSPI2_IRQHandler
INTSPI3_IRQHandler

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
