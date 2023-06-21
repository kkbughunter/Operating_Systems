#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h> //sem_wait, sem_post
#include <sys/wait.h>
#include <pthread.h> // for semaphore operations sem_init,sem_wait,sem_post
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX 100
#define SIZE 3
#define N 5
#define SHMPERM (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

#define SEM_KEY_1 111
#define SEM_KEY_2 222
#define SEM_KEY_3 333


void *consume(sem_t *full, sem_t *empty, sem_t *mutex)
{
	int id = shmget(5555, SIZE, IPC_CREAT | 0666), i = 1;
	char *buff = shmat(id, 0, 0);
	int n = buff[0];
	sleep(2);
	while(1)
	{
		sleep(1);
		if (i == n)
		{
			printf("\nConsumer %d exited \n",getpid());
			exit(1);
		}	
			
		
		/*int sem_value;
		sem_getvalue(empty, &sem_value);
		printf("	**empty cons semaphore value is: %d\n", sem_value);*/
		int sem_value;
		sem_getvalue(full, &sem_value);
		if (sem_value == 0) printf("Consumer waiting for full\n");
	
		
		sem_wait(full);
		printf("\nConsumer %d acquired Semaphore Full \n",getpid());
		sem_wait(mutex);
		printf("\nConsumer %d acquired Semaphore Mutex\n",getpid());
		printf("\nConsumer %d Consumed an Item [%d] \n",getpid(),buff[i%SIZE]);
		buff[i%SIZE] = ' ';
		sem_post(mutex);
		printf("\nConsumer %d released Semaphore Mutex \n",getpid());
		sem_post(empty);
		printf("\nConsumer %d released Semaphore Empty \n",getpid());
		i++;
	}
	shmdt(buff);
	shmctl(id, IPC_RMID, 0);
}


int main()
{
	sem_t *full = malloc(sizeof(sem_t)), *empty = malloc(sizeof(sem_t)), *mutex = malloc(sizeof(sem_t));
	key_t key = ftok(".", 12345);
	int empty_id = shmget(key,sizeof(sem_t)*3,IPC_CREAT|SHMPERM);
	//int full_id=shmget(2,sizeof(sem_t),IPC_CREAT|IPC_EXCL|SHMPERM);
	//int mutex_id=shmget(3,sizeof(sem_t),IPC_CREAT|IPC_EXCL|SHMPERM);
	empty = (sem_t *)shmat(empty_id,(char *)0,0);
	full = empty+1;//shmat(full_id,(char *)0,0);
	mutex = empty+2;//shmat(mutex_id,(char *)0,0);
	
	consume(full, empty, mutex);
	sem_destroy(full);
	sem_destroy(empty);
	sem_destroy(mutex);

	return 0;
}
