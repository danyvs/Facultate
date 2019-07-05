.data
	v: .byte 0, 0, 'b', 0, 'e', 'q', 0
	n: .word 7
	i: .space 4
	cnt: .space 4
	a: .byte 0
.text
	main:
		# i = 0
		addi $t0, $t0, 0
		sw $t0, i
		
		# cnt = 0
		addi $t0, $t0, 0
		sw $t0, cnt
	
	# for	
	intrare: 
		# $to = v[i]
		lw $t0, i
		lb $t0, v($t0)
		
		# if (v[i] == 'a')
		lb $t1, a
		bne $t0, $t1, next
		lw $t0, cnt
		addi $t0, $t0, 1
		sw $t0, cnt
	
	next:
		# ++i
		lw $t0, i
		addi $t0, $t0, 1
		sw $t0, i
		
		# Verificare daca i < n
		lw $t1, n
		bge $t0, $t1, afisare
		
		# i < n, deci se executa intrare din nou
		j intrare
		
	afisare:
		# Afisare cnt
		li $v0, 1
		lw $a0, cnt
		syscall
		
		j iesire
	
	iesire:	
		# Terminare program
		li $v0, 10
		syscall
