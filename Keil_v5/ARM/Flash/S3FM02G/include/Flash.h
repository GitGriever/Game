/*----------------------------------------------------------------------------
 * Name:    Flash.h
 * Purpose: Register Definition: S3FM02G Device Flash
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/



// Watchdog Registers
#define WDT_OMR         (*((volatile unsigned long  *) 0x4003000C))

// Clock Manager Controller Registers
#define CM_CSR          (*((volatile unsigned long  *) 0x40020008))
#define CM_PCSR0        (*((volatile unsigned long  *) 0x40020010))
#define CM_PCSR1        (*((volatile unsigned long  *) 0x40020014))
#define CM_MR1          (*((volatile unsigned long  *) 0x4002002C))
#define CM_SR           (*((volatile unsigned long  *) 0x40020040))
#define CM_SCDR         (*((volatile unsigned long  *) 0x40020044))
#define CM_PCDR         (*((volatile unsigned long  *) 0x40020048))

#define DFC_BASE_ADDRESS     (0x40011000UL)
#define PFC_BASE_ADDRESS     (0x40010000UL)

// Program Flash Controller(PFC) Registers
#define PFC_IDR         (*((volatile unsigned long  *) (PFC_BASE_ADDRESS + 0x00UL)))
#define PFC_CEDR        (*((volatile unsigned long  *) (PFC_BASE_ADDRESS + 0x04UL)))
#define PFC_SRR         (*((volatile unsigned long  *) (PFC_BASE_ADDRESS + 0x08UL)))
#define PFC_CR          (*((volatile unsigned long  *) (PFC_BASE_ADDRESS + 0x0CUL)))
#define PFC_MR          (*((volatile unsigned long  *) (PFC_BASE_ADDRESS + 0x10UL)))
#define PFC_IMSCR       (*((volatile unsigned long  *) (PFC_BASE_ADDRESS + 0x14UL)))
#define PFC_RISR        (*((volatile unsigned long  *) (PFC_BASE_ADDRESS + 0x18UL)))
#define PFC_MISR        (*((volatile unsigned long  *) (PFC_BASE_ADDRESS + 0x1CUL)))
#define PFC_ICR         (*((volatile unsigned long  *) (PFC_BASE_ADDRESS + 0x20UL)))
#define PFC_SR          (*((volatile unsigned long  *) (PFC_BASE_ADDRESS + 0x24UL)))
#define PFC_AR          (*((volatile unsigned long  *) (PFC_BASE_ADDRESS + 0x28UL)))
#define PFC_DR          (*((volatile unsigned long  *) (PFC_BASE_ADDRESS + 0x2CUL)))
#define PFC_KR          (*((volatile unsigned long  *) (PFC_BASE_ADDRESS + 0x30UL)))
#define PFC_SO_PSR      (*((volatile unsigned long  *) (PFC_BASE_ADDRESS + 0x34UL)))
#define PFC_SO_CSR      (*((volatile unsigned long  *) (PFC_BASE_ADDRESS + 0x38UL)))
#define PFC_IOTR        (*((volatile unsigned long  *) (PFC_BASE_ADDRESS + 0x3CUL)))


// Data Flash Controller(DFC) Registers
#define DFC_IDR         (*((volatile unsigned long  *) (DFC_BASE_ADDRESS + 0x00UL)))
#define DFC_CEDR        (*((volatile unsigned long  *) (DFC_BASE_ADDRESS + 0x04UL)))
#define DFC_SRR         (*((volatile unsigned long  *) (DFC_BASE_ADDRESS + 0x08UL)))
#define DFC_CR          (*((volatile unsigned long  *) (DFC_BASE_ADDRESS + 0x0CUL)))
#define DFC_MR          (*((volatile unsigned long  *) (DFC_BASE_ADDRESS + 0x10UL)))
#define DFC_IMSCR       (*((volatile unsigned long  *) (DFC_BASE_ADDRESS + 0x14UL)))
#define DFC_RISR        (*((volatile unsigned long  *) (DFC_BASE_ADDRESS + 0x18UL)))
#define DFC_MISR        (*((volatile unsigned long  *) (DFC_BASE_ADDRESS + 0x1CUL)))
#define DFC_ICR         (*((volatile unsigned long  *) (DFC_BASE_ADDRESS + 0x20UL)))
#define DFC_AR          (*((volatile unsigned long  *) (DFC_BASE_ADDRESS + 0x24UL)))
#define DFC_DR          (*((volatile unsigned long  *) (DFC_BASE_ADDRESS + 0x28UL)))
#define DFC_KR          (*((volatile unsigned long  *) (DFC_BASE_ADDRESS + 0x2CUL)))
#define DFC_PCR         (*((volatile unsigned long  *) (DFC_BASE_ADDRESS + 0x30UL)))

// PF Flash Command List
#define PFC_Program                 (1<<4)
#define PFC_PageErase               (2<<4)
#define PFC_SectorErase             (3<<4)
#define PFC_ChipErase               (4<<4)
#define PFC_SmartProgram            (5<<4)
#define PFC_SmartErase              (6<<4)

//DF Flash Command List
#define DFC_ByteProgram                     (1<<4)
#define DFC_HWProgram                       (2<<4)
#define DFC_WordProgram                     (3<<4)
#define DFC_PageErase                       (4<<4)
#define DFC_SectorErase                     (5<<4)
#define DFC_ChipErase                       (6<<4)
#define DFCPageErase                        (4<<4)
#define DFCSectorErase                      (5<<4)
#define DFCChipErase                        (6<<4)

// PF Bit Constants
#define PFC_START       (0x01ul << 0)
#define PFC_END         (0x01ul << 0)

//DF Bit Constant
#define DFC_START       (0x01ul << 0)
#define DFC_DFEN        (0x01ul << 1)
#define DFC_DFSTABLE    (0x01ul << 2)
#define DFC_CMD_MASK    (0x07ul << 4)

#define DFC_END         (0x01ul << 0)   
#define DFC_ERR0        (0x01ul << 8)   
#define DFC_ERR1        (0x01ul << 9)   
#define DFC_ERR2        (0x01ul << 10)  


#define PFC_END  	    (0x01ul << 0)
#define PFC_ERR0  	    (0x01ul << 8)
#define PFC_ERR1  	    (0x01ul << 9)
#define PFC_ERR2  	    (0x01ul << 10)


#define IOCONF_P0MLR      (*((volatile unsigned long  *) 0x40058000))
#define IOCONF_P1MLR      (*((volatile unsigned long  *) 0x40058010))

#define GPIO_CEDR         (*((volatile unsigned long  *) 0x40053004))
#define GPIO_OER          (*((volatile unsigned long  *) 0x4005301C))
#define GPIO_WODR         (*((volatile unsigned long  *) 0x40053028))
#define GPIO_ODSR         (*((volatile unsigned long  *) 0x40053034))


#define USART_CEDR        (*((volatile unsigned long  *) 0x40082004))
#define USART_CR          (*((volatile unsigned long  *) 0x4008200C))
#define USART_MR          (*((volatile unsigned long  *) 0x40082010))
#define USART_SR          (*((volatile unsigned long  *) 0x40082024))
#define USART_THR         (*((volatile unsigned long  *) 0x4008202C))
#define USART_BRGR         (*((volatile unsigned long  *) 0x40082030))

