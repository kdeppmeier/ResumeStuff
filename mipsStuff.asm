#Generic file meant to show off my MIPS skills

.data
x: .word 10
y: .word 1, 2, 3
z: .float 5.2

.text
.globl main

main:
	lw $s0, x($zero)	#$s0 = x
	lw $s1, y + 4		#$s1 = y[1]
	l.s $f1, z		#$f1 = z
	
	add $s2, $s0, $s1	#$s2 = x + y[1]
	
	li $v0, 1		#prints $s2
	add $a0, $s2, $zero
	syscall
	
	li $v0, 11		#prints newline character
	li $a0, 10		#10 is ascii for newline
	syscall
	
	li $t0, 0		#$t0 will serve as counter for for loop
forLoop:			#loop prints $f1 ten times
	li $v0, 2		#prints $f1
	mov.s $f12, $f1
	syscall
	
	li $v0, 11		#prints space character
	li $a0, 32		#32 is ascii for space
	syscall

	addi $t0, $t0, 1	#incriments $t0
	blt $t0, 10, forLoop	#end of forLoop

	li $v0, 10		#terminates the program
	syscall