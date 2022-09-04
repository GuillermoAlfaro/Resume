# Winter 2021 CSE12 Lab 4 Template
######################################################
# Macros made for you (you will need to use these)
######################################################
.data
newLine: .asciiz "\n"
coords: .asciiz "\nCoordinates of given 0x000000xx:^^^^^\n"
tzero: .asciiz "\n $t0: ^^^^ \n"
xval: .asciiz "\n x values:^^^\n"
yval: .asciiz "\n y values:^^^\n"
# Macro that stores the value in %reg on the stack 
#	and moves the stack pointer.
.macro push(%reg)
	subi $sp $sp 4
	sw %reg 0($sp)
.end_macro 

# Macro takes the value on the top of the stack and 
#	loads it into %reg then moves the stack pointer.
.macro pop(%reg)
	lw %reg 0($sp)
	addi $sp $sp 4	
.end_macro

#################################################
# Macros for you to fill in (you will need these)
#################################################

# Macro that takes as input coordinates in the format
#	(0x00XX00YY) and returns x and y separately.
# args: 
#	%input: register containing 0x00XX00YY
#	%x: register to store 0x000000XX in
#	%y: register to store 0x000000YY in
.macro getCoordinates(%input %x %y)
	
	andi %x, %input, 0x00ff0000				# masks everything but the XX
	srl %x, %x, 16						# rotates the masked xx to be at the end
	andi %y, %input, 0x000000ff				# masks everything but the YY
.end_macro

# Macro that takes Coordinates in (%x,%y) where
#	%x = 0x000000XX and %y= 0x000000YY and
#	returns %output = (0x00XX00YY)
# args: 
#	%x: register containing 0x000000XX
#	%y: register containing 0x000000YY
#	%output: register to store 0x00XX00YY in
.macro formatCoordinates(%output %x %y)
	add %output, %x, %y					# combines %x and %y with addition into a 0x
.end_macro 

# Macro that converts pixel coordinate to address
# 	  output = origin + 4 * (x + 128 * y)
# 	where origin = 0xFFFF0000 is the memory address
# 	corresponding to the point (0, 0), i.e. the memory
# 	address storing the color of the the top left pixel.
# args: 
#	%x: register containing 0x000000XX
#	%y: register containing 0x000000YY
#	%output: register to store memory address in
.macro getPixelAddress(%output %x %y)
	move $t7, $zero							# creates a temporary variable and makes sure its zero
	mul $t7, %y, 128						# multiples Y by 128 and moves it to $t7
	add $t7, $t7, %x						# adds X to $t7
	mul $t7, $t7, 4							# multiplies $t7 by 4
	add %output, $t7, 0xFFFF0000					# adds origin to $t7 and sends it on its merry way
	
.end_macro


.text

# prevent this file from being run as main
li $v0 10 
syscall

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#  Subroutines defined below
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#*****************************************************
# Clear_bitmap: Given a color, will fill the bitmap 
#	display with that color.
# -----------------------------------------------------
# Inputs:
#	$a0 = Color in format (0x00RRGGBB) 
# Outputs:
#	No register outputs
#*****************************************************
clear_bitmap: nop

	addi $t0, $zero, 0xffff0000					# sets $t0 to be origin
	addi $t1, $zero, 0x00000000					# sets $t1 to zero
	Loop:
		beq $t0, $t1, exit					# jumps to exit once $t0 has completly incremented and rolled over to 0
		sw $a0, ($t0)						# saves color to whatever $t0 is currently at
		add $t0, $t0, 0x00000004				# increments $t0 by one pixel worth
		j Loop							# loop
	exit:
 		jr $ra

#*****************************************************
# draw_pixel: Given a coordinate in $a0, sets corresponding 
#	value in memory to the color given by $a1
# -----------------------------------------------------
#	Inputs:
#		$a0 = coordinates of pixel in format (0x00XX00YY)
#		$a1 = color of pixel in format (0x00RRGGBB)
#	Outputs:
#		No register outputs
#*****************************************************
draw_pixel: nop
	getCoordinates($a0, $t0, $t1)					# gets coordinates in usable form
	getPixelAddress($t2, $t0, $t1)					# converts the single coordinates to a usable address
	sw $a1, ($t2)							# paints that specific address the correct color
	jr $ra
	
