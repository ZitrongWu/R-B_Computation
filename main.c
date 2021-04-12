#include "Board.h"

 char Sequancial(Board_Type *board);

int main(int argc , char **argv)
{
    Board_Type board;
    unsigned int n;
    // printf("number of rows per taill:");
    // scanf("%d",&board.npt);
    printtf("number of rows(n):");
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
    Grid_Disp(board,0);
    board->counter=0;
    do
    {
        if(Is_sotp(board)) 
        {
            Grid_Disp(board,1);
            printf("After %d interactions, terminal condition is met: in tile (%d,%d) marked by \"%%\" the %s cells are more than %d%% of cells in the tile. \r\n",board->counter ,board->tert[0],board->tert[1],board->terc=='R'?"Red":"Blue",board->c);          
            return 0;
        }
         Move_Red(board);
         Move_Blue(board);
         board->counter++;
         printf("After %d interactions: \r\n",board->counter);
         Grid_Disp(board,0);
    }while(board->counter != board->maxa);
    printf("Number of interactions = Max. \r\n",board->counter);
    return 0;
 }