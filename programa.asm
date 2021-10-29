main:
	addi $a0,$zero,10
	addi $a1,$zero,3
	jal comb
	j exit

fato:	addi $sp,$sp,-8
	sw $t0,4($sp)
	sw $a0,5($sp)

	bne $a0,$zero,recur
	addi $v0,$zero,1
	j fimFato
recur:	add $t0,$a0,$zero

	addi $a0,$a0,-1
	addi $sp,$sp,-4
	sw $ra,0($sp)
	jal fato
	lw $ra,0($sp)
	addi $sp,$sp,4

	mul $v0,$t0,$v0

fimFato:lw $a0,0($sp)
	lw $t0,4($sp)
	addi $sp,$sp,8
	jr $ra

comb:	addi $sp,$sp,-28
	sw $a0,0($sp)
	sw $a1,4($sp)
	sw $t0,8($sp)
	sw $t1,12($sp)
	sw $t2,16($sp)
	sw $t3,20($sp)
	sw $t4,24($sp)

if1:	slt $t0,$a0,$a1
	beq $t0,$zero,if2
	beq $a0,$a1,if2
	addi $v1,$zero,1
	j fimComb
if2:	slt $t0,$a0,$zero
	beq $t0,$zero,if21
	addi $v1,$zero,2
	j fimComb
if21:	slt $t0,$a1,$zero
	beq $t0,$zero,if3
	addi $v1,$zero,2
	j fimComb
if3:	bne $a0,$a1,if4
	addi $v1,$zero,3
	j fimComb
if4:	beq $a0,$zero,then
	bne $a1,$zero, else
then:	addi $v1,$zero,4
	j fimComb
else:	add $t0,$zero,$a0
	add $t1,$zero,$a1

	addi $sp,$sp,-4
	sw $ra,0($sp)
	jal fato
	lw $ra,0($sp)
	addi $sp,$sp,4

	add $t2,$zero,$v0
	add $a0,$zero,$t1

	addi $sp,$sp,-4
	sw $ra,0($sp)
	jal fato
	lw $ra,0($sp)
	addi $sp, $sp, 4

	add $t3,$zero,$v0
	sub $a0,$t0,$t1

	addi $sp,$sp,-4
	sw $ra,0($sp)
	jal fato
	lw $ra,0($sp)
	addi $sp,$sp,4

	mul $t4,$t3,$v0
	addi $t2,$t2,1
	div $v0,$t2,$t4