#*****************************************************
# get_pixel:
#  Given a coordinate, returns the color of that pixel	
#-----------------------------------------------------
#	Inputs:
#		$a0 = coordinates of pixel in format (0x00XX00YY)
#	Outputs:
#		Returns pixel color in $v0 in format (0x00RRGGBB)
#*****************************************************
get_pixel: nop
	# YOUR CODE HERE, only use t registers (and a, v where appropriate)
	getCoordinates($a0, $t0, $t1)					# gets coordinates in usable form
	getPixelAddress($t2, $t0, $t1)					# converts the single coordinates to a usable address
	lw $v0, 0($t2)							# loads the value at the address to $v0
	jr $ra

#*****************************************************
# draw_horizontal_line: Draws a horizontal line
# ----------------------------------------------------
# Inputs:
#	$a0 = y-coordinate in format (0x000000YY)
#	$a1 = color in format (0x00RRGGBB) 
# Outputs:
#	No register outputs
#*****************************************************
draw_horizontal_line: nop
	# YOUR CODE HERE, only use t registers (and a, v where appropriate)
	move $t1, $zero							# makes $t1 zero
	move $t2, $zero							# makes $t2 zer0
	
	getPixelAddress($t0, $t1, $a0)					# changes the Y value to an address
	
	loopz:
		
		beq $t2, 128, exitagain					# once $t2 has gone over one row length the loop ends
		sw $a1, ($t0)						# stores the color into address we just got above
		addi $t2, $t2, 1					# increments the counter ($t2) by one
		addi $t0, $t0, 4					# mooves the address over to the right by one pixel worth
		j loopz
		
	exitagain:
		
 		jr $ra


#*****************************************************
# draw_vertical_line: Draws a vertical line
# ----------------------------------------------------
# Inputs:
#	$a0 = x-coordinate in format (0x000000XX)
#	$a1 = color in format (0x00RRGGBB) 
# Outputs:
#	No register outputs
#*****************************************************
draw_vertical_line: nop
	# YOUR CODE HERE, only use t registers (and a, v where appropriate)
	move $t1, $zero							# makes $t1 zero
	getPixelAddress($t0, $a0, $t1)					# gets the starting address for the vertical line
	add $t2, $zero, 0						# makes $t2 zero
	loopTwo:
		
		beq $t2, 128, exitTwo					# once $t2 has gone down 128 pixels the loop exits
		sw $a1, ($t0)						# paints the address the color $a1 has
		addi $t0, $t0, 512					# increments the addresss enough to move over directly one down
		addi $t2, $t2, 1					# increments the counter ($t2) by one
		j loopTwo
 		
	exitTwo:
		jr $ra
#*****************************************************
# draw_crosshair: Draws a horizontal and a vertical 
#	line of given color which intersect at given (x, y).
#	The pixel at (x, y) should be the same color before 
#	and after running this function.
# -----------------------------------------------------
# Inputs:
#	$a0 = (x, y) coords of intersection in format (0x00XX00YY)
#	$a1 = color in format (0x00RRGGBB) 
# Outputs:
#	No register outputs
#*****************************************************
draw_crosshair: nop
	push($ra)
	
	# HINT: Store the pixel color at $a0 before drawing the horizontal and 
	# vertical lines, then afterwards, restore the color of the pixel at $a0 to 
	# give the appearance of the center being transparent.
	
	# Note: Remember to use push and pop in this function to save your t-registers
	# before calling any of the above subroutines.  Otherwise your t-registers 
	# may be overwritten.  
	
	# YOUR CODE HERE, only use t0-t7 registers (and a, v where appropriate)
	push($a0)							# saves $a0 for later use
	jal get_pixel							# gets the pixel that the crosshair crosses at 
	push($v0)							# saves color of pixel for later use
	getCoordinates($a0, $t0, $t1)					# gets the X and Y coordinates draw the lines of crosshair
	
	push($t1)							# saves $t1 for later use
	push($t0)							# saves $t0 for later use
	
	
	move $a0, $t0							# moves $t0 to $a0 to get passed along for vertical line
	
	jal draw_vertical_line						# vertical line is drawn with passed $t0 and already $a1 that is untouched
	
	pop($t1)							# brings back the $t1
	pop($t0)							# brings back the $t0
	
	move $a0, $t0							# moves old $t0 for horizontal line
	
	jal draw_horizontal_line					# draws horizontal line with passed $t0 and already present $a1
	pop($v0)							# brings $v0 back
	pop($a0)							# brings back original $a0
	move $a1, $v0							# moves saved color of intersection to palet
	jal draw_pixel							# changes intersection point to color prior to lines being drawn
	pop($ra)		
	# HINT: at this point, $ra has changed (and you're likely stuck in an infinite loop). 
	# Add a pop before the below jump return (and push somewhere above) to fix this.
	jr $ra
