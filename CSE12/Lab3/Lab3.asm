##########################################################################
# Created by: 	Alfaro, Guillermo
# 		gualfaro
# 		14 November 2021
#
# Assignment: Lab 3: ASCII-risks (Asterisks)
# CSE 12, Computer Systems and Assembly Language
# UC Santa Cruz, Fall 2021
#
# Description: This program prints a triangle with a height that user can enter in the I/O
#
# Notes: This program is intended to be run from the MARS IDE.
##########################################################################


.data
	prompt: .asciiz "Enter the height of the pattern (must be greater than 0):"
	newLine: .asciiz "\n"
	invald: .asciiz "Invalid Entry!\n"
	aster: .asciiz "	*"

.text
		loop:
							# prints question
			li $v0, 4
			la $a0, prompt
			syscall
							# keyboard request
			li $v0, 5
			syscall
			
							# $t3: user input
			move $t3, $v0
							# checks if valid number
			bgt $t3, $zero, main		# if input is greater than 0 it jumps to main code
			li $v0, 4
			la $a0, invald			# if fails it prints invalid and loops
			syscall
			j loop
	
	main:
							# prints user input
		li $v0, 1
		move $a0, $t3
		syscall
							# adds a new line after user input
		li $v0, 4
		la $a0, newLine
		syscall
							# $t0: temp counter variable that incrments up
		addi $t0, $zero, 0
		
							# while loop
		while:
			bge $t0, $t3, exit		# while t0 < t3:
			addi $t0, $t0, 1		# increases temp by one
			
			jal printNumber			# prints number

			j while				# loops
			
		exit:					# prints a new line after loop
			li $v0, 4
			la $a0, newLine
			syscall
							# system call 10 exits
		li $v0, 10
		syscall
		
		
	printNumber:					# printer number function
							# moves temp counter variable to address to be printed
		li $v0, 1
		add $a0, $t0, 0
		syscall
							# $t1: temp counter that goes down
		add $t1, $t0, $zero			# creates temp variable to count down
		
		inner:					# loop to print asterisks and tabs
			beq $t1, 1, outer		# once temp hits 1 it jumps out of loop into outer
			sub $t1, $t1, 1			# decrements the temp variable down by 1
			li $v0, 4
			la $a0, aster			# prints the tabs and asterisks
			syscall
			j inner				# loop
			
		outer:					# prints new line after all asterisks
			li $v0, 4
			la $a0, newLine
			syscall
			
			
		
		jr $ra
