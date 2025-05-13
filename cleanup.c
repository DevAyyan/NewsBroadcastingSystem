#include "shared.h"

void main() {
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_MUTEX);
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    sem_unlink(SEM_COMMENT_NOTIFY);
    printf("[Cleanup] Shared memory and semaphores unlinked.\n");
    exit(0);
}
