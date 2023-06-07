#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define BUFSIZE 3

struct memory {
  int buffer[BUFSIZE];
  int count;
  sem_t full;
  sem_t empty;
  sem_t mutex;
  int n;
  int nstatus;
};

struct memory *shmptr;

void *consumer(void *arg) {
  int c = 0;
  while (c < shmptr->n) {
    sem_wait(&(shmptr->full));
    sem_wait(&(shmptr->mutex));

    printf("Consumer consumed the number: %d\n",
           shmptr->buffer[shmptr->count - 1]);
    shmptr->count--;
    c++;

    sem_post(&(shmptr->mutex));
    sem_post(&(shmptr->empty));

    sleep(1);
  }

  printf("\n\tCONSUMER ON CONSUMING STAGE...\n\n");
  pthread_exit(NULL);
}

int main() {
  int shmid = shmget(111, BUFSIZE, IPC_CREAT | 0666);
  shmptr = (struct memory *)shmat(shmid, NULL, 0);

  pthread_t consumerThread;
  pthread_create(&consumerThread, NULL, consumer, NULL);

  while (shmptr->nstatus == 0) {
    // printf("Waiting for the producer to start...\n");
    sleep(1);
  }

  int c = 0;
  while (c < shmptr->n) {
    if ((c % BUFSIZE == 0) & (c < shmptr->n)) {
      printf("\n\nConsumer Waiting\n");
    }
    sem_wait(&(shmptr->full));
    sem_wait(&(shmptr->mutex));
    sleep(1);
    printf("Consumer consumed the number: %d\n",
           shmptr->buffer[shmptr->count - 1]);
    shmptr->count--;
    c++;

    sem_post(&(shmptr->mutex));
    sem_post(&(shmptr->empty));

    sleep(1);
  }

  printf("\nCONSUMER COMPLETE\n");

  shmdt(shmptr);
  shmctl(shmid, IPC_RMID, NULL);
  sem_destroy(&(shmptr->empty));
  sem_destroy(&(shmptr->full));
  sem_destroy(&(shmptr->mutex));

  exit(1);
}