/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    SMTP_DEMO.C
 *      Purpose: SMTP Client demo example
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <RTL.h>
#include <LM3Sxxxx.H>

/* Enter your valid SMTP Server IP address here. */
U8 srv_ip[4] = { 192,168,0,253 };

BOOL tick;
BOOL tick2;
BOOL sent;
U32  delay;

/*--------------------------- init ------------------------------------------*/

static void init () {
  /* Add System initialisation code here */

  /* Set the clocking to run from the PLL at 50 MHz */
  SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL |
                 SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

  /* Configure the GPIO for the LED. */
  SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOD);
  GPIOPadConfigSet (GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, 
                    GPIO_PIN_TYPE_STD_WPU);
  GPIODirModeSet (GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_DIR_MODE_OUT);

  /* Configure UART0 for 115200 baud. */
  SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOA);
  SysCtlPeripheralEnable (SYSCTL_PERIPH_UART0);
  GPIOPinTypeUART (GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  UARTConfigSet(UART0_BASE, 115200, (UART_CONFIG_WLEN_8 |
                                     UART_CONFIG_STOP_ONE |
                                     UART_CONFIG_PAR_NONE));
  UARTEnable(UART0_BASE);

  /* Setup and enable the SysTick timer for 100ms. */
  SysTickPeriodSet(SysCtlClockGet() / 10);
  SysTickEnable();
}


/*--------------------------- LED_out ---------------------------------------*/

void LED_out (U32 val) {
  GPIOPinWrite (GPIO_PORTD_BASE, GPIO_PIN_0, val & 1);
}


/*--------------------------- timer_poll ------------------------------------*/

static void timer_poll () {
  /* System tick timer running in poll mode */

  if ((HWREG (NVIC_ST_CTRL) >> 16) & 1) {
    /* Timer tick every 100 ms */
    timer_tick ();
    tick  = __TRUE;
    tick2 = __TRUE;
  }
}


/*--------------------------- fputc -----------------------------------------*/

int fputc (int ch, FILE *f)  {
  /* Debug output to serial port. */

  if (ch == '\n')  {
    UARTCharPut (UART0_BASE, '\r');          /* output CR                    */
  }
  UARTCharPut (UART0_BASE, ch);
  return (ch);
}


/*--------------------------- blink_led -------------------------------------*/

static void blink_led () {
  /* Blink the LEDs on MCB-STR9 board */
  static U32 LEDstat = 1;

  if (tick == __TRUE) {
    /* Every 100 ms */
    tick = __FALSE;
    LEDstat = ~LEDstat & 0x01;
    LED_out (LEDstat);
  }
}


/*--------------------------- AD_in ----------------------------------------*/

U16 AD_in (U32 ch) {
  /* Read ARM Analog Input */

  /* There is no POT on this board, return something. */
  return (ch + 10);
}


/*---------------------------------------------------------------------------*/

static void smtp_cback (U8 code) {
  /* This function is called by TcpNet to inform about SMTP status. */
  switch (code) {
    case SMTP_EVT_SUCCESS:
      printf ("Email successfully sent\n");
      sent = __TRUE;
      break;
    case SMTP_EVT_TIMEOUT:
      /* Timeout, try again. */
      printf ("Mail Server timeout.\n");
      delay = 0;
      break;
    case SMTP_EVT_ERROR:
      /* Error, try again. */
      printf ("Error sending email.\n");
      delay = 0;
      break;
  }
}

/*---------------------------------------------------------------------------*/

int main (void) {
  /* Main Thread of the TcpNet */

  init ();
  init_TcpNet ();

  delay = 0;
  sent  = __FALSE;
  while (1) {
    timer_poll ();
    main_TcpNet ();
    blink_led ();
    if (tick2 == __FALSE) {
      continue;
    }
    tick2 = __FALSE;
    if (sent == __TRUE) {
      continue;
    }
    if (++delay == 100) {
      /* After 10 seconds send an email. */
      printf ("Sending email...\n");
      smtp_connect ((U8 *)&srv_ip,25,smtp_cback);
    }
  }
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
