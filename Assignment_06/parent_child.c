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
#define SHMPERM (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

#define SEM_KEY_1 111
#define SEM_KEY_2 222
#define SEM_KEY_3 333


void *produce(sem_t *full, sem_t *empty, sem_t *mutex, char str[], int n)
{
	int id = shmget(5555, SIZE, IPC_CREAT | 0666), i = 0;
	char *buff = shmat(id, 0, 0);
	while(1)
	{
		sleep(1);
		if(i == n)
		{
			printf("\nProducer %d exited \n",getpid());
			exit(1);
		}
		/*int sem_value;
		sem_getvalue(empty, &sem_value);
		printf("**empty cons semaphore value is: %d\n", sem_value);*/
		sem_wait(empty);
		printf("\nProducer %d acquired Semaphore Empty \n",getpid());
		sem_wait(mutex);
		printf("\nProducer %d acquired Semaphore Mutex \n",getpid());
		buff[i%SIZE] = str[i];
		printf("\nProducer %d Produced an Item [%c] \n", getpid(), buff[i%SIZE]);
		sem_post(mutex);
		printf("\nProducer %d released Semaphore Mutex \n",getpid());
		sem_post(full);
		printf("\nProducer %d released Semaphore Full \n",getpid());
		
		i++;
	}
	shmdt(buff);
	shmctl(id, IPC_RMID, 0);
}


void *consume(sem_t *full, sem_t *empty, sem_t *mutex, int n)
{
	int id = shmget(5555, SIZE, IPC_CREAT | 0666), i = 0;
	char *buff = shmat(id, 0, 0);
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
		
		sem_wait(full);
		printf("\n	Consumer %d acquired Semaphore Full \n",getpid());
		sem_wait(mutex);
		printf("\n	Consumer %d acquired Semaphore Mutex\n",getpid());
		printf("\n	Consumer %d Consumed an Item [%c] \n",getpid(),buff[i%SIZE]);
		buff[i%SIZE] = ' ';
		sem_post(mutex);
		printf("\n	Consumer %d released Semaphore Mutex \n",getpid());
		sem_post(empty);
		printf("\n	Consumer %d released Semaphore Empty \n",getpid());
		i++;
	}
	shmdt(buff);
	shmctl(id, IPC_RMID, 0);
}


int main()
{
	pid_t curr_id;
	int i;
	char str[MAX];
	
	printf("Enter a string : ");
	for(i=0; !i || str[i-1]!='\n'; i++)
		scanf("%c", &str[i]);
	str[i-1] = '\0';
	int n = i-1;

	sem_t *full, *empty, *mutex;
	int empty_id=shmget(IPC_PRIVATE,sizeof(sem_t),IPC_CREAT|IPC_EXCL| SHMPERM);
	int full_id=shmget(IPC_PRIVATE,sizeof(sem_t),IPC_CREAT|IPC_EXCL|SHMPERM);
	int mutex_id=shmget(IPC_PRIVATE,sizeof(sem_t),IPC_CREAT|IPC_EXCL|SHMPERM);
	empty = shmat(empty_id,(char *)0,0);
	full = shmat(full_id,(char *)0,0);
	mutex = shmat(mutex_id,(char *)0,0);


	sem_init(full, 1, 0);
	sem_init(empty, 1, SIZE);
	sem_init(mutex, 1, 1);

	curr_id = fork();
	if (curr_id > 0)
	{
		produce(full, empty, mutex, str, n);
	}
	else
	{
		consume(full, empty, mutex, n);
		sem_destroy(full);
		sem_destroy(empty);
		sem_destroy(mutex);
	}
	return 0;
}
