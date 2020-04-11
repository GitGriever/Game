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
#ifndef __STK_COMM_PACKET_H_
#define __STK_COMM_PACKET_H_

#include <stdint.h>

/* STK Packet Types */
#define STKCOMM_PACKETTYPE_INVALID        0 /**< Invalid packet received */

#define STKCOMM_PACKETTYPE_FIRST          1 /**< Smallest numerical value of  message type */ 

#define STKCOMM_PACKETTYPE_ACK            5 /**< Generic ACK for one way packages */
#define STKCOMM_PACKETTYPE_ECHO_REQ      10 /**< EFM32 BC alive request */
#define STKCOMM_PACKETTYPE_ECHO_REPLY    11 /**< BC alive response */
#define STKCOMM_PACKETTYPE_CURRENT_REQ   14 /**< EFM32 Request AEM current */
#define STKCOMM_PACKETTYPE_CURRENT_REPLY 16 /**< BC Response AEM current */
#define STKCOMM_PACKETTYPE_VOLTAGE_REQ   18 /**< EFM32 Request AEM voltage */
#define STKCOMM_PACKETTYPE_VOLTAGE_REPLY 20 /**< BC Response AEM voltage */
#define STKCOMM_PACKETTYPE_ENERGYMODE    22 /**< EFM32 Report Energy Mode (for AEM) */
#define STKCOMM_PACKETTYPE_STDOUT        24 /**< Debug packet (not used)  */
#define STKCOMM_PACKETTYPE_STDERR        26 /**< Debug packet (not used)  */
#define STKCOMM_PACKETTYPE_TEST          32 /**< Reserved type for test */
#define STKCOMM_PACKETTYPE_TEST_REPLY    33 /**< Reserved type for test (reply) */

#define STKCOMM_PACKETTYPE_LAST         100 /**< Last defined message type */

/** Magic byte to indicate start of packet */
#define STKCOMM_MAGIC 0xF1

/** Maximum payload for packets */
#ifndef STKCOMM_PACKET_SIZE
#define STKCOMM_PACKET_SIZE             132
#endif

#if STKCOMM_PACKET_SIZE >= 255
#error "STK Board Controller communications packets must be less than 255 bytes in size!"
#endif

/** @brief STK Packet Structure - Simple Comm. Protocol */
typedef struct __STKCOMM_Packet
{

   uint8_t magic;                  /**< Magic - start of packet - must be STKCOMM_MAGIC */
   uint8_t type;                   /**< Type - packet type */
   uint8_t payloadLength;          /**< Length of data segment >=0 and <=STKCOMM_PACKET_SIZE */
   uint8_t reserved;
   uint8_t data[STKCOMM_PACKET_SIZE];  /**< STK Packet Data payload */

} STKCOMM_Packet;

/** @brief STK Packet Header definition */
typedef struct __STKCOMM_PacketHeader
{

   uint8_t magic;                  /**< Magic - start of packet - must be STKCOMM_MAGIC */
   uint8_t type;                   /**< Type - packet type */
   uint8_t payloadLength;          /**< Length of data segment >=0 and <=STKCOMM_PACKET_SIZE */
   uint8_t reserved;
   
} STKCOMM_PacketHeader;

#endif
