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


main:		
		
main_w0i:					# while(1)
		li $v0, RESET_CORETIMER		# {
		syscall				#	resetCoreTimer()
		li $a0, 1
		jal delay			#	delay(1)
		li $v0, READ_CORETIMER
		syscall				#	readCoreTimer()
		move $a0, $v0
		li $a1, 10
		li $v0, PRINT_INT
		syscall				#	printInt(readCoreTimer, 10)
		li $a0, '\n'
		li $v0, PUT_CHAR
		syscall				#	putChar('\n')
		j main_w0i			# }
		jr $ra				# return

delay:		addiu $sp, $sp, -8		# void delay(uint n_intervals)
		sw $ra, 0($sp)
		sw $s0, 4($sp)			# $s0: i
delay_w0i:	beqz $a0, delay_w0e		# while(n_intervals != 0)
						# {
		li $s0, 6000			#	i = 6000 // Calibration Value
delay_w1i:	beqz $s0, delay_w1e		#	while(i != 0)
		addiu $s0, -1			#		i--;
		j delay_w1i
delay_w1e:	addiu $a0, -1			#	n_intervals--
		j delay_w0i			# }
delay_w0e:	lw $ra, 0($sp)
		lw $s0, 4($sp)
		addiu $sp, $sp, 8
		jr $ra				# return
