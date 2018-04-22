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
	//int2led(value2display, false, 10);
	IFS0bits.T3IF = 0;
}

int main() 
{
	volatile char teste, teste2;
	int addr = 0;
	//debug = true;
	spi2_init();
	spi2_setClock(EEPROM_CLOCK);
	while(1)
	{
		teste = getChar();
		if(teste == 'R' || teste == 'r')
		{
			printStr("Indique o endereço a ler: ");
			addr = readInt(16);
			teste2 = eeprom_readData(addr);
			printStr("\nValor na memória: ");
			printInt(teste2, 16);
			putChar('\n');
		}
		else if(teste == 'W' || teste == 'w')
		{
			printStr("Indique o endereço a escrever: ");
			addr = readInt(16);
			printStr("\nIndique o valor a gravar: ");
			eeprom_writeStatusCommand(SPI2_WREN);
			eeprom_writeData(addr, readInt(16));
			putChar('\n');
		}
	}
	return 0;
}
