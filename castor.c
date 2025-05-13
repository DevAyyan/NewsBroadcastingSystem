#include "shared.h"
#include <signal.h>

int main() {
    int fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    SharedData* data = mmap(0, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    sem_t *mutex = sem_open(SEM_MUTEX, 0);
    sem_t *empty = sem_open(SEM_EMPTY, 0);
    sem_t *full = sem_open(SEM_FULL, 0);
    sem_t *comment_notify = sem_open(SEM_COMMENT_NOTIFY, 0);

    int count = 1;
    char news[MAX_NEWS_LENGTH];

    while (1) {
        // Wait for the empty semaphore to signal that a new broadcast can begin
        sem_wait(empty); 
        sem_wait(mutex); // Wait on mutex for synchronization
        printf("\n[Castor] Enter news to broadcast (or Ctrl+C to quit): ");
        fgets(news, MAX_NEWS_LENGTH - 10, stdin);
        news[strcspn(news, "\n")] = '\0';  // Remove newline from input

        // Reset shared memory to prepare for the next broadcast
        memset(data->news, 0, MAX_NEWS_LENGTH);
        for (int i = 0; i < MAX_COMMENTS; i++)
            memset(data->comments[i], 0, MAX_COMMENT_LENGTH);
        data->comment_count = 0;
        data->broadcast_done = 0;
        data->viewers_ready = 0;

        snprintf(data->news, MAX_NEWS_LENGTH, "News %d: %s", count++, news);
        sem_post(mutex);  // Release mutex to allow other processes to interact with shared memory
        sem_post(full);   // Signal that the broadcast is ready

        // Wait for viewers to read and comment on the news
        while (1) {
            sleep(1);
            sem_wait(mutex);

            // Update the screen to show the latest news and comments
            system("clear");
            printf("[Castor]\nBroadcasted: %s\n", data->news);
            printf("Live Comments:\n");
            for (int i = 0; i < data->comment_count; i++)
                printf(" - %s\n", data->comments[i]);

            // If all viewers are ready, finish the broadcast
           // printf("ready: %d,total:%d",data->viewers_ready,data->total_viewers  );
            if (data->viewers_ready == data->total_viewers) {
                memset(data->news, 0, MAX_NEWS_LENGTH);  // Clear news and comments for next round
                for (int i = 0; i < MAX_COMMENTS; i++)
                    memset(data->comments[i], 0, MAX_COMMENT_LENGTH);
                data->broadcast_done = 1;
                sem_post(mutex); // Release mutex before breaking the loop
                sem_post(empty);  
                break;
            }else{
             sem_post(mutex);
            }
          sem_wait(mutex); // Lock the mutex again to check updated data (if needed)
          if (data->viewers_ready != data->total_viewers) {
          sem_post(mutex);  // Release mutex and continue checking
          sleep(1); 
           }
          
      }
        
        sleep(1);  // Sleep briefly before starting the next input cycle
        
    }

    return 0;
}

