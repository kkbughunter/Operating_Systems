#include "ll9.h"


struct counter
{
	int f_no;
	int count;
};


int readInput(int *n, struct node *frames, char ref_str[]);
void fifo(struct node *frames, char ref_str[]);
void optimal(struct node *frames, char ref_str[]);
void lru(struct node *frames, char ref_str[]);
void lfu(struct node *frames, char ref_str[]);

int str_n;

int main()
{
	int n, ch;
	char ref_str[SIZE];
	struct node *frames = (struct node *) malloc(sizeof(struct node));
	frames->next = NULL;

 
	printf("\nPAGE REPLACEMENT ALGORITHMS\n");
	printf("1 Read Input\n2 FIFO REPLACEMENT\n3 OPTIMAL REPLACEMENT\n4 LRU REPLACEMENT\n5 LFU REPLACEMENT\n6 Exit\n");
	printf("Enter choice : ");
	scanf("%d", &ch);
	while (ch != 6)
	{
		switch(ch)
		{
			case 1 : 
				printf("\nREAD INPUTS\n");
				str_n = readInput(&n, frames, ref_str);
				break;
			case 2 : 
				printf("\nFIFO REPLACEMENT\n");
				fifo(frames, ref_str);
				break;
			case 3 : 
				printf("\nOPTIMAL REPLACEMENT\n");
				optimal(frames, ref_str);
				break;
			case 4 : 
				printf("\nLRU REPLACEMENT\n");
				lru(frames, ref_str);
				break;
			case 5 : 
				printf("\nLFU REPLACEMENT\n");
				lfu(frames, ref_str);
				break;
		}

		if (ch != 1) deleteAll(frames, n);

		printf("\nPAGE REPLACEMENT ALGORITHMS\n");
		printf("1 Read Input\n2 FIFO REPLACEMENT\n3 OPTIMAL REPLACEMENT\n4 LRU REPLACEMENT\n5 LFU REPLACEMENT\n6 Exit\n");
		printf("Enter choice : ");
		scanf("%d", &ch);
	}

	return 0;
}


int findFirstOcc(int arr[], int strt, int n, int key)
{
	for(int i=strt;i<n;i++){
		if (arr[i]==key){
			return i;
		}
	}
	return -1;
}


// 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1
int readInput(int *n, struct node *frames, char ref_str[])
{
	int i, N;
	printf("Enter total number of frames : ");
	scanf("%d", &N);

	for (i=0; i<N; i++) insert(frames, FREE_FRAME);

	printf("Enter reference string : ");

	char ch;
	for(i=0;!i || (i && ref_str[i-1] != '.'); i++)
		scanf(" %c", &ref_str[i]);
	ref_str[i--] = '\0';

	//scanf(" %[^\n]", ref_str);
	printf("\n");
	*n = N;
	return i;
}


void fifo(struct node *frames, char ref_str[])
{
	int pg_fault = 0;
	int f_no[str_n];

	for (int i=0; i < str_n; i++) f_no[i] = ref_str[i]-48;
	for (int i=0; i < str_n; i++)
	{
		if(!isHit(frames, f_no[i])){
			pg_fault++;
			printf("MISS ");
			delValue(frames, frames->next->data);
			insert(frames, f_no[i]);
		}
		else
			printf("HIT  ");
		display(frames);
 		printf("  |  %d Page fault(s)\n", pg_fault);
	}

}
 
//1 3 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1

// 1 3  1 7 8 3 0 2 0 3 5 4 0 6 1 - optimal ( 0 6 1 - 10 page faults)
void optimal(struct node *frames, char ref_str[])
{
	int f_no[str_n], pg_fault = 0, n = 0, temp, found=-1, j;

	for (int i=0; i < str_n; i++) f_no[n++] = ref_str[i]-48;

	for (int i=0; i<n; i++)
	{
		if(isHit(frames, f_no[i]))
		{
			//ADD AT THE END TO INDICATE REF IS MADE RECENTLY
			delValue(frames, f_no[i]);
			insert(frames, f_no[i]);
			printf("HIT  ");
		}
		else
		{
			if (isHit(frames, FREE_FRAME))
			{
				pg_fault++;
				delValue(frames, FREE_FRAME);
				insert(frames, f_no[i]);
				printf("MISS ");
			}
			else
			{

				struct node *ptr = frames->next;
				int max=-1, flag=0;

				while(ptr)// FINDING FRAME THAT IS NOT REF IN FUTURE
				{
					if(findFirstOcc(f_no, i+1, n, ptr->data) == -1){
						pg_fault++;
						delValue(frames, ptr->data);
						insert(frames, f_no[i]);
						flag =1 ;
						break;
					}
					ptr = ptr->next;
				}

				if (!flag){
				ptr=frames->next;
				while(ptr)
				{
					//for (j=i+1; j<n; j++)//searching for OPTIMAL victim page - LEAST ref in future
					//{
						if (findFirstOcc(f_no, i+1, str_n, ptr->data) != -1 && findFirstOcc(f_no, i+1, str_n, ptr->data) > max && isHit(frames, ptr->data)){
							max = findFirstOcc(f_no, i+1, str_n, ptr->data); //min first occ index
							//printf("\t\t%d \n", findFirstOcc(f_no, i+1, str_n, ptr->data));

						}
					
					//}
					ptr = ptr->next;
				}
				}
				if(max != -1)
				{
					pg_fault++;
					delValue(frames, f_no[max]);
					insert(frames, f_no[i]);
				}
				printf("MISS ");
			}
		}
		printf("%d : ", f_no[i]);
		display(frames);
 		printf("  |  %d Page fault(s)\n", pg_fault);
	}
}


