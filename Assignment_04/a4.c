#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct process {
  char pid[10];
  int arrive_time;
  int burst_time;
  int token_num;
  int waiting_time;
  int response_time;
  int turnaround_time;
  int remaining_time;
  int end_time;
  int present;
  int priority;
};
float compute_preemptive_waiting_time(struct process *p, int n) {
  printf("Gantt Chart:\n");

  int sum = 0;
  for (int i = 0; i < n; i++) {
    sum += p[i].burst_time;
    p[i].remaining_time = p[i].burst_time;
    p[i].waiting_time = 0;
  }
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if ((p[i].arrive_time == p[j].arrive_time) &&
          (p[j].priority < p[i].priority)) {
        struct process temp;
        temp = p[i];
        p[i] = p[j];
        p[j] = temp;
      }
    }
  }
  int index = 0;
  int counter = 0;
  int i = p[0].arrive_time;
  for (int i = 0; i <= n; i++) {
    printf("----------");
  }
  printf("\n|");
  printf("    %s     |", p[0].pid);

  int sum1 = p[0].arrive_time;
 
  int gantt[100];
  int indice = 0;
  gantt[indice++] = sum1;
  while (counter != n) {

    if (p[index].remaining_time == 0) {
      counter++;
      int temp = 100;
      int found = 0;
      for (int k = 0; k < n; k++) {
        if ((p[k].arrive_time <= i) && (p[k].remaining_time > 0)) {
          if (p[k].priority < temp) {
            temp = p[k].priority;
            index = k;
            found = 1;
   
          }
        }
      }

      if (found == 1) {
        printf("    %s     |", p[index].pid);
        gantt[indice++] = sum1;
        if (p[index].present == 1) {
          p[index].waiting_time += i - (p[index].end_time);
        } else {
          p[index].waiting_time += i - (p[index].arrive_time);
         
        }
      }
    }

    else {
      int found = 0;
      for (int j = 0; j < n; j++) {
        if ((p[j].arrive_time <= i) && (p[j].remaining_time > 0)) {
          if ((p[index].remaining_time >= 0) &&
              (p[j].priority < p[index].priority)) {
            p[index].end_time = i;
            p[index].present = 1;
            index = j;
            found = 1;

            if (found == 1) {
              gantt[indice++] = sum1;
              printf("    %s     |", p[index].pid);
            }
            if (p[index].present == 1) {

              p[index].waiting_time += i - (p[index].end_time);
            } else {
              p[index].waiting_time += i - (p[index].arrive_time);
            }
          }
        }
      }
    }

    if (p[index].remaining_time != 0) {

      p[index].remaining_time--;
    }
    if (counter == n) {
      break;
    }
    i++;
    sum1++;
  }
  gantt[indice] = sum1;
  printf("\n");
  for (int i = 0; i <= n; i++) {
    printf("----------");
  }
  printf("\n");
  for (int i = 0; i <= indice; i++) {
    printf("%-12d", gantt[i]);
  }
  printf("\n");
  float avg = 0;
  for (int i = 0; i < n; i++) {
    avg += p[i].waiting_time;
  }
  return avg / n;
}
float compute_waiting_time(struct process *p, int n, int time) {
  int sum = p[0].arrive_time;
  p[0].waiting_time = 0;
  printf("Gantt Chart:\n");
  double nlay1 = 0;
  for (int i = 0; i < n; i++) {
    nlay1 += (p[i].burst_time / time);
  }
  int nlay = (int)(nlay1);
  printf("%d", nlay);
 
  for (int i = 0; i < nlay; i++) {
    printf("----------------");
  }
  printf("\n|");
  int sum1 = p[0].arrive_time;

  for (int i = 0; i < n; i++) {
    p[i].remaining_time = p[i].burst_time;
  }
  int gantt[100];
  int temp = 0;
  gantt[0] = p[0].arrive_time;
  int count = 1;
  int counter = 0;
  int i = -1;
  while (counter != n) {

    while (p[(i + 1) % n].arrive_time > sum ||
           p[(i + 1) % n].remaining_time <= 0) {
      i = (i + 1) % n;
    }
    if (p[(i + 1) % n].present == 1) {
      p[(i + 1) % n].waiting_time += sum - p[(i + 1) % n].end_time;
    } else {
      p[(i + 1) % n].waiting_time += sum - p[(i + 1) % n].arrive_time;
    }
    if (p[(i + 1) % n].waiting_time < 0) {
      p[(i + 1) % n].waiting_time = 0;
    }
    int timer = p[(i + 1) % n].remaining_time;
    p[(i + 1) % n].remaining_time -= time;
    p[(i + 1) % n].end_time = sum + time;
    p[(i + 1) % n].present = 1;
    printf("    %s     |", p[(i + 1) % n].pid);
    if (timer < time) {
      sum += timer;
    } else {
      sum += time;
    }
    if (p[(i + 1) % n].remaining_time <= 0 && p[(i + 1) % n].present != -1) {
      counter++;
      p[(i + 1) % n].present = -1;
    }
    i = (i + 1) % n;
    gantt[count++] = sum;
  }
  printf("\n");
  for (int i = 0; i < nlay; i++) {
    printf("----------------");
  }
  printf("\n");
  for (int i = 0; i < count; i++) {
    printf("%-12d", gantt[i]);
  }
  printf("\n");
  float avg = 0;
  for (int i = 0; i < n; i++) {
    avg += p[i].waiting_time;
  }
  return avg / n;
}
float compute_turnaround_time(struct process *p, int n) {
  float sum = 0;
  for (int i = 0; i < n; i++) {
    p[i].turnaround_time = p[i].burst_time + p[i].waiting_time;
    p[i].response_time = p[i].turnaround_time;
    sum += p[i].turnaround_time;
  }
  return sum / n;
}

