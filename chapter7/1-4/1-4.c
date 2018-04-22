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

volatile uint value2display = 0;
void _int_(_ADC_VECTOR) isr_adc(void)
{
	uint *ADC1BUF = (uint *) &ADC1BUF0;
	uint sum;
	int i;
	sum = 0;
	for(i=0; i < 8; i++)
		sum += ADC1BUF[i<<2];
	volt = (sum >> 3) * 33 / 1023;
	value2display = volt;
	IFS1bits.AD1IF = 0;
	
}

void _int_(_TIMER_1_VECTOR) isr_T1(void)
{
	AD1CON1bits.ASAM = 1;		// Start converting
	IFS0bits.T1IF = 0;
}

void _int_(_TIMER_3_VECTOR) isr_T3(void)
{
	//putChar('.');
	int2led(volt, true, 10);
	IFS0bits.T3IF = 0;
}

int main() 
{
	init_led();
	init_analog_converter();
	init_timerT1(4);
	init_timerT3(100);
	init_adc_ints();
	init_TimerT1_ints();	
	init_TimerT3_ints();
	
	while(1);
	
	return 0;
}
