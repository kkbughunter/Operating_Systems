#include <stdio.h>
#include <stdlib.h>
// NON Preemptive
// Structure to represent a process
struct process {
    int pid;      // Process ID
    int burst;    // Burst time
    int remaining_burst; // Remaining burst time
    int arrival;  // Arrival time
    int priority; // Priority of process
    int turnaround; // Turnaround time
    int waiting;  // Waiting time
};

void merge(struct process arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    // create temporary arrays
    struct process L[n1], R[n2];
 
    // copy data to temporary arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    // merge the temporary arrays back into arr[l..r]
    i = 0; // initial index of first subarray
    j = 0; // initial index of second subarray
    k = l; // initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i].priority <= R[j].priority) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    // copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    // copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 
void mergeSort(struct process arr[], int l, int r) {
    if (l < r) {
        // find the middle point
        int m = l + (r - l) / 2;
 
        // sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
 
        // merge the sorted halves
        merge(arr, l, m, r);
    }
}

int findMinArrival(struct process arr[], int n) {
    int minArrival = arr[0].arrival;
    int minIndex = 0;
    for (int i = 1; i < n; i++) {
        if (arr[i].arrival < minArrival) {
            minArrival = arr[i].arrival;
            minIndex = i;
        }
    }
    return minIndex;
}


void gantt_chart(struct process processes[], int num_processes){
	
	int *visit, *pos, i = 0;
	visit = (int *) malloc(num_processes * sizeof(int));
	pos = (int *) malloc(num_processes * sizeof(int));
	for(i=0; i<num_processes; i++) visit[i] = 0;
	// find the minimim Arrival time.
	int mim_at = findMinArrival(processes, num_processes);
	
	int x = 0, flag = 1, total_burst_time = processes[mim_at].burst;	
	pos[x] = mim_at;
	visit[mim_at] = 1;
	
	while(flag != num_processes){
		x++;
		for(i=0; i<num_processes; i++){
			if(processes[i].arrival <= total_burst_time && visit[i] == 0){
				pos[x] = i;
				visit[i] = 1;
				total_burst_time += processes[x].burst;
				break;
			}
		}
		flag = 0;
		for(i=0; i<num_processes; i++){
			flag +=visit[i];
		}
		printf("%d \n\n ",flag);
		
		
	}
	
	for(i=0; i<num_processes; i++){
		printf("P%d  ",processes[pos[i]].pid);
	}
	
}

void print_question(struct process *processes, int num_processes){
	int i = 0;
	printf("Question...\n");
	printf("|---------------------------------------------------|\n");
	printf("| Process Id | priority | Arrival Time | Burst Time |\n");
	printf("|------------|----------|--------------|------------|\n");
	for (int i = 0; i < num_processes; i++) {
        printf("| P%-9d | %-7d  | %-12d | %-10d |\n", processes[i].pid ,processes[i].priority ,processes[i].arrival ,processes[i].burst);
    }
	printf("|---------------------------------------------------|\n");
}

int main() {
    int num_processes;
    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);

    // Allocate memory for the array of processes
    struct process *processes = (struct process *) malloc(num_processes * sizeof(struct process));

    // Get details for each process from the user
    for (int i = 0; i < num_processes; i++) {
        printf("\nEnter details for process %d:\n", i+1);
        processes[i].pid = i+1;
        printf("Burst time: ");
        scanf("%d", &processes[i].burst);
        processes[i].remaining_burst = processes[i].burst;
        printf("Arrival time: ");
        scanf("%d", &processes[i].arrival);
        printf("Priority: ");
        scanf("%d", &processes[i].priority);
        processes[i].turnaround = 0;
        processes[i].waiting = 0;
    }
    
	// sort the proccess based on the priority 
	mergeSort(processes, 0, num_processes-1);
	
	// Print the Question 
	print_question(processes, num_processes);
	
	// Print Gantt Chart
	gantt_chart(processes, num_processes);

    return 0;
}



/* NON Preemptive

5

10
0
3

1
0
1

2
0
4

1
0
5

5
0
2

*/


/*

7

8
0
3

2
1
4

4
3
4

1
4
5

6
5
2

5
6
6

1
10
1

*/

/*

5

10
0
3

1
1
1

2
2
4

1
3
5

5
4
2

*/

