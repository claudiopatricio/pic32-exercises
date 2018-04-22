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
		.equ	PRINT_INT10,7
		.equ	PRINT_INT,6
		.equ	READ_INT10,5
		.equ	PUT_CHAR,3
		.equ	GET_CHAR,2
		.equ	IN_KEY,1

		.data
msg0:		.asciiz "\nIntroduza um numero (sinal e módulo): "
msg1:		.asciiz "\nValor lido em base 2: "
msg2:		.asciiz "\nValor lido em base 16: "
msg3:		.asciiz "\nValor lido em base 10 (unsigned): "
msg4:		.asciiz "\nValor lido em base 10 (signed): "
		.text
		.globl main

main:						# while(1)
						# {
		la $a0, msg0
		li $v0, PRINT_STR
		syscall				#	printStr("\nIntroduza um numero (sinal e módulo): ")
		li $v0, READ_INT10
		syscall				# 	value = readInt10()
		move $t0, $v0			#	value
		la $a0, msg1
		li $v0, PRINT_STR
		syscall				#	printStr("\nValor lido em base 2: ")
		move $a0, $t0
		li $a1, 2
		li $v0, PRINT_INT
		syscall				#	printInt(value, 2)
		la $a0, msg2
		li $v0, PRINT_STR
		syscall				#	printStr("\nValor lido em base 16: ")
		move $a0, $t0
		li $a1, 16
		li $v0, PRINT_INT
		syscall				#	printInt(value, 16)
		la $a0, msg3
		li $v0, PRINT_STR
		syscall				#	printStr("\nValor lido em base 10 (unsigned): ")
		move $a0, $t0
		li $a1, 10
		li $v0, PRINT_INT
		syscall				#	printInt(value, 10)
		la $a0, msg4
		li $v0, PRINT_STR
		syscall				#	printStr("\nValor lido em base 10 (signed): ")
		move $a0, $t0
		li $v0, PRINT_INT10
		syscall				#	printInt10(value)
		j main				# }
main_w0e:	jr $ra
