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

int main(int argc, int *argv[])
{
    int id;
    char *b;
    printf("\nServer Program:\n");
	id = shmget(111,2,0);
	b = shmat(id,NULL,0);

    FILE *fp;
    char buffer[1000];
    fp = fopen(b, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    printf("File name : %s\n",b);
    while (fgets(buffer, 100, fp)) {
        printf("%s", buffer);
    }
    fclose(fp);
    shmctl(id,IPC_RMID, NULL);
}














