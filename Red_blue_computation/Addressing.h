#ifndef ADDRESSING_H
#define ADDRESSING_H

typedef struct
{
    unsigned int row;//coordinat address of iterm
    unsigned int column;//coordinat address of iterm
    unsigned int nrow;//maximum of x, e.t. number of columns
    unsigned int ncolumn;//maximum of y, e.t. number of rows
    unsigned int line;//linear addressing of block;
}Point_t;

void coor_to_linear(Point_t *b);
void linear_to_coor(Point_t *b);

#endif