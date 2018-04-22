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

volatile uchar value2display = 0;
volatile uchar voltMin = 0xFF;
volatile uchar voltMax = 0;

void printUARTVolt(uchar value)
{
	putc_val(value/10 + '0');
	putc_val('.');
	putc_val(value%10 + '0');
	putc_val('V');
	putc_val('\n');
}

void _int_(_ADC_VECTOR) isr_adc(void)
{
	uint *ADC1BUF = (uint *) &ADC1BUF0;
	uint sum;
	int i;
	sum = 0;
	for(i=0; i < 8; i++)
		sum += ADC1BUF[i<<2];
	value2display = (sum >> 3) * 33 / 1023;
	if(value2display > voltMax)
		voltMax = value2display;
	if(value2display < voltMin)
		voltMin = value2display;
	//value2display = volt;
	IFS1bits.AD1IF = 0;
}

void _int_(_TIMER_1_VECTOR) isr_T1(void)
{
	AD1CON1bits.ASAM = 1;		// Start converting
	IFS0bits.T1IF = 0;
}

void _int_(_TIMER_3_VECTOR) isr_T3(void)
{
	static int i = 0;
	char2led(value2display, true, 10);
	if(++i == 100)
	{
		printUARTVolt(value2display);
		i = 0;
	}
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
		if(U1RXREG == 'L')
		{
			puts_val("Max Voltage: ");
			printUARTVolt(voltMax);
			puts_val("Min Voltage: ");
			printUARTVolt(voltMin);
		}
		IFS0bits.U1RXIF = 0;		// Reset interrupt flag
	}
}

int main() 
{
	//debug = true;
	init_led();
	init_switch();
	init_analog_converter();
	init_timerT1(4);
	init_timerT3(100);
	init_adc_ints();
	init_TimerT1_ints();	
	init_TimerT3_ints();
	//config_UART(115200, 'N', 1);
	init_UART_ints();
	
	while(1)
	{
		
	}
	return 0;
}
