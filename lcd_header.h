#ifndef LCD_HEADER_H
#define LCD_HEADER_H

#include <lpc21xx.h>
#include <stdio.h>
#include <string.h>
#include"delay_header.h"
// ----------- LCD Pin Definitions (P1.20 to P1.23 - Data, P1.17=RS, P1.18=RW, P1.19=E) ----------
#define LCD_D 0xF << 20
#define RS 1 << 17
#define RW 1 << 18
#define E  1 << 19

// ----------- LCD Functions ----------
void LCD_COMMAND(unsigned char cmd) {
    IOPIN1 = (cmd & 0xF0) << 16;
    IOCLR1 = RS;
    IOCLR1 = RW;
    IOSET1 = E;
    delay_ms(2);
    IOCLR1 = E;

    IOPIN1 = (cmd & 0x0F) << 20;
    IOCLR1 = RS;
    IOCLR1 = RW;
    IOSET1 = E;
    delay_ms(2);
    IOCLR1 = E;
}

void LCD_Data(unsigned char data) {
    IOPIN1 = (data & 0xF0) << 16;
    IOSET1 = RS;
    IOCLR1 = RW;
    IOSET1 = E;
    delay_ms(2);
    IOCLR1 = E;

    IOPIN1 = (data & 0x0F) << 20;
    IOSET1 = RS;
    IOCLR1 = RW;
    IOSET1 = E;
    delay_ms(2);
    IOCLR1 = E;
}

void LCD_String(unsigned char *s) {
    while (*s)
        LCD_Data(*s++);
}

void LCD_Init(void) {
    PINSEL1 = 0x05000000; // Keep ADC config as is if required, else change as needed
    IODIR1 |= LCD_D | RS | RW | E;
    LCD_COMMAND(0x28);
    LCD_COMMAND(0x0C);
    LCD_COMMAND(0x06);
    LCD_COMMAND(0x01);
    LCD_COMMAND(0x80); // Line 1 start
}

void LCD_Integer(int num) {
    char buf[6];
    sprintf(buf, "%d", num);
    LCD_String(buf);
}

#endif
