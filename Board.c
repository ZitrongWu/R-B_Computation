#include "Board.h"

void Board_Grid_Init(Board_Type *board)
{
    unsigned int i;
    unsigned int r;

    board->tert = malloc(board->not * board->not * 2 * sizeof(*board->tert));
    board->terc = (char *)malloc(board->not * board->not);
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

void Board_Grid_Disp(Board_Type *board, char ifter)
{  
    unsigned int i, j, ishtt, isvtt;
    char *addr;//address of begin of rows
    char vsep='|', hsep='=';
    printf("\r\n");
    if(board->tert[1]==0 && ifter)
        for (i=0;i!=board->row;i++)
        {
            hsep = i/board->npt==board->tert[0] ?'%':'=';
            printf("%c%c%c%c",hsep,hsep,hsep,hsep);
        }
    else
        for (i=0;i!=board->row;i++)
            printf("====");

    printf("\r\n");
    for (i=0;i!=board->row;i++) //vertical loop
    {
        isvtt = i/board->npt==board->tert[1];
        ishtt = (i+1)/board->npt==board->tert[1]||(i+1)/board->npt==board->tert[1]+1;        

        addr = board->grid + i * board->row;

        vsep =ifter && board->tert[0]==0 && isvtt ?'%':'|';
        printf("%c",vsep);
        for (j=0;j!=board->row;j++)//horizontal loop
        {
            if((j+1) % board->npt == 0)//check if we are on the edg of a tile 
                vsep =ifter == 1 && isvtt && ((j+1)/board->npt==board->tert[0]||(j+1)/board->npt==board->tert[0]+1) ? '%':'|' ;
            else
                vsep = ' ';
            printf(" %c %c", *( addr  + j) == 'W' ? ' ' : *( addr + j),vsep);
        }
        printf("\r\n");

        if((i+1) % board->npt == 0)//check if we are on the edg of a tile
            for (j=0;j!=board->row;j++)//print the horizontal eage of grid
            {
                hsep =ifter == 1 && j/board->npt==board->tert[0] && ishtt ? '%':'=' ;
                printf("%c%c%c%c",hsep,hsep,hsep,hsep);
            }
        else
            for (j=0;j!=board->row;j++)
                printf("----");

        printf("\r\n");            
    }
    printf("\r\n");    
}


void Board_Move_Red(Board_Type *board)
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

void Board_Move_Blue(Board_Type *board)
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
char Board_Is_Sotp(Board_Type *board)
{
    unsigned int i,j,p,q,flag;
    unsigned int rcout,bcount;
    for (i=0;i!=board->not;i++)//loop of tile(i,j)
    {
        for(j=0;j!=board->not;j++)//loop of tile(i,j)
        {
            rcout=0;
            bcount=0;
            flag = 0;
            for(p=i*board->npt;p!=(i+1)*board->npt;p++)//loop of cells(p,q) in tile(i,j)
            {
                for(q=j*board->npt;q!=(j+1)*board->npt;q++)//loop of cells(p,q) in tile(i,j)
                {
                    if(*(board->grid + p*board->row + q )=='R')
                        rcout++;
                    else if(*(board->grid + p*board->row + q )=='B')
                        bcount++;
                }
            }
            *(board->terc+board->nott) = 0;
           if(  rcout >= board->ths )
           {
               *(board->tert+board->nott*2) = i;
               *(board->tert+board->nott*2+1) = j;

               *(board->terc+board->nott) += 1;
               flag = 1;

           }
           
           if(bcount >= board->ths)
           {
               *(board->tert+board->nott*2) = i;
               *(board->tert+board->nott*2+1) = j;

               *(board->terc+board->nott) += 2;
               flag = 1;
           }
           
            board->nott += flag;
            
        }
    }
    return 0;
}

void Board_Struct_Init(Board_Type *board)
{
    board->row = 0;
    board->cpt = 0;
    board->bsz = 0;
    board->ths = 0;
    board->nott = 0;
    board->bsz = 0;
    board->counter = 0;
    board->cpt = 0;
    board->grid = NULL;
    board->maxa = 0;
    board->not = 0;
    board->npt = 0;
    board->terc = NULL;
    board->tert = NULL;
    board->nott = 0; 
}

char Self_Check(Board_Type *b1,Board_Type *b2)
{
    unsigned int i;
    if(b1->bsz!=b2->bsz)
    {
        printf("The size of board dose not match!\r\n");
        return 1;
    }
    for (i=0;i!=b1->bsz;i++)
    {
        if(*(b1->grid+i)!=*(b2->grid+i))
        {
            printf("The grid dose not match!\r\n");
            return 1;
        }
    }
    if(b1->nott!=b2->nott)
    {
        printf("The terminated tiles dose not match!\r\n");
        return 1;
    }
    for(i=0;i!=b1->nott;i++)
    {
        if(*(b1->tert+i*2)!=*(b2->tert+i*2)||*(b1->tert+i*2+1)!=*(b2->tert+i*2+1))
        {
            printf("The coordinate of terminated tiles dose not match!\r\n");
            return 1;           
        }
        if(*(b1->terc+i)!=*(b2->terc+i))
        {
            printf("The color of terminated tiles dose not match!\r\n");
            return 1;                       
        }
    }
}