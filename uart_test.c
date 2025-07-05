#include <LPC21xx.h>

void delay_ms(unsigned int ms)
{
    T0PR = 15000 - 1;
    T0TCR = 0x01;
    while (T0TC < ms);
    T0TCR = 0x03;
    T0TCR = 0x00;
}

void UART0_CONFIG(void)
{
    PINSEL0 |= 0x05; 
    U0LCR = 0x83;     
    U0DLM = 97;       
    U0DLL = 0;       
    U0LCR = 0x03; 
}

void UART0_TX(char ch)
{
    while (!(U0LSR & (1 << 5)));
    U0THR = ch;
}

void UART0_STR(const char *s)
{
    while (*s)
        UART0_TX(*s++);
}

int main(void)
{
    UART0_CONFIG();

    while (1)
    {
        UART0_STR("UART working\r\n");
        delay_ms(1000);
    }
}
