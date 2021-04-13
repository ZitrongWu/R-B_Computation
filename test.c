#include "Board.h"

 char Sequancial(Board_Type *board);

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


    //Board_Struct_Init(&board);
    if(world_rank == 0)
    {
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

        // board_s.row = board_s.npt * board_s.not;
        // board_s.cpt = board_s.npt * board_s.npt;
        // board_s.bsz = (unsigned int)pow(board_s.row,2);
        // board_s.ths = board_s.npt * board_s.npt * board_s.c * 0.01;

        //Board_Grid_Init(&board_s);


        //Sequancial(&board_s);

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

