#include "Board.h"

int main(int argc, char **argv)
{
    Board_Type board_s,board_p;
    Board_index(&board_s);
    Board_Grid_Init(&board_s);
    Board_Grid_Disp(&board_s);
    board_p = board_s;
    board_s.nthread = 1;
    printf("Starting sequential computation...\r\n");
    Board_create_threads(&board_s);
    printf("Starting parallel computation...\r\n");
    Board_create_threads(&board_p);
    Self_Check(&board_p, &board_s);

    
    return 0;
}
