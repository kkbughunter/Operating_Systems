//Client

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

int main(int argc, char *argv[])
{
	int pid;
	char *a,*b,c, fname[SIZE];
	int id, fd, i, cnt;

	id = shmget(222,100,IPC_CREAT | 00666);

	char *buffer = shmat(id,NULL,0); //NULL- no specific address for attaching, 0-both r/w access
	buffer[0]='-';
	
	for (cnt = 0; argv[1][cnt]!='\0'; cnt++)
		buffer[cnt+1] = argv[1][cnt];

	while (buffer[0] == '-');

	cnt=0;
	for (i = 0; buffer[i]!='\0'; i++) {
		cnt++;
	}

	fd = open("dest.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		creat("dest.txt", O_RDWR);
	}
 
	int w = write(fd, buffer, cnt);
	printf("\nWritten the contents of shared memory into dest.txt\n");
	close(fd);

	shmdt(buffer);
	shmctl(id, IPC_RMID, NULL);
}

