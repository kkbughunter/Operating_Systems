#include <sys/ipc.h>
#define NULL 0
#include <sys/shm.h>
#include <sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/wait.h>
#include <stdio_ext.h>// parent writing a char in shared memory and child reads it and prints it.

void strUpper(char *s);

int main()
{
	int pid;
	char *a,*b,c;
	int id,n,i;

	id=shmget(111,50,IPC_CREAT | 00666); //int shmget(key_t key, size_t size, int shmflg);
	pid=fork();
	if(pid>0) //parent
	{
		//id=shmget(111,50,IPC_CREAT | 00666); 
		a=shmat(id,NULL,0); //void  *shmat(int shmid, const void *shmaddr, int shmflg);
		printf("Enter a string : ");
		scanf(" %s", a);
		wait(NULL);
		shmdt(a); //int shmdt(const void *shmaddr);
	}
	else //child
	{
		sleep(3);
		//id=shmget(111,50,0);
		b=shmat(id,NULL,0);

		strUpper(b);

		printf("\nUppercase : %s\n",b);
		shmdt(b);
	}
	shmctl(id, IPC_RMID,NULL); //int shmctl(int shmid, int cmd, struct shmid_ds *buf);

	/*IPC_RMID
	Remove the shared memory identified specified by shmid from the system
	and destroy the shared memory segment and shmid_ds data structure associated with shmid.
	When IPC_RMID is processed, no further attaches will be allowed.*/
}


void strUpper(char *s)
{
	for (int i = 0; s[i]!='\0'; i++) {
		if(s[i] >= 'a' && s[i] <= 'z') {
			s[i] -= 32;
		}
	}
}
