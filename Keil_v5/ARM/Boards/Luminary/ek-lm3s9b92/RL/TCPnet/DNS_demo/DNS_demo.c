/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    DNS_DEMO.C
 *      Purpose: DNS Resolver demo example
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2012 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <RTL.h>
#include <LM3Sxxxx.H>

BOOL tick;
BOOL tick2;

char const *hosts[] = {
  "www.google.com",
  "www.keil.com",
  "www.microsoft.com",
  "www.yahoo.com",
  "www.notexisting.site",
  "192.168.0.253",
  "www.google-.com",
  "goble"
};
unsigned int index;
unsigned int delay;

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


/*--------------------------- next_host -------------------------------------*/

static void next_host () {
  /* Select next host from the table. */

  if (++index == sizeof(hosts)/sizeof(hosts[0])) {
    index = 0;
  }
}


/*--------------------------- dns_cbfunc ------------------------------------*/

static void dns_cbfunc (unsigned char event, unsigned char *ip) {
  /* This function is called by the DNS Client when dns event occurs. */

  switch (event) {
    case DNS_EVT_SUCCESS:
      /* Host Address successfully resolved. When IP address is already */
      /* cached, there is no DNS Request sent to remote DNS Server.     */
      printf("IP Address    : %d.%d.%d.%d\n",ip[0],ip[1],ip[2],ip[3]);
      break;

    case DNS_EVT_NONAME:
      /* Host Name does not exist in DNS record database. */
      printf("Host name does not exist.\n");
      break;

    case DNS_EVT_TIMEOUT:
      /* All DNS Resolver retries used up and timeouts expired. */
      printf("DNS Resolver Timeout expired, Host Address not resolved.\n");
      break;

    case DNS_EVT_ERROR:
      /* DNS Protocol Error, invalid or corrupted reply received. */
      printf("DNS Resolver Protocol Error, Host Address not resolved.\n");
      delay = 0;
      return;
  }
  next_host();
  delay = 30;
}


/*---------------------------------------------------------------------------*/

int main (void) {
  /* Main Thread of the TcpNet */
  int res;

  init ();
  init_TcpNet ();

  printf ("\nDNS Resolver Demo Example\n");

  index = 0;
  delay = 0;
  while (1) {
    timer_poll ();
    main_TcpNet ();
    blink_led ();
    if (tick2 == __FALSE) {
      continue;
    }
    tick2 = __FALSE;
    if (++delay == 50) {
      /* After 5 seconds start DNS Resolver. */
      printf ("\nResolving host: %s\n",hosts[index]);
      res = get_host_by_name ((U8 *)hosts[index],dns_cbfunc);
      switch (res) {
        case DNS_RES_OK:
          /* Resolver started, wait for callback event. */
          break;

        case DNS_ERROR_BUSY:
          /* Busy, retry on next tick. */
          delay--;
          break;

        case DNS_ERROR_LABEL:
          printf ("Invalid label specified!\n");
          next_host();
          delay = 30;
          break;

        case DNS_ERROR_NAME:
          printf ("Hostname is not valid!\n");
          next_host();
          delay = 30;
          break;

        case DNS_ERROR_NOSRV:
          printf ("DNS Server unknown!\n");
          delay = 0;
          break;

        case DNS_ERROR_PARAM:
          printf ("Invalid parameters!\n");
          delay = 0;
          break;
      }
    }
  }
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
