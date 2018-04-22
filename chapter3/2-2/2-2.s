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
		
		.equ	SFR_BASE_HI,0xBF88	# 16 MSbits of SFR area
		.equ	TRISE,0x6100		# TRISE address is 0xBF886100
		.equ	PORTE,0x6110		# PORTE address is 0xBF886110
		.equ	LATE,0x6120		# LATE address is 0xBF886120
		
		.equ	CORE_SECOND, 0x01312D00
		.equ	CORE_QSEC, 0x004C4B40
		
		
		.data
		
		.text
		.globl main


main:		lui $t0, SFR_BASE_HI
		lw $t1, TRISE($t0)
		andi $t1, $t1, 0xFFF0		# send to RE0, RE1, RE2, RE3
		sw $t1, TRISE($t0)
		li $v0, RESET_CORETIMER
		syscall				# resetCoreTimer()
		li $t1, 0			# n = 0
		li $t4, 0			# j = 0
main_w0i:					# while(1)
		li $v0, READ_CORETIMER		# {
		syscall				#	readCoreTimer()
		bleu $v0, CORE_QSEC, main_w0i	#	if(CORE_QSEC < readCoreTimer())
						#	{
		lw $t5, PORTE($t0)		# Read Open ports
		andi $t6, $t5, 0x0040		# 0100 0000
		andi $t7, $t5, 0x0080		# 1000 0000
		srl $t6, $t6, 6			# RE6
		srl $t7, $t7, 7			# RE7
		beq $t4, $t6, main_nreset	#		if(j != RE6)
		li $t1, 0
		move $t4, $t6
main_nreset:	move $a0, $t1
		beqz $t6, main_nbin		# 		if(RE6 != 0
		beqz $t7, main_john		# 		&& RE7 != 0)
		jal johnsoneg			#			johnsoneg(n)
		j main_i0e			# 		elseif(RE6 != 0 && RE7 == 0)
main_john:	jal johnson			#			johnson(n)
		j main_i0e
main_nbin:	beqz $t7, main_bin		#		elseif(RE6 == 0 && RE7 != 0)
		jal countbineg			#			countbineg(n)
		j main_i0e			#		elseif(RE6 == 0 && RE7 == 0)
main_bin:	jal countbin			#			countbin(n)
main_i0e:	move $t1, $v0
		addiu $t3, $t1, 0xFFF0
		lw $t2, LATE($t0)
		or $t2, $t2, $t1
		and $t2, $t2, $t3
		sw $t2, LATE($t0)
		li $v0, RESET_CORETIMER
		syscall				#		resetCoreTimer()
						#	}
		j main_w0i			# }		
		jr $ra				# return


countbin:	beq $a0, 0xF, countbin_max
		addiu $a0, $a0, 1
		move $v0, $a0
		j countbin_end
countbin_max:	li $v0, 0
countbin_end:	jr $ra


countbineg:	beqz $a0, countbineg_max
		addiu $a0, $a0, -1
		move $v0, $a0
		j countbineg_end
countbineg_max:	li $v0, 0xF
countbineg_end:	jr $ra


johnson:	move $v0, $a0
		beqz $v0, johnson_0
		bgeu $v0, 0x8, johnson_end
		sll $v0, $v0, 1
		or $v0, $v0, $a0
		jr $ra
johnson_end:	sll $v0, $v0, 1
		andi $v0, 0x000F
		jr $ra
johnson_0:	li $v0, 1
		jr $ra


johnsoneg:	move $v0, $a0
		beqz $v0, johnsoneg_8
		beq $v0, 0xF, johnsoneg_f
		bgeu $v0, 0x8, johnsoneg_end
		srl $v0, $v0, 1
		jr $ra
johnsoneg_end:	addiu $v0, $v0, 0x0010
		srl $v0, $v0, 1
		andi $v0, 0x000F
		jr $ra
johnsoneg_8:	li $v0, 8
		jr $ra
johnsoneg_f:	li $v0, 7
		jr $ra
		
