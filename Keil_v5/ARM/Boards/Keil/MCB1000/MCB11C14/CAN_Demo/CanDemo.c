/*----------------------------------------------------------------------------
 * Name:    CanDemo.c
 * Purpose: CAN example for MCB1000 populated with LPC11C14
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2010-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "LPC11xx.h"                            /* LPC11xx definitions        */
#include "CAN.h"                                /* LPC1766 CAN adaption layer */
#include "LED.h"                                /* LED function prototypes */
#include "ADC.h"                                /* ADC function prototypes */


/* Import external functions from Serial.c file                               */
extern void SER_init (void);

uint8_t val_Tx = 0, val_Rx = 0;                 /* Globals used for display */

uint8_t hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                         '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};


/*----------------------------------------------------------------------------
  insert a delay time.
 *----------------------------------------------------------------------------*/
void delay(unsigned int nCount)	{

  for(; nCount != 0; nCount--);
}


/*----------------------------------------------------------------------------
  convert one byte to string in hexadecimal notation
 *----------------------------------------------------------------------------*/
void Hex_Str (uint8_t hex, char *str) {
  *str++ = '0';
  *str++ = 'x';
  *str++ = hex_chars[hex >>  4];
  *str++ = hex_chars[hex & 0xF];
}

/*----------------------------------------------------------------------------
  read a converted value from the Analog/Digital converter
 *----------------------------------------------------------------------------*/
unsigned char adc_Get (void) {
  unsigned char val;

  ADC_startCnv();                                 /* start A/D conversion */
  val = ((ADC_getCnv() >> 2) & 0xFF);             /* use upper 8 bits of 10 bit AD conversion */
  ADC_stopCnv();                                  /* stop A/D conversion */
	
  return (val);
}

/*----------------------------------------------------------------------------
  display transmit and receieve values
 *----------------------------------------------------------------------------*/
void val_display (void) {
  static char disp_buf[] = " Tx:    , Rx:    \r";   /* display string */

  Hex_Str(val_Tx, &disp_buf[ 4]);                 /* prepare display string */
  Hex_Str(val_Rx, &disp_buf[13]);
  printf(disp_buf);                               /* print string to serial port */
  LED_out (val_Rx);                               /* display RX val on LEDs */

  delay (1000000);                                /* Wait wait a while */
}


/*----------------------------------------------------------------------------
  initialize CAN interface
 *----------------------------------------------------------------------------*/
void CAN_init (void) {

  CAN_setup ();                                   /* setup CAN Controller   */
  CAN_wrFilter (0x21, STANDARD_FORMAT);           /* Enable reception of messages */

  /* COMMENT THE LINE BELOW TO ENABLE DEVICE TO PARTICIPATE IN CAN NETWORK   */
  CAN_testmode();                                 /* Loopback mode           */

  CAN_start ();                                   /* start CAN Controller    */

  CAN_waitReady ();                               /* wait til tx mbx is empty */
}



/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void)  {
  int i;

  ADC_init ();                                    /* initialize A/D converter */
  LED_init ();                                    /* initialize the LEDs */
  SER_init ();                                    /* initialize serial port */
  CAN_init ();                                    /* initialize CAN interface */

  printf("    MCB1000 Demo    \n\r");
  printf("        CAN         \n\r");
  printf("    www.keil.com    \n\r");
  printf("\r\n");
  printf(" CAN at 500kbit/s   \n\r\n\r");

  CAN_TxMsg.id     = 0x21;                        /* initialize message to send */
  for (i = 0; i < 8; i++) CAN_TxMsg.data[i] = i; //0;
  CAN_TxMsg.len    = 8;
  CAN_TxMsg.format = STANDARD_FORMAT;
  CAN_TxMsg.type   = DATA_FRAME;

  while (1) {
    val_Tx = adc_Get ();                          /* TX value changes in any case */
    if (CAN_TxRdy) {                              /* tx message on CAN Controller */
      CAN_TxRdy = 0;

      CAN_TxMsg.data[0] = val_Tx;                 /* data[0] field = ADC value */
      CAN_wrMsg (&CAN_TxMsg);                     /* transmit message */
    }

    delay (10000);                                /* Wait a while to receive the message */

    if (CAN_RxRdy) {                              /* rc message on CAN Controller */
      CAN_RxRdy = 0;

      val_Rx = CAN_RxMsg.data[0];
    }

    val_display ();                               /* display  RX values */

  }
}
