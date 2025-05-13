#include "shared.h"
#include <signal.h>
#include <setjmp.h>

jmp_buf env;

void timeout_handler(int sig) {
    longjmp(env, 1);
}


SharedData* data;
sem_t *mutex, *empty, *full, *comment_notify;
int viewer_id;

void handle_refresh(int sig) {
    if (sig == SIGTSTP) {
        sem_wait(mutex);
        system("clear");
        printf("[Viewer %d]\n%s\n", viewer_id, data->news);
        printf("Live Comments:\n");
        for (int i = 0; i < data->comment_count; i++) {
            printf(" - %s\n", data->comments[i]);
        }
        sem_post(mutex);
        printf("\n(Continue typing your comment...)\n");
        fflush(stdout);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ViewerID>\n", argv[0]);
        return 1;
    }

    viewer_id = atoi(argv[1]);

    signal(SIGTSTP, handle_refresh);
    signal(SIGALRM, timeout_handler);

    int fd = shm_open(SHM_NAME, O_RDWR, 0666);
    data = mmap(0, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    mutex = sem_open(SEM_MUTEX, 0);
    empty = sem_open(SEM_EMPTY, 0);
    full = sem_open(SEM_FULL, 0);
    comment_notify = sem_open(SEM_COMMENT_NOTIFY, 0);

    sem_wait(mutex);
    data->total_viewers++;
    sem_post(mutex);


    while (1) {
        sem_wait(full);
        sem_post(full);

        char comment[MAX_COMMENT_LENGTH];
        while (1) {
            sem_wait(mutex);
            int done = data->broadcast_done;
            sem_post(mutex);

            // Display the latest news and comments
            system("clear");
            printf("[Viewer %d]\n%s\n", viewer_id, data->news);
            printf("Live Comments:\n");

            sem_wait(mutex);
            for (int i = 0; i < data->comment_count; i++)
                printf(" - %s\n", data->comments[i]);
            
            //printf("ready: %d,total:%d",data->viewers_ready,data->total_viewers  );
            sem_post(mutex);


            printf("Enter comment (0 to stop, press Ctrl+Z to refresh comments, auto-skipping after 20 seconds):\n> ");
            
            if (setjmp(env) == 0) {
                alarm(20); 
                fgets(comment, MAX_COMMENT_LENGTH - 10, stdin);
                comment[strcspn(comment, "\n")] = '\0';
                alarm(0);
            } else {
                strcpy(comment, "0");
                printf("\n[Viewer %d] No input for 20 seconds, auto-skipping.\n", viewer_id);
                sleep(1);
            }

            if (strcmp(comment, "0") == 0) {
                sem_wait(mutex);
                data->viewers_ready++;
                sem_post(comment_notify);
                sem_post(mutex);
                break;
            }

            sem_wait(mutex);
            if (data->comment_count < MAX_COMMENTS) {
                snprintf(data->comments[data->comment_count++], MAX_COMMENT_LENGTH,
                         "Viewer %d: %s", viewer_id, comment);
            }
            sem_post(mutex);
            sem_post(comment_notify);
        }

        sleep(1);
        while (1) {
            sem_wait(mutex);
            if (data->broadcast_done) {
                system("clear");
                sem_post(mutex);
                break;
            }
            sem_post(mutex);
            sleep(1);
        }
    }
    

    return 0;
}

