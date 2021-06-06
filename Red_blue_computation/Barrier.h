#ifndef BARRIER_H
#define BARRIER_H
#include <pthread.h>
typedef struct 
{
    pthread_mutex_t count_lock;
    pthread_cond_t pass;
    int count;
    int total;
}Barrier_t;

void Barrier_init(Barrier_t *bar, int total);
void Barrier_destroy(Barrier_t *bar);
void Barrier_wait(Barrier_t *bar);



#endif