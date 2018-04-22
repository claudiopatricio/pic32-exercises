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

#include "../../ac2.c"

volatile int value2display = 0;
volatile bool dot = false;
int temp;
volatile bool eeprom_write = false;
volatile int eeprom_addr = 2;

volatile ring txb;
volatile ring rxb;

void _int_(_ADC_VECTOR) isr_adc(void)
{
	uint *ADC1BUF = (uint *) &ADC1BUF0;
	uint sum;
	int i;
	sum = 0;
	for(i=0; i < 8; i++)
		sum += ADC1BUF[i<<2];
	volt = (sum >> 3) * 33 / 1023;
	IFS1bits.AD1IF = 0;
}

void _int_(_TIMER_1_VECTOR) isr_T1(void)
{
	AD1CON1bits.ASAM = 1;		// Start converting
	IFS0bits.T1IF = 0;
}

void _int_(_TIMER_2_VECTOR) isr_T2(void)
{
	//AD1CON1bits.ASAM = 1;		// Start converting
	static int aux_timer = 0;
	if(getTemperature(&temp) != 0)
	{
		i2c1_stop();
		printStr("Erro: Não é possível ler temperatura\n");
	}
	value2display = temp;
	dot = false;
	if(eeprom_write)
	{
		if(aux_timer >= 150)
		{
			volatile char aux_count = eeprom_readData(0);
			volatile char aux_pos = eeprom_readData(1);
			eeprom_writeData(aux_pos, temp);
			if(aux_count < 64)
				eeprom_writeData(0, aux_count + 1);
			if(aux_pos == 65)
				eeprom_writeData(1, 2);
			else
				eeprom_writeData(1, aux_count+1);
			aux_timer = 0;
		}
		else
			aux_timer++;
	}
	IFS0bits.T2IF = 0;
}

void _int_(_TIMER_3_VECTOR) isr_T3(void)
{
	int2led(value2display, dot, 10);
	IFS0bits.T3IF = 0;
}

void _int_(_UART_1_VECTOR) usr_uart1(void)
{
	if(IFS0bits.U1EIF)
	{
		if(U1STAbits.OERR == 1)
			U1STAbits.OERR = 0;
		else
		{
			char dummy = U1RXREG;
			dummy = 0;
		}
		IFS0bits.U1EIF = 0;
	}
	if(IFS0bits.U1RXIF)
	{
		while(U1STAbits.URXDA == 1 && rxb.count < BUF_SIZE)
		{
			rxb.data[rxb.tail++] = U1RXREG;
			rxb.tail &= INDEX_MASK;
			rxb.count++;
		}
		if(rxb.count == BUF_SIZE) rxb.overrun = true;
		IFS0bits.U1RXIF = 0;
	}	
	if(IFS0bits.U1TXIF)
	{
		while(txb.count > 0 && U1STAbits.UTXBF == 0)
		{
			U1TXREG = txb.data[txb.head++];
			txb.head &= INDEX_MASK;
			txb.count--;
		}
		if(txb.count == 0)
			DisableUart1TxInterrupt();
		IFS0bits.U1TXIF = 0;
	}
}

int main() 
{
	int duty;
	int pwm[] = {3,15,40,90};
	char key;
	int aux;
	spi2_init();
	spi2_setClock(EEPROM_CLOCK);
	init_led();
	init_switch();
	init_analog_converter();
	init_timerT1(4);
	init_timerT2(10);
	init_timerT3(100);
	init_adc_ints();
	init_TimerT1_ints();	
	init_TimerT2_ints();	
	init_TimerT3_ints();
	init_i2c1(TC74_CLK_FREQ, 50);
	setT3PWM(90);
	init_UART_ints();
	
	comDrv_flush(&rxb);
	comDrv_flush(&txb);
	while(1)
	{
		if(PORTEbits.RE4 == 0 && PORTEbits.RE5 == 0)
		{
			LATDbits.LATD0 = 0;
			IEC0bits.T1IE = 1;
			IEC0bits.T2IE = 0;
			value2display = volt;
			dot = true;
		}
		else if(PORTEbits.RE4 == 1 && PORTEbits.RE5 == 0)
		{
			IEC0bits.T1IE = 0;
			IEC0bits.T2IE = 0;
			LATDbits.LATD0 = 1;
			value2display = volt;
			dot = true;
		}
		else if(PORTEbits.RE4 == 0 && PORTEbits.RE5 == 1)
		{
			IEC0bits.T1IE = 0;
			IEC0bits.T2IE = 0;
			LATDbits.LATD0 = 1;
			duty = (PORTE & 0x00c0) >> 6;
			setPWMT3(pwm[duty]);
			dot = false;
			value2display = pwm[duty];
		}
		else
		{
			IEC0bits.T1IE = 0;
			IEC0bits.T2IE = 1;
			LATDbits.LATD0 = 1;
			key = 0;
			comDrv_getc(&rxb, &key);
			if(key == 'R' || key == 'r')
			{
				eeprom_writeData(0, 0);
				eeprom_writeData(1, 2);
				comDrv_puts(&txb, "Reset da temperatura efectuado com sucesso!\n");
			}
			else if(key == 'L' || key == 'l')
			{
				getTemperature(&temp);
				eeprom_writeData(2, temp);
				eeprom_writeData(0, 3);
				//eeprom_addr = 3;
				eeprom_write = true;
				comDrv_puts(&txb, "Log de temperatura iniciado com sucesso\n");
			}
			else if(key == 'S' || key == 's')
			{
				eeprom_write = false;
				//eeprom_addr = 2;
				aux = 0;
				while(eeprom_readData(0) > aux)
				{
					comDrv_puti(&txb, (int)eeprom_readData(2 + aux++));
					comDrv_putc(&txb, ' ');
					//eeprom_writeData(0, eeprom_readData(0) - 1);
				}
				comDrv_putc(&txb, '\n');
			}
		}
	}
	return 0;
}
