#include<stdio.h>
#include<stdlib.h>

struct processer{
    char process_id[20];
    int burst_time;
    int arrival_time;
    float waiting_time;
    float turn_around_time;
};

void print_table(struct processer **p, int n);
void print_gunth_chart(struct processer **p, int n);
void insertion_sort(struct processer **p, int n);
struct processer **getInput(int n);

void insertion_sort(struct processer **p, int n) {
    int i, j;
    struct processer *key;

    for (i = 1; i < n; i++) {
        key = p[i];
        j = i - 1;
        while (j >= 0 && p[j]->arrival_time > key->arrival_time) {
            p[j + 1] = p[j];
            j = j - 1;
        }

        p[j + 1] = key;
    }
}

void print_table(struct processer **p, int n){
    int i=0,ct=0;
    float a_tut=0,a_wt=0;
    ct = p[0]->arrival_time;
    printf("First Come First Serve (FCFS) Scheduling Algorithm\n");
    printf("-------------------------------------------------\n");
    printf(".--------------------------------------------------------------------------------------------.\n");
    printf("| process_id | Arrival Time | Burst Time | Completion Time | Turn Around Time | Waiting Time |\n");
    printf("|------------|--------------|------------|-----------------|------------------|--------------|\n");
    for(i=0; i<n; i++){
        ct += p[i]->burst_time;  // finding Completion Time
        p[i]->turn_around_time = ct - p[i]->arrival_time;  // finding Turn Around Time
        p[i]->waiting_time = p[i]->turn_around_time - p[i]->burst_time;  // finding Waiting Time
        printf("|%-12s|%-14d|%-12d|%-17d|%-18.3f|%-14.3f|\n",p[i]->process_id,p[i]->arrival_time,p[i]->burst_time,ct,p[i]->turn_around_time,p[i]->waiting_time);
        a_tut += p[i]->turn_around_time; // finding Total Turn Around Time
        a_wt += p[i]->waiting_time;  // finding Total Waiting Time
    }
    printf("`--------------------------------------------------------------------------------------------`\n");
    printf("Average Turn Around Time = %.3f\n",a_tut/n);  // finding Average Turn Around Time
    printf("Average Waiting Time = %.3f\n",a_wt/n); // finding Average Waiting Time
}

void print_gunth_chart(struct processer **p, int n){
    printf("\n\nGantt Chart\n");
    int i=0,j=0;
    // Upper line
    printf("|");
    for(i=0; i<n; i++){
        for(j=0; j<p[i]->burst_time; j++){
            printf("--");
        }
        printf("|");
    }
    // process printing
    printf("\n|");
    for(i=0; i<n; i++){
        printf("%*s%*s",p[i]->burst_time,p[i]->process_id,p[i]->burst_time,"");
        printf("|");
    }
    // Bottom line
    printf("\n|");
    for(i=0; i<n; i++){
        for(j=0; j<p[i]->burst_time; j++){
            printf("--");
        }
        printf("|");
    }
    // printing the gantt chart values.
    int ct=p[0]->arrival_time;
    printf("\n%d",p[0]->arrival_time);
    for(i=0; i<n; i++){
        for(j=0; j<p[i]->burst_time; j++){
            printf("  ");
        }
        ct += p[i]->burst_time;
        printf("%d",ct);
    }
    printf("\n\n");
}


int main()
{
    int n,i;    
    printf("\nEnter the number of Processer: ");
    scanf("%d", &n);

    struct processer **p = (struct processer **) malloc(n * sizeof(struct processer *));
    
    for(i =0; i<n; i++){
        p[i] = (struct processer *) malloc(sizeof(struct processer));
        printf("\nEnter Process Name: ");
        scanf("%s", p[i]->process_id);
        printf("Enter Arrival Time of %s: ", p[i]->process_id);
        scanf("%d", &p[i]->arrival_time);
        printf("Enter Burst Time of %s: ", p[i]->process_id);
        scanf("%d", &p[i]->burst_time);
    }
    insertion_sort(p,n);
    print_table(p, n);
    print_gunth_chart(p,n);    
    return 0;
}

