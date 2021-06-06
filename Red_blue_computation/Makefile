CC = gcc
lib = -lpthread
RB_Computation.out: main.out Board.out Barrier.out RW_lock.out Addressing.out
	$(CC) -o RB_Computation.out main.out Board.out  Barrier.out RW_lock.out Addressing.out  $(lib)

main.out: main.c
	$(CC) -c -o main.out main.c

Board.out: Board.c
	$(CC) -c Board.c -o Board.out

Barrier.out: Barrier.c
	$(CC) -c Barrier.c -o Barrier.out

RW_lock.out: RW_lock.c
	$(CC) -c RW_lock.c -o RW_lock.out

Addressing.out: Addressing.c
	$(CC) -c -o Addressing.out Addressing.c

clean:
	rm *.out

