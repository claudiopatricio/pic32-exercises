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

#include "ac2.h"

/* Variables */
volatile bool led = true;
bool debug = false;
volatile uint volt = 0;

/* Functions */

/* Start of Global functions */

int powi(int x, int y)
{
	int res = x;
	while(y > 1)
	{
		res *= x;
		y--;
	}
	return res;
}

uint abs(int val)
{
	if(val > 0) return val;
	return -val;
}

void delay(uint n_intervals)
{
	volatile uint i;
	for(;n_intervals != 0; n_intervals--)
		for(i = 4997; i != 0; i--);
}

void delay_ct(uint n_intervals)
{
	for(;n_intervals != 0; n_intervals--)
	{
		resetCoreTimer();
		while(readCoreTimer() < 20000);
	}
}

/* Start of Global functions */


/* Start of display functions */

void init_led()
{
	TRISB = TRISB&(~RB_ALL);
	LATBbits.LATB8 = 0;
	LATBbits.LATB9 = 1;
}

void init_led_lo()
{
	TRISB = TRISB&(~RB_ALL);
	LATBbits.LATB8 = 1;
	LATBbits.LATB9 = 0;
}

void init_led_dual()
{
	TRISB = TRISB&(~RB_ALL);
	LATBbits.LATB8 = 1;
	LATBbits.LATB9 = 1;
}

int int2led(int value, bool dot, int base)
{
	static uchar bit2LED[] = {L_0,L_1,L_2,L_3,L_4,L_5,L_6,L_7,L_8,L_9,L_A,L_B,L_C,L_D,L_E,L_F};
	uint aux = abs(value);
	if(value > 99)
	{
		value = 10;
		dot = true;
	}
	if(led)
	{
		aux /= base;
		LATB = bit2LED[aux];
		if(dot)
			LATB |= DOT;
		LATBbits.LATB8 = 1;
		LATBbits.LATB9 = 0;
	}
	else
	{
		aux %= base;
		LATB = bit2LED[aux];
		if(value < 0)
			LATB |= DOT;
		LATBbits.LATB8 = 0;
		LATBbits.LATB9 = 1;
	}
	led = !led;
	return 0;
}

int char2led(uchar value, bool dot, int base)
{
	static uchar bit2LED[] = {L_0,L_1,L_2,L_3,L_4,L_5,L_6,L_7,L_8,L_9,L_A,L_B,L_C,L_D,L_E,L_F};
	if(led)
	{
		value /= base;
		LATB = bit2LED[value];
		if(dot)
			LATB |= DOT;
		LATBbits.LATB8 = 1;
		LATBbits.LATB9 = 0;
	}
	else
	{
		value %= base;
		LATB = bit2LED[value];
		LATBbits.LATB8 = 0;
		LATBbits.LATB9 = 1;
	}
	led = !led;
	return 0;
}

/* End of display functions */


/* Start of switch functions */

void init_switch()
{
	//TRISE = TRISE|RE4|RE5|RE6|RE7;
	TRISEbits.TRISE4 = 1;
	TRISEbits.TRISE5 = 1;
	TRISEbits.TRISE6 = 1;
	TRISEbits.TRISE7 = 1;
}

/* End of switch functions */


/* Start of ADC functions */

void init_analog_converter()
{
	TRISBbits.TRISB14 = 1;		// RB14 digital output disconnected
	AD1PCFGbits.PCFG14 = 0;		// RB14 configured as an analog input
	AD1CHSbits.CH0SA = 14;		// Selects AN14/RB14 as an input for the A/D converter
	AD1CON2bits.SMPI = 7;		// 16 samples will be converted and stored in ADC1BUF0 to ADC1BUF3
	
	// Additional configurations
	AD1CON1bits.SSRC = 7;		// Conversion trigger selection bits
	AD1CON1bits.CLRASAM = 1;	// Stop converting when the 4st A/D converter interrupt in generated
	AD1CON3bits.SAMC = 16;		// Sample time is 16 TAD (TAD = 100ns)
	AD1CHSbits.CH0SA = 14;		// Input channel
	AD1CON1bits.ON = 1;		// Enable A/D converter
}

void init_adc_ints()
{
	IPC6bits.AD1IP = 1;		// ADC interrupt priority (1 to 6)
	IEC1bits.AD1IE = 1;		// Enable interrupts
	EnableInterrupts();		// Enable interrupts (Global)
	IFS1bits.AD1IF = 0;
}

/* End of ADC functions */


/* Start of Timers functions */

