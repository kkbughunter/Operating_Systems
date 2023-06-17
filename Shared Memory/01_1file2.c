#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

int main(){
  key_t key = ftok("my key", 1234);
  int shm_id = shmget(key, sizeof(int), IPC_CREAT | 0666);
  int *data  = shmat(shm_id,NULL,0);

  int mydata = *data;
  printf("%d\n",mydata);
  shmdt(data);
  shmctl(shm_id, IPC_RMID, NULL);
  return 0;

}
