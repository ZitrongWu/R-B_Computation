#include "Board.h"

int main(int argc, char **argv)
{
    Board_Type board_s, board_p;

    Board_Struct_Init(&board_s);
    Board_Struct_Init(&board_p);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &board_s.world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &board_s.world_size);
    board_p = board_s;
    if (board_s.world_size == 1)
    {
        Board_Sequantial(&board_s);
    }
    else
    {
        if (board_s.world_rank == MASTER)
        {

            Board_Sequantial(&board_s);

            printf("starting parellel computing...\r\n");

            Board_Sch_Master(&board_s);

            board_p = board_s;

            Board_Decompose(&board_p, MASTER, board_p.world_size);

            memcpy(board_p.grid, board_s.grid + board_p.cell_start, board_p.size[0] * board_p.size[1]);
        }
        else
        {
            if (Board_Sch_Slave(&board_p))
            {
                MPI_Finalize();
                return 0;
            }
        }
        printf("process %d: begin at tile(%d,%d), cell %d ,control %d * %d tile, %d * %d cells\r\n",
               board_p.world_rank, board_p.tile_start[0], board_p.tile_start[1], board_p.cell_start, board_p.tile[0], board_p.tile[1], board_p.size[0], board_p.size[1]);

        if (Board_Parellel(&board_p))
            Self_Check(&board_p, &board_s);

        Board_Struct_Finalize(&board_p);

        Board_Struct_Finalize(&board_s);
    }
    MPI_Finalize();

    return 0;
}
