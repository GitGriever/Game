/*----------------------------------------------------------------------------
 * Name:    Flash.h
 * Purpose: Register Definition: S3FN429 Device Flash
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

/************************* Device Register & Bit Definition ***************************/
#define	__I     volatile const  /*!< defines 'read only' permissions */
#define __O     volatile        /*!< defines 'write only' permissions */
#define __IO    volatile        /*!< defines 'read / write' permissions */

/**
  * @brief Clock Manager (CM)
  */
typedef struct {                            /*!< CM Structure                          */
  __I  uint32_t  IDR;                       /*!< ID Register                           */
  __O  uint32_t  SRR;                       /*!< Software Reset Register               */
  __O  uint32_t  CSR;                       /*!< Control Set Register                  */
  __O  uint32_t  CCR;                       /*!< Control Clear Register                */
  __O  uint32_t  PCSR;                      /*!< Peripheral Clock Set Register         */
  __I  uint32_t  RESERVED0;
  __O  uint32_t  PCCR;                      /*!< Peripheral Clock Clear Register       */
  __I  uint32_t  RESERVED1;
  __I  uint32_t  PCKSR;                     /*!< Peripheral Clock Status Register      */
  __I  uint32_t  RESERVED2;
  __IO uint32_t  MR0;                       /*!< Mode Register 0                       */
  __IO uint32_t  MR1;                       /*!< Mode Register 1                       */
  __IO uint32_t  IMSCR;                     /*!< Interrupt Mask Set/Clear Register     */
  __I  uint32_t  RISR;                      /*!< Raw Interrupt Status Register         */
  __I  uint32_t  MISR;                      /*!< Masked Interrupt Status Register      */
  __O  uint32_t  ICR;                       /*!< Interrupt Clear Register              */
  __IO uint32_t  SR;                        /*!< Status Register                       */
  __IO uint32_t  SCDR;                      /*!< System Clock Divider Register         */
  __IO uint32_t  PCDR;                      /*!< Peripheral Clock Divider Register     */
  __I  uint32_t  RESERVED3[3];
  __IO uint32_t  PSTR;                      /*!< PLL Stabilization Time Register       */
  __IO uint32_t  PDPR;                      /*!< PLL Divider Parameter Register        */
  __I  uint32_t  RESERVED4[4];
  __IO uint32_t  BTCDR;                     /*!< Basic Timer Clock Divider Register    */
  __IO uint32_t  BTR;                       /*!< Basic Timer Value Register            */
  __IO uint32_t  EECR0;                     /*!< External Event Control Register 0     */
  __IO uint32_t  EECR1;                     /*!< External Event Control Register 1     */
  __I  uint32_t  RESERVED5[2];
  __IO uint32_t  EEIMSCR;                   /*!< External Event Interrupt Mask Set/Clear Register */
  __I  uint32_t  EERISR;                    /*!< External Event Raw Interrupt Status Register */
  __I  uint32_t  EEMISR;                    /*!< External Event Masked Interrupt Status Register */
  __O  uint32_t  EEICR;                     /*!< External Event Interrupt Clear Register */
  __IO uint32_t  NISR;                      /*!< NVIC Interrupt Status Regsiter        */
  __I  uint32_t  RESERVED6[2];
  __I  uint32_t  PSR;                       /*!< Power Status Register                 */
} CM_Type;

/**
  * @brief Internal Flash Controller (IFC) has an on-chip program flash ROM, internally. The flash memory size is 32 KB (IFC)
  */
typedef struct {                            /*!< IFC Structure                         */
  __I  uint32_t  IDR;                       /*!< ID Register                           */
  __IO uint32_t  CEDR;                      /*!< Clock Enable/Disable Register         */
  __O  uint32_t  SRR;                       /*!< Software Reset Register               */
  __IO uint32_t  CR;                        /*!< Control Register                      */
  __IO uint32_t  MR;                        /*!< Mode Register                         */
  __IO uint32_t  IMSCR;                     /*!< Interrupt Mask Set/ Clear Register    */
  __I  uint32_t  RISR;                      /*!< Raw Interrupt Status Register         */
  __I  uint32_t  MISR;                      /*!< Masked Interrupt Status Register      */
  __O  uint32_t  ICR;                       /*!< Interrupt Clear Register              */
  __I  uint32_t  SR;                        /*!< Status Register                       */
  __IO uint32_t  AR;                        /*!< Address Register                      */
  __IO uint32_t  DR;                        /*!< Data Register                         */
  __O  uint32_t  KR;                        /*!< Key Register                          */
  __I  uint32_t  SOPSR;                     /*!< Smart Option Protection Status Register  */
  __I  uint32_t  SOCSR;                     /*!< Smart Option Configuration Status Register  */
  __IO uint32_t  IOTR;                      /*!< Internal OSC Trimming Register        */
} IFC_Type;

