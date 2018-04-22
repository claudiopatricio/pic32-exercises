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
		
		
		.equ	CORE_SECOND,20000000
		
		.data
msg0:		.asciiz	"Indique o numero de horas:minutos:segundos: "
		
		.text
		.globl main


main:		la $a0, msg0
		li $v0, PRINT_STR
		syscall				# printStr("Indique o numero de horas:minutos:segundos: ")
		li $v0, READ_INT10
		syscall				# readInt10()
		move $t0, $v0
		li $a0, ':'
		li $v0, PUT_CHAR
		syscall				# putChar(':')
		li $v0, READ_INT10
		syscall				# readInt10()
		move $t1, $v0
		li $a0, ':'
		li $v0, PUT_CHAR
		syscall				# putChar(':')
		li $v0, READ_INT10
		syscall				# readInt10()
		move $t2, $v0
		li $a0, '\n'
		li $v0, PUT_CHAR
		syscall				# putChar('\n')
		li $v0, RESET_CORETIMER
		syscall				# resetCoreTimer()
		j main_print
main_w0i:					# while(1)
		li $v0, READ_CORETIMER		# {
		syscall				#	readCoreTimer()
		bleu $v0, CORE_SECOND, main_w0i	#	if(CORE_SECOND < readCoreTimer())
						#	{
		bltu $t2, 59, main_addsec	#		if(seconds >= 59)
		move $t2, $0			#			seconds = 0
		addiu $t1, $t1, 1		#			minutes++
		bltu $t1, 60, main_print	#		if(minutes >= 60)
		move $t1, $0			#			minutes = 0
		addiu $t0, $t0, 1		#			hours++
		bltu $t0, 23, main_print	#		if(hours >= 24)
		move $t0, $0			#			hours = 0
		j main_print
main_addsec:	addiu $t2, $t2, 1
main_print:	li $a0, '\r'
		li $v0, PUT_CHAR
		syscall				#		putChar('\r')
		bgeu $t0, 10, main_phour	#		if(hours < 10)
		li $a0, '0'
		li $v0, PUT_CHAR
		syscall				#		putChar('0')
main_phour:	move $a0, $t0
		li $v0, PRINT_INT10
		syscall				#		printInt10(hours)
		li $a0, ':'
		li $v0, PUT_CHAR
		syscall				#		putChar(':')
		bgeu $t1, 10, main_pminute	#		if(minutes < 10)
		li $a0, '0'
		li $v0, PUT_CHAR
		syscall				#		putChar('0')
main_pminute:	move $a0, $t1
		li $v0, PRINT_INT10
		syscall				#		printInt10(minutes)
		li $a0, ':'
		li $v0, PUT_CHAR
		syscall				#		putChar(':')
		bgeu $t2, 10, main_psecond	#		if(seconds < 10)
		li $a0, '0'
		li $v0, PUT_CHAR
		syscall				#		putChar('0')
main_psecond:	move $a0, $t2
		li $v0, PRINT_INT10
		syscall				#		printInt10(minutes)
		li $v0, RESET_CORETIMER
		syscall				#		resetCoreTimer()
						#	}
		j main_w0i			# }
		jr $ra				# return
