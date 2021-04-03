#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
//#include <mpi.h>

char* Grid_Init(int n);
void Grid_Disp(char *grid, unsigned int npt, unsigned int not);
void Move_Red(char *grid, unsigned int raw);
void Move_Blue(char *grid, unsigned int raw);
char Is_sotp(char *grid,float c ,unsigned int npt, unsigned int not, unsigned int gsz,unsigned int raw, unsigned int bot);


int main(int argc , char **argv)
{
    char *grid;

    unsigned int npt;
    unsigned int not;
    float c=5;
    unsigned int raw;
    unsigned int bot;
    unsigned int gsz;
    unsigned int ths;

    printf("number of raws per taill:");
    scanf("%d",&npt);
    printf("number of taills:");
    scanf("%d",&not);
    printf("threshold(c):");
    scanf("%f",&c);
    printf("\r\n npt=%d,not=%d,c=%f\r\n",npt,not,c);

    raw = npt * not;
    bot = npt * npt;
    gsz = (unsigned int)pow(raw,2);
    ths = npt * npt * c;

    grid = Grid_Init(gsz);
    Grid_Disp(grid,npt,not);
    while (!Is_sotp(grid, c,npt,not,gsz,raw,bot))
    {
         Move_Red(grid,raw);
         Move_Blue(grid,raw);
         Grid_Disp(grid,npt,not);
    }
    Grid_Disp(grid,npt,not);
    return 0;
}

char* Grid_Init(int n)
{
    char *grid;
    unsigned int i;
    unsigned int r;
    grid = (char *)malloc(n);
    srand(time(0));
    for (i = 0; i != n; i++)
    {
        r = rand()%100;
        if (r < 33)
            *(grid + i) = 'R';
        else if (r < 66)
            *(grid + i) = 'B';
        else
            *(grid + i) = 'W';
        }
    return grid;
}

void Grid_Disp(char *grid, unsigned int npt, unsigned int not)
{
    unsigned int i, j;
    unsigned int raw = npt*not;
    char sep;
    printf("\r\n");
    for (i=0;i!=raw*4+1;i++)
        printf("=");
    printf("\r\n");
    for (i=0;i!=raw;i++)
    {
        printf("|");
        for (j=0;j!=raw;j++)
        {
            if ((j+1)%not == 0)
            {
                if (*(grid+i*raw+j) != 'W')//only print R&B
                    printf(" %c |",*(grid+i*raw+j));
                else
                    printf("   |");                
            }
            else
            {
                if (*(grid+i*raw+j) != 'W')//only print R&B
                    printf(" %c  ",*(grid+i*raw+j));
                else
                    printf("    ");
            }
        }
        printf("\r\n");  
        if((i+1)%not == 0)
        {
            for (j=0;j!=raw*4+1;j++)
                printf("=");
        }
        else
        {
            for (j=0;j!=raw*4+1;j++)
                printf("-");
        }    
        printf("\r\n");            
    }
    printf("\r\n");    
}


void Move_Red(char *grid, unsigned int raw)
{
    unsigned int i, j;
    char *cur,*dist;
    for(i = 0; i!=raw ;i++)
    {
        for(j=0; j<raw ; j++)
        {
            cur = grid+i*raw+j;
            dist = grid+i*raw+((j+1)%raw);
            if (*cur == 'R' && *dist == 'W')
            {
                *dist='R';
                *cur = 'W';
                j++;    
            } 
        }
    } 
}

void Move_Blue(char *grid, unsigned int raw)
{
    unsigned int i, j;
    char *cur,*dist;
    for(j = 0; j!=raw ;j++)
    {
        for(i=0; i<raw ; i++)
        {
            cur = grid+i*raw+j;
            dist = grid+((i+1)%raw)*raw+j;
            if (*cur == 'B' && *dist == 'W')
            {
                *dist='B';
                *cur = 'W';
                i++;    
            } 
        }
    } 
}
char Is_sotp(char *grid,float c ,unsigned int npt, unsigned int not, unsigned int gsz,unsigned int raw, unsigned int bot)
{
    unsigned int i,j,p,q;
    unsigned int rint,bint;
    for (i=0;i!=not;i++)
    {
        for(j=0;j!=not;j++)
        {
            rint=0;
            bint=0;
            for(p=i*npt;p!=(i+1)*npt;p++)
            {
                for(q=j*npt;q!=(j+1)*npt;q++)
                {
                    if(*(grid + p*raw + q )=='R')
                        rint++;
                    else if(*(grid + p*raw + q )=='B')
                        bint++;
                }
            }
            if(rint>=c*bint || bint>=c*rint)
                return 1;       
        }
    }
    return 0;
}