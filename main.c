#include "Board.h"

int main(int argc, char **argv)
{
    Board_Type board_s, board_p;

    Board_Struct_Init(&board_s);
    Board_Struct_Init(&board_p);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &board_s.comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &board_s.comm_size);
    board_p = board_s;
    if (board_s.comm_size == 1)
    {
        Board_Sequantial(&board_s);
    }
    else
    {
        if (board_s.comm_rank == MASTER)
        {

            Board_Sequantial(&board_s);

            printf("starting parallel computing...\r\n");

            board_p = board_s;
        }

        if (Board_Sch_Slave(&board_p))
        {
            printf("process %d: nothing to do, terminated.\r\n", board_p.comm_rank);
            MPI_Finalize();
            return 0;
        }

        printf("process %d: begin at tile(%d,%d), cell %d ,control %d * %d tile, %d * %d cells\r\n",
               board_p.comm_rank, board_p.tile_start[0], board_p.tile_start[1], board_p.cell_start, board_p.tile[0], board_p.tile[1], board_p.size[0], board_p.size[1]);

        if (Board_Parellel(&board_p))
            Self_Check(&board_p, &board_s);

        Board_Struct_Finalize(&board_p);
        if (board_p.comm_rank == MASTER)
            Board_Struct_Finalize(&board_s);
    }
    MPI_Finalize();

    return 0;
}
