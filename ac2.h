// Copyright (c) 2018 Cláudio Patrício
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <detpic32.h>
#include "stdbool.h"

/* Defines */

#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0;
#define EnableUart1RxInterrupt() IEC0bits.U1RXIE = 1;
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0;
#define EnableUart1TxInterrupt() IEC0bits.U1TXIE = 1;
#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE-1)

/* SPI2 */

#define RDSR	0x05
#define WRITE	0x02
#define READ	0x03
#define EEPROM_CLOCK 500000
#define SPI2_WREN 0x06


/* Structures */

typedef unsigned char uchar;
typedef unsigned int uint;

typedef struct
{
	uchar data[BUF_SIZE];
	uint head;
	uint tail;
	uint count;
	bool overrun;
} ring;


/* Global Vars */

enum _REbits {
	RE0 = 0x0001,
	RE1 = 0x0002,
	RE2 = 0x0004,
	RE3 = 0x0008,
	RE4 = 0x0010,
	RE5 = 0x0020,
	RE6 = 0x0040,
	RE7 = 0x0080,
	RE_ALL = 0x00FF,
	RE1L = 0x0003,
	RE2L = 0x0007,
	RE3L = 0x000F,
	RE4L = 0x001F,
	RE5L = 0x003F,
	RE6L = 0x007F
};

enum _RBbits {
	RB0 = 0x0001,
	RB1 = 0x0002,
	RB2 = 0x0004,
	RB3 = 0x0008,
	RB4 = 0x0010,
	RB5 = 0x0020,
	RB6 = 0x0040,
	RB7 = 0x0080,
	RB8 = 0x0100,
	RB9 = 0x0200,
	RB_ALL = 0x03FF,
	RB1L = 0x0003,
	RB2L = 0x0007,
	RB3L = 0x000F,
	RB4L = 0x001F,
	RB5L = 0x003F,
	RB6L = 0x007F,
	RB7L = 0x00FF,
	RB8L = 0x01FF
};

enum _RBLEDbits {
	B = RB0,
	A = RB1,
	F = RB2,
	G = RB3,
	E = RB4,
	D = RB5,
	C = RB6,
	DOT = RB7,
	S_HI = RB8,
	S_LO = RB9
};

enum _LEDbits {
	L_0 = A|B|C|D|E|F,		//0x0077
	L_1 = B|C,			//0x0041
	L_2 = A|B|G|E|D,		//0x003B
	L_3 = A|B|G|C|D,		//0x006B
	L_4 = F|G|B|C,
	L_5 = A|F|G|C|D,
	L_6 = F|E|D|C|G,
	L_7 = A|B|C,
	L_8 = A|B|C|D|E|F|G,
	L_9 = A|B|C|D|F|G,
	L_A = A|B|C|E|F|G,
	L_B = F|E|D|C|G,
	L_C = A|F|E|D,
	L_D = G|E|D|C|B,
	L_E = A|F|G|E|D,
	L_F = A|F|G|E
};

/* I2C */

#define I2C_READ	1
#define I2C_WRITE	0
#define I2C_ACK		0
#define I2C_NACK	1

#define SENS_ADDRESS	0x4D	// For temperature device
#define TC74_CLK_FREQ	100000	// 100KHz
#define RTR		0
#define ADDR_WR		((SENS_ADDRESS << 1) | I2C_WRITE)
#define ADDR_RD		((SENS_ADDRESS << 1) | I2C_READ)

/* Global Variables */

extern volatile bool led;
extern volatile uint volt;
extern bool debug;


/* Functions */

/* Start of Global functions */
int powi(int x, int y);					// pow
uint abs(int val);					// Absolute value
void delay(uint n_intervals);				// Software delay in ms
void delay_ct(uint n_intervals);			// Hardware dealy in ms
/* Start of Global functions */

/* Start of display functions */
void init_led();					// Initialize hi led
void init_led_lo();					// Initialize lo led
void init_led_dual();					// Initialize both leds
int int2led(int value, bool dot, int base);
int char2led(uchar value, bool dot, int base);
/* End of display functions */

/* Start of switch functions */
void init_switch();
/* End of switch functions */

/* Start of ADC functions */
void init_analog_converter();				// Initialize analog converter RB14
void init_adc_ints();					// Enable ADC Interrupts
/* End of ADC functions */

/* Start of Timers functions */
void init_timerT1(int timer);				// Initialize timer T1
void init_timerT2(int timer);				// Initialize timer T2
void init_timerT3(int timer);				// Initialize timer T3
void init_TimerT1_ints();				// Enable timer T1 Interrupts
void init_TimerT2_ints();				// Enable timer T2 Interrupts
void init_TimerT3_ints();				// Enable timer T3 Interrupts
int getScalerT1(int scale);
int getScalerT2(int scale);
int getScalerT3(int scale);
int getPrescalerT1(int timer);
int getPrescalerT2(int timer);
int getPrescalerT3(int timer);
void setT3PWM(uint dutycycle);
void setPWMT3(uint dutycycle);
/* End of Timers functions */

/* Start of UART functions */
void config_UART(int baudrate, char parity, int stop);	// Configure UART
void init_UART();					// Initialize UART
void init_UART_ints();					// Enable UART Interrupts
void putc_val(char c);
void puts_val(char* s);
char getc_val();
void comDrv_flush(volatile ring *buffer);
void comDrv_putc(volatile ring *buffer, char c);
void comDrv_puts(volatile ring *buffer, char* s);
void comDrv_puti(volatile ring *buffer, int val);
bool comDrv_getc(volatile ring *buffer, char* c);
/* End of UART functions */

/* Start of I2C1 functions */
int getTemperature(int *temperature);
void init_i2c1(uint sclrate, uint dutycycle);
void i2c1_start();
void i2c1_stop();
int i2c1_send(uchar value);
char i2c1_receive(char ack);
/* End of I2C1 functions */

/* Start of EEPROM/SPI functions */
void spi2_setClock(uint clock_freq);
void spi2_init();
char eeprom_readStatus();
void eeprom_writeStatusCommand(char command);
void eeprom_writeData(int address, char value);
char eeprom_readData(int address);
void eeprom_clearData();
/* End of EEPROM/SPI functions */
