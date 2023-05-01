#include <sys/ipc.h>
# define NULL 0
#include <sys/shm.h>
#include <sys/types.h>
# include<unistd.h>
# include<stdio.h>
# include<stdlib.h>
# include<string.h>
#include <sys/wait.h>
#include <stdio_ext.h>
#include <ctype.h> // required for toupper function

void uppercase(char *str){
	// printf("\nFunction call start\n");
	int i=0;
	while (str[i]) {
        // Convert the character to uppercase
        str[i] = toupper(str[i]);
        i++;
    }
	// printf("\nFunction call end");
}

int main(int argc, char const *argv[])
{
	int pid;
	char *a,*b,c;
	int id,n,i;
	// shared memory get -> shmget()
	id = shmget(111,50,IPC_CREAT | 00666); //Create the shared memory segment or use an already created shared memory segment (shmget())

	pid = fork();

	if(pid > 0) // parent
	{
		printf("\nParent Process:\n");
		id = shmget(111,2,IPC_CREAT|00666);
		a = shmat(id,NULL,0); //Attach the process to the already created shared memory segment (shmat())
		// printf("\nParent Process start\n");
		printf("Enter a name to convert into uppercase: ");
		scanf("%s",a);
		wait(NULL);
		shmdt(a);
	}
	else // child
	{
		sleep(3);
		printf("\nChild Process:");
		id = shmget(111,2,0);
		b = shmat(id,NULL,0);
		uppercase(b);
		printf("\nName in Uppercase : %s\n", b);
		shmdt(b);
	}

	shmctl(id,IPC_RMID, NULL);
	return 0;
}

// KKBUGHUNTER
