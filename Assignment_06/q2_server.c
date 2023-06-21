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
#include <time.h>

#define MAX 100
#define SIZE 3
#define SHMPERM 0666

#define SEM_KEY_1 111
#define SEM_KEY_2 222
#define SEM_KEY_3 333


void *produce(sem_t *full, sem_t *empty, sem_t *mutex, int rand_arr[], int n)
{
	int id = shmget(5555, SIZE, IPC_CREAT | 0666), i = 1;
	char *buff = shmat(id, 0, 0);
	buff[0] = n;
	sleep(3);
	while(1)
	{
		sleep(1);
		//printf("%d %d\n", i, n);
		if(i == n)
		{
			printf("\nProducer %d exited \n",getpid());
			exit(1);
		}
		/*int sem_value;
		sem_getvalue(empty, &sem_value);
		printf("**empty cons semaphore value is: %d\n", sem_value);*/
		int sem_value;
		sem_getvalue(empty, &sem_value);
		if (sem_value == 0) printf("Producer waiting for empty\n");

		sem_wait(empty);
		printf("\nProducer %d acquired Semaphore Empty \n",getpid());
		sem_wait(mutex);
		printf("\nProducer %d acquired Semaphore Mutex \n",getpid());
		buff[i%SIZE] = rand_arr[i];
		printf("\nProducer %d Produced an Item [%d] \n", getpid(), buff[i%SIZE]);
		sem_post(mutex);
		printf("\nProducer %d released Semaphore Mutex \n",getpid());
		sem_post(full);
		printf("\nProducer %d released Semaphore Full \n",getpid());
		
		i++;
	}
	shmdt(buff);
	shmctl(id, IPC_RMID, 0);
}


int main()
{
	int i, n;	
	printf("Enter a number : ");
	scanf("%d", &n);
	
	int rand_arr[n];
	srand(time(NULL));

	// generate a rand_arrom number between 0 and MAX
	for(i=0; i<n; i++)
		rand_arr[i] = rand() % MAX;		


	sem_t *full = malloc(sizeof(sem_t)), *empty = malloc(sizeof(sem_t)), *mutex = malloc(sizeof(sem_t));
	key_t key = ftok(".", 12345);
	int empty_id = shmget(key,sizeof(sem_t)*3,IPC_CREAT|SHMPERM);
	//int full_id=shmget(2,sizeof(sem_t),IPC_CREAT|IPC_EXCL|SHMPERM);
	//int mutex_id=shmget(3,sizeof(sem_t),IPC_CREAT|IPC_EXCL|SHMPERM);
	empty = (sem_t *)shmat(empty_id,(char *)0,0);
	full = empty+1;//shmat(full_id,(char *)0,0);
	mutex = empty+2;//shmat(mutex_id,(char *)0,0);

	sem_init(full, 1, 0);
	sem_init(empty, 1, SIZE);
	sem_init(mutex, 1, 1);
	

	produce(full, empty, mutex, rand_arr, n);

	return 0;
}
