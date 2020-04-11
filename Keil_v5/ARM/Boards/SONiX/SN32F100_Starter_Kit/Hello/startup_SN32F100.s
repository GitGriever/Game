;/******************************************************************************
; * @file     startup_SN32F100.s
; * @brief    CMSIS Cortex-M0 Core Device Startup File
; *           for the SONIX SN32F100 Device Series
; * @version  V1.0
; * @date     November 2012
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; * @note
; * Copyright (C) 2009-2010 ARM Limited. All rights reserved.
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

Stack_Size		EQU		0x00000200

				AREA	STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem		SPACE	Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size		EQU		0x00000000

				AREA	HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem		SPACE	Heap_Size
__heap_limit


				PRESERVE8
				THUMB


; Vector Table Mapped to Address 0 at Reset

				AREA	RESET, DATA, READONLY
				EXPORT	__Vectors

__Vectors		DCD		__initial_sp				; Top of Stack
				DCD		Reset_Handler				; Reset Handler
				DCD		NMI_Handler					; NMI Handler
                DCD     HardFault_Handler			; Hard Fault Handler
				DCD		0							; Reserved
				DCD		0							; Reserved
				DCD		0							; Reserved
				DCD		0							; Reserved
				DCD		0							; Reserved
				DCD		0							; Reserved
				DCD		0							; Reserved
				DCD		SVC_Handler					; SVCall Handler
				DCD		0							; Reserved
				DCD		0							; Reserved
				DCD		PendSV_Handler				; PendSV Handler
				DCD		SysTick_Handler				; SysTick Handler

				; External Interrupts
				DCD		P0_IRQHandler				; 16+ 0: GPIO INT0
				DCD		P1_IRQHandler				; 16+ 1: GPIO INT1
				DCD		P2_IRQHandler				; 16+ 2: GPIO INT2
				DCD		P3_IRQHandler				; 16+ 3: GPIO INT3
				DCD		0							; 16+ 4: Reserved
				DCD		0							; 16+ 5: Reserved
				DCD		0							; 16+ 6: Reserved
				DCD		0							; 16+ 7: Reserved
				DCD		0							; 16+ 8: Reserved
				DCD		0							; 16+ 9: Reserved
				DCD		0							; 16+10: Reserved
				DCD		0							; 16+11: Reserved
				DCD		0							; 16+12: Reserved
				DCD		0 							; 16+13: Reserved
				DCD		0 							; 16+14: Reserved
				DCD     0 							; 16+15: Reserved
				DCD     0 							; 16+16: Reserved
				DCD     CMP_IRQHandler		 		; 16+17: Comparator
				DCD     CT16B0_IRQHandler  			; 16+18: 16-bit Counter-Timer 0
				DCD     CT16B1_IRQHandler  			; 16+19: 16-bit Counter-Timer 1
				DCD     CT32B0_IRQHandler  			; 16+20: 32-bit Counter-Timer 0
				DCD     CT32B1_IRQHandler  			; 16+21: 32-bit Counter-Timer 1
				DCD     I2S_IRQHandler     			; 16+22: I2S
				DCD		SSP0_IRQHandler  			; 16+23: SPI/SSP0
				DCD		SSP1_IRQHandler   			; 16+24: SPI/SSP1
				DCD		UART0_IRQHandler 			; 16+25: UART0
				DCD		UART1_IRQHandler 			; 16+26: UART1
				DCD		I2C0_IRQHandler 			; 16+27: I2C0
				DCD		I2C1_IRQHandler				; 16+28: I2C1
				DCD		WDT_IRQHandler				; 16+29: Watchdog Timer
				DCD		LVD_IRQHandler				; 16+30: Low Voltage Detect
				DCD		RTC_IRQHandler				; 16+31: Real Time Clock




				AREA	|.text|, CODE, READONLY

; Reset Handler

Reset_Handler	PROC
				EXPORT  Reset_Handler			[WEAK]
				IMPORT  SystemInit
				IMPORT  __main
				LDR     R0, =SystemInit
				BLX     R0
				LDR     R0, =__main
				BX      R0
				ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler		PROC
				EXPORT	NMI_Handler				[WEAK]
				B		.
				ENDP
HardFault_Handler	PROC
				EXPORT	HardFault_Handler		[WEAK]
				B		.
				ENDP
SVC_Handler		PROC
				EXPORT	SVC_Handler				[WEAK]
				B		.
				ENDP
PendSV_Handler	PROC
				EXPORT	PendSV_Handler			[WEAK]
				B		.
				ENDP
SysTick_Handler	PROC
				EXPORT	SysTick_Handler			[WEAK]
				B		.
				ENDP
Default_Handler	PROC
				EXPORT		P0_IRQHandler			[WEAK]
				EXPORT		P1_IRQHandler			[WEAK]
				EXPORT		P2_IRQHandler			[WEAK]
				EXPORT		P3_IRQHandler			[WEAK]
				EXPORT		CMP_IRQHandler		 	[WEAK]
				EXPORT		CT16B0_IRQHandler 		[WEAK]
				EXPORT		CT16B1_IRQHandler		[WEAK]
				EXPORT		CT32B0_IRQHandler		[WEAK]
				EXPORT		CT32B1_IRQHandler		[WEAK]
				EXPORT		I2S_IRQHandler			[WEAK]
				EXPORT		SSP0_IRQHandler			[WEAK]
				EXPORT		SSP1_IRQHandler			[WEAK]
				EXPORT		UART0_IRQHandler		[WEAK]
				EXPORT		UART1_IRQHandler		[WEAK]
				EXPORT		I2C0_IRQHandler			[WEAK]
				EXPORT		I2C1_IRQHandler			[WEAK]
				EXPORT		WDT_IRQHandler			[WEAK]
				EXPORT		LVD_IRQHandler			[WEAK]
				EXPORT		RTC_IRQHandler			[WEAK]

P0_IRQHandler
P1_IRQHandler
P2_IRQHandler
P3_IRQHandler
CMP_IRQHandler
CT16B0_IRQHandler
CT16B1_IRQHandler
CT32B0_IRQHandler
CT32B1_IRQHandler
I2S_IRQHandler
SSP0_IRQHandler
SSP1_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
WDT_IRQHandler
LVD_IRQHandler
RTC_IRQHandler

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
