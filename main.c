#include "Board.h"

 char Sequancial(Board_Type *board);

int main(int argc , char **argv)
{
    Board_Type board;
    unsigned int n;
    // printf("number of rows per taill:");
    // scanf("%d",&board.npt);
    
    Board_Struct_Init(&board);

    printf("number of rows(n):");
    scanf("%d",&n);
    printf("number of taills(t):");
    scanf("%d",&board.not);
    board.npt = n/board.not;
    printf("threshold(c%%):");
    scanf("%d",&board.c);
    printf("Maximum interactions:");
    scanf("%d",&board.maxa);
    printf("\r\n npt=%d,not=%d,c=%d%%\r\n",board.npt,board.not,board.c);

    


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