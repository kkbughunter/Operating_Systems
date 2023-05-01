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
void merge_sort(struct processer **p, int left, int right);
void merge(struct processer **p, int left, int mid, int right);
FILE *input_file, *output_file;

void insertion_sort(struct processer **p, int n) {
    int i, j;
    struct processer *key;

    for (i = 1; i < n; i++) {
        key = p[i];
        j = i - 1;

        // Move elements of arr[0..i-1], that are greater than key, to one position ahead of their current position
        while (j >= 0 && p[j]->burst_time > key->burst_time) {
            p[j + 1] = p[j];
            j = j - 1;
        }

        p[j + 1] = key;
    }
}

void print_table(struct processer **p, int n){
    int i=0,ct=0,tut=0,wt=0;
    float a_tut=0,a_wt=0;
    ct = p[0]->arrival_time;
    // 					  |     12     |       14     |     12     |       17        |        18        |      14      |
    fprintf(output_file, "Shortest Job First (SJF) Scheduling Algorithm\n");
    fprintf(output_file, "---------------------------------------------\n");
    fprintf(output_file, ".--------------------------------------------------------------------------------------------.\n");
    fprintf(output_file, "| process_id | Arrival Time | Burst Time | Completion Time | Turn Around Time | Waiting Timw |\n");
    fprintf(output_file, "|------------|--------------|------------|-----------------|------------------|--------------|\n");
    for(i=0; i<n; i++){
        ct += p[i]->burst_time;  // finding Completion Time
        p[i]->turn_around_time = ct - p[i]->arrival_time;  // finding Turn Around Time
        p[i]->waiting_time = p[i]->turn_around_time - p[i]->burst_time;  // finding Waiting Time
        fprintf(output_file, "|%-12s|%-14d|%-12d|%-17d|%-18.3f|%-14.3f|\n",p[i]->process_id,p[i]->arrival_time,p[i]->burst_time,ct,p[i]->turn_around_time,p[i]->waiting_time);
        a_tut += p[i]->turn_around_time; // finding Total Turn Around Time
        a_wt += p[i]->waiting_time;  // finding Total Waiting Time
    }
    fprintf(output_file, "`--------------------------------------------------------------------------------------------`\n");
    fprintf(output_file, "Average Turn Around Time = %.3f\n",a_tut/n);  // finding Average Turn Around Time
    fprintf(output_file, "Average Waiting Time = %.3f\n",a_wt/n); // finding Average Waiting Time
}
void print_gunth_chart(struct processer **p, int n){
    fprintf(output_file, "\n\nGunth Chart");
    int i=0,j=0;
    // Upper line
    fprintf(output_file, "\n|");
    for(i=0; i<n; i++){
        for(j=0; j<p[i]->burst_time; j++){
            fprintf(output_file, "--");
        }
        fprintf(output_file, "|");
    }
    // process printing
    fprintf(output_file, "\n|");
    for(i=0; i<n; i++){
        fprintf(output_file, "%*s%*s",p[i]->burst_time,p[i]->process_id,p[i]->burst_time,"");
        fprintf(output_file, "|");
    }
    // Bottem line
    fprintf(output_file, "\n|");
    for(i=0; i<n; i++){
        for(j=0; j<p[i]->burst_time; j++){
            fprintf(output_file, "--");
        }
        fprintf(output_file, "|");
    }
    // printing the gunth chart values.
    int ct=p[0]->arrival_time;
    fprintf(output_file, "\n%d",p[0]->arrival_time);
    for(i=0; i<n; i++){
        for(j=0; j<p[i]->burst_time; j++){
            fprintf(output_file, "  ");
        }
        ct += p[i]->burst_time;
        fprintf(output_file, "%d",ct);
    }
}

int main()
{
	input_file = fopen("input2.txt", "r");
	output_file = fopen("output.txt", "w");
    if (input_file == NULL || output_file == NULL) {
        printf("Failed to open input / output file.");
        return 1;
    }
    int n,i;
    fscanf(input_file, "%d", &n);
    struct processer **p = (struct processer **) malloc(n * sizeof(struct processer));
    for(i =0; i<n; i++){
        p[i] = (struct processer *) malloc(sizeof(struct processer));
        fscanf(input_file, "%s %d %d", p[i]->process_id, &p[i]->arrival_time, &p[i]->burst_time);
    }
    insertion_sort(p,n);
    print_table(p, n);
    print_gunth_chart(p,n);
    fclose(input_file);
    fclose(output_file);
    return 0;
}
// by KKBUGHUNTER