// Copyright (c) 2018 Cl�udio Patr�cio
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
volatile bool dot = true;
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

void _int_(_TIMER_3_VECTOR) isr_T3(void)
{
	//putChar('.');
	int2led(value2display, dot, 10);
	//LATDbits.LATD0 = !LATDbits.LATD0;
	IFS0bits.T3IF = 0;
}

int main() 
{
	int duty;
	int pwm[] = {3,15,40,90};
	init_led();
	init_switch();
	init_analog_converter();
	init_timerT1(4);
	init_timerT3(100);
	init_adc_ints();
	init_TimerT1_ints();	
	init_TimerT3_ints();
	setT3PWM(90);
	while(1)
	{
		if(PORTEbits.RE4 == 0 && PORTEbits.RE5 == 0)
		{
			LATDbits.LATD0 = 0;
			IEC0bits.T1IE = 1;
			value2display = volt;
		}
		else if(PORTEbits.RE4 == 1 && PORTEbits.RE5 == 0)
		{
			IEC0bits.T1IE = 0;
			LATDbits.LATD0 = 1;
			value2display = volt;
		}
		else if(PORTEbits.RE4 == 0 && PORTEbits.RE5 == 1)
		{
			IEC0bits.T1IE = 0;
			LATDbits.LATD0 = 1;
			duty = (PORTE & 0x00c0) >> 6;
			setPWMT3(pwm[duty]);
			dot = false;
			value2display = pwm[duty];
		}
		else
		{
			IEC0bits.T1IE = 1;
			LATDbits.LATD0 = 1;
			duty = (volt * 100) / 33;
			//printInt(volt, 2<<16 | 10);
			//putChar('\r');
			setPWMT3(duty);
			if(duty < 100)
			{
				dot = false;
				value2display = duty;
			}
			else
			{
				value2display = duty / 10;
				dot = true;
			}
		}
	}
	return 0;
}
