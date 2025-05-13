#include "shared.h"

int main(){
	int fd = shm_open(SHM_NAME, O_RDWR, 0666);
    SharedData* data = mmap(0, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    while(1){
    system("clear");
    printf("News: %s\n", data->news);
    printf("Comments (total %d):\n", data->comment_count);
    for (int i = 0; i < data->comment_count; i++) {
        printf("  Comment %d: %s\n", i + 1, data->comments[i]);
    }
    printf("Viewers Ready: %d\n", data->viewers_ready);
    printf("Total Viewers: %d\n", data->total_viewers);
    printf("Broadcast Done: %d\n", data->broadcast_done);
    sleep(1);
    }
}