void init_timerT1(int timer)
{
	T1CONbits.TCKPS = getPrescalerT1(timer);		// prescaler
	PR1 = ((PBCLK / powi(2, getScalerT1(getPrescalerT1(timer)))) / timer) - 1;
	TMR1 = 0;			// Reset Timer T1 Register
	T1CONbits.TON = 1;		// Enable timer T1
	if(debug)
	{
		printStr("\nTimer 1: ");
		printInt10(timer);
		printStr("Hz");
		printStr("\nT1 TCKPS: ");
		printInt10(getPrescalerT1(timer));
		printStr(" (1:");
		printInt10(powi(2, getScalerT1(getPrescalerT1(timer))));
		printStr(" prescaler)");
		printStr("\nPR1: ");
		printInt10(((PBCLK / powi(2, getScalerT1(getPrescalerT1(timer)))) / timer) - 1);
		printStr("\n");
	}
}

void init_timerT2(int timer)
{
	T2CONbits.TCKPS = getPrescalerT2(timer);		// prescaler
	PR2 = ((PBCLK / powi(2, getScalerT2(getPrescalerT2(timer)))) / timer) - 1;
	TMR2 = 0;			// Reset Timer T2 Register
	T2CONbits.TON = 1;		// Enable timer T2
	if(debug)
	{
		printStr("\nTimer 2: ");
		printInt10(timer);
		printStr("Hz");
		printStr("\nT2 TCKPS: ");
		printInt10(getPrescalerT2(timer));
		printStr(" (1:");
		printInt10(powi(2, getScalerT2(getPrescalerT2(timer))));
		printStr(" prescaler)");
		printStr("\nPR2: ");
		printInt10(((PBCLK / powi(2, getScalerT2(getPrescalerT2(timer)))) / timer) - 1);
		printStr("\n");
	}
}

void init_timerT3(int timer)
{
	T3CONbits.TCKPS = getPrescalerT3(timer);		// prescaler
	PR3 = ((PBCLK / powi(2, getScalerT3(getPrescalerT3(timer)))) / timer) - 1;
	TMR3 = 0;			// Reset Timer T3 Register
	T3CONbits.TON = 1;		// Enable timer T3
	if(debug)
	{
		printStr("\nTimer 3: ");
		printInt10(timer);
		printStr("Hz");
		printStr("\nT3 TCKPS: ");
		printInt10(getPrescalerT3(timer));
		printStr(" (1:");
		printInt10(powi(2, getScalerT3(getPrescalerT3(timer))));
		printStr(" prescaler)");
		printStr("\nPR3: ");
		printInt10(((PBCLK / powi(2, getScalerT3(getPrescalerT3(timer)))) / timer) - 1);
		printStr("\n");
	}
}

void init_TimerT1_ints()
{
	IPC1bits.T1IP = 2;		// Interrupt Priority
	IEC0bits.T1IE = 1;		// Enable interrupts
	EnableInterrupts();		// Enable interrupts (Global)
	IFS0bits.T1IF = 0;		// Reset Timer T1 interrupt flag
}

void init_TimerT2_ints()
{
	IPC2bits.T2IP = 3;		// Interrupt Priority
	IEC0bits.T2IE = 1;		// Enable interrupts
	EnableInterrupts();		// Enable interrupts (Global)
	IFS0bits.T2IF = 0;		// Reset Timer T2 interrupt flag
}

void init_TimerT3_ints()
{
	IPC3bits.T3IP = 4;		// Interrupt Priority
	IEC0bits.T3IE = 1;		// Enable interrupts
	EnableInterrupts();		// Enable interrupts (Global)
	IFS0bits.T3IF = 0;		// Reset Timer T2 interrupt flag
}

int getScalerT1(int scale)
{
	switch(scale)
	{
		case 0:
			return 0;
		case 1:
			return 3;
		case 2:
			return 6;
		default:
			return 8;
	}
}

int getScalerT2(int scale)
{
	if(scale < 7)
		return scale;
	return 8;
}

int getScalerT3(int scale)
{
	if(scale < 7)
		return scale;
	return 8;
}
	
int getPrescalerT1(int timer)
{
	if(timer < 5)
		return 3;
	if(timer < 39)
		return 2;
	if(timer < 306)
		return 1;
	return 0;
}

int getPrescalerT2(int timer)
{
	if(timer < 5)
		return 7;
	if(timer < 10)
		return 6;
	if(timer < 20)
		return 5;
	if(timer < 39)
		return 4;
	if(timer < 77)
		return 3;
	if(timer < 153)
		return 2;
	if(timer < 306);
		return 1;
	return 0;
}

int getPrescalerT3(int timer)
{
	if(timer < 5)
		return 7;
	if(timer < 10)
		return 6;
	if(timer < 20)
		return 5;
	if(timer < 39)
		return 4;
	if(timer < 77)
		return 3;
	if(timer < 153)
		return 2;
	if(timer < 306);
		return 1;
	return 0;
}

