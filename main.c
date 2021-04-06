#include "Board.h"


int main(int argc , char **argv)
{
    Board_Type board;
    unsigned int c;
    printf("number of rows per taill:");
    scanf("%d",&board.npt);
    printf("number of taills:");
    scanf("%d",&board.not);
    printf("threshold(c%%):");
    scanf("%d",&c);
    printf("\r\n npt=%d,not=%d,c=%d%%\r\n",board.npt,board.not,c);

    board.row = board.npt * board.not;
    board.cpt = board.npt * board.npt;
    board.bsz = (unsigned int)pow(board.row,2);
    board.ths = board.npt * board.npt * c * 0.01;

    Grid_Init(&board);
    Grid_Disp(&board,0);
    while (!Is_sotp(&board))
    {
         Move_Red(&board);
         Move_Blue(&board);
         Grid_Disp(&board,0);
    }
    Grid_Disp(&board,1);
    return 0;
}


