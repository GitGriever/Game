/**************************************************************************//**
 * @file     common.h
 * @brief    Kinetis-M Common Header File
 * @version  V3.02
 * @date     16. April 2013
 *
 * @note
 * Copyright (C) 2009-2013 ARM Limited. All rights reserved.
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
/******************************************************************************
* List of the basic data types
*
*//*! @addtogroup basic_data_types
* @{
 ******************************************************************************/
typedef unsigned char		    uint8;    ///< unsigned  8 bits
typedef unsigned short int  uint16;   ///< unsigned 16 bits
typedef unsigned long int	  uint32;   ///< unsigned 32 bits
typedef unsigned long long  uint64;   ///< unsigned 64 bits

typedef char			          int8;     ///< signed  8 bits
typedef short int	          int16;    ///< signed 16 bits
typedef int		              int32;    ///< signed 32 bits
typedef long long           int64;    ///< signed 64 bits

typedef volatile int8		    vint8;    ///< volatile signed  8 bits
typedef volatile int16		  vint16;   ///< volatile signed 16 bits
typedef volatile int32		  vint32;   ///< volatile signed 32 bits
typedef volatile int64      vint64;   ///< volatile signed 64 bits

typedef volatile uint8		  vuint8;   ///< volatile unsigned  8 bits
typedef volatile uint16		  vuint16;  ///< volatile unsigned 16 bits
typedef volatile uint32		  vuint32;  ///< volatile unsigned 32 bits
typedef volatile uint64     vuint64;  ///< volatile unsigned 64 bits
/*! @} End of basic_data_types */

/******************************************************************************
 * Common macros and defines                                                  *
 ******************************************************************************/
#ifndef NULL
  #define NULL (void*)0
#endif
#define TRUE                    1
#define FALSE                   0

#define SET(mask)               mask
#define CLR(mask)               0
#define REG_SetVal(reg,mask,x)  { reg=((reg&~mask##_MASK)|((x)<<mask##_SHIFT)); }
