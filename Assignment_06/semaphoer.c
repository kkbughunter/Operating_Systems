// final 1
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 3
#define INPUT_SIZE 20

typedef struct {
  char buffer[BUFFER_SIZE];
  sem_t empty;
  sem_t full;
  sem_t mutex;
  int in;
  int out;
} SharedData;

void *producer(void *arg) {
  SharedData *sharedData = (SharedData *)arg;
  char input[INPUT_SIZE];

  printf("Enter the input string : ");
  fgets(input, sizeof(input), stdin);
  input[strcspn(input, "\n")] = '\0';

  int len = strlen(input);
  for (int i = 0; i < len; i++) {
    sem_wait(&sharedData->empty);
    printf("Producer acquired Semaphore Empty\n");

    sem_wait(&sharedData->mutex);
    printf("Producer acquired Semaphore Mutex\n");

    // Critical section: Add item to buffer
    sharedData->buffer[sharedData->in] = input[i];
    sharedData->in = (sharedData->in + 1) % BUFFER_SIZE;

    sem_post(&sharedData->mutex);
    printf("Producer released Semaphore Mutex\n");

    sem_post(&sharedData->full);
    printf("Producer released Semaphore Full\n");

    printf("Producer produced the item : %c\n", input[i]);
  }

  sem_wait(&sharedData->empty);
  printf("Producer acquired Semaphore Empty\n");
  sem_wait(&sharedData->mutex);
  printf("Producer acquired Semaphore Mutex\n");

  // Critical section: Add null terminator to mark end of input
  sharedData->buffer[sharedData->in] = '\0';
  sharedData->in = (sharedData->in + 1) % BUFFER_SIZE;

  sem_post(&sharedData->mutex);
  printf("Producer released Semaphore Mutex\n");

  sem_post(&sharedData->full);
  printf("Producer released Semaphore Full\n");

  pthread_exit(NULL);
}

void *consumer(void *arg) {
  SharedData *sharedData = (SharedData *)arg;

  while (1) {
    sem_wait(&sharedData->full);
    printf("Consumer acquired Semaphore Full\n");

    sem_wait(&sharedData->mutex);
    printf("Consumer acquired Semaphore Mutex\n");

    // Critical section: Remove item from buffer
    char item = sharedData->buffer[sharedData->out];
    sharedData->out = (sharedData->out + 1) % BUFFER_SIZE;

    sem_post(&sharedData->mutex);
    printf("Consumer released Semaphore Mutex\n");

    sem_post(&sharedData->empty);
    printf("Consumer released Semaphore Empty\n");

    if (item != '\0') {
      printf("Consumer consumed item: %c\n", item);
    } else {
      printf("Consumer exited :)\n");
      break;
    }
  }
  pthread_exit(NULL);
}

int main() {
  int shmid;
  key_t key = IPC_PRIVATE;
  size_t shmSize = sizeof(SharedData);

  // Create shared memory segment
  if ((shmid = shmget(key, shmSize, IPC_CREAT | 0666)) == -1) {
    perror("shmget");
    exit(1);
  }

  char choice;
  do {
    // Attach shared memory segment
    SharedData *sharedData = (SharedData *)shmat(shmid, NULL, 0);
    if (sharedData == (void *)-1) {
      perror("shmat");
      exit(1);
    }

    // Initialize shared data
    memset(sharedData->buffer, '\0', sizeof(sharedData->buffer));
    sem_init(&sharedData->empty, 1, BUFFER_SIZE);
    sem_init(&sharedData->full, 1, 0);
    sem_init(&sharedData->mutex, 1, 1);
    sharedData->in = 0;
    sharedData->out = 0;

    pthread_t producerThread, consumerThread;

    // Create producer thread
    if (pthread_create(&producerThread, NULL, producer, (void *)sharedData) !=
        0) {
      perror("pthread_create");
      exit(1);
    }

    // Create consumer thread
    if (pthread_create(&consumerThread, NULL, consumer, (void *)sharedData) !=
        0) {
      perror("pthread_create");
      exit(1);
    }

    // Wait for producer thread to finish
    if (pthread_join(producerThread, NULL) != 0) {
      perror("pthread_join");
      exit(1);
    }

    // Wait for consumer thread to finish
    if (pthread_join(consumerThread, NULL) != 0) {
      perror("pthread_join");
      exit(1);
    }

    // Detach shared memory segment
    if (shmdt(sharedData) == -1) {
      perror("shmdt");
      exit(1);
    }

    // Menu-driven loop
    printf("\nDo you want to enter another input? (Y/N): ");
    scanf(" %c", &choice);
    while (getchar() != '\n')
      ; // Clear input buffer

    if (choice == 'Y' || choice == 'y') {
      // Remove shared memory segment
      if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
      }

      // Create new shared memory segment
      if ((shmid = shmget(key, shmSize, IPC_CREAT | 0666)) == -1) {
        perror("shmget");
        exit(1);
      }
    }
  } while (choice == 'Y' || choice == 'y');

  // Remove the last shared memory segment
  if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    perror("shmctl");
    exit(1);
  }

  return 0;
}