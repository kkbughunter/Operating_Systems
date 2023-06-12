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

struct IndexedAllocation
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

struct IndexedAllocation *getFileDetails(int no_of_files)
{
    struct IndexedAllocation *ia = (struct IndexedAllocation *)malloc(sizeof(struct IndexedAllocation));
    int i;

    ia->fileName = malloc(no_of_files * sizeof(char *));
    ia->length = malloc(no_of_files * sizeof(int));
    ia->startingBlock = malloc(no_of_files * sizeof(struct Block *));
    ia->endingBlock = malloc(no_of_files * sizeof(struct Block *));
    ia->starting = malloc(no_of_files * sizeof(int *));
    ia->ending = malloc(no_of_files * sizeof(int));

    for (i = 0; i < no_of_files; i++)
    {
        ia->fileName[i] = (char *)malloc(10 * sizeof(char));
        printf("\nEnter the File Name: ");
        scanf("%s", ia->fileName[i]);
        printf("Enter the File Size: ");
        scanf("%d", &ia->length[i]);
    }

    for (i = 0; i < no_of_files; i++)
    {
        ia->starting[i] = malloc(ia->length[i] * sizeof(int));
        ia->startingBlock[i] = malloc(ia->length[i] * sizeof(struct Block));
    }

    // print file details....
    // for (i = 0; i < no_of_files; i++)
    // {
    //     printf("\nEnter the File Name: %s\n", ia->fileName[i]);
    //     printf("Enter the File Size: ");
    //     printf("%d\n", ia->length[i]);
    // }
    return ia;
}

void IndexedAllocation(struct MainMemory *mm, struct IndexedAllocation *ia, int files)
{
    int q = 0;
    for (q = 0; q < files; q++)
    {
        int i, j, file = q;
        int num, fiag = 0, final_fiag = 0, count = 0;
        num = rand() % (mm->numOfBlock - 0) + 0;
        // printf("%d debug",num);

        // Set the seed value using the current time
        struct Block *temp = mm->startingAddress;
        for (i = 0; i < num; i++)
            temp = temp->next; // move in main memory.
        // printf("Debug %d \n", num);

        int adj = 0;
        if (ia->length[file] % 2 == 0)
            adj = ia->length[file] + 1;
        int trv = ia->length[file] / mm->size; // finding how many move to save the data.
        do
        {
            // printf("Try to find..\n");
            int check_size = 0;
            fiag = 1;
            // printf("%d trv debug\n", trv);
            i = 0;
            do
            {
                // move in main memory and check the loiation if free or not for the file size.
                if (temp->status == 0)
                {
                    check_size += mm->size;
                    i++;
                }
                // printf("\n check_size in loop %d", check_size);
                if (temp->next == NULL)
                {
                    // printf("List out of range");
                    final_fiag = 1;
                    break;
                }
                else
                    temp = temp->next;
            } while (trv > i);

            if (check_size >= ia->length[file])
            {
                fiag = 1;
                // printf("success");
            }
            else
                count++;

            if (count == 3)
            {
                final_fiag = 1;
                fiag = 1;
            }
        } while (fiag == 0);

        printf("\nRandomly selected piace in Main Memory is : %d\n", num);
        // ia->startingBlock[file] = num;
        if (final_fiag == 0 && fiag == 1)
        {
            // printf("debug %d", num);
            printf("Block found successfuly... for %s\n", ia->fileName[file]);

            temp = mm->startingAddress;
            for (i = 0; i < num; i++)
                temp = temp->next; // move in main memory.
            // printf("%d status\n", temp->status);

            ia->startingBlock[q][0] = temp;
            ia->starting[q][0] = temp->blockNo;
            i = 0;
            do
            {
                // move in main memory and check the location if free or not for the file size.
                if (temp->status == 0)
                {
                    ia->startingBlock[q][i] = temp;
                    ia->starting[q][i] = temp->blockNo;
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
            ia->endingBlock[file] = temp;
            ia->ending[file] = temp->blockNo;
        }
        else
        {
            ia->startingBlock[file] = NULL;
            printf("\nBlock Not found...\n");
        }
    }
}

void printFileData(struct MainMemory *mm, struct IndexedAllocation *ia, int file)
{
    int i = 0, j = 0;
    int trv = ia->length[file] / mm->size; // finding how many move to save the data.

    struct Block *temp = ia->startingBlock[file][0];
    printf("\n\nData : ");
    for (i = 0; i < trv; i++)
    {
        for (j = 0; j < mm->size; j++)
        {
            printf("%d ", temp->data[j]);
        }
        temp = temp->next;
    }
}

void printStatus(struct MainMemory *mm, struct IndexedAllocation *ia, int no_of_files)
{
    // printf("debug\n");
    int i = 0, j, k;
    printf(".------------------------Directory-----------------------\n");
    printf("|%-20s|%15s\n", "fileName", "Index Block");
    printf("|--------------------------------------------------------\n");
    for (k = 0; k < no_of_files; k++)
    {
        printf("|  %-18s|", ia->fileName[k]); // ia->starting[i][0]

        int trv = ia->length[k] / mm->size; // finding how many move to save the data.
        for (i = 0; i < trv; i++)
        {
            for (j = 0; j < mm->size; j++)
            {
                printf("%d ", ia->starting[k][j]);
            }
        }
        printf("\n");
    }
    printf("`--------------------------------------------------------\n");
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
    struct IndexedAllocation *ia = getFileDetails(m);

    // Alloiate a file.
    IndexedAllocation(mm, ia, m);

    printStatus(mm, ia, m);

    while (1)
    {
        printf("Enter the file that you want to print the data: ");
        scanf("%d", &k);
        printFileData(mm, ia, k - 1);
    }

    return 0;
}

/*

100
20

2

file-1
20

file-2
10


*/