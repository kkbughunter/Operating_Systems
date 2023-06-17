#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>

int main(){
  key_t key = ftok("My Key",1234);
  int shm_id = shmget(key, sizeof(int), IPC_CREAT | 0666);
  int *data = shmat(shm_id, NULL, 0);
  *data = 23;
  shmdt(data);
  return 0; 
}
