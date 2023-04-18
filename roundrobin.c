#include <stdio.h>
#include <stdbool.h>
float avg_wait_time, avg_turnaround_time;
struct process {
    int pid;
    int burst_time;
    int remaining;
    int arrival_time;
    int waiting_time;
    int turnaround_time;
};

void calculateTime(struct process p[], int n, int quantum) {
    int i, total_time = 0;
    bool done = false;

    while(!done) {
        done = true;

        for(i=0; i<n; i++) {
            if(p[i].remaining > 0) {
                done = false;

                if(p[i].remaining > quantum) {
                    total_time += quantum;
                    p[i].remaining -= quantum;
                } else {
                    total_time += p[i].remaining;
                    p[i].turnaround_time = total_time - p[i].arrival_time;
                    p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
                    p[i].remaining = 0;
                }
            }
        }
    }
}

void calculateAvgTime(struct process p[], int n, float *avg_wait_time, float *avg_turnaround_time) {
    int i;

    *avg_wait_time = 0;
    *avg_turnaround_time = 0;

    for(i=0; i<n; i++) {
        *avg_wait_time += p[i].waiting_time;
        *avg_turnaround_time += p[i].turnaround_time;
    }

    *avg_wait_time /= n;
    *avg_turnaround_time /= n;
}

void displayGanttChart(struct process p[], int n) {
    int i, j, k;


	printf("\n");
	printf("\n");
	
	for(i=0; i<n; i++){
		printf("%d\n",p[i].burst_time);
	}
	
	printf("\n");
	printf("\n");



    // print table headers
    printf("\nProcess Data:\n\n");
    
	for(i=0; i<39; i++) {
        printf("--");
    }
    printf("\n");
    printf("| PID\t   | Burst Time\t   | Arrival Time  | Waiting Time  | Turnaround Time |\n");

    // print process data
    for(i=0; i<78; i++) {
        printf("-");
    }
    printf("\n");
    for(i=0; i<n; i++) {
        printf("|P%-9d|%-15d|%-15d|%-15d|%-15d  |\n", p[i].pid, p[i].burst_time, p[i].arrival_time, p[i].waiting_time, p[i].turnaround_time);
    }
	for(i=0; i<78; i++) {
        printf("-");
    }
    printf("\n");
    // print average waiting and turnaround time
    printf("\nAverage Waiting Time: %.2f\n", avg_wait_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
}

int main() {
    int i, n=3, quantum;
    

    //printf("Enter the number of processes: ");
    //scanf("%d", &n);
    int arr[10]={24,3,3};
    struct process p[n];

    for(i=0; i<n; i++) {
        p[i].burst_time = arr[i];
        p[i].remaining = p[i].burst_time;
        p[i].pid = i+1;
        
	p[i].arrival_time =0;
    }

    printf("Enter time quantum: ");
    scanf("%d", &quantum);

    calculateTime(p, n, quantum);
    calculateAvgTime(p, n, &avg_wait_time, &avg_turnaround_time);
    displayGanttChart(p, n);

    return 0;
}
/*
3
24
0
3
0
3
0
4
*/
