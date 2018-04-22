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

		.equ	RESET_CORETIMER,12
		.equ	READ_CORETIMER,11
		.equ	READ_STR,9
		.equ	PRINT_STR,8
		.equ	PRINT_INT10,7
		.equ	PRINT_INT,6
		.equ	READ_INT10,5
		.equ	READ_INT,4
		.equ	PUT_CHAR,3
		.equ	GET_CHAR,2
		.equ	IN_KEY,1
		
		
		.equ	CORE_SECOND, 0x01312D00
		
		.data
		
		.text
		.globl main


main:		li $t0, 0			# n = 0
		move $a0, $t0
		li $a1, 10
		li $v0, PRINT_INT
		syscall				# printInt(0, 10)
		li $v0, RESET_CORETIMER
		syscall				# resetCoreTimer()
main_w0i:					# while(1)
		li $v0, READ_CORETIMER		# {
		syscall				#	readCoreTimer()
		bleu $v0, CORE_SECOND, main_w0i	#	if(i < readCoreTimer())
						#	{
		addiu $t0, $t0, 1		#		i++
		li $a0, '\r'
		li $v0, PUT_CHAR
		syscall				#		putChar('\r')
		move $a0, $t0
		li $a1, 10
		li $v0, PRINT_INT
		syscall				#		printInt(n, 10)
		li $v0, RESET_CORETIMER
		syscall				#		resetCoreTimer()
						#	}
		j main_w0i			# }
		jr $ra				# return
