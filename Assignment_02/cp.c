#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //O_RDONLY, O_WRONLY, O_RDWR, open(), close()
#include <unistd.h>//read(), write()
#include <string.h>

void main(int argc, char **argv)
{
	if (argc<3)
		printf("Too few aguments\n");
	else if (argc>4)
		printf("Too many aguments\n");
	else if (argc == 4 && argv[3][0] != '-')
		printf("Invalid Option\n");
	else
	{

		char flag;
		//creating buffer
		char *buff = (char *) calloc(100, sizeof(char));
		int fd1 = open (argv[1], O_RDONLY);
		int r = read(fd1, buff, 100);
		
		if (r && argc == 4 && argv[3][1] == 'i')//-i
		{	
			printf("cp: overwrite '%s'? ", argv[2]);
			scanf(" %c", &flag);
		}
		if (argc==3 | !r | (argc == 4 && argv[3][1]=='f') | flag=='y' | flag=='Y')//Basic, -f
		{
			int fd2 = open (argv[2], O_WRONLY);
			
			if (fd1 > -1)
			{
				if (fd2 == -1)
				{
					if (creat(argv[2], O_RDWR) ==-1)
					{
						printf("Error occured when trying to create destination file\n");
						exit(0);
					}
					char cmd[32];
					int ret = 0;
					sprintf(cmd, "chmod 755 %s", argv[2]);
					//chmod 755 : Only owner can write, read and execute for everyone
					ret = system(cmd);
					if (ret == 0)
					{
						//printf("Permissions of file changed successfully\n");
						fd2 = open (argv[2], O_WRONLY);
					}
					else
						printf("Unable to change the permissions of file\n");
				}
				
				
				printf("read(%d, buff, 100) returns that %d bytes were read.\n", fd1, r);
				int w = write(fd2, buff, strlen(buff));
				printf("write(%d, buff, %ld) returns %d\n", fd2, strlen(buff), w);

				close(fd1);
				close(fd2);
				exit(0);
			}
		else
			printf("Error occured when trying to open source file\n");
		}
	}
	printf("Failed to Copy\n");
}
