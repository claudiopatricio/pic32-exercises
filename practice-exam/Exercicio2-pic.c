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

#include <detpic32.h>

int main()
{
	static unsigned char display7SCodes[] = {0x77,0x41,0x3B,0x6B,0x4D,0x6E,0x7E,0x43,0x7F,0x6F,0x5F,0x7C,0x36,0x79,0x3E,0x1E};
	TRISE |= 0xF0;			// RE4 to RE7 as input (To read from switch)
	TRISB &= 0xFF00;		// RB0 to RB7 as output (To write to displays)
	TRISB &= 0xFC0F;		// RB8 to RB9 as output (To write to High and Low)
	LATBbits.LATB8 = 0;		// High display inactive
	LATBbits.LATB9 = 1;		// Low display active
	int sw;
	while(1)
	{
		LATB &= 0xFF00;			// Clear Display
		sw = (PORTE & 0xF0) >> 4;	// Switch value
		LATB |= display7SCodes[sw];	// Write on displays (Shift 8 to do from RB0 to RB8)
	}
}
