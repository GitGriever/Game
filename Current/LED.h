#ifndef BIT
#define BIT

#define PORTF_DATA  (*((volatile unsigned long *)0x40025038))   //offset of DATA register for PF1, PF2, PF3 is 0x38 [PF7:PF0::9:2]

#define bit0(x) (x & 0x01)	 // 0 = off
#define bit1(x) (x & 0x02)	 // R = red
#define bit2(x) (x & 0x04)	 // RG = 
#define bit3(x) (x & 0x08)	 // G = green
#define bit4(x) (x & 0x10)	 // GB =
#define bit5(x) (x & 0x20)	 // B = blue
#define bit6(x) (x & 0x40)	 // RB = 
#define bit7(x) (x & 0x80)	 // RGB = white
#define bit8(x) (x & 0x0100)
#define bit9(x) (x & 0x0200)
#define bit10(x) (x & 0x0400)
#define bit11(x) (x & 0x0800)
#define bit12(x) (x & 0x1000)
#define bit13(x) (x & 0x2000)
#define bit14(x) (x & 0x4000)
#define bit15(x) (x & 0x8000)

#endif

// unsigned int s = 1;

void Red_on(void);

void Red_off(void);
	
void Green_on(void);
	
void Green_off(void);

void Blue_on(void);

void Blue_off(void);

void Yellow_on(void);

void Yellow_off(void);

void Cyan_on(void);

void Cyan_off(void);

void LED_off(void);

void Rainbow(void);


