#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct Block
{
    int blockNo;        // name of the block.
    int *data;          // data stored in the block.
    struct Block *next; // pointer to the next block.
    int status;
};

struct MainMemory
{
    int mainMemoryId;              // name of the main memory
    int numOfBlock;                // number of block in the main memory.
    int size;                      // size of the block .
    struct Block *startingAddress; // starting address of the block.
    struct MainMemory *next;       // pointer to the next main memory.
};

struct LinkedAllolation
{
    char **fileName;               // name of the file.
    int *length;                   // length of the file.
    struct Block ***startingBlock; // starting address of the file.
    struct Block **endingBlock;    // End of the block
    int **starting;                // starting address of the file. (in humar understand)
    int *ending;                   // End of the block. (in humar understand)
};

struct Block *createNewBlock(struct Block *starting_block, int blockId, int blocksize)
{
    struct Block *b = (struct Block *)malloc(sizeof(struct Block));
    if (starting_block == NULL)
        b->next = NULL;
    else
        b->next = starting_block;

    b->data = (int *)malloc(blocksize * sizeof(int));
    b->blockNo = blockId;
    b->status = 0;
    return b;
}

struct Block *createBlocks(int no_of_block, int size_of_block)
{

    struct Block *b = NULL;
    int i;
    for (i = no_of_block - 1; i >= 0; i--)
    {
        b = createNewBlock(b, i, size_of_block);
    }
    // // printing the created blog id's.
    // struct Block *temp = b;
    // for (i=0; i<no_of_block; i++){
    //     printf("%d  =>", temp->blockNo);
    //     temp = temp->next;
    // }
    return b;
}

struct MainMemory *createMainMomery_WithBlocks(int size_of_mainmemory, int no_of_block, int id)
{
    struct MainMemory *mm = (struct MainMemory *)malloc(sizeof(struct MainMemory));
    mm->next = NULL;
    mm->numOfBlock = no_of_block;
    mm->size = (int)round(size_of_mainmemory / no_of_block);
    mm->mainMemoryId = id;

    mm->startingAddress = createBlocks(mm->numOfBlock, mm->size);

    // printing the main memory
    printf("\n   Main Memory ID: %d\n\n", mm->mainMemoryId);
    int i;
    struct Block *temp = mm->startingAddress;
    for (i = 1; i < no_of_block + 1; i++)
    {
        printf("%4d", temp->blockNo);
        temp = temp->next;
        if (i % 5 == 0)
            printf("\n");
    }
    printf("\n");

    return mm;
}

struct LinkedAllolation *getFileDetails(int no_of_files)
{
    struct LinkedAllolation *la = (struct LinkedAllolation *)malloc(sizeof(struct LinkedAllolation));
    int i;

    la->fileName = malloc(no_of_files * sizeof(char *));
    la->length = malloc(no_of_files * sizeof(int));
    la->startingBlock = malloc(no_of_files * sizeof(struct Block *));
    la->endingBlock = malloc(no_of_files * sizeof(struct Block *));
    la->starting = malloc(no_of_files * sizeof(int *));
    la->ending = malloc(no_of_files * sizeof(int));

    for (i = 0; i < no_of_files; i++)
    {
        la->fileName[i] = (char *)malloc(10 * sizeof(char));
        printf("\nEnter the File Name: ");
        scanf("%s", la->fileName[i]);
        printf("Enter the File Size: ");
        scanf("%d", &la->length[i]);
    }

    for (i = 0; i < no_of_files; i++)
    {
        la->starting[i] = malloc(la->length[i] * sizeof(int));
        la->startingBlock[i] = malloc(la->length[i] * sizeof(struct Block));
    }

    // print file details....
    // for (i = 0; i < no_of_files; i++)
    // {
    //     printf("\nEnter the File Name: %s\n", la->fileName[i]);
    //     printf("Enter the File Size: ");
    //     printf("%d\n", la->length[i]);
    // }
    return la;
}

