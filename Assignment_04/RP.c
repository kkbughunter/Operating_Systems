#include "rpADT.h"

int main() {
  int n, op, ch;
  float *Arr_time, *burst_Time, *bcopy, *wait_time, *turn_around_time,
      *response_time;
  int *priority;
  char *VID[5];
  do {
    printf("\n\nCPU SCHEDULING ALGORITHM \n1-ROUND ROBIN \n2-PRIORITY \n3-EXIT \nChoose an option: ");
    scanf("%d", &op);

    switch (op) {
    case 1:
      printf("\n\tROUND ROBIN SCHEDULING ALGORITHM");
      printf("\nEnter no of process : ");
      scanf("%d", &n);
      int QT, time;
      for (int i = 0; i < n; i++) {
        VID[i] = (char *)malloc(n * sizeof(char));
      }
      Arr_time = (float *)malloc(n * sizeof(float));
      burst_Time = (float *)malloc(n * sizeof(float));
      bcopy = (float *)malloc(n * sizeof(float));
      wait_time = (float *)malloc(n * sizeof(float));
      turn_around_time = (float *)malloc(n * sizeof(float));
      response_time = (float *)malloc(n * sizeof(float));
      for (int i = 0; i < n; i++) {
        printf("\nEnter ID of process               : ");
        scanf("%s", VID[i]);
        printf("Enter arrival time of process     : ");
        scanf("%f", &Arr_time[i]);
        printf("Enter burst time of process       : ");
        scanf("%f", &burst_Time[i]);
        bcopy[i] = burst_Time[i];
      }
      printf("\nEnter TIME QUANTUM: ");
      scanf("%d", &QT);
      Sort1(Arr_time, burst_Time, bcopy, VID, n);
      time = RR_waittime(wait_time, burst_Time, bcopy, Arr_time, QT, n);
      RR_calc_turn_around(turn_around_time, wait_time, burst_Time, n);
      displayTable(n, VID, Arr_time, burst_Time, wait_time, turn_around_time);
      gantt_chart_1(n, time, QT, VID, burst_Time);
      break;
    case 2:
      printf("\n\t	PRIORITY SCHEDULING ALGORITHM");
      printf("\n\n\t1-Preemptive\n\t2-Non-Preemptive\nChoose your option: ");
      scanf("%d", &ch);
      printf("\nEnter no of process : ");
      scanf("%d", &n);
      for (int i = 0; i < n; i++) {
        VID[i] = (char *)malloc(n * sizeof(char));
      }
      Arr_time = (float *)malloc(n * sizeof(float));
      burst_Time = (float *)malloc(n * sizeof(float));
      bcopy = (float *)malloc(n * sizeof(float));
      priority = (int *)malloc(n * sizeof(int));
      wait_time = (float *)malloc(n * sizeof(float));
      turn_around_time = (float *)malloc(n * sizeof(float));
      response_time = (float *)malloc(n * sizeof(float));
      for (int i = 0; i < n; i++) {
        printf("\nEnter ID of process            : ");
        scanf("%s", VID[i]);
        printf("Enter arrival time of process  : ");
        scanf("%f", &Arr_time[i]);
        printf("Enter burst time of process    : ");
        scanf("%f", &burst_Time[i]);
        bcopy[i] = burst_Time[i];
        printf("Enter priority of process    : ");
        scanf("%d", &priority[i]);
      }
      if (ch == 1) {
        Sort3(Arr_time, burst_Time, priority, VID, n);
        P_PRE_waittime(wait_time, Arr_time, turn_around_time, priority, bcopy,response_time, n);
        P_PRE_calc_turn_around(turn_around_time, wait_time, burst_Time, n);
        displayTable(n, VID, Arr_time, burst_Time, wait_time, turn_around_time);
        gantt_chart_3(n, VID, Arr_time, burst_Time, priority, wait_time,turn_around_time);

      } else if (ch == 2) {
        Sort3(Arr_time, burst_Time, priority, VID, n);
        P_nonPRE_waittime(wait_time, burst_Time, Arr_time, n);
        P_nonPRE_calc_turn_around(turn_around_time, wait_time, burst_Time,Arr_time, n);
        displayTable(n, VID, Arr_time, burst_Time, wait_time, turn_around_time);
        gantt_chart_2(n, VID, burst_Time);
      }
      break;
    case 3:
      printf("\n*Program Exited...\n");
      break;
    }

  } while (op != 3);
}

/*

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



2
2
5

p1
0
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
3
1
5

p5
4
5
2


*/