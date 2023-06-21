#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 100

void readInput(int np, int nr, char processIDs[][SIZE], char resources[][SIZE], int available[], int max[][SIZE], int allocated[][SIZE]);
void printData(int np, int nr, char processIDs[][SIZE], char resources[][SIZE], int available[], int max[][SIZE], int allocated[][SIZE], int need[][SIZE]);
void findNeed(int np, int nr, int max[][SIZE], int allocated[][SIZE], int need[][SIZE]);
int findSafetySeq(int np, int nr, char processIDs[][SIZE], char resources[][SIZE], int available[], int max[][SIZE], int allocated[][SIZE], int need[][SIZE], char safety_seq[][SIZE]);
int checkRequest(int np, int nr, char processIDs[][SIZE], char resources[][SIZE], int available[], int max[][SIZE], int allocated[][SIZE], int need[][SIZE], char safety_seq[][SIZE], char req_process[], int req[]);
//5 3 1 p0 p1 p2 p3 p4 a b c 3 3 2 7 5 3 3 2 2 9 0 2 2 2 2 4 3 3 0 1 0 2 0 0 3 0 2 2 1 1 0 0 2 2 3 y P1 1 0 2

int cpy1DArr(int a[], int b[], int n)
{
	int i;
	for (i = 0; i < n; i++) {
		b[i] = a[i];
	}
}

int cpy2DArr(int a[][SIZE], int b[][SIZE], int n1, int n2)
{
	for (int x=0 ; x < n1; x++)
	{
		for (int y=0 ; y < n2; y++)
		{
			b[x][y] = a[x][y];
		}
	}
}

int main()
{
	int np, nr, ch;
	char req_ch, temp[SIZE];

	printf("Enter number of process : ");
	scanf("%d", &np);
	printf("Enter number of resource types : ");
	scanf("%d", &nr);

	char processIDs[np][SIZE];
	char safety_seq[np][SIZE];
	char resources[nr][SIZE];
	int available[nr], max[np][SIZE], allocated[np][nr], need[np][nr], req[nr], a[nr], n[np][nr], al[np][nr];

	printf("\nDEADLOCK AVOIDANCE - BANKER'S ALGORITHM\n1 Read input\n2 Print data\n3 Get safety sequence\n4 Exit\nEnter choice : ");
	scanf("%d", &ch);

		while (ch != 4)
	{
		switch(ch){
		case 1:
			readInput(np, nr, processIDs, resources, available, max, allocated);
			break;
		case 2 :
			findNeed(np, nr, max, allocated, need);
			printData(np, nr, processIDs, resources, available, max, allocated, need);
			break;
		case 3 :
			cpy1DArr(available, a, nr);
			cpy2DArr(allocated, al, np, nr);
			cpy2DArr(need, n, np, nr);

			printf("Do you want to request resource? ");
			scanf(" %c", &req_ch);
			if (tolower(req_ch) == 'n'){
				if (findSafetySeq(np, nr, processIDs, resources, a, max, al, n, safety_seq))
				//if (findSafetySeq(np, nr, processIDs, resources, available, max, allocated, need, safety_seq))
				{
					printf("Safety sequence is successfully generated!\n\nSAFETY SEQUENCE\n");
					for(int i=0;i<np;i++)
					{
						if (!i) printf("< ");
						printf("%s", safety_seq[i]);
						if (i == np-1)
							printf(" >");
						else
							printf(", ");
					}
				}
				else
					printf("SYSTEM IS IN UNSAFE STATE!\nSafety sequence can't be generated!\n");
			}
			else
			{
				printf("Enter PID which is requesting : ");
				scanf(" %s", temp);
				printf("Enter request : ");
				for (int i=0; i<nr; i++)
				{
					scanf("%d", &req[i]);
				}
				
				if (checkRequest(np, nr, processIDs, resources, a, max, al, n, safety_seq, temp, req))
				{
					printf("Safety sequence is successfully generated!\n\nSAFETY SEQUENCE\n");
					for(int i=0;i<np;i++)
					{
						if (!i) printf("< ");
						printf("%s", safety_seq[i]);
						if (i == np-1)
							printf(" >");
						else
							printf(", ");
					}
				}
				else
					printf("SYSTEM WILL BE IN UNSAFE STATE IF REQUEST IS MADE!\nSafety sequence can't be generated!\n");
			}
			break;
		}
	printf("\nDEADLOCK AVOIDANCE - BANKER'S ALGORITHM\n1 Read input\n2 Print data\n3 Get safety sequence\n4 Exit\nEnter choice : ");
	scanf("%d", &ch);

	}
	return 0;
}



