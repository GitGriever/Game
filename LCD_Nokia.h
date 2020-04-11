// Nokia 5110 LCD (PCD8544)
// datasheet found at
// https://www.sparkfun.com/datasheets/LCD/Monochrome/Nokia5110.pdf

#include <stdbool.h>

#define MAX_X	84
#define MAX_Y	48

#define LCD_W     84
#define LCD_H     48

void LCD_init(void);

void LCD_clear(void);

void LCD_clearBuffer(void)

void LCD_DisplayBuffer(void);

void LCD_SaveLastBuffer(void);

void LCD_char(unsigned char data);

void LCD_string(char *ptr);

void LCD_uDec(char *ptr);

void LCD_setCursor(unsigned char newX, unsigned char newY);

void LCD_fullImage(const char *ptr);

void LCD_bmp(unsigned char xval, unsigned char yval, const unsigned char *ptr, unsigned char threshold);

void LCD_setPixel(unsigned char x, unsigned char y);

void LCD_clearPixel(unsigned char x, unsigned char y);

bool LCD_askPixel(unsigned char x, unsigned char y);

bool LCD_askLastPixel(unsigned char x, unsigned char y);

void LCD_char_4x4pix(unsigned char data);

void LCD_string_4x4pix(char *ptr);

void LCD_char_4x4pix_toBuffer(unsigned char xval, unsigned char yval, unsigned char data);

void LCD_string_4x4pix_toBuffer(unsigned char xval, unsigned char yval, char *ptr);

void LCD_uDec_4x4pix_toBuffer(unsigned char xval, unsigned char yval, unsigned short n);