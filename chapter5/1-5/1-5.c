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

/* Includes */
#include "detpic32.h"


/* Structures */

typedef unsigned char uchar;
typedef unsigned int uint;

enum _REbits {
	B = 0x0001,
	A = 0x0002,
	F = 0x0004,
	G = 0x0008,
	E = 0x0010,
	D = 0x0020,
	C = 0x0040,
	DOT = 0x0080,
	S_HI = 0x0100,
	S_LO = 0x0200
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
	

/* Global Variables */


/* Functions */

void delay(uint n_intervals)
{
	uint i;
	for(;n_intervals != 0; n_intervals--)
		for(i = 1000000; i != 0; i--);
}

void init()
{
	TRISBbits.TRISB14 = 1;		// RB14 digital output disconnected
	AD1PCFGbits.PCFG14 = 0;		// RB14 configured as an analog input
	AD1CHSbits.CH0SA = 14;		// Selects AN14/RB14 as an input for the A/D converter
	AD1CON2bits.SMPI = 15;		// 16 samples will be converted and stored in ADC1BUF0 to ADC1BUF3
	
	// Additional configurations
	AD1CON1bits.SSRC = 7;		// Conversion trigger selection bits
	AD1CON1bits.CLRASAM = 4;	// Stop converting when the 1st A/D converter interrupt in generated
	AD1CON3bits.SAMC = 16;		// Sample time is 16 TAD (TAD = 100ns)
	AD1CHSbits.CH0SA = 14;		// Input channel
	AD1CON1bits.ON = 1;		// Enable A/D converter
}

int main() 
{
	init();
	int *aux = (int *)(&ADC1BUF0);
	int i;
	while(1)
	{
		AD1CON1bits.ASAM = 1;		// Start converting
		while(IFS1bits.AD1IF == 0);
		for(i = 0; i < 16; i++)
		{
			printInt(aux[i*4],10);
			putChar(' ');
		}
		putChar('\n');
	}
	
	return 0;
}
