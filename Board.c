#include "Board.h"

void Board_Grid_Init(Board_Type *board)
{
    unsigned int i;
    unsigned int r;

    board->grid = (char *)malloc(board->size[0] * board->size[1]);
    srand(time(0));
    for (i = 0; i != board->bsz; i++)
    {
        r = rand() % 100;
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
    char *addr; //address of begin of rows
    char vsep = '|', hsep = '=';
    printf("\r\n");
    if (board->tert[1] == 0 && ifter)
        for (i = 0; i != board->n; i++)
        {
            hsep = i / board->npt == board->tert[0] ? '%' : '=';
            printf("%c%c%c%c", hsep, hsep, hsep, hsep);
        }
    else
        for (i = 0; i != board->n; i++)
            printf("====");

    printf("\r\n");
    for (i = 0; i != board->n; i++) //vertical loop
    {
        isvtt = i / board->npt == board->tert[1];
        ishtt = (i + 1) / board->npt == board->tert[1] || (i + 1) / board->npt == board->tert[1] + 1;

        addr = board->grid + i * board->n;

        vsep = ifter && board->tert[0] == 0 && isvtt ? '%' : '|';
        printf("%c", vsep);
        for (j = 0; j != board->n; j++) //horizontal loop
        {
            if ((j + 1) % board->npt == 0) //check if we are on the edg of a tile
                vsep = ifter == 1 && isvtt && ((j + 1) / board->npt == board->tert[0] || (j + 1) / board->npt == board->tert[0] + 1) ? '%' : '|';
            else
                vsep = ' ';
            printf(" %c %c", *(addr + j) == 'W' ? ' ' : *(addr + j), vsep);
        }
        printf("\r\n");

        if ((i + 1) % board->npt == 0)      //check if we are on the edg of a tile
            for (j = 0; j != board->n; j++) //print the horizontal eage of grid
            {
                hsep = ifter == 1 && j / board->npt == board->tert[0] && ishtt ? '%' : '=';
                printf("%c%c%c%c", hsep, hsep, hsep, hsep);
            }
        else
            for (j = 0; j != board->n; j++)
                printf("----");

        printf("\r\n");
    }
    printf("\r\n");
}

void Board_Move_Red(Board_Type *board)
{
    unsigned int i, j;
    char *cur, *dist;
    for (i = 0; i != board->size[0]; i++)
    {
        for (j = 0; j < board->size[1]; j++)
        {
            cur = board->grid + i * board->size[0] + j;
            dist = board->grid + i * board->size[0] + ((j + 1) % board->size[0]);
            if (*cur == 'R' && *dist == 'W')
            {
                *dist = 'R';
                *cur = 'W';
                j++;
            }
        }
    }
}

void Board_Move_Blue(Board_Type *board)
{
    unsigned int i, j;
    char *cur, *dist;
    for (j = 0; j != board->size[1]; j++)
    {
        for (i = 0; i < board->size[0]; i++)
        {
            cur = board->grid + i * board->size[0] + j;
            dist = board->grid + ((i + 1) % board->size[1]) * board->size[0] + j;
            if (*cur == 'B' && *dist == 'W')
            {
                *dist = 'B';
                *cur = 'W';
                i++;
            }
        }
    }
}

char Board_Is_Sotp(Board_Type *board)
{
    unsigned int i, j, p, q, flag;
    unsigned int rcout, bcount;
    for (i = 0; i != board->tile[1]; i++) //loop of tile(i,j)
    {
        for (j = 0; j != board->tile[1]; j++) //loop of tile(i,j)
        {
            rcout = 0;
            bcount = 0;
            flag = 0;
            for (p = i * board->npt; p != (i + 1) * board->npt; p++) //loop of cells(p,q) in tile(i,j)
            {
                for (q = j * board->npt; q != (j + 1) * board->npt; q++) //loop of cells(p,q) in tile(i,j)
                {
                    if (*(board->grid + p * board->n + q) == 'R')
                        rcout++;
                    else if (*(board->grid + p * board->n + q) == 'B')
                        bcount++;
                }
            }
            *(board->terc + board->nott) = 0;
            if (rcout >= board->ths)
            {
                *(board->tert + board->nott * 2) = i;
                *(board->tert + board->nott * 2 + 1) = j;

                *(board->terc + board->nott) += 1;
                flag = 1;
            }

            if (bcount >= board->ths)
            {
                *(board->tert + board->nott * 2) = i;
                *(board->tert + board->nott * 2 + 1) = j;

                *(board->terc + board->nott) += 2;
                flag = 1;
            }

            board->nott += flag;
        }
    }
    return 0;
}

void Board_Struct_Init(Board_Type *board)
{
    board->c = 0;       //terminate condition in percent
    board->n = 0;       //number of column
    board->t = 0;       //number of tile in one dimension
    board->size[0] = 0; //size of cell [n.row n.column]
    board->size[1] = 0;
    board->tile[0] = 0; //size of tile [n.row n.column]
    board->tile[1] = 0;
    board->tile_start[1] = 0; //the absolute coordinate [row column] y] of up left tile
    board->tile_start[0] = 0;
    board->maxa = 0;  //maximum number of interactio ns
    board->lastp = 0; //last processor in rank
    board->nextp = 0; //next processor in rank

    board->npt = 0;     //number of rows per tile
    board->ths = 0;     //terminal threshold in number
    board->bsz = 0;     //board size (number of cells in the board}
    board->nott = 0;    //number of tiles meet the terminal condition
    board->tert = NULL; //the rank of tile meet the terminal condition (x,y)
    board->terc = NULL; //wich color meet the terminal condition 1:Red 2:Blue 3:both
    board->counter = 0; //interactions conter

    board->grid = NULL; //pointer to the satrt address of the board
}

char Self_Check(Board_Type *b1, Board_Type *b2)
{
    unsigned int i;
    if (b1->bsz != b2->bsz)
    {
        printf("The size of board dose not match!\r\n");
        return 1;
    }
    for (i = 0; i != b1->bsz; i++)
    {
        if (*(b1->grid + i) != *(b2->grid + i))
        {
            printf("The grid dose not match!\r\n");
            return 1;
        }
    }
    if (b1->nott != b2->nott)
    {
        printf("The terminated tiles dose not match!\r\n");
        return 1;
    }
    for (i = 0; i != b1->nott; i++)
    {
        if (*(b1->tert + i * 2) != *(b2->tert + i * 2) || *(b1->tert + i * 2 + 1) != *(b2->tert + i * 2 + 1))
        {
            printf("The coordinate of terminated tiles dose not match!\r\n");
            return 1;
        }
        if (*(b1->terc + i) != *(b2->terc + i))
        {
            printf("The color of terminated tiles dose not match!\r\n");
            return 1;
        }
    }
    return 0;
}

char Board_Decompose(Board_Type *board, int rank, int size)
{
    if (rank >= board->t)
        return 1;

    if (size > board->t)
        size = board->t;

    board->lastp = rank == 0 ? size - 1 : rank - 1;
    board->nextp = rank == size - 1 ? 0 : rank + 1;

    board->tile_start[0] = board->t / size * rank;
    board->tile_start[1] = 0;

    board->cell_start = board->n / size * rank * board->n;

    board->tile[0] = rank == size - 1 ? board->t / size + (board->t % size) : board->t / size;
    board->tile[1] = board->t;

    board->size[0] = board->tile[0] * board->n / board->t;
    board->size[1] = board->n;

    board->tert = malloc(board->tile[1] * board->tile[0] * 2 * sizeof(*board->tert));

    board->terc = (char *)malloc(board->tile[0] * board->tile[1]);

    board->bsz = board->size[1] * board->size[0];

    return 0;
}

void Board_Sch_Master(Board_Type *board)
{

    MPI_Bcast(board, 4, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    MPI_Bcast(board->grid, board->n * board->n, MPI_CHAR, 0, MPI_COMM_WORLD);

    Board_Decompose(board, MASTER, board->world_size);
}

char Board_Sch_Slave(Board_Type *board)
{
    unsigned int i;
    void *g;

    MPI_Bcast(board, 4, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    g = malloc(board->n * board->n);
    
    if (Board_Decompose(board, board->world_rank, board->world_size))
    {
        printf("already decompse every tile, process %d will be finalized", board->world_rank);
        return 1;
    }
    board->grid = malloc(board->size[0] * board->size[1]);
    MPI_Bcast(g, board->n * board->n, MPI_CHAR, 0, MPI_COMM_WORLD);

    memcpy(board->grid, (char *)g + board->cell_start, board->size[0] * board->size[1]);

    free((void *)g);

    printf("process %d received massages n=%d,t=%d,c=%d%%,M=%d\r\n",
           board->world_rank, board->n, board->t, board->c, board->maxa);

    return 0;
}

void Board_index(Board_Type *board)
{
    printf("Number of rows(n) in the board:\r\n");
    scanf("%d", &board->n);

    while (1)
    {
        printf("Number of taills(t) in a dimension:\r\n");
        scanf("%d", &board->t);
        if (board->t <= board->n)
            break;
        else
            printf("There should be at lest one n per tile.\r\n");
    }

    while (1)
    {
        printf("Threshold(0<=c%%<=100):\r\n");
        scanf("%d", &board->c);
        if (board->c <= 100 || board->c >= 0)
            break;
        else
            printf("c%% shoudl be in range from 0 to 100.\r\n");
    }

    printf("Maximum interactions:\r\n");
    scanf("%d", &board->maxa);

    board->npt = board->n / board->t;
    board->ths = board->npt * board->npt * board->c * 0.01;
}