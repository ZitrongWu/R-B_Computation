CC = mpicc

RB_Computation.out : main.out Board.out
	$(CC) -o RB_Computation.out main.out Board.out

main.out:main.c
	$(CC) -c -o main.out main.c

Board.out:Board.c
	$(CC) -c -o Board.out Board.c

clean:
	rm *.out RB_Computation.out

