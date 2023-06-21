//Server

#define NULL 0
#define SIZE 100

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio_ext.h>// parent writing a char in shared memory and child reads it and prints it.

int bye(char *buffer);

int main()
{
	int pid;
	char *a,*b,c, fname[SIZE];
	int id, fd, i;

	id = shmget(123,50,IPC_CREAT | 00666);

	char *buffer = shmat(id,NULL,0);
	buffer[0] = '\0';
	
	while (1){
		printf("\nServer : ");
		for (i = 1; !i || i && buffer[i-1]!='.'; i++) {
			scanf("%c", &buffer[i]);
		}
		buffer[0]='s';
		if (bye(buffer)) break;
	
		printf("Message from Client : ");
		while(buffer[0] != 'c');	
		
		for (i = 1; buffer[i]!='.'; i++) {
			if (buffer[i] != '\n') printf("%c", buffer[i]);
		}
		printf(".");
		if (bye(buffer)) break;
	}
	printf("\n");
	shmdt(buffer);
	shmctl(id, IPC_RMID, NULL);
}


int bye(char *buff)
{
	if (tolower(buff[1])=='b' && tolower(buff[2])=='y' && tolower(buff[3])=='e' && tolower(buff[4])=='.')
		return 1;
	else return 0;
}


