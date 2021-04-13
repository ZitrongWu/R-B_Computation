#include "Board.h"

char Sequancial(Board_Type *board);
char Parallel(Board_Type *board);
int main(int argc , char **argv)
{
    int world_rank, world_size;
    Board_Type board;
    unsigned int m[4];//n,t,c,m
    unsigned int n;
    // printf("number of rows per taill:");
    // scanf("%d",&board.npt);
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);


   
    if(world_rank == 0)
    {
         Board_Struct_Init(&board);
        printf("Number of rows(n) in the board:\r\n");
        scanf("%d",&n);

        while(1)
        {
            printf("Number of taills(t) in a dimension:\r\n");
            scanf("%d",&board.not);
            if (board.not<=n)
                break;
            else
                printf("There should be at lest one row per tile.\r\n");     
        }

        board.npt = n/board.not;

        while(1)
        {
            printf("Threshold(0<=c%%<=100):\r\n");
            scanf("%d",&board.c);
            if (board.c<=100 || board.c>=0)
                break;
            else
                printf("c%% shoudl be in range from 0 to 100.\r\n");
        }

        printf("Maximum interactions:\r\n");
        scanf("%d",&board.maxa);

        printf("\r\n n=%d,t=%d,c=%d%%\r\n",n,board.not,board.c);

        board_s.row = board_s.npt * board_s.not;
        board_s.cpt = board_s.npt * board_s.npt;
        board_s.bsz = (unsigned int)pow(board_s.row,2);
        board_s.ths = board_s.npt * board_s.npt * board_s.c * 0.01;

        Board_Grid_Init(&board_s);


        Sequancial(&board_s);

        m[0]=n;
        m[1]=board.not;
        m[2]=board.c;
        m[3]=board.maxa;

        MPI_Bcast(m,4,MPI_INT,0,MPI_COMM_WORLD);
    }
    MPI_Bcast(m,4,MPI_INT,0,MPI_COMM_WORLD);

    printf("processor %d received massages %d %d %d %d\r\n",world_rank,m[0],m[1],m[2],m[3]);

    MPI_Finalize();
    return 0;
}


 char Sequancial(Board_Type *board)
 {
    unsigned int i;
    Board_Grid_Disp(board,0);
    board->counter=0;
    do
    {
        Board_Is_Sotp(board);
        if(board->nott!=0) 
        {
            Board_Grid_Disp(board,0);
            printf("After %d interactions, terminal condition is met! \r\n",board->counter);          
            for (i=0;i!=board->nott;i++)
                printf("the number of %s cells %s more than %d%% cells in tile(%d,%d)\r\n",(*board->terc)==1?"Red":(*board->terc)==2?"Blue":"Red and Blue",(*board->terc)==3?"are":"is",board->c,*(board->tert+2*i),*(board->tert+2*i+1));
            return 0;
        }
         Board_Move_Red(board);
         Board_Move_Blue(board);
         board->counter++;
         printf("After %d interactions: \r\n",board->counter);
         Board_Grid_Disp(board,0);
    }while(board->counter != board->maxa);
    printf("Number of interactions = Max. \r\n");
    return 0;
 }

 char Parallel(Board_Type *board)
 {
     return 0;
 }