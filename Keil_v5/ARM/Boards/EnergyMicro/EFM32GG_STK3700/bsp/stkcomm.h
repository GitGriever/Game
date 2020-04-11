/**************************************************************************//**
 * @file
 * @brief Generic starter kit board control communications package header file
 * @author Energy Micro AS
 * @version 1.0.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2012 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 4. The source and compiled code may only be used on Energy Micro "EFM32"
 *    microcontrollers and "EFR4" radios.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 *****************************************************************************/
#ifndef __STKCOMM_H_
#define __STKCOMM_H_
#include <stdint.h>
#include <stdbool.h>
#include "stkcomm_packets.h"
#include "efm32.h"
#include "em_usart.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t STKCOMM_bcInit( USART_TypeDef *usart, uint32_t baudrate, uint32_t location );
void     STKCOMM_bcSendPacket( STKCOMM_Packet *pkt );
bool     STKCOMM_bcReceivePacket( STKCOMM_Packet *pkt );
void     STKCOMM_bcStdOut( char *buffer, int length );
float    STKCOMM_bcGetCurrent( void );
float    STKCOMM_bcGetVoltage( void );
bool     STKCOMM_bcPing( void );

#ifdef __cplusplus
}
#endif

#endif
