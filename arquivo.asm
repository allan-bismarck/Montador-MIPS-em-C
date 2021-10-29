	add $s0, $s1, $s2
	sub $s0, $s1, $s2 #primeiro teste
	and $s0, $s1, $s2
inicio:
#linha com comentario 
	or  $t0, $t1, $t2
	xor $t0, $t1, $t2
	sll $s0, $s0, 1
	slt $t0, $s1, $t2
	jr $ra
	jr $s0
	j fim
	j inicio
	jal inicio
fim:
	addi $t0, $zero, 32800
	ori $s0, $s0, 0x8022
	slti $t0, $t1, 16421
	lui $s0, 0x8024
	beq $s0, $s1, ult
	lw $s0, 4($s3)
ult: sw $t0 , 4 ( $t1 )
  
addi $t0, $zero, 32800
lui $s0, 0x8024