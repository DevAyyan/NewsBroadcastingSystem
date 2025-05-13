#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define SHM_NAME "/news_shm" //shared memory for NewsItem struct
#define SEM_MUTEX "/sem_mutex"	//sem for differentiating between castor and viewer
#define SEM_EMPTY "/sem_empty" 
#define SEM_FULL "/sem_full"
#define SEM_COMMENT_NOTIFY "/sem_comment_notify"

#define MAX_NEWS_LENGTH 256
#define MAX_COMMENTS 10
#define MAX_COMMENT_LENGTH 200
#define NUM_VIEWERS 3

typedef struct {
    char news[MAX_NEWS_LENGTH];
    char comments[MAX_COMMENTS][MAX_COMMENT_LENGTH];
    int comment_count;
    int viewers_ready;
    int total_viewers;
    int broadcast_done;
} SharedData;


#endif
