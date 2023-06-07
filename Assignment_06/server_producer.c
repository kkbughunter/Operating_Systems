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

void *producer(void *arg) {
  int i = shmptr->n;
  while (i > 0) {
    sem_wait(&(shmptr->empty));
    sem_wait(&(shmptr->mutex));

    shmptr->buffer[shmptr->count++] = rand() % 100;
    printf("Producer produced the number: %d\n",
           shmptr->buffer[shmptr->count - 1]);
    i--;

    sem_post(&(shmptr->mutex));
    sem_post(&(shmptr->full));

    sleep(1);
  }

  printf("\nPRODUCER COMPLETE\n");
  pthread_exit(NULL);
}

int main() {
  srand(time(0));
  int shmid = shmget(111, BUFSIZE, IPC_CREAT | 0666);
  shmptr = (struct memory *)shmat(shmid, NULL, 0);
  sem_init(&(shmptr->full), 1, 0);
  sem_init(&(shmptr->empty), 1, BUFSIZE);
  sem_init(&(shmptr->mutex), 1, 1);
  shmptr->nstatus = 0;

  pthread_t producerThread;

  printf("\n\t\tPRODUCER CONSUMER PROBLEM\n\n");

  while (shmptr->nstatus == 0) {
    printf("Number of terms: ");
    scanf("%d", &(shmptr->n));
    shmptr->nstatus = 1;
  }

  pthread_create(&producerThread, NULL, producer, NULL);

  pthread_join(producerThread, NULL);

  shmdt(shmptr);
  shmctl(shmid, IPC_RMID, NULL);
  sem_destroy(&(shmptr->empty));
  sem_destroy(&(shmptr->full));
  sem_destroy(&(shmptr->mutex));

  exit(1);
}