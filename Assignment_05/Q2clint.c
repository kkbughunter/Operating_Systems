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
#include <ctype.h>

int main(int argc, char *argv[])
{
	char *a,data[100];
	int id;
    FILE *fp;

    id = shmget(IPC_PRIVATE, 50, 00666);

	id = shmget(111,2,IPC_CREAT|00666);
	a = shmat(id,NULL,0); //Attach the process to the already created shared memory segment (shmat())
	// printf("\nParent Process start\n");
	printf("Enter a File name: ");
    scanf("%s",a);

    strcat(a, ".txt");
    fp = fopen(a,"a");

    strcpy(data," ");
    printf("Start Writing:\nTo stop the process use #\n");
    while (strcmp(data,"#"))
    {
        fwrite(data, sizeof(char), strlen(data), fp); 
        scanf("%s",data);
        fprintf(fp, "\n"); 
    }
    fclose(fp);
    
	wait(NULL);
	shmdt(a);
    return 0;
}














