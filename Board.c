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
            *(board->grid + i) = RED;
        else if (r < 66)
            *(board->grid + i) = BLUE;
        else
            *(board->grid + i) = WITHE;
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
            printf(" %c %c", *(addr + j) == WITHE ? ' ' : *(addr + j) == RED ? 'R'
                                                                             : 'B',
                   vsep);
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
            if (*cur == RED && *dist == WITHE)
            {
                *dist = RED;
                *cur = WITHE;
                j++;
            }
        }
    }
}

void Board_Move_Blue(Board_Type *board)
{
    unsigned int i, j;
    char *cur, *dist, *head, *tail, *temp;
    head = malloc(board->size[1]);
    tail = malloc(board->size[1]);
    temp = malloc(board->size[1] * (board->size[0] + 2));
    MPI_Sendrecv(board->grid + (board->size[0] - 1) * board->size[1], board->size[1], MPI_CHAR, board->nextp, 0, head, board->size[1], MPI_CHAR, board->lastp, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //transmit the last row to next process and recieve from last process

    memcpy(temp, head, board->size[1]); //expand the head
    memcpy(temp + board->size[1], board->grid, board->bsz);
    MPI_Sendrecv(board->grid, board->size[1], MPI_CHAR, board->lastp, 0, tail, board->size[1], MPI_CHAR, board->nextp, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //transmit the first row to next process and recieve from last process
    memcpy(temp + board->size[1] + board->bsz, tail, board->size[1]);                                                                                         //expand the tail
    for (j = 0; j != board->size[1]; j++)
    {
        for (i = 0; i < board->size[0] + 1; i++) //as a ordinary board added by 2 extra row fome neighbor
        {
            cur = temp + i * board->size[1] + j;
            dist = temp + (i + 1) * board->size[1] + j;
            if (*cur == BLUE && *dist == WITHE)
            {
                *dist = BLUE;
                *cur = WITHE;
                i++;
            }
        }
    }
    memcpy(board->grid, temp + board->size[1], board->bsz);

    free(tail);
    free(head);
    free(temp);
}

char Board_Is_Sotp(Board_Type *board)
{
    unsigned int i, j, p, q, flag;
    unsigned int rcout, bcount;
    // for (i = 0; i != board->size[0] * board->size[1]; i++)
    // {
    //     printf("%d ", *((char *)board->grid + i));
    // }
    // printf("\r\n");
    for (i = 0; i != board->tile[0]; i++) //loop of tile(i,j)
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

                    if (*(board->grid + p * board->n + q) == RED)
                        rcout++;
                    else if (*(board->grid + p * board->n + q) == BLUE)
                        bcount++;
                }
            }
            *(board->terc + board->nott) = 0;

            if (rcout >= board->ths)
            {
                *(board->tert + board->nott * 2) = i + board->tile_start[0];
                *(board->tert + board->nott * 2 + 1) = j + board->tile_start[1];

                *(board->terc + board->nott) += RED;
                flag = 1;
            }

            if (bcount >= board->ths)
            {
                *(board->tert + board->nott * 2) = i + board->tile_start[0];
                *(board->tert + board->nott * 2 + 1) = j + board->tile_start[1];

                *(board->terc + board->nott) += BLUE;
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

void Board_Struct_Finalize(Board_Type *board)
{
    free((void *)board->tert);
    board->tert = NULL; //the rank of tile meet the terminal condition (x,y)
    free((void *)board->terc);
    board->terc = NULL; //wich color meet the terminal condition 1:Red 2:Blue 3:both
    free((void *)board->grid);
    board->grid = NULL; //pointer to the satrt address of the board
}

char Self_Check(Board_Type *board_l, Board_Type *board_std)
{
    unsigned int i;
    char local_sum = 0;
    char remote_sum = 0;

    for (i = 0; i != board_l->nott; i++)
    {
        local_sum += (char)(*(board_l->tert + i * 2) + *(board_l->tert + i * 2 + 1) +1) * *(board_l->terc + i); //coordinate of terminated tile * the color
    }

    MPI_Reduce(&local_sum, &remote_sum, 1, MPI_CHAR, MPI_SUM, MASTER, MPI_COMM_WORLD);

    if (board_l->world_rank == MASTER)
    {

        printf("Conducting self-checking...\r\n");
        local_sum = 0;
        for (i = 0; i != board_std->nott; i++)
        {
            local_sum += (char)(*(board_std->tert + i * 2) + *(board_std->tert + i * 2 + 1) + 1) * *(board_std->terc + i); //coordinate of terminated tile * the color
        }
        if (local_sum == remote_sum)
            printf("Self-checking complete, all results match!\r\n");
        else
            printf("Self-checking complete, ruslts did not match!\r\n");
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

    board->npt = board->n / board->t;
    board->ths = board->npt * board->npt * board->c * 0.01;

    board->tile_start[0] = board->t / size * rank;
    board->tile_start[1] = 0;

    board->cell_start = board->tile_start[0] * board->npt * board->n;

    board->tile[0] = rank == size - 1 ? board->t / size + (board->t % size) : board->t / size;
    board->tile[1] = board->t;

    board->size[0] = board->tile[0] * board->n / board->t;
    board->size[1] = board->n;

    board->tert = malloc(board->tile[1] * board->tile[0] * 2 * sizeof(*board->tert));

    board->terc = (char *)malloc(board->tile[0] * board->tile[1]);

    board->grid = malloc(board->size[0] * board->size[1]);

    board->bsz = board->size[1] * board->size[0];

    return 0;
}

void Board_Sch_Master(Board_Type *board)
{

    MPI_Bcast(board, 4, MPI_UNSIGNED, MASTER, MPI_COMM_WORLD);

    MPI_Bcast(board->grid, board->n * board->n, MPI_CHAR, MASTER, MPI_COMM_WORLD);
}

char Board_Sch_Slave(Board_Type *board)
{
    unsigned int i;
    void *g;

    MPI_Bcast(board, 4, MPI_UNSIGNED, MASTER, MPI_COMM_WORLD);

    g = malloc(board->n * board->n);

    if (Board_Decompose(board, board->world_rank, board->world_size))
    {
        printf("already decompse every tile, process %d will be finalized", board->world_rank);
        return 1;
    }

    MPI_Bcast(g, board->n * board->n, MPI_CHAR, MASTER, MPI_COMM_WORLD);

    memcpy(board->grid, (char *)g + board->cell_start, board->size[0] * board->size[1]);

    free((void *)g);

    printf("process %d: received massages n=%d,t=%d,c=%d%%,M=%d\r\n",
           board->world_rank, board->n, board->t, board->c, board->maxa);

    return 0;
}

void Board_index(Board_Type *board)
{
    printf("Number of rows(n) in the board n:\r\n");
    scanf("%d", &board->n);

    while (1)
    {
        printf("Number of taills(t) in a dimension t:\r\n");
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

    printf("\r\n n=%d,t=%d,c=%d%%,M=%d\r\n", board->n, board->t, board->c, board->maxa);
}

char Board_Sequantial(Board_Type *board)
{
    unsigned int i;
    Board_index(board); //input 4 user defined parameters

    Board_Decompose(board, MASTER, 1);

    Board_Grid_Init(board);
    board->nott = 0;
    for (board->counter = 0; board->counter != board->maxa + 1; board->counter++)
    {
        printf("After %d interactions: \r\n", board->counter);
        Board_Grid_Disp(board, 0);

        Board_Is_Sotp(board);

        if (board->nott != 0)
        {
            printf("terminal condition is met! \r\n");
            for (i = 0; i != board->nott; i++)
                printf("After %d interactions the number of %s cells %s more than %d%% cells in tile(%d,%d)\r\n",board->counter, (*board->terc) == RED ? "Red" : (*board->terc) == BLUE ? "Blue"
                                                                                                                                                   : "Red and Blue",
                       (*board->terc) == BOTH ? "are" : "is", board->c, *(board->tert + 2 * i), *(board->tert + 2 * i + 1));
            return 0;
        }

        Board_Move_Red(board);
        Board_Move_Blue(board);
    }
    printf("terminal condition is met! \r\n");
    printf("Number of interactions = Max. \r\n");
    return 0;
}

char Board_Parellel(Board_Type *board)
{
    unsigned int i;
    char rec = 0, stop = 0;
    board->nott = 0;
    for (board->counter = 0; board->counter != board->maxa + 1; board->counter++)
    {

        Board_Is_Sotp(board);

        if (board->nott != 0)
        {
            printf("process %d: terminal condition is met! \r\n", board->world_rank);
            for (i = 0; i != board->nott; i++)
                printf("process %d: After %d interactions the number of %s cells %s more than %d%% cells in tile(%d,%d)\r\n", board->world_rank, board->counter, (*board->terc) == RED ? "Red" : (*board->terc) == BLUE ? "Blue"
                                                                                                                                                                                                                       : "Red and Blue",
                       (*board->terc) == BOTH ? "are" : "is", board->c, *(board->tert + 2 * i), *(board->tert + 2 * i + 1));
            stop = 1;
            MPI_Allreduce(&stop, &rec, 1, MPI_CHAR, MPI_LOR, MPI_COMM_WORLD);
            return 1;
        }
        MPI_Allreduce(&stop, &rec, 1, MPI_CHAR, MPI_LOR, MPI_COMM_WORLD);

        if (rec == 1)
            return 1;
        Board_Move_Red(board);
        Board_Move_Blue(board);
    }

    return 0;
}