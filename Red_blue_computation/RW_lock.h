#ifndef RW_LOCK_H
#define RW_LOCK_H
#include "pthread.h"
typedef struct
{
    int reading;
    int writing;
    int pending_writers;
    pthread_cond_t readers_proceed;
    pthread_cond_t writer_proceed;
    pthread_mutex_t read_writ_lock;
}RW_lock_t;
void RW_lock_init(RW_lock_t *lock);
void RW_lock_destroy(RW_lock_t *lock);
void RW_lock_read_lock(RW_lock_t *lock);
void RW_lock_read_unlock(RW_lock_t *lock);
void RW_lock_write_lock(RW_lock_t *lock);
void RW_lock_write_unlock(RW_lock_t *lock);
#endif