// 1 3   1 2 1 0 3 0 4 2 4 - lru (2 4 0 - 6 faults)
void lru(struct node *frames, char ref_str[])
{
	int f_no[str_n], pg_fault = 0, n = 0, temp, found=-1;

	for (int i=0; i < str_n; i++) f_no[n++] = ref_str[i]-48;

	for (int i=0; i<n; i++)
	{
		if(isHit(frames, f_no[i]))
		{
			//ADD AT THE END TO INDICATE REF IS MADE RECENTLY
			delValue(frames, f_no[i]);
			insert(frames, f_no[i]);
			printf("HIT  ");
		}
		else
		{
			if (isHit(frames, FREE_FRAME))
			{
				pg_fault++;
				delValue(frames, FREE_FRAME);
				insert(frames, f_no[i]);
			}
			else
			{
				found = -1;
				
				struct node *ptr = frames->next;
				while(ptr)
				{
					for (int j=0; j<i; j++)//searching for lru victim page
					{
						if (ptr->data == f_no[j])
							found = j;						
					}
					if (found!=-1) break;
					ptr = ptr->next;
				}
	
				if (found!=-1)
				{
					pg_fault++;
					delValue(frames, f_no[found]);
					insert(frames, f_no[i]);
				}
			}
			printf("MISS ");
		}
		printf("%d : ", f_no[i]);
		display(frames);
 		printf("  |  %d Page fault(s)\n", pg_fault);
	}

}



int isInCounter(struct counter *a[], int n, int key){
	for(int i=0;i<n;++i)
		if(a[i]->f_no == key)
			return i;

	return -1;
}


// 1 3  7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 - lfu (0 3 2 - 10 faults)
void lfu(struct node *frames, char ref_str[])
{	
	int f_no[str_n], pg_fault = 0, n = 0, temp, ind=0 ;

	for (int i=0; i < str_n; i++) f_no[n++] = ref_str[i]-48;

	struct counter *C[n];
	for (int i=0; i < n; i++)
	{
		C[ind] = (struct counter *) malloc(sizeof(struct counter));
		if (isInCounter(C, ind, f_no[i]) == -1)
		{
			C[ind]->f_no = f_no[i];
			C[ind]->count = 0;
			ind++;
		}
	}

	//for (int i=0; i < ind; i++)	printf("%d %d\n", C[i]->f_no, C[i]->count);


	for (int i=0; i < n; i++)
	{
		if(isHit(frames, f_no[i]))
		{
			//ADD AT THE END TO INDICATE REF IS MADE RECENTLY
			delValue(frames, f_no[i]);
			insert(frames, f_no[i]);
			printf("HIT  ");
		}
		else
		{
			if (isHit(frames, FREE_FRAME))
			{
				pg_fault++;
				delValue(frames, FREE_FRAME);
				insert(frames, f_no[i]);
			}
			else
			{
				int min = SIZE*SIZE;
				
				struct node *ptr = frames->next;
				while(ptr)
				{
					for (int j=0; j<ind; j++)//searching for lru victim page
					{
						if (ptr->data == C[j]->f_no && min > C[j]->count)
							min = C[j]->count;
					}
					ptr = ptr->next;
				}
	
				if (min != SIZE*SIZE)//min contains least count
				{
					//FIFO
					for (int j=0; j<ind; j++)
					{
						if (min == C[j]->count && isHit(frames, C[j]->f_no)){
							min = C[j]->f_no;// min contains f_no with least count
							break;
						}
					}
					ptr = frames->next;
					pg_fault++;
					delValue(frames, min);
					insert(frames, f_no[i]);
				}
			}
			printf("MISS ");
		}

		for (int j=0; j<ind; j++) 
			if (C[j]->f_no == f_no[i])
				C[j]->count++;

		printf("%d : ", f_no[i]);
		display(frames);
 		printf("  |  %d Page fault(s)\n", pg_fault);
	}
}
