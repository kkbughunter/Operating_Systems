#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct SubDirectory
{
    int dirLocation;
    char fileName[30];         // Name of the file.
    struct SubDirectory *next; // pointer to the next directory.
};

struct Directory
{
    int limit;
    int nooffiles;
    int dirLocation;           // location of the directory.
    char fileName[30];         // Name of the file.
    struct SubDirectory *snext; // pointer to the first sub-directory.
    struct Directory *next;    // pointer to the next directory.
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
    // Add a new directory.
    strcpy(d->fileName, filename);
    d->dirLocation = (rand() % (99999 - 99) - 99);
    d->snext = NULL;
    d->next = NULL;
    d->nooffiles = 0;
    d->limit = 3;

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
        printf("No Files Found..\n");
        return;
    }

    struct Directory *temp = sld->dir;
    printf("Contents of the Root Directory\n\n"
           ".----------------------------------------------------------.\n"
           "|%*s%s%*s|%*s%s%*s|%*s%s%*s|\n",
           5, "", "Folder Name ", 5, "", 5, "", "File Name ", 5, "", 3, "", "Location", 3, "");
    printf("|----------------------------------------------------------|\n");
    while (temp != NULL)
    {
        printf("|  %-18s|%-22s|%12d  |\n", temp->fileName, "-", temp->dirLocation);
        struct SubDirectory *subTemp = temp->snext;

        while (subTemp != NULL)
        {
            printf("|  %-18s|%-22s|%12d  |\n", "-", subTemp->fileName, subTemp->dirLocation);
            subTemp = subTemp->next;
        }
        temp = temp->next;
    }

    printf("`----------------------------------------------------------`\n\n");
}

void CreateSubFile(struct SingleLevelDirectory *sld, char folder[30], char filename[30])
{
    // Find the directory
    struct Directory *temp = sld->dir;
    while (temp != NULL)
    {
        if (strcmp(temp->fileName, folder) == 0)
        {
            // Check if the file limit has been reached
            if (temp->nooffiles < temp->limit)
            {
                printf("Folder '%s' Found.\n", folder);
                srand(time(0));
                struct SubDirectory *subFile = (struct SubDirectory *)malloc(sizeof(struct SubDirectory));
                // Add a new sub-file.
                strcpy(subFile->fileName, filename);
                subFile->dirLocation = (rand() % (99999 - 99) - 99);
                subFile->next = NULL;

                if (temp->snext == NULL)
                {
                    temp->snext = subFile;
                }
                else
                {
                    struct SubDirectory *subTemp = temp->snext;
                    while (subTemp->next != NULL)
                        subTemp = subTemp->next;

                    subTemp->next = subFile;
                }

                temp->nooffiles++;
                printf("File '%s' added to folder '%s'.\n", filename, folder);
                return;
            }
            else
            {
                printf("Folder '%s' has reached its file limit.\n", folder);
                return;
            }
        }

        temp = temp->next;
    }

    printf("Folder '%s' not found.\n", folder);
}

int main()
{
    struct SingleLevelDirectory *sld = CreateSingleLevelDirectory(20);
    int flag = 0, opt = 0;
    char folder[30];
    char filename[30];
    printf("\t\tSingle Level File Directory\n1. Create Folder\n2. Add File\n3. Display File\n4. Exit\n\n");
    do
    {
        printf("Enter your Option: ");
        scanf("%d", &opt);
        switch (opt)
        {
        case 1:
            printf("\nEnter the New Folder Name: ");
            scanf("%s", folder);
            CreateDirectory(sld, folder);
            break;
        case 2:
            printf("\nEnter the Folder Name: ");
            scanf("%s", folder);
            printf("\nEnter the New File Name: ");
            scanf("%s", filename);
            CreateSubFile(sld, folder, filename);
            break;
        case 3:
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
folder-3
3
2
folder-1
1-file-1
2
folder-1
1-file-2
3


*/