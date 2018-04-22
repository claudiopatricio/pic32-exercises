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
	static unsigned char display7SCodes[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7c,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
	TRISB |= 0xF;			// RB0 to RB3 as input (To read from switch)
	TRISB &= 0x80FF;		// RB8 to RB14 as output (To write to displays)
	TRISD &= 0xFF9F;		// RD5 to RD6 as output (To write to High and Low)
	LATDbits.LATD5 = 1;		// Low display active
	LATDbits.LATD6 = 0;		// High display inactive
	int sw;
	while(1)
	{
		LATB &= 0x80FF;		// Clear Display
		sw = PORTB & 0xF;	// Switch value
		LATB |= display7SCodes[sw] << 8;	// Write on displays (Shift 8 to do from RB0 to RB8)
	}
}
