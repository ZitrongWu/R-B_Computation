#include "Board.h"

int main(int argc, char **argv)
{
    Board_Type board_s,board_p;

    Board_Struct_Init(&board_s);
    Board_Struct_Init(&board_p);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &board_s.world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &board_s.world_size);
    board_p = board_s;
    if (board_s.world_rank == MASTER)
    {
        //Board_index(&board_s);

        board_s.n = 9;
        board_s.t = 3;
        board_s.c = 60;
        board_s.maxa = 1000;


        printf("\r\n n=%d,t=%d,c=%d%%\r\n", board_s.n, board_s.t, board_s.c);

        Board_Decompose(&board_s, MASTER, 1);

        Board_Grid_Init(&board_s);

        Board_Grid_Disp(&board_s, 0);

        Board_Sequantial(&board_s);


        printf("starting parellel computing...\r\n");

        Board_Sch_Master(&board_s);

        board_p = board_s;

        Board_Decompose(&board_p, MASTER, board_p.world_size);
        
        memcpy(board_p.grid, board_s.grid + board_p.cell_start, board_p.size[0] * board_p.size[1]);
    
        Board_Struct_Finalize(&board_s);
    }
    else
    {
        if (Board_Sch_Slave(&board_p))
        {
            MPI_Finalize();
            return 0;
        }
    }
    printf("process %d ,begin at tile(%d,%d), cell %d ,control %d * %d tile, %d * %d cells\r\n",
           board_p.world_rank, board_p.tile_start[0], board_p.tile_start[1], board_p.cell_start, board_p.tile[0], board_p.tile[1], board_p.size[0], board_p.size[1]);

    Board_Parellel(&board_p);

    Board_Struct_Finalize(&board_p);

    MPI_Finalize();
    return 0;
}
