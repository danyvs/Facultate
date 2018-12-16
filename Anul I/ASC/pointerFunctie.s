.data
	vf: .word f1, f2, f3
	v: .space 12
	i: .word 0
.text
	main:
		
	for:
		lw $t0, i
		sll $t0, $t0, 2		# $t0 = 4 * i
		
		subu $sp, $sp, 8
		lw $t1, vf($t0)		# $t1 = vf[i]
		lw $t2, i		
		addi $t2, $t2, 1	
		sw $t1, 0($sp)		# stiva: $sp:(vf[i])
		sw $t2, 4($sp)		# stiva: $sp:(vf[i])(i + 1)
		
		jal aplica
		
		addu $sp, $sp, 8	# restaurare stiva
		
		lw $t0, i
		sll $t0, $t0, 2
		sw $v0, v($t0)		# v[i] = aplica(vf[i], i + 1)
		
		lw $t0, i
		addi $t0, $t0, 1		# ++i
		sw $t0, i
		
		blt $t0, 3, for
			
	iesire:		
		li $v0, 10
		syscall
		
	aplica:
		subu $sp, $sp 8
		sw $ra, 4($sp)
		sw $fp, 0($sp)
		addiu $fp, $sp, 0
		
		lw $a0, 8($fp)
		lw $a1, 12($fp)
		
		jalr $a0
		
		lw $v0, -4($fp)
		
		lw $ra, 4($fp)
		lw $fp, 0($fp)
		
		addu $sp, $sp, 12
		
		jr $ra
		
		
	f1:
		#stiva: $sp:(y)
		subu $sp, $sp, 4
		sw $fp, 0($sp)		#salveaza $fp
		addiu $fp, $sp, 0	# stiva: $sp:$fp:($fp v)(y)
		
		move $t0, $a1		# $t0 = y
		add $t0, $t0, $t0	# t0 = y + y
		
		lw $fp, 0($fp)		# restaurarea fp-ului
		sw $t0, 0($sp)
		
		jr $ra
		
	f2:
		#stiva: $sp:(y)
		subu $sp, $sp, 4
		sw $fp, 0($sp)		# salveaza $fp
		addiu $fp, $sp, 0	# stiva: $sp:$fp:($fp v)(y)
		
		move $t0, $a1		# $t0 = y
		mul $t0, $t0, $t0	# $t0 = $t0 * $t0
		
		lw $fp, 0($fp)		# restaureaza fp
		sw $t0, 0($sp)
		
		jr $ra
		
	f3:
		#stiva: $sp:(y)
		subu $sp, $sp, 4
		sw $fp, 0($sp)		#salveaza $fp
		addiu $fp, $sp, 0	# stiva: $sp:$fp:($fp v)(y)
		
		move $t0, $a1		# $t0 = y
		sub $t0, $zero, $t0	# $t0 = -$t0
		
		lw $fp, 0($fp)		# restaureaza fp
		sw $t0, 0($sp)
		
		jr $ra	
