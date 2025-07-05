#include <lpc214x.h>
#include <stdint.h>
#include "lcd_header.h"
#include "delay_header.h"

#define DHT11 (1 << 4)  // DHT11 connected to P0.4

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

void DHT11_Request(void)
{
    IODIR0 |= DHT11;    // Set as output
    IOCLR0 = DHT11;     // Pull LOW for 18ms
    delay_ms(20);
    IOSET0 = DHT11;     // Pull HIGH for 20-40us
    delay_us(30);
    IODIR0 &= ~DHT11;   // Set as input
}

void DHT11_Response(void)
{
    while (IOPIN0 & DHT11);        // Wait for LOW
    while (!(IOPIN0 & DHT11));     // Wait for HIGH
    while (IOPIN0 & DHT11);        // Wait for LOW again
}

uint8_t DHT11_Read(void)
{
    uint8_t i, result = 0;
    for (i = 0; i < 8; i++)
    {
        while (!(IOPIN0 & DHT11));   // Wait for HIGH
        delay_us(30);                // Wait for 30us

        if (IOPIN0 & DHT11)
            result = (result << 1) | 1;
        else
            result = (result << 1);

        while (IOPIN0 & DHT11);      // Wait for LOW
    }
    return result;
}