void roundrobin() {
  struct process *p;
  int n;
  printf("Enter no of Processes: ");
  scanf("%d", &n);
  p = (struct process *)malloc(n * sizeof(struct process));
  for (int i = 0; i < n; i++) {
    printf("\nEnter Process ID: ");
    scanf("%s", p[i].pid);
    printf("Enter arrival time: ");
    scanf("%d", &p[i].arrive_time);
    printf("Enter the burst time: ");
    scanf("%d", &p[i].burst_time);
  }
  printf("Enter the time quantum: ");
  int time;
  scanf("%d", &time);
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (p[j].arrive_time < p[i].arrive_time) {
        struct process temp;
        temp = p[i];
        p[i] = p[j];
        p[j] = temp;
      }
    }
  }

  for (int i = 0; i < n; i++) {
    p[i].token_num = i + 1;
  }
  float average_waiting_time = compute_waiting_time(p, n, time);

  float average_turnaround_time = compute_turnaround_time(p, n);

  float average_response_time = average_turnaround_time;
  printf("\nTable1:\n");
  printf("|--------------------------------------------------------------------"
         "--------------------------|\n");
  printf("|%s\t%s\t%s\t%s\t%s\t%s\n", "Process ID", "Arrival Time",
         "  Burst Time  ", "Turnaround Time", "Waiting Time",
         "Response Time  |");
  printf("|--------------------------------------------------------------------"
         "--------------------------|\n");
  for (int i = 0; i < n; i++) {
    printf("| %s \t\t %d \t\t %d \t\t %d \t\t %d \t\t\t %-6d|\n",
           p[i].pid, p[i].arrive_time, p[i].burst_time,
           p[i].turnaround_time, p[i].waiting_time, p[i].response_time);
  }
  printf("|-------------------------------------------------------------------"
         "---------------------------|\n");
  printf("\nAverage Waiting Time: %f\n", average_waiting_time);
  printf("Average Turnaround Time: %f\n", average_turnaround_time);
  printf("Average Response Time: %f\n", average_response_time);
}

void priority() {
  struct process *p;
  int n;
  printf("Enter the number of Processes: ");
  scanf("%d", &n);
  p = (struct process *)malloc(n * sizeof(struct process));
  for (int i = 0; i < n; i++) {
    printf("\nEnter the Process ID: ");
    scanf("%s", p[i].pid);
    printf("Enter arrival time: ");
    scanf("%d", &p[i].arrive_time);
    printf("Enter burst time: ");
    scanf("%d", &p[i].burst_time);
    printf("Enter the priority: ");
    scanf("%d", &p[i].priority);
  }

  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (p[j].arrive_time < p[i].arrive_time) {
        struct process temp;
        temp = p[i];
        p[i] = p[j];
        p[j] = temp;
      }
    }
  }

  for (int i = 0; i < n; i++) {
    p[i].token_num = i + 1;
  }
  float average_waiting_time = compute_preemptive_waiting_time(p, n);

  float average_turnaround_time = compute_turnaround_time(p, n);

  float average_response_time = average_turnaround_time;
  printf("\nTable1:\n");
  printf("|--------------------------------------------------------------------"
         "--------------------------|\n");
  printf("|%s\t%s\t%s\t%s\t%s\t%s\n", "Process ID", "Arrival Time",
         "  Burst Time  ", "Turnaround Time", "Waiting Time",
         "Response Time  |");
  printf("|--------------------------------------------------------------------"
         "--------------------------|\n");
  for (int i = 0; i < n; i++) {
    printf("| %s \t\t %d \t\t %d \t\t %d \t\t %d \t\t\t %-6d|\n",
           p[i].pid, p[i].arrive_time, p[i].burst_time,
           p[i].turnaround_time, p[i].waiting_time, p[i].response_time);
  }
  printf("|-------------------------------------------------------------------"
         "---------------------------|\n");
  printf("\nAverage Waiting Time: %f\n", average_waiting_time);
  printf("Average Turnaround Time: %f\n", average_turnaround_time);
  printf("Average Response Time: %f\n", average_response_time);
}

void main() {

  while (1) {
    int op;
    printf("\nCPU SCHEDULING\n\t1.Round Robin "
           "\n\t2.Priority Algorithm\n\t3.Exit\n\nEnter your option:");
    scanf("%d", &op);
    printf("\n");
    if (op == 1) {
      roundrobin();
      printf("\n");
    } else if (op == 2) {

      priority();
      printf("\n");

    } else {
      printf("Exited!!\n");
      exit(0);
    }
  }
}


/* RR

1
3

p1
0
24

p2
0
3

p3
0
3

4

*/

/* pnp

2
5

p1
0
10
3

p2
0
1
1

p3
0
2
4

p4
0
1
5

p5
0
5
2

*/

/* pp

2
5

p1
10
10
3

p2
1
1
1

p3
2
2
4

p4
1
1
5

p5
5
5
2

*/

