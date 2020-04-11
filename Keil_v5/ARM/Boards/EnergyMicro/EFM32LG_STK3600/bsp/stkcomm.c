/**************************************************************************//**
 * @file
 * @brief EFM32_Gxxx_STK Starter Kit Board Support functions
 * @author Energy Micro AS
 * @version 1.0.0                                              modified by Keil
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
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "efm32.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_usart.h"
#include "stk.h"
#include "stkcomm.h"

/***************************************************************************//**
 * @addtogroup BSP
 * @{
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#define LOCAL static

/**************************************************************************//**
 * @brief Local prototypes
 *****************************************************************************/
LOCAL void STKCOMM_USARTInit( USART_TypeDef *usart, uint32_t baudrate, uint32_t location );
LOCAL void STKCOMM_CMUInit( USART_TypeDef *usart );

/**************************************************************************//**
 * @brief Module globals (local)
 *****************************************************************************/
LOCAL USART_TypeDef *cfgUSART;

/** @endcond */

/**************************************************************************//**
 * @brief Initializes STK
 * @note If HFPERCLK is changed, this routine needs to be called again
 *****************************************************************************/
uint32_t STKCOMM_bcInit( USART_TypeDef *usart, uint32_t baudrate, uint32_t location ){

   /* Configuration */
   cfgUSART = usart;

   /* Enable CMU clocks */
   STKCOMM_CMUInit( cfgUSART );

   /* Enable USART */
   STKCOMM_USARTInit( cfgUSART, baudrate, location );

   return baudrate;

}

/**************************************************************************//**
 * @brief Receive a packet over the UART interface
 * @return false on failure, or true on success
 *****************************************************************************/
bool STKCOMM_bcReceivePacket(STKCOMM_Packet *pkt){

   uint8_t *rxPtr = (uint8_t *) pkt;
   int     length;

   /* Receive packet magic */
   while (!(cfgUSART->STATUS & USART_STATUS_RXDATAV)) ;
   *rxPtr++ = cfgUSART->RXDATA;
   if (pkt->magic != STKCOMM_MAGIC){
      /* Invalid packet */
      memset(pkt, 0x00, sizeof(STKCOMM_Packet));
      return false;
   }

   /* Receive packet type */
   while (!(cfgUSART->STATUS & USART_STATUS_RXDATAV)) ;
   *rxPtr++ = cfgUSART->RXDATA;
   if ((pkt->type < STKCOMM_PACKETTYPE_FIRST) || (pkt->type > STKCOMM_PACKETTYPE_LAST)){
      /* Invalid packet */
      memset(pkt, 0x00, sizeof(STKCOMM_Packet));
      return false;
   }

   /* Receive packet length */
   while (!(cfgUSART->STATUS & USART_STATUS_RXDATAV)) ;
   *rxPtr++ = cfgUSART->RXDATA;
   if (pkt->payloadLength > STKCOMM_PACKET_SIZE){
      /* Invalid packet */
      memset(pkt, 0x00, sizeof(STKCOMM_Packet));
      return false;
   }

   /* Receive reserved byte */
   while (!(cfgUSART->STATUS & USART_STATUS_RXDATAV)) ;
   *rxPtr++ = cfgUSART->RXDATA;

   /* Receive packet data */
   length = pkt->payloadLength;
   if( length > STKCOMM_PACKET_SIZE ){
      length = STKCOMM_PACKET_SIZE;
   }
   while (length){
      while (!(cfgUSART->STATUS & USART_STATUS_RXDATAV)) ;
      *rxPtr++ = cfgUSART->RXDATA;
      length--;
   }

   return true;

}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/**************************************************************************//**
 * @brief Transmit single byte with given USART
 * @param data Character to transmit
 *****************************************************************************/
__STATIC_INLINE void USART_TxByte(USART_TypeDef *usart, uint8_t data){

   /* Check that transmit buffer is empty */
   while (!(usart->STATUS & USART_STATUS_TXBL)) ;
   usart->TXDATA = (uint32_t) data;

}
/** @endcond */


/**************************************************************************//**
 * @brief Transmit packet to board controller
 * @param pkt Packet to transmit
 *****************************************************************************/
void STKCOMM_bcSendPacket( STKCOMM_Packet *pkt ){

   int i;

   /* Apply magic */
   pkt->magic = 0xf1;

   /* Transmit packet magic */
   USART_TxByte(cfgUSART, pkt->magic);

   /* Transmit packet type */
   USART_TxByte(cfgUSART, pkt->type);

   /* Transmit packet length */
   USART_TxByte(cfgUSART, pkt->payloadLength);

   /* Transmit reserved byte */
   USART_TxByte(cfgUSART, pkt->reserved);

   /* Transmit packet payload */
   for (i = 0; i < pkt->payloadLength; i++){
      USART_TxByte(cfgUSART, pkt->data[i]);
   }

   return;

}


/**************************************************************************//**
 * @brief Transmit packets to transport output to Board Controller
 *****************************************************************************/
