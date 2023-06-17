#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct shm{
  int access;
  char data[25];
};
int main() {
    struct shm *s;
    key_t key = ftok("shared_memory", 1234);
    int shm_id = shmget(key, sizeof(struct shm), IPC_CREAT | 0666);
    s = shmat(shm_id, NULL, 0);
    
    printf("Client: Start.\n");
    while(1){
      while (s->access == 0)  sleep(1);
      
      printf("Server: Received message: %s\n", s->data);
      scanf("%s",s->data);
      s->access = 0;
      }
      shmdt(s);

    printf("Client: Exiting.\n");
    return 0;
}

