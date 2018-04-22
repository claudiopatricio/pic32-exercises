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
		
		
		.data
		
		.text
		.globl main


main:		lui $t0, SFR_BASE_HI
		lw $t1, TRISE($t0)
		andi $t1, $t1, 0xFFF0		# send to RE0, RE1, RE2, RE3
		sw $t1, TRISE($t0)
		#lw $t1, LATE($t0)
		#andi $t1, $t1, 0xFFFE		# Deactivate led
		#ori $t1, $t1, 0x0001		# Activate led
		#sw $t1, LATE($t0)
main_w0i:	lw $t1, PORTE($t0)		# Read Open ports
		andi $t6, $t1, 0x0040		# 0100 0000
		andi $t7, $t1, 0x0080		# 1000 0000
		srl $t6, $t6, 6			# RE6
		srl $t7, $t7, 7			# RE7
		nor $t1, $t6, $t7		# RE3 = ~(RE6 | RE7)
		andi $t1, $t1, 1		# Get last bit of nor
		sll $t2, $t1, 1
		xor $t1, $t6, $t7		# RE2 = RE6 ^ RE7
		add $t2, $t2, $t1
		sll $t2, $t2, 1
		or $t1, $t6, $t7		# RE1 = RE6 | RE7
		add $t2, $t2, $t1
		sll $t2, $t2, 1
		and $t1, $t6, $t7		# RE0 = RE6 & RE7
		add $t2, $t2, $t1
		addiu $t3, $t2, 0xFFF0
		lw $t1, LATE($t0)
		or $t1, $t1, $t2
		and $t1, $t1, $t3
		sw $t1, LATE($t0)
		j main_w0i
		jr $ra				# return
