#include "Board.h"

char Sequancial(Board_Type *board);
char Parallel(Board_Type *board);
int main(int argc , char **argv)
{
    int rank, size;
    Board_Type board_s,board_p;
    unsigned int n;
    // printf("number of rows per taill:");
    // scanf("%d",&board.npt);
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    Board_Struct_Init(&board_s);

    printf("Number of rows(n) in the board:");
    scanf("%d",&n);

    while(1)
    {
        printf("Number of taills(t) in a dimension:");
        scanf("%d",&board_s.not);
        if (board_s.not<=n)
            break;
        else
            printf("There should be at lest one row per tile.\r\n");     
    }

    board_s.npt = n/board_s.not;

    while(1)
    {
        printf("Threshold(0<=c%%<=100):");
        scanf("%d",&board_s.c);
        if (board_s.c<=100 || board_s.c>=0)
            break;
        else
            printf("c%% shoudl be in range from 0 to 100.\r\n");
    }

    printf("Maximum interactions:");
    scanf("%d",&board_s.maxa);

    printf("\r\n n=%d,t=%d,c=%d%%\r\n",n,board_s.not,board_s.c);

    board_s.row = board_s.npt * board_s.not;
    board_s.cpt = board_s.npt * board_s.npt;
    board_s.bsz = (unsigned int)pow(board_s.row,2);
    board_s.ths = board_s.npt * board_s.npt * board_s.c * 0.01;

    Grid_Init(&board_s);


    Sequancial(&board_s);

    MPI_Finalize();
    return 0;
}


 char Sequancial(Board_Type *board)
 {
    unsigned int i;
    Grid_Disp(board,0);
    board->counter=0;
    do
    {
        Is_sotp(board);
        if(board->nott!=0) 
        {
            Grid_Disp(board,0);
            printf("After %d interactions, terminal condition is met! \r\n",board->counter);          
            for (i=0;i!=board->nott;i++)
                printf("the number of %s cells %s more than %d%% cells in tile(%d,%d)\r\n",(*board->terc)==1?"Red":(*board->terc)==2?"Blue":"Red and Blue",(*board->terc)==3?"are":"is",board->c,*(board->tert+2*i),*(board->tert+2*i+1));
            return 0;
        }
         Move_Red(board);
         Move_Blue(board);
         board->counter++;
         printf("After %d interactions: \r\n",board->counter);
         Grid_Disp(board,0);
    }while(board->counter != board->maxa);
    printf("Number of interactions = Max. \r\n");
    return 0;
 }

 char Parallel(Board_Type *board)
 {
     return 0;
 }