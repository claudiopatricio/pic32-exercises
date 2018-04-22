# Copyright (c) 2018 Cláudio Patrício
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

		.equ	PRINT_STR,8
		.equ	PUT_CHAR,3
		.equ	GET_CHAR,2
		.equ	IN_KEY,1

		.data

		.text
		.globl main

main:						# while(1)
						# {
		li $v0, GET_CHAR
		syscall				# 	c = getChar()
		beq $v0, '\n', main		# 	if(c == '\n') break;
		move $a0, $v0
		li $v0, PUT_CHAR
		syscall				# 	putChar(c);
		j main			# }
main_w0e:	li $v0, 1			# return 1
		jr $ra
