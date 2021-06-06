#include "Barrier.h"

void Barrier_init(Barrier_t *bar, int total)
{
    bar->count = 0;
    bar->total = total;
    pthread_mutex_init(&(bar->count_lock),NULL);
    pthread_cond_init(&(bar->pass),NULL);
}
void Barrier_destroy(Barrier_t *bar)
{
    pthread_mutex_destroy(&(bar->count_lock));
    pthread_cond_destroy(&(bar->pass));
}
void Barrier_wait(Barrier_t *bar)
{
    pthread_mutex_lock(&(bar->count_lock));
    bar->count++;
    if(bar->count<bar->total)
        pthread_cond_wait(&(bar->pass),&(bar->count_lock));
    else
    {
        bar->count = 0;
        pthread_cond_broadcast(&(bar->pass));
    }
    pthread_mutex_unlock(&(bar->count_lock));
}