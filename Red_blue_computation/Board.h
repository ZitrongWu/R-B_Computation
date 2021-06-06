#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include "Barrier.h"
#include "RW_lock.h"
#include "Addressing.h"
#define MASTER  0
#define WITHE   0
#define RED     1
#define BLUE    2
#define BOTH    RED+BLUE

typedef struct
{
    unsigned int addr;//linear address of tiles that terminated
    char color; //wich color meet the terminal condition 1:Red 2:Blue 3:both
}tile_t;


typedef struct
{
    //Consensus of threads
    unsigned int c;       //terminate condition in percent   
    unsigned int n;     //number of column 
    unsigned int t;     //number of tile in one dimension 
    unsigned int maxa;    //maximum number of interactions    
    unsigned int npt;     //number of rows per tile
    unsigned int ths;     //terminal threshold in number
    unsigned int bsz;     //board size (number of cells in the board}
    unsigned int nthread; //number of threads;
    unsigned int rthread;  //rank of current thread
    unsigned int not;       //total number of tiles;

        //address of global memory, allocated only by thread 0.
        unsigned int *nott;    //pint to address of a shared mamory storage number of tiles meet the terminal condition   
        tile_t *tert;         //pint to start address of a shared mamory storage imformation about terminated tile
        char *grid;           //pointto the satrt address of a shared mamory storage  the board


    //personal information of thread, allocated by threads.
    unsigned int tile_start;//the start possition of tile inspected by thread
    unsigned int tile_stop ;//the number of tiles inspected by thread
    unsigned int tile_length;
    unsigned int row_start;//the star possision of cell controled by thread
    unsigned int row_stop;//the number of cells conroled by thread
    unsigned int row_length;
    unsigned int cell_start;//the star possision of cell controled by thread
    unsigned int cell_stop;//the number of cells conroled by thread
    unsigned int cell_length;   
    tile_t *tert_locla;//pointer to a memory store imformation about terminated tile impsected by thread
    unsigned int nott_local;////storage number of terminated tile impsected by thread
    unsigned int counter; //interactions conter
    char *grid_local;//avoid fack sharing


} Board_Type;
void Board_index(Board_Type *board);
void Board_Grid_Init(Board_Type *board);
void Board_Struct_Finalize(Board_Type *board);
void Board_Grid_Disp(Board_Type *board);
void Board_Move_Red(Board_Type *board);
void Board_Move_Blue(Board_Type *board);
char Board_Is_Sotp(Board_Type *board);
void Board_Struct_Init(Board_Type *board);
void Board_Decompose(Board_Type *board);
void Board_create_threads(Board_Type *board);
char Self_Check(Board_Type *board_l, Board_Type *board_std);
#endif