void LinkedAllolation(struct MainMemory *mm, struct LinkedAllolation *la, int files)
{
    int q = 0;
    for (q = 0; q < files; q++)
    {
        int i, j, file = q;
        int num, flag = 0, final_flag = 0, count = 0;
        num = rand() % (mm->numOfBlock - 0) + 0;
        // printf("%d debug",num);

        // Set the seed value using the current time
        struct Block *temp = mm->startingAddress;
        for (i = 0; i < num; i++)
            temp = temp->next; // move in main memory.
        // printf("Debug %d \n", num);

        int adj = 0;
        if (la->length[file] % 2 == 0)
            adj = la->length[file] + 1;
        int trv = la->length[file] / mm->size; // finding how many move to save the data.
        do
        {
            // printf("Try to find..\n");
            int check_size = 0;
            flag = 1;
            // printf("%d trv debug\n", trv);
            i = 0;
            do
            {
                // move in main memory and check the lolation if free or not for the file size.
                if (temp->status == 0)
                {
                    check_size += mm->size;
                    i++;
                }
                // printf("\n check_size in loop %d", check_size);
                if (temp->next == NULL)
                {
                    // printf("List out of range");
                    final_flag = 1;
                    break;
                }
                else
                    temp = temp->next;
            } while (trv > i);

            if (check_size >= la->length[file])
            {
                flag = 1;
                // printf("success");
            }
            else
                count++;

            if (count == 3)
            {
                final_flag = 1;
                flag = 1;
            }
        } while (flag == 0);

        printf("\nRandomly selected place in Main Memory is : %d\n", num);
        // la->startingBlock[file] = num;
        if (final_flag == 0 && flag == 1)
        {
            // printf("debug %d", num);
            printf("Block found successfuly... for %s\n", la->fileName[file]);

            temp = mm->startingAddress;
            for (i = 0; i < num; i++)
                temp = temp->next; // move in main memory.
            // printf("%d status\n", temp->status);

            la->startingBlock[q][0] = temp;
            la->starting[q][0] = temp->blockNo;
            i = 0;
            do
            {
                // move in main memory and check the location if free or not for the file size.
                if (temp->status == 0)
                {
                    la->startingBlock[q][i] = temp;
                    la->starting[q][i] = temp->blockNo;
                    temp->status = 1;
                    for (j = 0; j < mm->size; j++)
                    {
                        temp->data[j] = rand() % (2343 - 99) + 99;
                        ;
                        // printf("%d ", temp->data[j]);
                    }
                    i++;
                }
                else
                    temp = temp->next;
            } while (trv > i);

            // Note the ending address and the block ID.
            la->endingBlock[file] = temp;
            la->ending[file] = temp->blockNo;
        }
        else
        {
            la->startingBlock[file] = NULL;
            printf("\nBlock Not found...\n");
        }
    }
}

void printFileData(struct MainMemory *mm, struct LinkedAllolation *la, int file)
{
    int i = 0, j = 0;
    int trv = la->length[file] / mm->size; // finding how many move to save the data.

    struct Block *temp = la->startingBlock[file][0];
    printf("Data : ");
    for (i = 0; i < trv; i++)
    {
        for (j = 0; j < mm->size; j++)
        {
            printf("%d ", temp->data[j]);
        }
        temp = temp->next;
    }
}

void printStatus(struct LinkedAllolation *la, int no_of_files)
{
    // printf("debug\n");
    int i = 0;
    printf(".------------------------Directory-----------------------.\n");
    printf("|%-20s|%15s|%7s|%7s  |\n", "fileName", "starting Block", "End Block", "Length");
    printf("|--------------------------------------------------------|\n");
    for (i = 0; i < no_of_files; i++)
    {
        // if (la->starting[i][0] == la->ending[i])
        //     printf("|  %-18s|%13s  |%7s  |%7d  |\n", la->fileName[i], "(null)", "(null)", la->length[i]);
        // else
        printf("|  %-18s|%13d  |%7d  |%7d  |\n", la->fileName[i], la->starting[i][0], la->ending[i], la->length[i]);
    }
    printf("`--------------------------------------------------------`\n");
}

int main()
{
    int i, j, k;

    // create a main memory of size X and Number fo block Y
    int x = 32; //
    printf("Enter the Main Memory Size: ");
    scanf("%d", &x);
    int y = 16; //
    printf("Enter the Number of Blocks: ");
    scanf("%d", &y);
    struct MainMemory *mm = createMainMomery_WithBlocks(x, y, 0);
    printf("Number of Block in the Main Memory: %d\n", mm->numOfBlock);
    printf("Size of each Block in the Main Memory: %d\n", mm->size);

    // Get File data.
    int m = 1;
    printf("Enter the No.of.Files: ");
    scanf("%d", &m);
    struct LinkedAllolation *la = getFileDetails(m);

    // Allolate a file.
    LinkedAllolation(mm, la, m);

    printStatus(la, m);

    while (1)
    {
        printf("Enter the file that you want to print the data: ");
        scanf("%d", &k);
        printFileData(mm, la, k - 1);
    }

    return 0;
}

/*

30
20

2

f1
5

f2
10


*/