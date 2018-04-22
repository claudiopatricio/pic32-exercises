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

		.equ	READ_STR,9
		.equ	PRINT_STR,8
		.equ	PRINT_INT10,7
		.equ	PRINT_INT,6
		.equ	READ_INT10,5
		.equ	PUT_CHAR,3
		.equ	GET_CHAR,2
		.equ	IN_KEY,1

		.equ	STR_MAX_SIZE,20
		.equ	STR_SIZE1,21
		.equ	STR_SIZE2,41

		.data
msg0:		.asciiz "Introduza 2 strings: "
msg1:		.asciiz "\nResultados:\n"

str1:		.space	STR_SIZE1
str2:		.space	STR_SIZE1
str3:		.space	STR_SIZE2
		.text
		.globl main

main:		addiu $sp, $sp, -4
		sw $ra, 0($sp)
		la $a0, msg0
		li $v0, PRINT_STR
		syscall				# printStr("Introduza 2 strings: ")
		la $a0, str1
		li $a1, STR_MAX_SIZE
		li $v0, READ_STR
		syscall				# readStr(str1, STR_MAX_SIZE)
		li $a0, '\n'
		li $v0, PUT_CHAR
		syscall				# putChar('\n')
		la $a0, str2
		li $a1, STR_MAX_SIZE
		li $v0, READ_STR
		syscall				# readStr(str2, STR_MAX_SIZE)
		la $a0, msg1
		li $v0, PRINT_STR
		syscall				# printStr("\nResultados:\n")
		la $a0, str1
		jal strlen			# strlen(str1)
		move $a0, $v0
		li $a1, 10
		li $v0, PRINT_INT
		syscall				# printInt(strlen(str1), 10)
		li $a0, '\n'
		li $v0, PUT_CHAR
		syscall				# putChar('\n')
		la $a0, str2
		jal strlen			# strlen(str2)
		move $a0, $v0
		li $a1, 10
		li $v0, PRINT_INT
		syscall				# printInt(strlen(str2), 10)
		li $a0, '\n'
		li $v0, PUT_CHAR
		syscall				# putChar('\n')
		la $a0, str3
		la $a1, str1
		jal strcpy			# strcpy(str3, str1)
		la $a0, str3
		la $a1, str2
		jal strcat			# strcat(str3, str2)
		move $a0, $v0
		li $v0, PRINT_STR
		syscall				# printStr(strcat(str3, str2))
		li $a0, '\n'
		li $v0, PUT_CHAR
		syscall				# putChar('\n')
		la $a0, str1
		la $a1, str2
		jal strcmp			# strcmp(str1, str2)
		move $a0, $v0
		li $v0, PRINT_INT10
		syscall				# printInt10(strcmp(str1, str2))
		li $v0, 0			# return 0
		lw $ra, 0($sp)
		addiu $sp, $sp, 4
		jr $ra


strlen:						# int strlen(char *s)
		li $v0, 0			# len = 0
strlen_w0i:	lb $t0, 0($a0)			# while(len != 0)
		beqz $t0, strlen_w0e		# {
		addiu $v0, $v0, 1		#	len++
		addiu $a0, $a0, 1		#	s++
		j strlen_w0i			# }
strlen_w0e:	jr $ra				# return len


strcat:		addiu $sp, $sp, -8		# char* strcat(char* dst, char* src)
		sw $ra, 0($sp)
		sw $s0, 4($sp)
		move $s0, $a0			# char* rp = dst
strcat_w0i:	lb $t0, 0($a0)			# while(*dst != 0)
		beqz $t0, strcat_w0e		# {
		addiu $a0, $a0, 1		#	dst++
		j strcat_w0i			# }
strcat_w0e:	jal strcpy			# strcpy(dst, src)
		move $v0, $s0
		lw $ra, 0($sp)
		addiu $sp, $sp, 8
		jr $ra				# return rp


strcpy:						# char* strcpy(char* dst, char* src)
		move $v0, $a0			# char* rp = dst
strcpy_w0i:	lb $t0, 0($a1)
		sb $t0, 0($a0)			# while((*dst = *src) != 0)
		beqz $t0, strcpy_w0e		# {
		addiu $a0, $a0, 1		#	dst++
		addiu $a1, $a1, 1		#	src++
		j strcpy_w0i			# }
strcpy_w0e:	jr $ra				# return rp


strcmp:						# int strcmp(char* s1, char* s2)
strcmp_w0i:	lb $v0, 0($a0)
		lb $t0, 0($a1)			# while((*s1 == *s2)
		bne $v0, $t0, strcmp_w0e	# && (*s1 != 0))
		beqz $v0, strcmp_w0e		# {
		addiu $a0, $a0, 1		#	*s1++
		addiu $a1, $a1, 1		#	*s2++
		j strcmp_w0i			# }
strcmp_w0e:	sub $v0, $v0, $t0
		jr $ra				# return (*s1 - *s2)

