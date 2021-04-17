#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mpi.h"
#include "string.h"

#define MASTER  0
#define WITHE   0
#define RED     1
#define BLUE    2
#define BOTH    RED+BLUE
typedef struct
{
    unsigned int c;       //terminate condition in percent   
    unsigned int n;     //number of column 
    unsigned int t;     //number of tile in one dimension 
    unsigned int maxa;    //maximum number of interactions        

    int world_rank;
    int world_size;

    unsigned int size[2];       //size of cell [n.row n.column]
    unsigned int tile[2] ;    //size of tile [n.row n.column]  
    unsigned int tile_start[2];//the absolute coordinate [row column] y] of up left tile 
    unsigned int cell_start;//the absolute possision of start cell
    unsigned int lastp;   //last processor in rank
    unsigned int nextp;   //next processor in rank    


    unsigned int npt;     //number of rows per tile
    unsigned int ths;     //terminal threshold in number
    unsigned int bsz;     //board size (number of cells in the board}
    unsigned int nott;    //number of tiles meet the terminal condition
    unsigned int *tert;   //the rank of tile meet the terminal condition (x,y)
    char *terc;           //wich color meet the terminal condition 1:Red 2:Blue 3:both
    unsigned int counter; //interactions conter

    char *grid;           //pointer to the satrt address of the board
} Board_Type;

void Board_Grid_Init(Board_Type *board);
void Board_Grid_Disp(Board_Type *board, char ifter);
void Board_Move_Red(Board_Type *board);
void Board_Move_Blue(Board_Type *board);
char Board_Is_Sotp(Board_Type *board);
void Board_Struct_Init(Board_Type *board);
char Board_Decompose(Board_Type *board, int rank, int size);
void Board_Sch_Master(Board_Type* board);
char Board_Sch_Slave(Board_Type* board);
void Board_index(Board_Type *board);
char Board_Operation(Board_Type *board);
#endif