#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mpi.h"

typedef struct
{
    unsigned int c;
    unsigned int max; 
    unsigned int size[2];   //size(x,y) the processor control a x*y matrix of the board
    unsigned int tile[2];   //tile(x,y) the processor control x*y tiles
    unsigned int lastp;     //last processor in rank
    unsigned int nextp;     //next processor in rank
}Proc_Type;



typedef struct
{
    unsigned int npt;   //number of rows per tile
    unsigned int not;   //number of tiles in one dimension
    unsigned int c;     //terminate condition in percent
    unsigned int ths;   //terminal threshold in number
    unsigned int row;   //number of rows 
    unsigned int cpt;   //number of cells per tile
    unsigned int bsz;   //board size (number of cells in the board}
    unsigned int nott;  //number of tiles meet the terminal condition
    unsigned int *tert;  //the rank of tile meet the terminal condition (x,y)
    char *terc;           //wich color meet the terminal condition 1:Red 2:Blue 3:both
    unsigned int counter; //interactions conter
    unsigned int maxa; //maximum number of interactions
    char *grid;         //pointer to the satrt address of the board   
}Board_Type;

void Board_Grid_Init(Board_Type *board);
void Board_Grid_Disp(Board_Type *board, char ifter);
void Board_Move_Red(Board_Type *board);
void Board_Move_Blue(Board_Type *board);
char Board_Is_sotp(Board_Type *board);
void Board_Struct_Init(Board_Type *board);
#endif