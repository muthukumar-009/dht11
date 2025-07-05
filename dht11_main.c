#include <lpc21xx.h>
#include <stdint.h>
#include "lcd_header.h"
#include "delay_header.h"

void dht11_request(void);
void dht11_response(void);
uint8_t dht11_data(void);

int main(void) {
    uint8_t h_int, h_dec, t_int, t_dec, checksum;

    LCD_Init();

    while (1) {
        dht11_request();
        dht11_response();

        h_int = dht11_data();
        h_dec = dht11_data();
        t_int = dht11_data();
        t_dec = dht11_data();
        checksum = dht11_data();

        LCD_COMMAND(0x80);
        if ((h_int + h_dec + t_int + t_dec) != checksum) {
            LCD_String("Checksum Error   ");
        } else {
            LCD_String("H:");
            LCD_Integer(h_int);
            LCD_Data('.');
            LCD_Integer(h_dec);
            LCD_Data('%');

            LCD_COMMAND(0xC0);
            LCD_String("T:");
            LCD_Integer(t_int);
            LCD_Data('.');
            LCD_Integer(t_dec);
            LCD_Data('C');
        }
        delay_ms(2000);
        LCD_COMMAND(0x01);
    }
}

void dht11_request(void) {
    IO0DIR |= 1 << 4; //OUTPUT
    IO0CLR = 1 << 4;
    delay_ms(20);
    IO0SET = 1 << 4;
    delay_us(30);
    IO0DIR &= ~(1 << 4); //INPUT
}

void dht11_response(void) {
    while (IO0PIN & (1 << 4));
    while (!(IO0PIN & (1 << 4)));
    while (IO0PIN & (1 << 4));
}

uint8_t dht11_data(void) {
    uint8_t i, data = 0;
    for (i = 0; i < 8; i++) {
        while (!(IO0PIN & (1 << 4)));
        delay_us(30);
        if (IO0PIN & (1 << 4))
            data = (data << 1) | 1;
        else
            data = (data << 1);
        while (IO0PIN & (1 << 4));
    }
    return data;
}