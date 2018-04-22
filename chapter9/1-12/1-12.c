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
volatile ring txb;
volatile ring rxb;

void _int_(_ADC_VECTOR) isr_adc(void)
{
	//uint *ADC1BUF = (uint *) &ADC1BUF0;
	IFS1bits.AD1IF = 0;
}

void _int_(_TIMER_1_VECTOR) isr_T1(void)
{
	//AD1CON1bits.ASAM = 1;		// Start converting
	IFS0bits.T1IF = 0;
}

void _int_(_TIMER_3_VECTOR) isr_T3(void)
{
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
	//debug = true;
	//config_UART(115200, 'N', 1);
	init_UART_ints();
	
	comDrv_flush(&rxb);
	comDrv_flush(&txb);
	
	char temp;
	comDrv_puts(&txb, "PIC32 UART Device-Driver\n");
	while(1)
	{
		if(comDrv_getc(&rxb, &temp))
		{
			if(temp == 'S')
				comDrv_puts(&txb, "\nThank you for using this device driver for PIC32\n");
			else
				comDrv_putc(&txb, temp);
		}
	}
	return 0;
}
