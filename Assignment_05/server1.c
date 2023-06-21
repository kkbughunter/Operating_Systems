//Server

#define NULL 0
#define SIZE 100

#include <fcntl.h>
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
	int id, i;

	id = shmget(222,100,IPC_CREAT | 00666);

	char *buffer = shmat(id,NULL,0);
	while (buffer[0] != '-');
	
	printf("Filename received from client : ");
	for (i = 1; buffer[i]!='\0'; i++) {
		printf("%c", buffer[i]);
		fname[i-1] = buffer[i];
	}
	fname[i-1] = '\0';

	char *file_content = (char *) calloc(100, sizeof(char));
	int fd = open (fname, O_RDONLY);

	if (fd < 0)
	{
		printf("Error in opening file\n");
		exit(1);
	}

	int r = read(fd, file_content, 100);	
	file_content[r] = '\0';
	printf("\nContents of file written in shared memory\n");
	for (i = 0; file_content[i]!='\0'; i++) {
		buffer[i] = file_content[i];
	}
	buffer[i] = '\0';

	close(fd);
	shmdt(buffer);
	shmctl(id, IPC_RMID, NULL);
}

