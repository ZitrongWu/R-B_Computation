#include "Board.h"

char Sequancial(Board_Type *board);
char Parallel(Board_Type *board);
int main(int argc , char **argv)
{
    int world_rank, world_size;
    Board_Type board;
    Proc_Type Proc;
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
        scanf("%d",&board.n);

        while(1)
        {
            printf("Number of taills(t) in a dimension:\r\n");
            scanf("%d",&board.tile[1]);
            if (board.tile[1]<=n)
                break;
            else
                printf("There should be at lest one n per tile.\r\n");     
        }

        

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

        printf("\r\n n=%d,t=%d,c=%d%%\r\n",n,board.tile[1],board.c);

        
        board.size[0] = board.n;
        board.bsz = board.n * board.size[0];
        board.npt = board.n / board.tile[1];
        board.ths = board.npt * board.npt * board.c * 0.01;

        Board_Grid_Init(&board);


        Sequancial(&board);

        m[0]=board.n;
        m[1]=board.tile[1];
        m[2]=board.c;
        m[3]=board.maxa;

        MPI_Bcast(m,4,MPI_INT,0,MPI_COMM_WORLD);
    }
    MPI_Bcast(m,4,MPI_INT,0,MPI_COMM_WORLD);


        board.n=m[0];
        board.tile[1]=m[1];
        board.c=m[2];
        board.maxa=m[3];

    if(Board_Decompose(&Proc))
    {
        
        MPI_Finalize();
        return 0;
    }


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