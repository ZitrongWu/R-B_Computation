#include "Board.h"

int main(int argc , char **argv)
{
    Board_Type board;
    
    board.npt = 4;
    board.not = 5;
    board.row = board.npt * board.not;
    board.cpt = board.npt * board.npt;
    board.bsz = (unsigned int)pow(board.row,2);
    Grid_Init(&board);
    Grid_Disp(&board,1);
    return 0;
}