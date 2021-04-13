#include "Board.h"

 char Sequancial(Board_Type *board);

int main(int argc , char **argv)
{
    Board_Type board;
    unsigned int n;
    // printf("number of rows per taill:");
    // scanf("%d",&board.npt);
    
    Board_Struct_Init(&board);

    printf("Number of rows(n) in the board:");
    scanf("%d",&n);

    while(1)
    {
        printf("Number of taills(t) in a dimension:");
        scanf("%d",&board.not);
        if (board.not<=n)
            break;
        else
            printf("There should be at lest one row per tile.\r\n");     
    }

    board.npt = n/board.not;
    
    while(1)
    {
        printf("Threshold(0<=c%%<=100):");
        scanf("%d",&board.c);
        if (board.c<=100 || board.c>=0)
            break;
        else
            printf("c%% shoudl be in range from 0 to 100.\r\n");
    }

    printf("Maximum interactions:");
    scanf("%d",&board.maxa);

    printf("\r\n n=%d,t=%d,c=%d%%\r\n",n,board.not,board.c);

    


    board.row = board.npt * board.not;
    board.cpt = board.npt * board.npt;
    board.bsz = (unsigned int)pow(board.row,2);
    board.ths = board.npt * board.npt * board.c * 0.01;

    Grid_Init(&board);
    Sequancial(&board);
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