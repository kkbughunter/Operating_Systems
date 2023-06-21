#include "ll.h"

#define EMPTY 0
#define HOLE "H"

struct process
{
	char pid[SIZE];
	int size;
	struct process *next;
};


int firstFit(char *pid, int size, struct node *free_pool, struct node *alloted);
int bestFit(char *pid, int size, struct node *free_pool, struct node *alloted);
int worstFit(char *pid, int size, struct node *free_pool, struct node *alloted);
void deallocate(struct node *free_pool, struct node *alloted, char *pid);
void coalesceHoles(struct node *free_pool);

int main ()
{
	struct node *free_pool = (struct node *) malloc(sizeof(struct node));
	free_pool->next = NULL;
	struct node *alloted = (struct node *) malloc(sizeof(struct node));
	alloted->next = NULL;

	int n_partitions, n_processes, ch;
	int start, end;
	struct process *P[SIZE*SIZE];
	char *pid = (char *) malloc(sizeof(char) * SIZE);

	printf("Enter number of partitions : ");
	scanf("%d", &n_partitions);

	for (int i=0; i<n_partitions; i++)
	{
		printf("\nPartition %d : ", i+1);
		printf("\nEnter starting address : ");
		scanf("%d", &start);
		printf("Enter ending address : ");
		scanf("%d", &end);
		insert(free_pool, start, end, "H", end-start+1, EMPTY);
	}

	struct node *org_free_pool = (struct node *) malloc(sizeof(struct node));
	org_free_pool->next = NULL;
	copyLL(org_free_pool, free_pool);

	struct node *org_alloted = (struct node *) malloc(sizeof(struct node));
	org_alloted->next = NULL;
	copyLL(org_alloted, alloted);


	int option;
	do
	{
		printf("\n\n\n");
		printf("1. Allocate\n");
		printf("2. Deallocate\n");
		printf("3. Reset\n");
		printf("4. Display\n");
		printf("5. Coalescing Holes\n");
		printf("6. Quit\n");
		printf("Enter your option: ");
		scanf("%d", &option);

		switch (option) {
		case 1:
			printf("Enter number of processes : ");
			scanf("%d", &n_processes);
			for(int i=0; i<n_processes; i++)
			{
				P[i] = (struct process *) malloc(sizeof(struct process));
				printf("Enter process ID : ");
				scanf(" %s", P[i]->pid);
				printf("Enter size of the process : ");
				scanf("%d", &P[i]->size);
			}
			printf("\nMEMORY ALLOCATION\n1.First Fit\n2.Best Fit\n3.Worst Fit\nEnter your choice : ");
			scanf("%d", &ch);
			for(int i=0; i<n_processes; i++)
			{
				switch(ch){
				case 1:
					if (firstFit(P[i]->pid, P[i]->size, free_pool, alloted))
						printf("First fit has successfully alloted memory!\n");
					else printf("First fit failed to allot memory!\n");
					break;
				case 2:
					if (bestFit(P[i]->pid, P[i]->size, free_pool, alloted))
						printf("Best fit has successfully alloted memory!\n");
					else printf("Best fit failed to allot memory!\n");
					break;
				case 3:
					if (worstFit(P[i]->pid, P[i]->size, free_pool, alloted))
						printf("Worst fit has successfully alloted memory!\n");
					else printf("Worst fit failed to allot memory!\n");
					break;
				default : printf("Invalid choice!\n");
				}
			}
			break;

            case 2:
			printf("Enter process ID : ");
			scanf(" %s", pid);
			deallocate(free_pool, alloted, pid);
  			break;

            case 3:
			copyLL(free_pool, org_free_pool);
			copyLL(alloted, org_alloted);
			break;

            case 4:
			printf("\nFREE POOL");
			display(free_pool, NULL);
			printf("\nALLOTED");
			display(alloted, NULL);
			printf("\nPHYSICAL MEMORY");
			display(free_pool, alloted);
  			break;

            case 5:
			coalesceHoles(free_pool);
			break;

            case 6:
			break;

            default:
                printf("Invalid option. Please try again.\n");
                break;
        }

        printf("\n");
    } while (option != 6);

	return 0;
}


