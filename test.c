#include "Board.h"

 char Sequancial(Board_Type *board);

int main(int argc , char **argv)
{
    Board_Type board;
    unsigned int n;
    // printf("number of rows per taill:");
    // scanf("%d",&board.npt);
    
    Board_Struct_Init(&board);

    board.npt=3;
    board.not=3;
    board.c=10;
    board.maxa=10;
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

        Is_sotp(board);
        Grid_Disp(board,0);
        i=4;
        printf("the number of %s cells %s more than %d%% cells in tile(%d,%d)\r\n",(*board->terc)==1?"Red":(*board->terc)==2?"Blue":"Red and Blue",(*board->terc)==3?"are":"is",board->c,*(board->tert+2*i),*(board->tert+2*i+1));

    return 0;
 }