void setT3PWM(uint dutycycle)
{
	OC1CONbits.OCM = 6;			// PWM mode on OCX; fault pin disabled
	OC1CONbits.OCTSEL = 1;			// T3
	setPWMT3(dutycycle);
	OC1CONbits.ON = 1;
}

void setPWMT3(uint dutycycle)
{
	OC1RS = ((PR3 + 1) * dutycycle)/100;
}

/* End of Timers functions */


/* Start of UART functions */

void config_UART(int baudrate, char parity, int stop)
{
	U1MODEbits.BRGH = 0;		// 0 - Normal speed (16 bits) | 1 - High Speed (4 bits)
	U1BRG = 10;
	if(baudrate >= 600 && baudrate < 115200)
		U1BRG = (PBCLK / (16 * baudrate)) - 1;
	switch(parity)					// Parity bits (N = no parity | E = even | O = odd | S = 9-bit)
	{
		case 'O':
			U1MODEbits.PDSEL = 2;
			break;
		case 'E':
			U1MODEbits.PDSEL = 1;
			break;
		case 'S':
			U1MODEbits.PDSEL = 3;
			break;
		default:
			U1MODEbits.PDSEL = 0;
			break;
	}
	U1MODEbits.STSEL = stop - 1;	// Number of stop bits (0 = 1 bit)
	U1STAbits.UTXEN = 1;		// Enable transmit bit
	U1STAbits.URXEN = 1;		// Enable receiver bit
	U1MODEbits.ON = 1;		// Enable UART
}

void init_UART()
{
	U1STAbits.UTXEN = 1;		// Enable transmit bit
	U1STAbits.URXEN = 1;		// Enable receiver bit
	U1MODEbits.ON = 1;		// Enable UART
}

void init_UART_ints()
{
	// Some bits that need to be enabled.
	U1STAbits.UTXEN = 1;		// Enable transmit bit
	U1STAbits.URXEN = 1;		// Enable receiver bit
	U1MODEbits.ON = 1;		// Enable UART
	//
	IPC6bits.U1IP = 1;		// Interrupt Priority
	IEC0bits.U1RXIE = 1;		// Enable Rx interrupt (U1RXIF)
	IEC0bits.U1EIE = 1;		// Enable error interrupt (U1EIF)
	EnableInterrupts();		// Enable interrupts (Global)
	IFS0bits.U1RXIF = 0;		// Reset Rx interrupt flag
}

void putc_val(char c)
{
	while(U1STAbits.UTXBF == 1);
	U1TXREG = c;
}

void puts_val(char* s)
{
	while(*s != '\0')
	{
		while(U1STAbits.UTXBF == 1);
		U1TXREG = *s;
		s++;
	}
}

char getc_val()
{
	char dummy;
	if(U1STAbits.OERR == 1)
		U1STAbits.OERR = 0;
	while(U1STAbits.URXDA == 0);
	if(U1STAbits.FERR == 1 || U1STAbits.PERR == 1)
	{
		dummy = U1RXREG;
		return '\0';
	}
	return U1RXREG;
}

void comDrv_flush(volatile ring *buffer)
{
	buffer->head = 0;
	buffer->tail = 0;
	buffer->count = 0;
	buffer->overrun = false;
}

void comDrv_putc(volatile ring *buffer, char c)
{
	while(buffer->count == BUF_SIZE);
	DisableUart1TxInterrupt();
	buffer->count++;
	buffer->data[buffer->tail++] = c;
	buffer->tail &= INDEX_MASK;
	EnableUart1TxInterrupt();
}

void comDrv_puts(volatile ring *buffer, char* s)
{
	int i = 0;
	while(s[i] != '\0')
	{
		comDrv_putc(buffer, s[i++]);
	}
}

void comDrv_puti(volatile ring *buffer, int val)
{
	int n = 0;
	int aux_int[100];
	while(val > 10)
	{
		aux_int[n++] = val%10;
		val /= 10;
	}
	
	comDrv_putc(buffer, val + '0');
	while(n > 0)
	{
		comDrv_putc(buffer, aux_int[--n] + '0');
	}
}

bool comDrv_getc(volatile ring *buffer, char* c)
{
	if(buffer->count == 0) return false;
	DisableUart1RxInterrupt();
	*c = buffer->data[buffer->head++];
	buffer->count--;
	buffer->head &= INDEX_MASK;
	buffer->overrun = false;
	EnableUart1RxInterrupt();
	return true;
}

/* End of UART functions */


/* Start of I2C1 functions */

int getTemperature(int *temperature)
{
	int ack;
	i2c1_start();
	ack = i2c1_send(ADDR_WR);
	ack = i2c1_send(RTR);
	i2c1_start();
	ack = i2c1_send(ADDR_RD);
	if(ack != 0)
	{
		i2c1_stop();
		printStr("Erro: Não é possível ler temperatura\n");
		return ack;
	}
	*temperature = i2c1_receive(I2C_NACK);
	return ack;
}

