#ifndef	SWITCHES_H
#define SWITCHES_H
 
 
#define SW1 (GPIO_PORTF_DATA_R&0x10)  // PF4
#define SW2 (GPIO_PORTF_DATA_R&0x01)  // PF0

// for interrupts
#define PF0 (GPIO_PORTE_RIS_R&0x01) // SW2
#define FF4 (GPIO_PORTE_RIS_R&0x10)  // SW1

void Switch_Init(void);

#endif
