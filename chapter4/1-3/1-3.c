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
		for(i = 4997; i != 0; i--);
}

int sendToLed(char temp)
{
	switch(temp)
	{
		case '0':
			LATB = (LATB & 0xFF80) | L_0;
			break;
		case '1':
			LATB = (LATB & 0xFF80) | L_1;
			break;
		case '2':
			LATB = (LATB & 0xFF80) | L_2;
			break;
		case '3':
			LATB = (LATB & 0xFF80) | L_3;
			break;
		case '4':
			LATB = (LATB & 0xFF80) | L_4;
			break;
		case '5':
			LATB = (LATB & 0xFF80) | L_5;
			break;
		case '6':
			LATB = (LATB & 0xFF80) | L_6;
			break;
		case '7':
			LATB = (LATB & 0xFF80) | L_7;
			break;
		case '8':
			LATB = (LATB & 0xFF80) | L_8;
			break;
		case '9':
			LATB = (LATB & 0xFF80) | L_9;
			break;
		case 'a':
		case 'A':
			LATB = (LATB & 0xFF80) | L_A;
			break;
		case 'b':
		case 'B':
			LATB = (LATB & 0xFF80) | L_B;
			break;
		case 'c':
		case 'C':
			LATB = (LATB & 0xFF80) | L_C;
			break;
		case 'd':
		case 'D':
			LATB = (LATB & 0xFF80) | L_D;
			break;
		case 'e':
		case 'E':
			LATB = (LATB & 0xFF80) | L_E;
			break;
		case 'f':
		case 'F':
			LATB = (LATB & 0xFF80) | L_F;
			break;
		case 's':
		case 'S':
			LATBbits.LATB8 = !LATBbits.LATB8;
			LATBbits.LATB9 = !LATBbits.LATB9;
			break;
		default:
			LATB = (LATB & 0xFF80) | L_8 | DOT;
			LATBbits.LATB8 = 0;
			LATBbits.LATB9 = 1;
			break;
	}
	return 0;
}

int main() 
{
	char temp;
	TRISBbits.TRISB0 = 0;
	TRISBbits.TRISB1 = 0;
	TRISBbits.TRISB2 = 0;
	TRISBbits.TRISB3 = 0;
	TRISBbits.TRISB4 = 0;
	TRISBbits.TRISB5 = 0;
	TRISBbits.TRISB6 = 0;
	TRISBbits.TRISB7 = 0;
	TRISBbits.TRISB8 = 0;
	TRISBbits.TRISB9 = 0;
	LATB = L_A;
	LATBbits.LATB8 = 0;
	LATBbits.LATB9 = 1;
	while(1)
	{
		temp = getChar();
		sendToLed(temp);
	}
	
	return 0;
}
