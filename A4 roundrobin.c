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

    // print top bar
    printf("");
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].burst_time; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n|");

    // print process id in the middle
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].burst_time-1; j++) {
            printf(" ");
        }
        printf("P%d", p[i].pid);
        for(j=0; j<p[i].burst_time-1; j++) {
            printf(" ");
        }
        printf("|");
    }
    printf("\n ");

    // print bottom bar
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].burst_time; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n");

    // print turnaround time below
    printf(" ");
    for(i=0; i<n; i++) {
        printf("%2d ", p[i].turnaround_time);
    }
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
        printf("|%-10d|%-15d|%-15d|%-15d|%-15d  |\n", p[i].pid, p[i].burst_time, p[i].arrival_time, p[i].waiting_time, p[i].turnaround_time);
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
    int i, n, quantum;
    

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct process p[n];

    for(i=0; i<n; i++) {
        printf("Enter burst time for process %d: ", i+1);
        scanf("%d", &p[i].burst_time);
        p[i].remaining = p[i].burst_time;
        p[i].pid = i+1;
        printf("Enter arrival time for process %d: ", i+1);
        scanf("%d", &p[i].arrival_time);
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
