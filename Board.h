#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


typedef struct
{
    unsigned int npt;   //number of rows per tile
    unsigned int not;   //number of tiles in one dimension
    //unsigned int c;     //terminate condition in percent
    unsigned int ths;   //terminal threshold in number
    unsigned int row;   //number of rows 
    unsigned int cpt;   //number of cells per tile
    unsigned int bsz;   //board size (number of cells in the board}
    char *grid;         //pointer to the satrt address of the board
    unsigned int tert[2];  //the rank of tile meet the terminal condition (x,y)
}Board_Type;

void Grid_Init(Board_Type *board);
void Grid_Disp(Board_Type *board, char ifter);
void Move_Red(Board_Type *board);
void Move_Blue(Board_Type *board);
char Is_sotp(Board_Type *board);

#endif