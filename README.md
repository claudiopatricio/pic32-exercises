# Assembly and C exercises for DETPIC32 (PIC32 microcontroller)

![DETPIC32](https://github.com/claudiopatricio/pic32-exercises/blob/master/practice-exam/detpic32.jpg)

Collection of exercises from Computer Architecture II at University of Aveiro @ 2013/14

**Note**: The exercises reports are written in Portuguese, and the solutions may contain some notes in Portuguese as well.

## How to run

* 1. Install the right compiler for your computer (32bit or 64bit)
* 2. Compile with "**pcompile file.c**", or "**pcompile file.c ../../ac2.c**" when you need the additional library **ac2**
* 3. Send to DETIPIC32 with "ldpic32 -w file.hex"

## Important files

Some of the code are needed for multiple exercises, for that reason there are 2 files (code and header) that works as a library for DETPIC32.

You can also use this files in your projects as a standalone library.

### ac2.c & ac2.h

Library for main actions needed in DETPIC32

# Donate

If you like my work and want to contribute by donating money, check my [Donation page](http://anjo2.com/donate/)

# License
Copyright (c) 2018 Cláudio Patrício <<cpatricio@ua.pt>>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.