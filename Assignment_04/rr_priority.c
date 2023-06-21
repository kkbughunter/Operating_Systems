#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>

#define MAX 150

struct process 
{
	char pid[20];
	int at;//Arrival time
	int bt;//Burst time
	int wt;//Waiting time
	int tat;//Turnaround time
	int et;//end time
	int priority;
	int remaining_bt;//for preemptive 
};


void RR(struct process *processes[], int n, int quantum);
void priority(struct process *processes[], int n);
void dispProcesses(struct process *arr[], int n, int flag);
void dispArray(int arr[], int n);
void printGantt(struct process *arr[], int n);


int main ()
{

	int ch, n, q;
	char opt;
	struct process *processes[100];

	printf("\nCPU SCHEDULING ALGORITHM\n1 Priority\n2 Round Robbin\n3 EXIT\nEnter your choice : ");
	scanf(" %d", &ch);
	while(ch!=3){

		printf("Enter number of processes : ");
		scanf(" %d", &n);

		for (int i=0;i<n;i++)
		{
			processes[i] = (struct process *) malloc(sizeof(struct process));
			printf("\nProcess %d:\n", i+1);
			printf("Enter process ID : ");
			scanf(" %s", processes[i]->pid);
			printf("Enter arrival time : ");
			scanf(" %d", &processes[i]->at);
			printf("Enter burst time : ");
			scanf(" %d", &processes[i]->bt);
			if (ch == 1)
			{
				printf("Enter priority : ");
				scanf(" %d", &processes[i]->priority);
			}
		}

		switch(ch)
		{
			case 1:
				printf("\n\nPRIORITY CPU SCHEDULER\n");
				priority(processes, n);
				break;
			case 2:
				
				printf("\n\nROUND ROBBIN CPU SCHEDULER\n");
				printf("Enter quantum : ");
				scanf("%d", &q);
				RR(processes, n, q);
				break;
			case 3:
				exit(0);
		}
		printf("\n\nCPU SCHEDULING ALGORITHM\n1 Priority\n2 Round Robbin\n3 EXIT\nEnter your choice : ");
		scanf(" %d", &ch);
	}

}


void dispProcesses(struct process *arr[], int n, int flag)
{
	printf("OBSERVATION TABLE\n");

	if (flag) // Priority : 1
	{
		printf("+----------+--------------+--------------+-------------------+--------------+-----------------+\n");
		printf("|Process ID| Arrival time |   Priority   |     Burst Time    | Waiting Time | Turnaround Time |\n");
	 	printf("+----------+--------------+--------------+-------------------+--------------+-----------------+\n");
		for (int i=0;i<n;i++)
			printf("|    %s    |      %2d      |      %2d      |        %2d         |      %2d      |       %2d        |\n", arr[i]->pid, arr[i]->at, arr[i]->priority, arr[i]->bt, arr[i]->wt, arr[i]->tat);
		printf("+----------+--------------+--------------+-------------------+--------------+-----------------+\n");
	}
	else{
		printf("+----------+--------------+-------------------+--------------+-----------------+\n");
		printf("|Process ID| Arrival time |     Burst Time    | Waiting Time | Turnaround Time |\n");
	 	printf("+----------+--------------+-------------------+--------------+-----------------+\n");
		for (int i=0;i<n;i++)
			printf("|    %s    |      %2d      |        %2d         |      %2d      |       %2d        |\n", arr[i]->pid, arr[i]->at, arr[i]->bt, arr[i]->wt, arr[i]->tat);
		printf("+----------+--------------+-------------------+--------------+-----------------+\n");
	}
}


void priority(struct process *processes[], int n)//PREEMPTIVE
{
	int indices[n];
	int tot_wt=0, tot_tat=0, curr_time = 0, tot_time = 0;
	double avg_wt, avg_tat;


	//finding total burst time
	for (int i=0;i<n;i++) // Find total time, assign remaining bt, set wt as 0
	{
		tot_time += processes[i]->bt;
		processes[i]->remaining_bt = processes[i]->bt;
		processes[i]->wt = 0;
	}

	int sorted[tot_time];
	while (curr_time < tot_time) //Pick the incomplete arrived shortest job
	{
		int min = INT_MAX, ind; //min : highest priority
		for(int i=0;i<n;i++)
		{
			if ((processes[i]->remaining_bt > 0) && (processes[i]->at <= curr_time) && (processes[i]->priority < min))
			{
				min = processes[i]->priority;
				ind = i;
			}
		}
		
		processes[ind]->remaining_bt--;
		sorted[curr_time] = ind;
		
		for (int i=0;i<n;i++)
			if (processes[i]->remaining_bt && processes[i]->at <= curr_time && i!=ind )
				processes[i]->wt++;
		curr_time++;
	}


	for (int i=0;i<n;i++) //total wt, tat
	{
		processes[i]->tat = processes[i]->wt + processes[i]->bt ;
		tot_wt += processes[i]->wt;
		tot_tat += processes[i]->tat;
	}
		
	avg_wt = (double)tot_wt/(double)n;
	avg_tat = (double)tot_tat/(double)n;
		

	// Since indices[] is used, need a temp to store the processes in the order of exe.
	struct process *temp[tot_time];
	int temp_n = 0;
	for (int i=0;i<tot_time;i++)
	{
		if ((i && sorted[i] != sorted[i-1]) || !i)
		{
			temp[temp_n] = (struct process *) malloc(sizeof (struct process));
			strcpy(temp[temp_n]->pid, processes[sorted[i]]->pid);
			temp[temp_n]->at = processes[sorted[i]]->at;
			temp[temp_n]->bt = processes[sorted[i]]->bt;
			temp[temp_n]->remaining_bt = 0;
			temp[temp_n]->wt = processes[sorted[i]]->wt;
			temp[temp_n]->tat = processes[sorted[i]]->tat;
			temp_n++;
		}
		temp[temp_n-1]->remaining_bt++; 
	}
	
	for (int i=0;i<temp_n;i++) 
	{
		if (!i) temp[i]->et = temp[i]->remaining_bt;
		else temp[i]->et = temp[i-1]->et + temp[i]->remaining_bt;
	}
	printGantt(temp, temp_n);

	dispProcesses(processes, n, 1);
	printf("Average Waiting time : %f\nAverage Turn around time : %f\n", avg_wt, avg_tat);

}


