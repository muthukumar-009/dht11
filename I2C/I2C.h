#include <LPC21xx.h>

#ifndef __I2C_DEFINES_H__
#define __I2C_DEFINES_H__

#define SCL_EN  0x00000010
#define SDA_EN  0x00000040

// Defines for I2C_SPEED Configuration 
#define CCLK      60000000  // Hz
#define PCLK      CCLK/4    // Hz
#define I2C_SPEED 100000    // Hz
#define LOADVAL   ((PCLK/I2C_SPEED)/2)

// Bit defines for I2CONSET sfr
#define AA_BIT    2
#define SI_BIT    3
#define STO_BIT   4
#define STA_BIT   5
#define I2EN_BIT  6

#endif // End of __I2C_DEFINES_H__

#ifndef __TYPES_H__

#define __TYPES_H_


typedef unsigned char u8;

typedef signed   char s8;

typedef unsigned short u16;

typedef signed   short s16;

typedef unsigned int  u32;

typedef signed   int  s32;

typedef float  f32;

typedef double f64;

#endif
// Function declarations
void init_i2c(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_restart(void);
void i2c_write(u8);
u8   i2c_read(void);
u8 i2c_ack(void);
u8 i2c_nack(void);
u8 i2c_masterack(void);

// Delay function for milliseconds
void delay_ms(unsigned int ms)
{
    T0PR = 15000 - 1;  // Set the prescaler for 1 ms
    T0TCR = 0x01;      // Start timer
    while (T0TC < ms); // Wait for the desired time
    T0TCR = 0x03;      // Reset timer
    T0TCR = 0x00;      // Disable timer
}

// I2C initialization function
void init_i2c(void)
{
    // Configure I/O pin for SCL & SDA functions using PINSEL0
    PINSEL0 = SCL_EN | SDA_EN;
    
    // Configure Speed for I2C Serial Communication using I2CSCLL and I2CSCLH
    I2SCLL = LOADVAL;
    I2SCLH = LOADVAL;
    
    // I2C Peripheral Enable for Communication
    I2CONSET = 1 << I2EN_BIT; 
}

// Start condition for I2C
void i2c_start(void)
{
    I2CONSET = 1 << STA_BIT;  // Set Start condition
    while (((I2CONSET >> SI_BIT) & 1) == 0);  // Wait for SI bit to set (interrupt flag)
    I2CONCLR = 1 << STA_BIT;  // Clear Start condition
}

// Restart condition for I2C
void i2c_restart(void)
{
    I2CONSET = 1 << STA_BIT;  // Set Restart condition
    I2CONCLR = 1 << SI_BIT;   // Clear SI flag to prepare for next operation
    while (((I2CONSET >> SI_BIT) & 1) == 0);  // Wait for SI bit to set (interrupt flag)
    I2CONCLR = 1 << STA_BIT;  // Clear Restart condition
}

// Write data to I2C bus
void i2c_write(u8 dat)
{
    I2DAT = dat;             // Load the data into I2DAT register
    I2CONCLR = 1 << SI_BIT;  // Clear SI flag
    while (((I2CONSET >> SI_BIT) & 1) == 0);  // Wait for SI bit to set (interrupt flag)
}

// Stop condition for I2C
void i2c_stop(void)
{
    I2CONSET = 1 << STO_BIT;  // Set Stop condition
    I2CONCLR = 1 << SI_BIT;   // Clear SI flag
    // Stop condition will automatically clear after completion
}

// Read data with ACK
u8 i2c_ack(void)
{
    I2CONSET = 1 << AA_BIT;  // Set ACK bit
    I2CONCLR = 1 << SI_BIT;  // Clear SI flag
    while (((I2CONSET >> SI_BIT) & 1) == 0);  // Wait for SI bit to set (interrupt flag)
    I2CONCLR = 1 << AA_BIT;  // Clear ACK bit after reading
    return I2DAT;            // Return the received data
}

// Read data with NACK
u8 i2c_nack(void)
{
    I2CONCLR = 1 << AA_BIT;  // Clear ACK bit to send NACK
    I2CONCLR = 1 << SI_BIT;  // Clear SI flag
    while (((I2CONSET >> SI_BIT) & 1) == 0);  // Wait for SI bit to set (interrupt flag)
    return I2DAT;            // Return the received data
}

// Master acknowledge (ACK)
u8 i2c_masterack(void)
{
    I2CONSET = 1 << AA_BIT;  // Set ACK bit
    I2CONCLR = 1 << SI_BIT;  // Clear SI flag
    while (((I2CONSET >> SI_BIT) & 1) == 0);  // Wait for SI bit to set (interrupt flag)
    I2CONCLR = 1 << AA_BIT;  // Clear ACK bit after reading
    return I2DAT;            // Return the received data
}
