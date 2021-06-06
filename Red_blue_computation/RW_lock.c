#include "RW_lock.h"
void RW_lock_init(RW_lock_t *lock)
{
    lock->writing = 0;
    lock->reading = 0;
    lock->pending_writers = 0;
    pthread_mutex_init(&(lock->read_writ_lock),NULL);
    pthread_cond_init(&(lock->readers_proceed),NULL);
    pthread_cond_init(&(lock->writer_proceed),NULL);
}
void RW_lock_destroy(RW_lock_t *lock)
{
    pthread_mutex_destroy(&(lock->read_writ_lock));
    pthread_cond_destroy(&(lock->readers_proceed));
    pthread_cond_destroy(&(lock->writer_proceed));
}
void RW_lock_read_lock(RW_lock_t *lock)
{
    pthread_mutex_lock(&(lock->read_writ_lock));
    while(lock->writing + lock->pending_writers != 0)
        pthread_cond_wait(&(lock->readers_proceed),&(lock->read_writ_lock));
    lock->reading ++;
    pthread_mutex_unlock(&(lock->read_writ_lock));
}
void RW_lock_read_unlock(RW_lock_t *lock)
{
    pthread_mutex_lock(&(lock->read_writ_lock));
    lock->reading --;
    if(lock->reading == 0)
        pthread_cond_signal(&(lock->writer_proceed));
    pthread_mutex_unlock(&(lock->read_writ_lock));
}
void RW_lock_write_lock(RW_lock_t *lock)
{
    pthread_mutex_lock(&(lock->read_writ_lock));
    lock->pending_writers ++;
    while(lock->writing + lock->reading != 0)
        pthread_cond_wait(&(lock->writer_proceed),&(lock->read_writ_lock));
    lock->pending_writers ++;
    lock->writing ++;
    pthread_mutex_unlock(&(lock->read_writ_lock));
}
void RW_lock_write_unlock(RW_lock_t *lock)
{
    pthread_mutex_lock(&(lock->read_writ_lock));
    lock->writing =0;
    if(lock->pending_writers !=0)
        pthread_cond_signal(&(lock->writer_proceed));
    else
        pthread_cond_broadcast(&(lock->readers_proceed));
    pthread_mutex_unlock(&(lock->read_writ_lock));
}