void RR(struct process *processes[], int n, int quantum)
{
	int curr_time = 0,  tot_time = 0, i=0, indices[100], tot_tat = 0, tot_wt = 0, diff;
	double avg_tat, avg_wt;
	struct process *temp[100]; int temp_n=0;
	
	for (int i=0;i<n;i++)
	{
		tot_time += processes[i]->bt;
		processes[i]->remaining_bt = processes[i]->bt;
		processes[i]->wt = 0;
	}

	//sort by arrival time ----------------------------- IMPORTANT -----------------------------
	for (int i=0; i<n;i++)
		for (int j=0; j<n-i-1;j++)
			if (processes[j]->at > processes[j+1]->at)
			{
				struct process *temp = processes[j];
				processes[j] = processes[j+1];
				processes[j+1] = temp;
			}


	while (curr_time < tot_time)
	{
		if ((processes[i]->remaining_bt > 0) && (processes[i]->at <= curr_time)) // Pick an incomplete arrived process
		{
			indices[temp_n] = i;// process 'i' is selected

			temp[temp_n] = (struct process *) malloc(sizeof (struct process));
			strcpy(temp[temp_n]->pid, processes[i]->pid);
			temp[temp_n]->at = processes[i]->at;
			temp[temp_n]->wt = processes[i]->wt;
			temp[temp_n]->tat = 0;

			if (processes[i]->remaining_bt >= quantum)
				diff = quantum;
			else	diff = processes[i]->remaining_bt;

			curr_time += diff;
			temp[temp_n++]->bt = diff;
			processes[i]->remaining_bt -= diff;
			
		}
		i = (i+1) % n;
	}

	//Calculate et, wt
	temp[0]->et = temp[0]->bt;
	for (int i=0; i<temp_n;i++)
	{
		if (i) temp[i]->et = temp[i-1]->et + temp[i]->bt;
		processes[indices[i]]->wt = temp[i]->et - temp[i]->at - temp[i]->bt - (processes[indices[i]]->remaining_bt * quantum);

		processes[indices[i]]->remaining_bt++; // To remember the already no of executed slots of the same process
	}

	for (int i=0;i<n;i++) // Turn Around time
	{
		processes[i]->tat = processes[i]->wt + processes[i]->bt ;
		tot_wt += processes[i]->wt;
		tot_tat += processes[i]->tat;
	}
	
	avg_wt = (float)tot_wt/(float)n;
	avg_tat = (float)tot_tat/(float)n;


	printGantt(temp, temp_n);
	dispProcesses(processes, n, 0);
	printf("Average Waiting time : %f\nAverage Turn around time : %f\n", avg_wt, avg_tat);
}


void printGantt(struct process *arr[], int n)
{
	int ratio[n], tot_et=0;
	for (int i=0;i<n;i++) tot_et += arr[i]->et;

	for (int i=0;i<n;i++){
		if (!i) ratio[i] = round( ( ((float)arr[i]->et) / (float)tot_et ) * MAX);
		else ratio[i] = round( ( ((float)arr[i]->et - (float)arr[i-1]->et) / (float)tot_et ) * MAX);
	}



	printf("\n\nGANTT CHART\n");
	
	//printing line
	for (int i=0;i<=n;i++)
	{
		printf("+");
		if (i!=n)
		{
			for(int j=0; j < (ratio[i]/2)-1 ;j++)	printf("=");
			printf("==");
			for(int j=0; j < (ratio[i]/2) ;j++)	printf("=");
		}
	}
	
	//printing chart
	printf("\n|");
	for (int i=0;i<n;i++)
	{
		for(int j=0; j < (ratio[i]/2)-1 ;j++)	printf(" ");
		printf("%2s", arr[i]->pid);
		for(int j=0; j < (ratio[i]/2) ;j++)	printf(" ");
		printf("|");
	}
	printf("\n");

	//printing line
	for (int i=0;i<=n;i++)
	{
		printf("+");
		if (i!=n)
		{
			for(int j=0; j < (ratio[i]/2)-1 ;j++)	printf("=");
			printf("==");
			for(int j=0; j < (ratio[i]/2) ;j++)	printf("=");
		}
	}

	//printing timeline
	printf("\n");
	for (int i=0;i<=n;i++)
	{

		if (!i) printf("0");
		else
		{
			if (arr[i-1]->et > 9) printf("\b");
			printf("%d", arr[i-1]->et);
		}

		if (i!=n){
			for(int j=0; j < (ratio[i]/2)-1 ;j++)	printf(" ");
			printf("  ");
			for(int j=0; j < (ratio[i]/2) ;j++)	printf(" ");
		}
	}
	
	printf("\n\n");
}


void dispArray(int arr[], int n)
{
	for (int i=0;i<n;i++)
		printf("%d ", arr[i]);
	printf("\n");
}


