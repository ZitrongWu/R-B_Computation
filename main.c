#include "Board.h"



int main(int argc, char **argv)
{
    Board_Type board;

    Board_Struct_Init(&board);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &board.world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &board.world_size);

    if (board.world_rank == MASTER)
    {
        //Board_index(&board);

        board.n = 10;
        board.t = 5;
        board.c = 10;
        board.maxa = 1000;
        board.npt = board.n / board.t;
        board.ths = board.npt * board.npt * board.c * 0.01;

        printf("\r\n n=%d,t=%d,c=%d%%\r\n", board.n, board.t, board.c);

        Board_Decompose(&board, MASTER, 1);

        Board_Grid_Init(&board);

        Board_Grid_Disp(&board, 0);

        Board_Operation(&board);

        printf("starting parellel computing...\r\n");

        Board_Sch_Master(&board);
    }
    else
    {
        if (Board_Sch_Slave(&board))
        {
            MPI_Finalize();
            return 0;
        }
    }
    printf("process %d ,bigin at tile(%d,%d), cell %d ,control %d * %d tile, %d * %d cells\r\n",
           board.world_rank, board.tile_start[0], board.tile_start[1], board.cell_start, board.tile[0], board.tile[1], board.size[0], board.size[1]);

    MPI_Finalize();
    return 0;
}

