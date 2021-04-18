RB_Computation.out : main.out Board.out
	mpicc -o RB_Computation.out main.out Board.out

main.out:main.c
	mpicc -c -o main.out main.c

Board.out:Board.c
	mpicc -c -o Board.out Board.c

