#include "Board.h"
void Board_Grid_Init(Board_Type *board)
{
    unsigned int i;
    unsigned int r;

    board->grid = (char *)malloc(sizeof(*(board->grid)) * board->bsz);
    board->nott = (unsigned int *)malloc(sizeof(*(board->nott)));
    board->tert = (tile_t *)malloc(sizeof(*(board->tert)) * board->not );
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

void Board_Grid_Disp(Board_Type *board)
{
    unsigned int i, j, ishtt, isvtt;
    char *addr; //address of begin of rows
    char vsep = '|', hsep = '=';
    printf("\r\n");

    for (i = 0; i != board->n; i++)
        printf("====");

    printf("\r\n");
    for (i = 0; i != board->n; i++) //vertical loop
    {
        addr = board->grid + i * board->n;
        printf("%c", '|');
        for (j = 0; j != board->n; j++) //horizontal loop
        {
            if ((j + 1) % board->npt == 0) //check if we are on the edg of a tile
                vsep = '|';
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
                hsep = '=';
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
    unsigned int cur, dist;
    for (i = 0; i != board->row_length; i++)
    {
        for (j = 0; j < board->n; j++)
        {

            cur = i * board->n + j;

            dist = i * board->n + (j + 1) % board->n;

            if (*(board->grid + cur) == RED && *(board->grid + dist) == WITHE)
            {
                *(board->grid_local + dist) = RED;
                *(board->grid_local + cur) = WITHE;
                j++;
            }
            else
            {
                *(board->grid_local + cur) = *(board->grid + cur);
            }
        }
    }
    for (i = 0; i != board->row_length; i++)
        for (j = 0; j < board->n; j++)

            *(board->grid + i * board->n + j) = *(board->grid_local + i * board->n + j);
}

void Board_Move_Blue(Board_Type *board)
{
    unsigned int i, j;
    unsigned int cur, dist;

    for (j = 0; j != board->row_length; j++)
    {
        for (i = 0; i < board->n; i++)
        {

            cur = i * board->n + j;

            dist = (i + 1) % board->n * board->n + j;

            if (*(board->grid + cur) == BLUE && *(board->grid + dist) == WITHE)
            {
                *(board->grid_local + dist) = BLUE;
                *(board->grid_local + cur) = WITHE;
                i++;
            }
            else
            {
                *(board->grid_local + cur) = *(board->grid + cur);
            }
        }
    }
    for (j = 0; j < board->row_length; j++)
        for (i = 0; i != board->n; i++)
            *(board->grid + i * board->n + j) = *(board->grid_local + i * board->n + j);
}

char Board_Is_Sotp(Board_Type *board)
{
    unsigned int i, j, p, q, flag;
    unsigned int rcout, bcount;
    Point_t pointer;
    tile_t t;
    board->nott_local = 0;
    pointer.nrow = board->t;
    pointer.ncolumn = board->t;
    for (i = board->tile_start; i != board->tile_stop + 1; i++) //loop of i-th tile]
    {
        rcout = 0;
        bcount = 0;
        flag = 0;
        pointer.line = i;
        linear_to_coor(&pointer);
        for (p = pointer.row * board->npt; p != (pointer.row + 1) * board->npt; p++) //loop of cells(p,q) in tile(i,j)
        {
            for (q = pointer.column * board->npt; q != (pointer.column + 1) * board->npt; q++) //loop of cells(p,q) in tile(i,j)
            {
                //printf("%d,%d,%d\r\n",p,q,i);
                if (*(board->grid + p * board->n + q) == RED)

                    rcout++;

                else if (*(board->grid + p * board->n + q) == BLUE)
                    bcount++;
            }
        }

        t.color = 0;

        if (rcout >= board->ths)
        {
            t.addr = i;
            t.color += RED;
            flag = 1;
            *(board->tert_locla + board->nott_local) = t;
        }

        if (bcount >= board->ths)
        {
            t.addr = i;
            t.color += BLUE;
            flag = 1;
            *(board->tert_locla + board->nott_local) = t;
        }

        board->nott_local += flag;
    }
    return 0;
}

void Board_Struct_Init(Board_Type *board)
{
    board->c = 0;    //terminate condition in percent
    board->n = 0;    //number of column
    board->t = 0;    //number of tile in one dimension
    board->maxa = 0; //maximum number of interactio ns

    board->npt = 0;     //number of rows per tile
    board->ths = 0;     //terminal threshold in number
    board->bsz = 0;     //board size (number of cells in the board}
    board->counter = 0; //interactions conter

    board->nott = NULL; //number of tiles meet the terminal condition
    board->tert = NULL;
    board->grid = NULL; //pointer to the satrt address of the board

    board->tile_start = 0; //the absolute coordinate [row column] y] of up left tile
    board->tile_stop = 0;
    board->row_start = 0;
    board->row_stop = 0;
}
char Self_Check(Board_Type *board_l, Board_Type *board_std)
{
    unsigned int i;
    char local_sum = 0;
    char remote_sum = 0;
    printf("Conducting self-checking...\r\n");
    for (i = 0; i != *(board_l->nott); i++)
    {
        local_sum += (char)(board_l->tert + i)->addr * (board_l->tert + i)->color; //address of terminated tile * the colorhhh
    }

    for (i = 0; i != *(board_std->nott); i++)
    {
        remote_sum += (char)(board_std->tert + i)->addr * (board_std->tert + i)->color; //address of terminated tile * the colorhhh
    }

    if (local_sum == remote_sum)
    {
        printf("Self-checking complete, all results match!\r\n");
        return 0;
    }
    else
    {
        printf("Self-checking complete, ruslts did not match!\r\n");
        return 1;
    }
}

void Board_Decompose(Board_Type *board)
{
    board->tile_start = board->not / board->nthread * board->rthread;
    board->tile_length = board->rthread == board->nthread - 1 ? board->not / board->nthread + (board->not % board->nthread) : board->not / board->nthread;
    board->tile_stop = board->tile_start + board->tile_length - 1;

    board->row_start = board->n / board->nthread * board->rthread;
    board->row_length = board->rthread == board->nthread - 1 ? board->n / board->nthread + (board->n % board->nthread) : board->n / board->nthread;
    board->row_stop = board->row_start + board->row_length - 1;

    board->cell_start = board->row_start * board->n;
    board->cell_length = board->row_length * board->n;
    board->cell_stop = board->cell_start + board->cell_length - 1;

    board->tert_locla = (tile_t *)malloc(sizeof(*(board->tert_locla)) * board->tile_length);

    board->grid_local = (char *)malloc(sizeof(*(board->grid_local)) * board->cell_length);
}
// void Board_index(Board_Type *board)
// {
//     unsigned int min;
//     board->n = 9;
//     board->bsz = board->n * board->n;
//     min = board->n;
//     board->t = 3;

//     board->not = board->t * board->t;
//     if (board->not<min)
//         min = board->not;
//     board->npt = board->n / board->t;
//     board->c = 100;
//     board->ths = board->npt * board->npt * board->c * 0.01;
//     board->nthread = 8;

//     board->maxa = 50;
//     printf("\r\n n=%d,t=%d,c=%d%%,M=%d,thread=%d\r\n", board->n, board->t, board->c, board->maxa,board->nthread);
// }
void Board_index(Board_Type *board)
{
    unsigned int min;
    printf("Number of rows(n) in the board n:\r\n");
    scanf("%d", &board->n);
    board->bsz = board->n * board->n;
    min = board->n;
    while (1)
    {
        printf("Number of taills(t) in a dimension t:\r\n");
        scanf("%d", &board->t);
        if (board->t <= board->n)
            break;
        else
            printf("There should be at lest one n per tile.\r\n");
    }

    board->not = board->t * board->t;
    if (board->not < min)
        min = board->not ;
    board->npt = board->n / board->t;
    while (1)
    {
        printf("Threshold(0<=c%%<=100):\r\n");
        scanf("%d", &board->c);
        if (board->c <= 100 || board->c >= 0)
            break;
        else
            printf("c%% shoudl be in range from 0 to 100.\r\n");
    }
    board->ths = board->npt * board->npt * board->c * 0.01;
    while (1)
    {
        printf("Nomber of threads(1<=thr<=%d):\r\n", min);
        scanf("%d", &board->nthread);
        if (board->nthread >= 1)
            break;
        else
            printf("thr shoudl be at leas 1.\r\n");
    }

    printf("Maximum interactions:\r\n");
    scanf("%d", &board->maxa);

    if (board->nthread > min)
    {
        printf("Too many thread, the Nomber of threads will be set to maximum: %d", min);
        board->nthread = min;
    }

    printf("\r\n n=%d,t=%d,c=%d%%,M=%d\r\n", board->n, board->t, board->c, board->maxa);
}
RW_lock_t rwlock;
Barrier_t barrier;
void *workload(void *board)
{
    Board_Type local;
    unsigned int cpnott, i;

    local = *((Board_Type *)board);

    Board_Decompose(&local);
    printf("Thread %d: Start inspecting %d-th to %d-th tile, controling %d-th to %d-th row\r\n", local.rthread, local.tile_start, local.tile_stop, local.cell_start, local.cell_stop);
    for (local.counter = 0; local.counter != local.maxa; local.counter++)
    {
        Board_Is_Sotp(&local);
        if (local.nott_local == 0)
        {

            Barrier_wait(&barrier);

            cpnott = *(local.nott);

            if (cpnott == 0)
            {

                Board_Move_Red(&local);

                Barrier_wait(&barrier);

                Board_Move_Blue(&local);
                if (local.nthread == 1)
                {
                    printf("After %d interaction:\r\n", local.counter + 1);
                    Board_Grid_Disp(&local);
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            for (i = 0; i != local.nott_local; i++)
                printf("Thread %d: The number of %s cells in tile (%d,%d) is more than or equal to %d .\r\n", local.rthread, (local.tert_locla + i)->color == RED ? "red" : (local.tert_locla + i)->color == BLUE ? "blue"
                                                                                                                                                                                                                  : "both",
                       (local.tert_locla + i)->addr / local.t, (local.tert_locla + i)->addr % local.t, local.ths);

            RW_lock_write_lock(&rwlock);
            memcpy(local.tert + *(local.nott), local.tert_locla, sizeof(*(local.tert_locla)) * local.nott_local);
            *(local.nott) += local.nott_local;

            RW_lock_write_unlock(&rwlock);
            Barrier_wait(&barrier);
            break;
        }
    }
    if (local.rthread == 1)
    {
        printf("%d interactions have been simulated, terminated.\r\n", local.counter);
        Board_Grid_Disp(&local);
    }
    pthread_exit(NULL);
    return NULL;
}

void Board_create_threads(Board_Type *board)
{
    pthread_t *thr;
    Board_Type *b;
    unsigned int n;
    RW_lock_init(&rwlock);
    Barrier_init(&barrier, board->nthread);
    thr = (pthread_t *)malloc(sizeof(pthread_t) * board->nthread);
    b = (Board_Type *)malloc(sizeof(Board_Type) * board->nthread);
    for (n = 0; n != board->nthread; n++)
    {
        *(b + n) = *(board);
        (b + n)->rthread = n;
        pthread_create(thr + n, NULL, workload, b + n);
    }
    for (n = 0; n != board->nthread; n++)
        pthread_join(*(thr + n), NULL);

    RW_lock_destroy(&rwlock);
    Barrier_destroy(&barrier);
}