void STKCOMM_bcStdOut( char *buffer, int length )
{
   STKCOMM_Packet pkt;
   int        pktLength;

   /* Split buffer into multiple packets if necessary */
   while (length){

      pkt.type = STKCOMM_PACKETTYPE_STDOUT;

      if (length > STKCOMM_PACKET_SIZE){
         pktLength = STKCOMM_PACKET_SIZE;
      }
      else{
         pktLength = length;
      }
      pkt.payloadLength = pktLength;
      memcpy(pkt.data, buffer, pktLength);

      /* Transfer packet */
      STKCOMM_bcSendPacket(&pkt);
      if (!STKCOMM_bcReceivePacket(&pkt)){
         return;
      }

      /* Update pointers */
      length -= pktLength;
      buffer += pktLength;

   }

   return;

}


/**************************************************************************//**
 * @brief STK Check if Board Controller is alive
 *****************************************************************************/
bool STKCOMM_bcPing( void ){

   STKCOMM_Packet pkt;

   /* Transfer Echo packet */
   pkt.type          = STKCOMM_PACKETTYPE_ECHO_REQ;
   pkt.payloadLength = 0;

   /* Send Request/Get reply */
   STKCOMM_bcSendPacket(&pkt);
   STKCOMM_bcReceivePacket(&pkt);

   if (pkt.type == STKCOMM_PACKETTYPE_ECHO_REPLY){
      return true;
   }

   return false;

}


/**************************************************************************//**
 * @brief Request AEM current from Board Controller
 *****************************************************************************/
float STKCOMM_bcGetCurrent( void ){

   STKCOMM_Packet pkt;
   float      current;

   pkt.type          = STKCOMM_PACKETTYPE_CURRENT_REQ;
   pkt.payloadLength = 0;

   /* Send Request/Get reply */
   STKCOMM_bcSendPacket(&pkt);
   STKCOMM_bcReceivePacket(&pkt);

   /* Process reply */
   if (pkt.type == STKCOMM_PACKETTYPE_CURRENT_REPLY){

      memcpy(&current, pkt.data, sizeof(float));
      return current;

   }
   else{

      return (float) 0.0;

   }

}


/**************************************************************************//**
 * @brief Request AEM current from Board Controller
 *****************************************************************************/
float STKCOMM_bcGetVoltage( void ){

   STKCOMM_Packet pkt;
   float      voltage;

   pkt.type          = STKCOMM_PACKETTYPE_VOLTAGE_REQ;
   pkt.payloadLength = 0;

   /* Send Request/Get reply */
   STKCOMM_bcSendPacket(&pkt);
   STKCOMM_bcReceivePacket(&pkt);

   /* Process reply */
   if (pkt.type == STKCOMM_PACKETTYPE_VOLTAGE_REPLY){
      memcpy(&voltage, pkt.data, sizeof(float));
      return voltage;
   }
   else{
      return (float) 0.0;
   }

}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/**************************************************************************//**
 * @brief Intializes STK Clock Management Unit
 *****************************************************************************/
LOCAL void STKCOMM_CMUInit( USART_TypeDef *usart ){

   CMU_Clock_TypeDef cmuClock;

   /* Set cmuClock */
   if( usart == NULL ){
      return;
   }
#if defined(_CMU_HFPERCLKEN0_USART0_MASK)
   else if( usart == USART0 ){
      cmuClock = cmuClock_USART0;
   }
#endif
#if defined(_CMU_HFPERCLKEN0_USART1_MASK)
   else if( usart == USART1 ){
      cmuClock = cmuClock_USART1;
   }
#endif
#if defined(_CMU_HFPERCLKEN0_USART2_MASK)
   else if( usart == USART2 ){
      cmuClock = cmuClock_USART2;
   }
#endif
#if defined(_CMU_HFPERCLKEN0_UART0_MASK)
   else if( usart == UART0 ){
      cmuClock = cmuClock_UART0;
   }
#endif
#if defined(_CMU_HFPERCLKEN0_UART1_MASK)
   else if( usart == UART1 ){
      cmuClock = cmuClock_UART1;
   }
#endif
   else{
      return;
   }

   /* Enable High Frequency Peripherals */
   CMU_ClockEnable( cmuClock_HF, true );

   /* Enable clocks to GPIO */
   CMU_ClockEnable( cmuClock_GPIO, true );
   CMU_ClockEnable( cmuClock, true );

   return;

}

/**************************************************************************//**
 * @brief Intializes STK UART0 for UART mode towards Board Controller
 *****************************************************************************/
LOCAL void STKCOMM_USARTInit( USART_TypeDef *usart, uint32_t baudrate, uint32_t location ){

   USART_InitAsync_TypeDef usartCfg = USART_INITASYNC_DEFAULT;

   /* Override baudrate */
   usartCfg.baudrate = baudrate;

   /* Initialize USART */
   USART_InitAsync( usart, &usartCfg );

   if( (usart == USART0) || (usart == USART0) || (usart == USART0) ){
      usart->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | location;
   }
   else if( (usart == UART0) || (usart == UART1) ){
      usart->ROUTE = UART_ROUTE_RXPEN | UART_ROUTE_TXPEN | location;
   }

   return;

}
/** @endcond */

/** @} (end group BSP) */
