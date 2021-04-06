#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


typedef struct
{
    unsigned int npt;   //number of rows per tile
    unsigned int not;   //number of tiles
    unsigned int c;     //terminate condition in percent
    unsigned int ths;   //terminal threshold in number
    unsigned int row;   //number of rows 
    unsigned int cpt;   //number of cells per tile
    unsigned int bsz;   //board size (number of cells in the board}
    char *grid;         //pointer to the satrt address of the board
}Board_Type;


void Grid_Init(Board_Type *board);
void Grid_Disp(Board_Type *board);
void Move_Red(Board_Type *board);
void Move_Blue(Board_Type *board);
char Is_sotp(Board_Type *board);


int main(int argc , char **argv)
{
    Board_Type board;
    
    // char *grid;
    // unsigned int npt;
    // unsigned int not;
    // unsigned int c;
    // unsigned int row;
    // unsigned int cpt;
    // unsigned int bsz;
    // unsigned int ths;

    printf("number of rows per taill:");
    scanf("%d",&board.npt);
    printf("number of taills:");
    scanf("%d",&board.not);
    printf("threshold(c%%):");
    scanf("%d%%",&board.c);
    printf("\r\n npt=%d,not=%d,c=%d%%\r\n",board.npt,board.not,board.c);

    board.row = board.npt * board.not;
    board.cpt = board.npt * board.npt;
    board.bsz = (unsigned int)pow(board.row,2);
    board.ths = board.npt * board.npt * board.c * 0.01;

    Grid_Init(&board);
    Grid_Disp(&board);
    while (!Is_sotp(&board))
    {
         Move_Red(&board);
         Move_Blue(&board);
         Grid_Disp(&board);
    }
    Grid_Disp(&board);
    return 0;
}


void Grid_Init(Board_Type *board)
{
    unsigned int i;
    unsigned int r;
    board->grid = (char *)malloc(board->bsz);
    srand(time(0));
    for (i = 0; i != board->bsz; i++)
    {
        r = rand()%100;
        if (r < 33)
            *(board->grid + i) = 'R';
        else if (r < 66)
            *(board->grid + i) = 'B';
        else
            *(board->grid + i) = 'W';
    }
}

void Grid_Disp(Board_Type *board)
{  
    unsigned int i, j;
    printf("\r\n");
    for (i=0;i!=board->row*4+1;i++)
        printf("=");
    printf("\r\n");
    for (i=0;i!=board->row;i++)
    {
        printf("|");
        for (j=0;j!=board->row;j++)
        {
            if ((j+1)%board->not == 0)
            {
                if (*(board->grid+i*board->row+j) != 'W')//only print R&B
                    printf(" %c |",*(board->grid+i*board->row+j));
                else
                    printf("   |");                
            }
            else
            {
                if (*(board->grid+i*board->row+j) != 'W')//only print R&B
                    printf(" %c  ", *(board->grid+i*board->row+j) );
                else
                    printf("    ");
            }
        }
        printf("\r\n");  
        if((i+1)%board->not == 0)
        {
            for (j=0;j!=board->row*4+1;j++)
                printf("=");
        }
        else
        {
            for (j=0;j!=board->row*4+1;j++)
                printf("-");
        }    
        printf("\r\n");            
    }
    printf("\r\n");    
}


void Move_Red(Board_Type *board)
{
    unsigned int i, j;
    char *cur,*dist;
    for(i = 0; i!=board->row ;i++)
    {
        for(j=0; j<board->row ; j++)
        {
            cur = board->grid+i*board->row+j;
            dist = board->grid+i*board->row+((j+1)%board->row);
            if (*cur == 'R' && *dist == 'W')
            {
                *dist='R';
                *cur = 'W';
                j++;    
            } 
        }
    } 
}

void Move_Blue(Board_Type *board)
{
    unsigned int i, j;
    char *cur,*dist;
    for(j = 0; j!=board->row ;j++)
    {
        for(i=0; i<board->row ; i++)
        {
            cur = board->grid+i*board->row+j;
            dist = board->grid+((i+1)%board->row)*board->row+j;
            if (*cur == 'B' && *dist == 'W')
            {
                *dist='B';
                *cur = 'W';
                i++;    
            } 
        }
    } 
}
char Is_sotp(Board_Type *board)
{
    unsigned int i,j,p,q;
    unsigned int rint,bint;
    for (i=0;i!=board->not;i++)
    {
        for(j=0;j!=board->not;j++)
        {
            rint=0;
            bint=0;
            for(p=i*board->npt;p!=(i+1)*board->npt;p++)
            {
                for(q=j*board->npt;q!=(j+1)*board->npt;q++)
                {
                    if(*(board->grid + p*board->row + q )=='R')
                        rint++;
                    else if(*(board->grid + p*board->row + q )=='B')
                        bint++;
                }
            }
//            if(rint>=c*bint || bint>=c*rint)
//                return 1;       
        }
    }
    return 0;
}