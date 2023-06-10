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

struct ContiguousAllocation
{
    char **fileName;    // name of the file.
    int *startingBlock; // starting address of the file.
    int *length;        // length of the file.
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
    //
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

struct ContiguousAllocation *getFileDetails(int no_of_files)
{
    struct ContiguousAllocation *ca = (struct ContiguousAllocation *)malloc(sizeof(struct ContiguousAllocation));
    int i;

    ca->fileName = (char **)malloc(no_of_files * sizeof(char *));
    ca->length = (int *)malloc(no_of_files * sizeof(int));
    ca->startingBlock = (int *)malloc(no_of_files * sizeof(int));

    for (i = 0; i < no_of_files; i++)
    {
        ca->fileName[i] = (char *)malloc(10 * sizeof(char));
        printf("\nEnter the File Name: ");
        scanf("%s", ca->fileName[i]);
        printf("Enter the File Size: ");
        scanf("%d", &ca->length[i]);
    }

    // print file details....
    // for (i = 0; i < no_of_files; i++)
    // {
    //     printf("\nEnter the File Name: %s\n", ca->fileName[i]);
    //     printf("Enter the File Size: ");
    //     printf("%d\n", ca->length[i]);
    // }
    return ca;
}

void ContiguousAllocation(struct MainMemory *mm, struct ContiguousAllocation *ca, int files)
{
    int q = 0;
    srand(time(0));
    for (q = 0; q < files; q++)
    {
        int i, j,file=q;
        int num, flag = 0, final_flag = 0, count = 0;
        num = rand() % (mm->numOfBlock - 0) + 0;
        // printf("%d debug",num);

        // Set the seed value using the current time
        struct Block *temp = mm->startingAddress;
        for (i = 0; i < num; i++)
            temp = temp->next; // move in main memory.
        int adj = 0;
        if (ca->length[file] % 2 == 0)
            adj = ca->length[file] + 1;
        int trv = ca->length[file] / mm->size; // finding how many move to save the data.
        do
        {
            // printf("Try to find..\n");
            int check_size = 0;
            flag = 1;
            // printf("%d trv\n", trv);
            for (i = 0; i < trv; i++)
            {
                // move in main memory and check the location if free or not for the file size.
                if (temp->status == 0)
                {
                    check_size += mm->size;
                }
                else{
                    flag = 0;
                    break;
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
            }
            if (check_size >= ca->length[file])
            {
                flag = 1;
                // printf("success");
            }
            else
                count++;

            if (count == 5)
            {
                final_flag = 1;
                flag = 1;
            }
        } while (flag == 0);

        printf("\nRandomly selected place in Main Memory is : %d\n", num);
        ca->startingBlock[file] = num;
        if (final_flag == 0 && flag == 1)
        {
            // printf("debug %d", num);
            printf("\nBlock found successfuly... for %s\n", ca->fileName[file]);

            temp = mm->startingAddress;
            for (i = 0; i < num; i++)
                temp = temp->next; // move in main memory.
            // printf("%d status\n", temp->status);
            for (i = 0; i < trv; i++)
            {
                // printf("debug\n");
                // move in main memory and check the location if free or not for the file size.
                // Inserting the data in the blocks..
                for (j = 0; j < mm->size; j++)
                {
                    temp->data[j] = (rand() % (0 - mm->numOfBlock + 1)) + 0;
                    // printf("test %d ", temp->data[j]);
                }
                temp->status = 1; // changing the state to full.
                temp = temp->next;
            }
        }
        else
        {
            ca->startingBlock[file] = -1;
            printf("\nBlock Not found...\n");
        }
    }

    // printf("\n\n printinf data : ");
    // temp = mm->startingAddress;
    // for (i = 0; i < num; i++)
    //     temp = temp->next; // move in main memory.
    // for (i = 0; i < ca->length[file] / mm->size; i++)
    // {
    //     // move in main memory and check the location if free or not for the file size.
    //     // Inserting the data in the blocks..
    //     for (j = 0; j < mm->size; j++)
    //     {
    //         printf("%d ", temp->data[j]);
    //     }
    //     temp = temp->next;
    // }
    printf("End \n\n");
}

void printStatus(struct ContiguousAllocation *ca, int no_of_files)
{
    // printf("debug\n");
    int i = 0;
    printf(".--------------------------------------------.\n");
    printf("|%-20s|%15s|%7s|\n", "fileName", "starting Block", "length");
    printf("|--------------------------------------------|\n");
    for (i = 0; i < no_of_files; i++)
    {
        printf("|%-20s|%15d|%7d|\n", ca->fileName[i], ca->startingBlock[i], ca->length[i]);
    }
    printf("`--------------------------------------------`\n");
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
    int m = 0;
    printf("Enter the No.of.Files: ");
    scanf("%d", &m);
    struct ContiguousAllocation *ca = getFileDetails(m);

    // Allocate a file.
    ContiguousAllocation(mm, ca, m);

    printStatus(ca, m);
    return 0;
}