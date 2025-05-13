#include "shared.h"

int main() {
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(SharedData));
    SharedData* data = mmap(0, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    memset(data, 0, sizeof(SharedData));

    sem_open(SEM_MUTEX, O_CREAT, 0666, 1);
    sem_open(SEM_EMPTY, O_CREAT, 0666, 1);
    sem_open(SEM_FULL, O_CREAT, 0666, 0);
    sem_open(SEM_COMMENT_NOTIFY, O_CREAT, 0666, 0);
    
    printf("[Init] Shared memory and semaphores initialized.\n");
    return 0;
}
