#ifndef DELAY_HEADER_H
#define DELAY_HEADER_H

#include <lpc21xx.h>

void delay_ms(unsigned int ms)
{
    T0PR = 15000 - 1;   // 1ms for PCLK = 15MHz
    T0TCR = 0x01;       // Enable Timer
    while (T0TC < ms);  // Wait until Timer Counter reaches ms
    T0TCR = 0x03;       // Disable Timer
	T0TCR = 0x00; 
}

void delay_us(unsigned int us)
{
    T0PR = 15 - 1;   // 1ms for PCLK = 15MHz
    T0TCR = 0x01;       // Enable Timer
    while (T0TC < us);  // Wait until Timer Counter reaches ms
    T0TCR = 0x03;       // Disable Timer
	T0TCR = 0x00; 
}

#endif