void deallocate(struct node *free_pool, struct node *alloted, char *pid)
{
	struct node *temp = (struct node *) malloc(sizeof(struct node)), *key = (struct node *) malloc(sizeof(struct node));

	temp = alloted->next;
	while (temp && strcmp(temp->status, pid)) temp = temp->next;

	if(!strcmp(temp->status, pid)){
		key->start = temp->start;
		key->end = temp->end;
		key->byte_avl = temp->byte_avl;
		key->byte_alloc = temp->byte_alloc;
		key->status = (char *) malloc(sizeof(char) * SIZE);
		strcpy(key->status, temp->status);
	}

	delete(alloted, key->start, key->end);
	insert(free_pool, key->start, key->end, "H", key->start-key->end+1, EMPTY);

	coalesceHoles(free_pool);
}


int firstFit(char *pid, int size, struct node *free_pool, struct node *alloted)
{
	int start_h, end_h;
	struct node *temp = (struct node *) malloc(sizeof(struct node));
	temp = free_pool->next;
	while (temp != NULL)
	{
		if (!strcmp(temp->status, HOLE) && temp->byte_avl>=size)
		{
			insert(alloted, temp->start+temp->byte_alloc, temp->start+temp->byte_alloc+size-1, pid, EMPTY, size);

			start_h = temp->start+temp->byte_alloc+size;
			end_h = temp->end;
			if (end_h-start_h+1)
				insert(free_pool, start_h, end_h, "H", end_h-start_h+1, EMPTY);
			delete(free_pool, temp->start, temp->end);
			

			//display(alloted, NULL);
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}


int bestFit(char *pid, int size, struct node *free_pool, struct node *alloted)
{
	int start_h, end_h, diff, min = 999999;
	struct node *temp = (struct node *) malloc(sizeof(struct node));
	struct node *best = NULL;
	temp = free_pool->next;

	while (temp != NULL)
	{
		if (!strcmp(temp->status, HOLE) && temp->byte_avl>=size)
		{
			diff = temp->byte_avl - size;
			if(diff<min){
				min = diff;
				best = temp;
			}
		}
		temp = temp->next;
	}

	if (best)
	{
			insert(alloted, best->start+best->byte_alloc, best->start+best->byte_alloc+size-1, pid, EMPTY, size);

			start_h = best->start+best->byte_alloc+size;
			end_h = best->end;
			if (end_h-start_h+1)
				insert(free_pool, start_h, end_h, "H", end_h-start_h+1, EMPTY);
			delete(free_pool, best->start, best->end);
			
			//display(alloted, NULL);
			return 1;
	}
	else	return 0;
}


int worstFit(char *pid, int size, struct node *free_pool, struct node *alloted)
{
	int start_h, end_h, diff, max = -1;
	struct node *temp = (struct node *) malloc(sizeof(struct node));
	struct node *worst = NULL;
	temp = free_pool->next;

	while (temp != NULL)
	{
		if (!strcmp(temp->status, HOLE) && temp->byte_avl>=size)
		{
			diff = temp->byte_avl - size;
			if(diff>max){
				max = diff;
				worst = temp;
			}
		}
		temp = temp->next;
	}

	if (worst)
	{
			insert(alloted, worst->start+worst->byte_alloc, worst->start+worst->byte_alloc+size-1, pid, EMPTY, size);

			start_h = worst->start+worst->byte_alloc+size;
			end_h = worst->end;
			if (end_h-start_h+1)
				insert(free_pool, start_h, end_h, "H", end_h-start_h+1, EMPTY);
			delete(free_pool, worst->start, worst->end);
			
			//display(alloted, NULL);
			return 1;
	}
	else	return 0;
}


void coalesceHoles(struct node *free_pool)
{
	struct node *temp = free_pool->next;
	int count, start, end;

	while (temp)
	{
		count = 0;
		while (temp->next && temp->end+1 == temp->next->start)
		{
			printf("%d %d\n", temp->end+1, temp->next->start);
			count++;
			if (count == 1)
				start = temp->start;
			end = temp->next->end;
			strcpy(temp->status, "*");
			strcpy(temp->next->status, "*");
			temp = temp->next;
		}
		if (count)
		{
			insert(free_pool, start, end, "H", count, 0);
		}
		temp = temp->next;
	}

	temp = free_pool->next;
	while (temp)
	{
		if (!strcmp(temp->status, "*"))
			delete(free_pool, temp->start, temp->end);
		temp = temp->next;
	}
	display(free_pool, NULL);
}



//4 1 100 101 140 141 200 201 300 1 3 p1 50 p2 100 p3 10 1 4 2 p1 4 2 p2 4 2 p3 4 5

