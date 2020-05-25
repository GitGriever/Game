// Implementation of three LEDs on PF1, PF2, and PF3.

#include <stdint.h>
#include "..//tm4c123gh6pm.h"
#include "LED.h"
#include "Timer.h"

unsigned int s = 1;
	
void LED_Init(void)
{
	
volatile unsigned long delay;

  SYSCTL_RCGC2_R |= 0x00000020;     // activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // enable digital I/O on PF4-0
}

void Red_on(void)
{
	GPIO_PORTF_DATA_R |= 0x02;
	GPIO_PORTF_DATA_R &= ~0x08;	// Turn on green LED.
	GPIO_PORTF_DATA_R &= ~0x04; // Turn on blue LED.
}

void Red_off(void)
{
	GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
	GPIO_PORTF_DATA_R &= ~0x08;	// Turn on green LED.
	GPIO_PORTF_DATA_R &= ~0x04; // Turn on blue LED.
}

void Green_on(void)
{
	GPIO_PORTF_DATA_R &= ~0x02;	// Turn on green LED.
	GPIO_PORTF_DATA_R &= ~0x04; // Turn on blue LED.
	GPIO_PORTF_DATA_R |= 0x08;
}

void Green_off(void)
{
	GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
	GPIO_PORTF_DATA_R &= ~0x08;	// Turn on green LED.
	GPIO_PORTF_DATA_R &= ~0x04; // Turn on blue LED.
}

void Blue_on(void)
{
	GPIO_PORTF_DATA_R &= ~0x02;	// Turn on green LED.
	GPIO_PORTF_DATA_R |= 0x04;
	GPIO_PORTF_DATA_R &= ~0x08; // Turn on blue LED.
}

void Blue_off(void)
{
	GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
	GPIO_PORTF_DATA_R &= ~0x08;	// Turn on green LED.
	GPIO_PORTF_DATA_R &= ~0x04; // Turn on blue LED.
}

void Cyan_on(void)		// green, blue on
{
	GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
	GPIO_PORTF_DATA_R |= 0x08;	// Turn on green LED.
	GPIO_PORTF_DATA_R |= 0x04; // Turn on blue LED.
}

void Cyan_off(void)
{
	GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
	GPIO_PORTF_DATA_R &= ~0x08;	// Turn on green LED.
	GPIO_PORTF_DATA_R &= ~0x04; // Turn on blue LED.
}

void Yellow_on(void)		// green, blue on
{
	GPIO_PORTF_DATA_R |= 0x02; // Turn off red LED.
	GPIO_PORTF_DATA_R |= 0x08;	// Turn on green LED.
	GPIO_PORTF_DATA_R &= ~0x04; // Turn on blue LED.
}

void Yellow_off(void)		// green, blue on
{
	GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
	GPIO_PORTF_DATA_R &= ~0x08;	// Turn on green LED.
	GPIO_PORTF_DATA_R &= ~0x04; // Turn on blue LED.
}

void LED_off(void)
{
	GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
	GPIO_PORTF_DATA_R &= ~0x08;	// Turn on green LED.
	GPIO_PORTF_DATA_R &= ~0x04; // Turn on blue LED.
}

void Rainbow(void)
{
	// unsigned int s = 1;
	PORTF_DATA = 0;
	PORTF_DATA = (s << 1);
	s = (bit2(~s) >> 2) | (((bit2(~s) >> 1) & bit1(s) | (bit2(s) >> 1) & (bit0(~s) << 1))) | ((bit2(s) & (bit1(~s) << 1) | bit2(~s) & (bit0(s) << 2))); //flash all combinations
	Delay100ms(3); // 3 red, yellow, green, cyan, blue, pink, white, off
}




