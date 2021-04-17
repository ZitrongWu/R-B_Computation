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
        Board_index(&board);


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
           board.world_rank, board.tile_start[1], board.tile_start[0], board.cell_start, board.tile[0], board.tile[1], board.size[0], board.size[1]);

    MPI_Finalize();
    return 0;
}

char Board_Operation(Board_Type *board)
{
    unsigned int i;

    board->counter = 0;
    do
    {
        Board_Is_Sotp(board);
        if (board->nott != 0)
        {
            Board_Grid_Disp(board, 0);
            printf("After %d interactions, terminal condition is met! \r\n", board->counter);
            for (i = 0; i != board->nott; i++)
                printf("the number of %s cells %s more than %d%% cells in tile(%d,%d)\r\n", (*board->terc) == 1 ? "Red" : (*board->terc) == 2 ? "Blue"
                                                                                                                                              : "Red and Blue",
                       (*board->terc) == 3 ? "are" : "is", board->c, *(board->tert + 2 * i), *(board->tert + 2 * i + 1));
            return 0;
        }
        Board_Move_Red(board);
        Board_Move_Blue(board);
        board->counter++;
        printf("After %d interactions: \r\n", board->counter);
        Board_Grid_Disp(board, 0);
    } while (board->counter != board->maxa);
    printf("Number of interactions = Max. \r\n");
    return 0;
}