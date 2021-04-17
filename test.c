#include "Board.h"

char Sequancial(Board_Type *board);

int main(int argc, char **argv)
{
    Board_Type board;

    Board_Struct_Init(&board);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &board.world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &board.world_size);

    if (board.world_rank == 0)
    {
        // printf("Number of rows(n) in the board:\r\n");
        // scanf("%d", &board.n);

        // while (1)
        // {
        //     printf("Number of taills(t) in a dimension:\r\n");
        //     scanf("%d", &board.t);
        //     if (board.t <= board.n)
        //         break;
        //     else
        //         printf("There should be at lest one n per tile.\r\n");
        // }

        // while (1)
        // {
        //     printf("Threshold(0<=c%%<=100):\r\n");
        //     scanf("%d", &board.c);
        //     if (board.c <= 100 || board.c >= 0)
        //         break;
        //     else
        //         printf("c%% shoudl be in range from 0 to 100.\r\n");
        // }

        // printf("Maximum interactions:\r\n");
        // scanf("%d", &board.maxa);

        board.n = 9;
        board.t = 3;
        board.c = 100;
        board.maxa=100;

        printf("\r\n n=%d,t=%d,c=%d%%\r\n", board.n, board.t, board.c);

        // board.size[0] = board.n;
        // board.size[1] = board.n;
        // board.bsz = board.n * board.size[0];
        // board.tile[0]=board.n;
        // board.tile[1]=board.t;
        // board.npt = board.n / board.tile[1];
        // board.ths = board.npt * board.npt * board.c * 0.01;
        Board_Decompose(&board,0,1);
        
        Board_Grid_Disp(&board, 0);

        Sequancial(&board);

        Board_Sch_Master(&board);
    }
    else
    {
        if (Board_Sch_Slave(&board))
        {
            MPI_Finalize();
            return 0;
        }

        printf("process %d ,bigin at (%d,%d),control %d * %d tile, control %d * %d cells\r\n",
               board.world_rank, board.tile_start[1], board.tile_start[0], board.tile[0], board.tile[1], board.size[0], board.size[1]);
    }



    MPI_Finalize();
    return 0;
}

char Sequancial(Board_Type *board)
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