void init_i2c1(uint sclrate, uint dutycycle)
{
	I2C1BRG = ((PBCLK + sclrate) / ((100 / dutycycle) * sclrate)) - 1;
	I2C1CONbits.ON = 1;
}

void i2c1_start()
{
	I2C1CONbits.SEN = 1;
	while(I2C1CONbits.SEN == 1);
}

void i2c1_stop()
{
	while((I2C1CON & 0x001F) != 0);
	I2C1CONbits.PEN = 1;
	while(I2C1CONbits.PEN == 1);
}

int i2c1_send(uchar value)
{
	I2C1TRN = value;
	while(I2C1STATbits.TRSTAT == 1);
	return I2C1STATbits.ACKSTAT;
}

char i2c1_receive(char ack)
{
	while((I2C1CON & 0x001F) != 0);
	I2C1CONbits.RCEN = 1;
	while(I2C1STATbits.RBF == 0);
	if(ack != 0) ack = 1;
	I2C1CONbits.ACKDT = ack;
	I2C1CONbits.ACKEN = 1;
	while(I2C1CONbits.ACKEN == 1);
	return I2C1RCV;
}

/* End of I2C1 functions */


/* Start of EEPROM/SPI functions */

void spi2_setClock(uint clock_freq)
{
	SPI2BRG = ((PBCLK + clock_freq) / (2 * clock_freq)) - 1;
}

void spi2_init()
{
	volatile char trash;
	SPI2CONbits.ON = 0;		// Disable SPI2 Module
	//SPI2CONbits.SIDL = 0;		// Idle State
	SPI2CONbits.CKP = 0;		// Idle state as logic 0
	SPI2CONbits.CKE = 1;		// Active Clock to Idle Clock State
	SPI2CONbits.SMP = 0;		// Sample phase bit at middle of data output time
	//SPI2CON2bits.AUDEN = 0;	// 8 bit
	SPI2CONbits.MODE16 = 0;		// 16-bits if enabled
	SPI2CONbits.MODE32 = 0;		// 32-bits if enabled
	SPI2CONbits.ENHBUF = 1;		// Enable enhanced buffer mode
	SPI2CONbits.MSSEN = 1;		// Enable Slave select support
	//SPI2CONbits.SSEN = 1;
	SPI2CONbits.MSTEN = 1;		// Master mode
	while(SPI2STATbits.SPIRBE == 0)
	{
		trash = SPI2BUF;
	}
	SPI2STATbits.SPIROV = 0;	// Clear overflow error flag
	SPI2STATCLR;			// Clear all stats
	SPI2CONbits.ON = 1;		// Enable SPI2 module
}

char eeprom_readStatus()
{
	volatile char trash;
	while(SPI2STATbits.SPIRBE == 0) trash = SPI2BUF;	// Clear RX FIFO
	SPI2STATbits.SPIROV = 0;	// Clear overflow error flag
	SPI2STATCLR;			// Clear all stats
	SPI2BUF = RDSR;			// Send RDSR command
	SPI2BUF = 0;			// Send empty
	while(SPI2STATbits.SPIBUSY);	// Wait while SPI is working
	trash = SPI2BUF;		// First bit is trash
	return SPI2BUF;
}

void eeprom_writeStatusCommand(char command)
{
	while(eeprom_readStatus() & 0x01);	// Wait while WIP is true (Write in progress)
	SPI2BUF = command;		// TX FIFO
	while(SPI2STATbits.SPIBUSY);
}

void eeprom_writeData(int address, char value)
{
	address &= 0x01FF;
	while(eeprom_readStatus() & 0x01);	// Wait while WIP is true (Write in progress)
	eeprom_writeStatusCommand(SPI2_WREN);
	SPI2BUF = WRITE | ((address & 0x100) >> 5);
	SPI2BUF = address & 0x00FF;
	SPI2BUF = value;
	while(SPI2STATbits.SPIBUSY);	// Wait while SPI is working
}

char eeprom_readData(int address)
{
	volatile char trash;
	address &= 0x01FF;
	while(eeprom_readStatus() & 0x01);	// Wait while WIP is true (Write in progress)
	SPI2BUF = READ | ((address & 0x100) >> 5);
	SPI2BUF = address & 0x00FF;
	SPI2BUF = 0;
	while(SPI2STATbits.SPIBUSY);	// Wait while SPI is working
	trash = SPI2BUF;
	trash = SPI2BUF;
	return SPI2BUF;
}

void eeprom_clearData()
{
	int aux = 0;
	while(aux < 512)
		eeprom_writeData(aux++, 0);
}

/* End of EEPROM/SPI functions */
