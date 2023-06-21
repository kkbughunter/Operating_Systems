#include <stdio.h> //NULL, printf(), sprintf()
#include <dirent.h>//DIR, struct dirent, opendir(), readdir()
#include <stdlib.h>
#define BLUE    "\x1b[34m"
#define WHITE   "\x1b[37m"

void ls(char *dh, int rec);

int main(int argc, char *argv[])
{
	if (argc == 2 && argv[1][0] != '-')
		printf("Invalid Option\n");
	
	else if (argc == 1){
		ls(".", 0);
		printf("\n");
	}
	else if (argc==2 && argv[1][1]=='R')//-R (Recursive)
		ls(".", 1);
	else if (argc==2 && argv[1][1]=='1')//-1 (newline)
		ls(".", -1);
	return 0;
}


void ls(char *path, int rec)
{
	printf("\n\n%s :\n", path);
	struct dirent *d;
	DIR *directory = opendir(path);
	if (!directory)
		printf("Unable to open directory");
		
	//Current directory contents
	while ((d = readdir(directory)) != NULL)
	{
		if (d->d_name[0] == '.')//hidden files 
			continue;
		if (d->d_type == 4)
			printf(BLUE "%s  " WHITE, d->d_name);
		else printf("%s  ", d->d_name);
		if (rec == -1)	printf("\n");
	}
	closedir(directory);
	
	//Recursive calls of sub-directory
	directory = opendir(path);
	char newdir[512];
	if (rec == 1)
	{
		while ((d = readdir(directory)) != NULL)
		{
			if (d->d_name[0] == '.')//hidden files 
				continue;
			if (d->d_type == 4)//hidden files
			{
				//printf("%s\n", d->d_name);
				sprintf(newdir, "%s/%s", path, d->d_name);
				ls(newdir, 1);
			}
		}
	}
	closedir(directory);
}
