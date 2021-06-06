#include "Addressing.h"
void coor_to_linear(Point_t *b)
{
    b->line = b->row * b->ncolumn + b->column;
}
void linear_to_coor(Point_t *b)
{
    b->row = b->line/b->ncolumn;
    b->column = b->line%b->ncolumn;
}