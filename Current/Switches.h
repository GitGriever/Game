<<<<<<< HEAD
#ifndef	SWITCHES_H
#define SWITCHES_H
 
 
#define SW1 (GPIO_PORTF_DATA_R&0x10)
#define SW2 (GPIO_PORTF_DATA_R&0x01)

extern void PortF_Init(void);

void Interrupt_Init(void);

#endif
=======
#ifndef	SWITCHES_H
#define SWITCHES_H
 
 
#define SW1 (GPIO_PORTF_DATA_R&0x10)
#define SW2 (GPIO_PORTF_DATA_R&0x01)

extern void PortF_Init(void);

void Interrupt_Init(void);

#endif
>>>>>>> 5082458ed674645c16ab8796016d3c9a3917f69f
