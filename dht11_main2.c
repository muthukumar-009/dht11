/*
  DHT11 interfacing with LPC2148(ARM7)
  http://www.electronicwings.com/arm7/dht11-sensor-interfacing-with-lpc2148
*/

#include <lpc21xx.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include"delay_header.h"

void UART0_init(void)
{
	PINSEL0 = PINSEL0 | 0x00000005;	/* Enable UART0 Rx0 and Tx0 pins of UART0 */
	U0LCR = 0x83;	/* DLAB = 1, 1 stop bit, 8-bit character length */
	U0DLM = 0;	/* For baud rate of 9600 with Pclk = 15MHz */
	U0DLL = 97;	/* We get these values of U0DLL and U0DLM from formula */
	U0LCR = 0x03; /* DLAB = 0 */
}

void UART0_TxChar(char ch) /* A function to send a byte on UART0 */
{
	U0THR = ch;
	while( (U0LSR & 0x40) == 0 );	/* Wait till THRE bit becomes 1 which tells that transmission is completed */
}

void UART0_SendString(char* str) /* A function to send string on UART0 */
{
	uint8_t i = 0;
	while( str[i] != '\0' )
	{
		UART0_TxChar(str[i]);
		i++;
	}
}

unsigned char UART0_RxChar(void) /* A function to receive a byte on UART0 */
{
	while( (U0LSR & 0x01) == 0);	/* Wait till RDR bit becomes 1 which tells that receiver contains valid data */
		return U0RBR;
}

void dht11_request(void)
{
	IO0DIR = IO0DIR | 0x00000010;	/* Configure DHT11 pin as output (P0.4 used here) */
	IO0PIN = IO0PIN & 0xFFFFFFEF; /* Make DHT11 pin LOW for minimum 18 seconds */
	delay_ms(20);
	IO0PIN = IO0PIN | 0x00000010; /* Make DHT11 pin HIGH and wait for response */
}

void dht11_response(void)
{
	IO0DIR = IO0DIR & 0xFFFFFFEF;	/* Configure DHT11 pin as output */
	while( IO0PIN & 0x00000010 );	/* Wait till response is HIGH */
	while( (IO0PIN & 0x00000010) == 0 );	/* Wait till response is LOW */
	while( IO0PIN & 0x00000010 );	/* Wait till response is HIGH */	/* This is end of response */
}

uint8_t dht11_data(void)
{
	int8_t count;
	uint8_t data = 0;
	for(count = 0; count<8 ; count++)	/* 8 bits of data */
	{
		while( (IO0PIN & 0x00000010) == 0 );	/* Wait till response is LOW */
		delay_us(30);	/* delay greater than 24 usec */
		if ( IO0PIN & 0x00000010 ) /* If response is HIGH, 1 is received */
			data = ( (data<<1) | 0x01 );
		else	/* If response is LOW, 0 is received */
			data = (data<<1);
		while( IO0PIN & 0x00000010 );	/* Wait till response is HIGH (happens if 1 is received) */
	}
	return data;
}

int main (void)
{
	uint8_t humidity_integer, humidity_decimal, temp_integer, temp_decimal, checksum; 
	char data[7]; 
	UART0_init();
	while(1)
	{
		dht11_request();
		dht11_response();
		humidity_integer = dht11_data();
		humidity_decimal = dht11_data();
		temp_integer = dht11_data();
		temp_decimal = dht11_data();
		checksum = dht11_data();
		if( (humidity_integer + humidity_decimal + temp_integer + temp_decimal) != checksum )
			UART0_SendString("Checksum Error\r\n");
		else
		{			
			UART0_SendString("Relative Humidity : ");
			memset(data, 0, 7);
			sprintf(data, "%d.", humidity_integer);
			UART0_SendString(data);
			memset(data, 0, 7);
			sprintf(data, "%d\r\n", humidity_decimal);
			UART0_SendString(data);
			UART0_SendString("Temperature : ");
			memset(data, 0, 7);
			sprintf(data, "%d.", temp_integer);
			UART0_SendString(data);
			memset(data, 0, 7);
			sprintf(data, "%d\r\n", temp_decimal);
			UART0_SendString(data);
			UART0_SendString("Checksum : ");
			memset(data, 0, 7);
			sprintf(data, "%d\r\n", checksum);
			UART0_SendString(data);			
			delay_ms(2000);
		}
	}
}