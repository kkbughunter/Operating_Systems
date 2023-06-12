#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct Directory
{
    int dirLocation;        // location of the directory.
    char fileName[30];      // Name of the file.
    struct Directory *next; // pointer to the next directory.
};

struct SingleLevelDirectory
{
    int dirSize;           // size of the directory.
    struct Directory *dir; // pointer to the first directory.
};

void CreateDirectory(struct SingleLevelDirectory *sld, char filename[30])
{
    // Check if the file name already exists
    struct Directory *temp = sld->dir;
    while (temp != NULL)
    {
        if (strcmp(temp->fileName, filename) == 0)
        {
            printf("File name '%s' already exists.\n", filename);
            return;
        }
        temp = temp->next;
    }

    srand(time(0));
    struct Directory *d = (struct Directory *)malloc(sizeof(struct Directory));
    // Add a new file.
    strcpy(d->fileName, filename);
    d->dirLocation = (rand() % (99999 - 99) - 99);
    d->next = NULL;

    if (sld->dir == NULL)
    {
        sld->dir = d;
        return;
    }

    temp = sld->dir;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = d;
}


struct SingleLevelDirectory *CreateSingleLevelDirectory(int size)
{
    struct SingleLevelDirectory *sld = (struct SingleLevelDirectory *)malloc(sizeof(struct SingleLevelDirectory));
    sld->dirSize = size;
    sld->dir = NULL;
    return sld;
}

void DisplayDirectory(struct SingleLevelDirectory *sld)
{
    if (sld->dir == NULL)
    {
        printf("No File Found..\n");
        return;
    }

    struct Directory *temp = sld->dir;
    printf("Contents of root directory\n\n.----------------------------------.\n|%*s%s%*s|%*s%s%*s|\n", 5, "", "File Name ", 5, "", 3, "", "Location", 3, "");

    printf("|-----------------------------------|\n");
    while (temp != NULL)
    {
        printf("|  %-18s|%12d  |\n", temp->fileName, temp->dirLocation);
        temp = temp->next;
    }
    printf("`----------------------------------`\n\n");
}

int main()
{
    struct SingleLevelDirectory *sld = CreateSingleLevelDirectory(20);
    int flag = 0, opt = 0;
    char filename[30];
    printf("\t\tSingle Level File Directory\n1. Create File\n2. Display File\n3. Exit\n\n");
    do
    {
        printf("Enter your Option: ");
        scanf("%d", &opt);
        switch (opt)
        {
        case 1:
            printf("\nEnter the file Name: ");
            scanf("%s", filename);
            CreateDirectory(sld, filename);
            break;
        case 2:
            DisplayDirectory(sld);
            break;
        default:
            flag = 1;
            break;
        }
    } while (flag != 1);

    return 0;
}

/*
1
folder-1
1
folder-2
1
folder-1
1
folder-3
2


*/