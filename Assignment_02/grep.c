#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	if (argc<3 || argc == 4 && argv[3][0] != '-')
		printf("Invalid Option\n");
	if (argc == 3)//Basic
	{
		char *buff = (char *) calloc(10000, sizeof(char));
		int fd = open (argv[2], O_RDONLY);
		int r = read(fd, buff, 10000);

		char* token = strtok(buff, "\n");

		while (token != NULL)
		{
			if (strstr(token, argv[1]) ) printf("%s\n", token);
			token = strtok(NULL, "\n");
		}
	}
	else if (argv[3][1] == 'v')//-v (inverse)
	{
		char *buff = (char *) calloc(1000, sizeof(char));
		int fd = open (argv[2], O_RDONLY);
		int r = read(fd, buff, 1000);

		char* token = strtok(buff, "\n");
		while (token != NULL)
		{
			if (!strstr(token, argv[1]) ) printf("%s\n", token);
			token = strtok(NULL, "\n");
		}
	}
	else if (argv[3][1] == 'n')//-n newline
	{
		char *buff = (char *) calloc(1000, sizeof(char));
		int fd = open (argv[2], O_RDONLY);
		int r = read(fd, buff, 1000), line_no=0;

		char* token = strtok(buff, "\n");
		while (token != NULL)
		{
			line_no++;
			if (strstr(token, argv[1]) ) printf("%d] %s\n", line_no, token);
			token = strtok(NULL, "\n");
		}
	}
	else if (argv[3][1] == 'c')//-c count
	{	
	
		char *buff = (char *) calloc(1000, sizeof(char));
		int fd = open (argv[2], O_RDONLY);
		int r = read(fd, buff, 1000), line_no=0;
		int l1=strlen(buff), l2 = strlen(argv[1]), count1=0;

	    	for (int i = 0; i < l1;)
		{
			int j = 0, count = 0;
			while (j<l2 && (buff[i] == argv[1][j]))
			{
				count++;
				i++;
				j++;
			}
			
			if (count == l2)
				count1++;
			else i++;
		}
		printf("%s occurs %d times\n", argv[1], count1);
	}
}