/**
  * @brief Watchdog Timer (WDT)
  */
typedef struct {                            /*!< WDT Structure                         */
  __I  uint32_t  IDR;                       /*!< Watchdog ID Register                  */
  __O  uint32_t  CR;                        /*!< Watchdog Control Register             */
  __IO uint32_t  MR;                        /*!< Watchdog Mode Register                */
  __IO uint32_t  OMR;                       /*!< Watchdog Overflow Mode Register       */
  __I  uint32_t  SR;                        /*!< Watchdog Status Register              */
  __IO uint32_t  IMSCR;                     /*!< Watchdog Interrupt Mask Set /Clear Register */
  __I  uint32_t  RISR;                      /*!< Watchdog Raw Interrupt Status Register */
  __I  uint32_t  MISR;                      /*!< Watchdog Masked Interrupt Status Register */
  __O  uint32_t  ICR;                       /*!< Watchdog Interrupt Clear Register     */
  __IO uint32_t  PWR;                       /*!< Watchdog Pending Window Register      */
  __I  uint32_t  CTR;                       /*!< Watchdog Counter Test Register        */
} WDT_Type;

/// ------------------------------------------------------------------------------------------------
// -----                                 Peripheral memory map                                -----
// ------------------------------------------------------------------------------------------------
#define CM_BASE			0x40020000
#define IFC_BASE        0x40010000
#define WDT_BASE        0x40030000 
/// ------------------------------------------------------------------------------------------------
// -----                                Peripheral declaration                                -----
// ------------------------------------------------------------------------------------------------
#define CM0             ((CM_Type                 *) CM_BASE)
#define IFC0            ((IFC_Type                *) IFC_BASE)
#define WDT0            ((WDT_Type                *) WDT_BASE)
// ------------------------------------------------------------------------------------------------
// -----                                    IFC Bit/Field                                     -----
// ------------------------------------------------------------------------------------------------
// CEDR : Clock Enable/Disable Register 
#define IFC_CLKEN      	(0x01ul << 0)  	 /**< flash clock enable	     */
// SRR : Software Reset Register 
#define IFC_SWRST      	(0x01ul << 0)  	 /**< software reset	     */
// CR Register 
#define IFC_START       (0x01ul << 0)    /**< flash operation (P/ER) start bit*/
#define IFC_CMD(val)	((0x7ul & val) << 4)
#define IFC_CMD_MASK    (0x7ul << 4)
#define IFC_NP       	(0x01ul)     /**< flash normal program            */
#define IFC_PE       	(0x02ul)     /**< flash page erase              */
#define IFC_SE       	(0x03ul)     /**< flash sector erase                */
#define IFC_CE       	(0x04ul)     /**< flash chip erase                */
#define IFC_SOP      	(0x05ul)     /**< flash smart option program      */
#define IFC_SOE      	(0x06ul)     /**< flash smart option erase        */
// MR Register 
#define IFC_FSMODE    	(0x01ul << 7)     /**< flash high speed mode selection  */
// IMSCR: Interrupt Mask Set/Clear Register 
// RISR : Raw Interrupt Status Register	
// MISR : Masked Interrupt Status Registe
// ICR	: Interrupt Clear Register
#define IFC_END			(0x01ul << 0)  /**< Sable	                     */
#define IFC_ERR0		(0x01ul << 8)  /**< ESCLK fail status end         */
#define IFC_ERR1   		(0x01ul << 9)  /**< ESCLK fail (clock monitor)    */
#define IFC_ERR2   		(0x01ul << 10) /**< EMCLk fail status end         */
// SR : Status Register 
#define IFC_BUSY     (0x01ul << 0)     /**< Busy flag */
// AR : Address Register
#define IFC_ADDR_MASK	(0xFFFFFFFFul << 0)
#define IFC_ADDR(val)   ((0xFFFFFFFFul & val) << 0)
#define IFC_SOCADDR		(0x000000C0ul << 0)
#define IFC_SOPADDR		(0x000000C4ul << 0)
// KR : KEY Registers 
#define IFC_KEY         (0x5A5A5A5Aul <<0)

