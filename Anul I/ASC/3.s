.data
	vf: .word f1, f2, f3
	v: .space 12
	i: .word 0
.text
	main:
		
	for:
		# $t0 = i 
		lw $t0, i
		
		# $t1 = vf[i]
		sll $t0, $t0, 2
		lw $t1, vf($t0)
		
		# $t2 = i + 1
		lw $t2, i		
		addi $t2, $t2, 1
		
		# asezare in stiva
		subu $sp, $sp, 8	
		sw $t1, 0($sp)		# stiva: $sp:(vf[i])
		sw $t2, 4($sp)		# stiva: $sp:(vf[i])(i + 1)
		
		jal aplica
		
		# restaurare stiva
		addu $sp, $sp, 8	
		
		# v[i] = aplica(vf[i], i + 1)
		lw $t0, i
		sll $t0, $t0, 2
		sw $v0, v($t0)
		
		# ++i
		lw $t0, i
		addi $t0, $t0, 1
		sw $t0, i
		
		blt $t0, 3, for
			
	iesire:		
		li $v0, 10
		syscall
		
	aplica:
		subu $sp, $sp 8		# alocare 2 worduri stiva
		sw $fp, 0($sp)		# pune $fp in stiva
		sw $ra, 4($sp)		# pune $ra in stiva
		addiu $fp, $sp, 0
		
		# preluare din stiva
		lw $a0, 8($fp)		# a0 = fi
		lw $a1, 12($fp)		# a1 = i + 1
		
		jalr $a0		# executare fi
		
		lw $v0, -4($fp)		# preluare rezultat in $v0
		
		lw $ra, 4($fp)		# restaureaza $ra
		lw $fp, 0($fp)		# restaureaza $fp
		
		addu $sp, $sp, 12	# $fp, $ra si rezultatul functiei
		
		jr $ra
		
	f1:
		subu $sp, $sp, 4	# alocare stiva
		sw $fp, 0($sp)		# salveaza $fp
		addiu $fp, $sp, 0
		
		move $t0, $a1		# $t0 = y, i.e. $t0 = i + 1
		add $t0, $t0, $t0	# t0 = y + y
		
		lw $fp, 0($fp)		# restaureaza $fp
		sw $t0, 0($sp)		# pune y + y in stiva
		
		jr $ra
		
	f2:
		subu $sp, $sp, 4	# alocare stiva
		sw $fp, 0($sp)		# salveaza $fp
		addiu $fp, $sp, 0	
		
		move $t0, $a1		# $t0 = y, i.e. $t0 = i + 1
		mul $t0, $t0, $t0	# $t0 = $t0 * $t0
		
		lw $fp, 0($fp)		# restaureaza $fp
		sw $t0, 0($sp)		# pune y * y in stiva
		
		jr $ra
		
	f3:
		subu $sp, $sp, 4	# alocare stiva
		sw $fp, 0($sp)		# salveaza $fp
		addiu $fp, $sp, 0	
		
		move $t0, $a1		# $t0 = y, i.e. $t0 = i + 1
		sub $t0, $zero, $t0	# $t0 = -$t0
		
		lw $fp, 0($fp)		# restaureaza $fp
		sw $t0, 0($sp)		# pune -y in stiva
		
		jr $ra	
