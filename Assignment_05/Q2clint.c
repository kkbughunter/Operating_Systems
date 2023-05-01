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
	a = shmat(id,NULL,0); // Attach the process to the already created shared memory segment (shmat())
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
	shmdt(a); //shmat() function in C is used to attach a shared memory segment to the address space of a process
    // shmid: This is the identifier of the shared memory segment obtained from shmget().
    // shmaddr: This argument specifies the address at which the segment is to be attached. If it is set to NULL, the system chooses a suitable (unused) address to attach the segment. Otherwise, it specifies the address at which the segment is to be attached.
    // shmflg: This argument specifies the action to be taken when attaching the segment. It can be set to 0 for read-only access
	
    return 0;
}














