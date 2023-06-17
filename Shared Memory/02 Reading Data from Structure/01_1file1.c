#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

struct datacenter{
  int access;
  char data[25];
};

int main(){
  key_t key = ftok("my shared memory", 1234);
  int shm_id = shmget(key, sizeof(struct datacenter), IPC_CREAT | 0666);
  struct datacenter *shm  = shmat(shm_id,NULL,0);
  
  char mydata[25];
  shm->access = 1;
  strcpy(shm->data, "data");
  
  
  shmdt(shm);
  return 0;

}
