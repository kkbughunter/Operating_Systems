#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
int main(int argc, char *argv[])
{    
	if(argc < 3){
		printf("Syntax: mycp [Option] <SOR> <DEST>\n\n");
		return 0;
	}
    else 
		copy_to(argv);
    
    return 0;
}

void copy_to(char *argv[])
{
	int d,f1,f2;
    char data[100];

	f1 = open(argv[1], O_RDONLY);
	f2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (f1 < 0 && f2 < 0){
        perror("Program");
		return 0;  
    }


    d = read(f1, data, 100);
	close(f1);
	write(f2, data, strlen(data));
	close(f2);

	printf("FILE WRITEN\n\n");
}