int checkRequest(int np, int nr, char processIDs[][SIZE], char resources[][SIZE], int available[], int max[][SIZE], int allocated[][SIZE], int need[][SIZE], char safety_seq[][SIZE], char req_process[], int req[])
{
	int ind=-1, flag;
	for (int i=0;i<np; i++)
	{
		flag = 1;
		for(int j=0;j<strlen(req_process); j++)
		{
			if (tolower(processIDs[i][j]) != tolower(req_process[j]))
			{
				flag = 0;
			}
		}
		if (flag)
			ind = i;
	}

	if (ind == -1)
	{
		printf("Invalid Process ID\n");
		return 0;
	}
	for(int i=0; i<nr; i++)
	{
		if (need[ind][i]<req[i])
		{
			printf("Request can't be made as request made is greater than need!\n");
			return 0;
		}
		if (available[i]<req[i])
		{
			printf("%di\n", i);
			printf("Request can't be made due to lack of resources!\n");
			return 0;
		}
	}

	for(int i=0; i<nr; i++){
		available[i] -= req[i];
		allocated[ind][i] += req[i];
		need[ind][i] += req[i];
	}

	return findSafetySeq(np, nr, processIDs, resources, available, max, allocated, need, safety_seq);
}


void readInput(int np, int nr, char processIDs[][SIZE], char resources[][SIZE], int available[], int max[][SIZE], int allocated[][SIZE])
{
	printf("Enter ID of the processes : ");
	for(int i=0; i<np; i++)
		scanf("%s", processIDs[i]);
	printf("Enter name of the resource types : ");
	for(int i=0; i<nr; i++)
		scanf("%s", resources[i]);

	for(int i=0; i<nr; i++)
	{
		printf("Enter number of Available instances of %s : ", resources[i]);
		scanf("%d", &available[i]);
	}

	for(int i=0; i<np; i++)
	{
		printf("Enter Maximum requirement for %s : ", processIDs[i]);
		for(int j=0; j<nr; j++)
		{
			scanf("%d", &max[i][j]);
		}
	}

	for(int i=0; i<np; i++)
	{
		printf("Enter Allocated instances to %s : ", processIDs[i]);
		for(int j=0; j<nr; j++)
		{
			scanf("%d", &allocated[i][j]);
		}
	}

}


void printData(int np, int nr, char processIDs[][SIZE], char resources[][SIZE], int available[], int max[][SIZE], int allocated[][SIZE], int need[][SIZE])
{
	printf("\n|   PID  |");

	for(int i=0;i<nr;i++)
	{
		printf("  ");
		if (i == (int)nr/2){
			printf("\b\b\b\b");	
			printf("ALLOC");
		}
	}
	printf("|");

	for(int i=0;i<nr;i++)
	{
		printf("  ");
		if (i == (int)nr/2){
			printf("\b\b");	
			printf("MAX");
		}
	}
	printf("|");

	for(int i=0;i<nr;i++)
	{
		printf("  ");
		if (i == (int)nr/2){
			printf("\b\b\b");	
			printf("NEED");
		}
	}
	printf("|");

	printf("\n");
	findNeed(np, nr, max, allocated, need);
	for (int i=0; i<np;i++)
	{
		printf("|   %s   | ", processIDs[i]);
		for(int j=0;j<nr;j++)
			printf("%d ", allocated[i][j]);
		printf("| ");
		for(int j=0;j<nr;j++)
			printf("%d ", max[i][j]);
		printf("| ");
		for(int j=0;j<nr;j++)
			printf("%d ", need[i][j]);
		printf("| ");


		if (!i)
		{
			for(int j=0;j<nr;j++)
				printf("%d ", available[j]);
			printf(" |");

		}
		printf("\n");
	}
}


void findNeed(int np, int nr, int max[][SIZE], int allocated[][SIZE], int need[][SIZE])
{
	for (int i=0;i<np;i++)
	{
		for(int j=0; j<nr; j++)
		{
			need[i][j] = max[i][j] - allocated[i][j];
		}
	}
}


int allFinished(int finish[], int np)
{
	for(int i=0;i<np;i++)
		if(!finish[i])
			return 0;
	return 1;
}


int findSafetySeq(int np, int nr, char processIDs[][SIZE], char resources[][SIZE], int available[], int max[][SIZE], int allocated[][SIZE], int need[][SIZE], char safety_seq[][SIZE])
{
	int finish[np], i = 0, ind = 0, temp_ind, count=0, possibility = 1;
	for (int a=0;a<np;a++) finish[a] = 0;

	while (!allFinished(finish, np))
	{
		temp_ind = ind;
		if (finish[i] == 0)
		{
			int flag = 1;
			for(int j=0;j<nr;j++)
			{
				if (need[i][j] > available[j])
				{
					flag = 0;
					break;
				}
			}
			if(flag)
			{
				strcpy(safety_seq[ind++], processIDs[i]);
				for(int j=0;j<nr;j++)
				{
					available[j] += allocated[i][j];
				}
				finish[i] = 1;
			}
		}
printData(np, nr, processIDs, resources, available, max, allocated, need);
		i = (i+1)%np;
		if(temp_ind == ind)
		{
			count++;
			if (count>np*2)
			{
				possibility = 0;
				break;
			}
		}
	}
	return possibility;
}

