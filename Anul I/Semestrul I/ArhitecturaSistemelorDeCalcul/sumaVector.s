.data
	v: .word 1, 1, 1, 1, 1, 1, 1
	n: .word 7
	i: .space 4
	sum: .space 4
.text
	main:
		# Initializari
		
		# i = 0
		addi $t0, $t0, 0
		sw $t0, i
		
		# sum = 0
		addi $t0, $t0, 0
		sw $t0, sum
	
	# for	
	intrare: 
		# $to = v[i]
		lw $t0, i
		sll $t0, $t0, 2
		lw $t0, v($t0)
		
		# sum += v[i]
		lw $t1, sum
		add $t1, $t1, $t0
		sw $t1, sum
		
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
		# Afisare suma
		li $v0, 1
		lw $a0, sum
		syscall
		
		j iesire
	
	iesire:	
		# Terminare program
		li $v0, 10
		syscall
