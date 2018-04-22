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
		.equ	READ_INT,4
		.equ	PUT_CHAR,3
		.equ	GET_CHAR,2
		.equ	IN_KEY,1
		
		.equ	N_INT,5
		.equ	TRUE,1
		.equ	FALSE,0

		.data

lista:		.word	0:N_INT
msg0:		.asciiz	"\nLeitura e ordenacao de inteiros em base 10\n"
msg1:		.asciiz	"Introduza 5 inteiros: "
msg2:		.asciiz	"\nNumero de trocas realizado: "
msg3:		.asciiz	"\nResultado da ordenacao: "

		.text
		.globl	main


main:		addiu $sp, $sp, -8
		sw $ra, 0($sp)
		sw $s0, 4($sp)
		la $s0, lista
		la $a0, msg0
		li $v0, PRINT_STR
		syscall				# printStr("\nLeitura e ordenacao de inteiros em base 10\n")
		la $a0, msg1
		li $v0, PRINT_STR
		syscall				# printStr("Introduza 5 inteiros: ")
		ori $t0, $0, 0			# i = 0
main_w0i:	bgeu $t0, N_INT, main_w0e	# while(i < N_INT)
						# {
		li $v0, READ_INT10
		syscall				#	readInt10()
		sll $t1, $t0, 2			#	Each word have 4 bytes
		sw $v0, lista($t1)		#	lista[i] = readInt(10)
		li $a0, ' '
		li $v0, PUT_CHAR
		syscall				#	putChar(' ')
		addiu $t0, $t0, 1		#	i++
		j main_w0i
main_w0e:					# }
		li $a0, N_INT
		la $a1, lista
		jal seqSort			# sequentialSort(N_INT, lista)
		move $t0, $v0			# n_trocas = sequentialSort(N_INT, lista)
		la $a0, msg2
		li $v0, PRINT_STR
		syscall				# printStr("\nNumero de trocas realizado: ")
		move $a0, $t0
		li $v0, PRINT_INT10
		syscall				# printInt10(n_trocas)
		la $a0, msg3
		li $v0, PRINT_STR
		syscall				# printStr("\nResultado da ordenacao: ")
		la $t0, lista			# ptr = lista
		li $t1, N_INT
		sll $t1, $t1, 2
		addu $t1, $t0, $t1		# lista + N_INT
main_w1i:	bgeu $t0, $t1, main_w1e		# while(ptr < lista + N_INT)
						# {
		lw $a0, 0($t0)
		li $v0, PRINT_INT10
		syscall				#	printInt10(*ptr)
		li $a0, ' '
		li $v0, PUT_CHAR
		syscall				#	putChar(' ')
		addiu $t0, $t0, 4		#	ptr++
		j main_w1i			# }
main_w1e:	lw $s0, 4($sp)
		lw $ra, 0($sp)
		addiu $sp, $sp, 8
		jr $ra				# return


seqSort:	addiu $sp, $sp, -28		# uint seqSort(uint n_val, uint* array)
		sw $ra, 0($sp)
		sw $s0, 4($sp)			# $s0: n_val
		sw $s1, 8($sp)			# $s1: array
		sw $s2, 12($sp)			# $s2: i
		sw $s3, 16($sp)			# $s3: j
		sw $s4, 20($sp)			# $s4: n_trocas
		sw $s5, 24($sp)			# $s5: n_val - 1
		move $s0, $a0
		move $s1, $a1
		li $s2, 0			# i = 0
		li $s4, 0			# n_trocas = 0
		addiu $s5, $s0, -1		# nval - 1
seqSort_w0i:	bgeu $s2, $s5, seqSort_w0e	# while(i < nval -1)
						# {
		addiu $s3, $s2, 1		#	j = i + 1
seqSort_w1i:	bgeu $s3, $s0, seqSort_w1e	#	while(j < n_val)
						#	{
		sll $a0, $s2, 2			#		Each word have 4 bytes
		addu $a0, $a0, $s1		#		&array[i]
		sll $a1, $s3, 2			#		Each word have 4 bytes
		addu $a1, $a1, $s1		#		&array[j]
		lw $t0, 0($a0)			#		array[i]
		lw $t1, 0($a1)			#		array[j]
		ble $t0, $t1, seqSort_i0e	#		if(array[i] < array[j])
						#		{
		jal troca			#			troca(&array[i], &array[j])
		addiu $s4, $s4, 1		#			n_trocas++
seqSort_i0e:					#		}
		addiu $s3, $s3, 1		#		j++
		j seqSort_w1i			# 	}
seqSort_w1e:	addiu $s2, $s2, 1		#	i++
		j seqSort_w0i			# }
seqSort_w0e:	move $v0, $s5
		lw $ra, 0($sp)
		lw $s0, 4($sp)
		lw $s1, 8($sp)
		lw $s2, 12($sp)
		lw $s3, 16($sp)
		lw $s4, 20($sp)
		lw $s5, 24($sp)
		addiu $sp, $sp, 28
		jr $ra				# return n_trocas


troca:		lw $t0, 0($a0)			# *v1 (aux)
		lw $t1, 0($a1)			# *v2 (aux)
		sw $t0, 0($a1)			# *v2 = *v1
		sw $t1, 0($a0)			# *v1 = *v2
		jr $ra
