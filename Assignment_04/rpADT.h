#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Sort1(float A[], float B[], float bcopy[], char *pid[], int n);
void Sort2(float A[], float B[], int priority[], char *pid[], int n);
void Sort3(float A[], float B[], int priority[], char *vid[], int n);

int RR_waittime(float w_time[], float b_time[], float bcopy[], float A_time[],
                int QT, int n);

void P_nonPRE_waittime(float w_time[], float b_time[], float A_time[], int n);
void P_PRE_waittime(float w_time[], float A_time[], float TA_time[], int priority[], float b_time[], float r_time[], int n);
void RR_calc_turn_around(float ta_time[], float w_time[], float b_time[], int n);
void P_nonPRE_calc_turn_around(float ta_time[], float w_time[], float b_time[],  float A_time[], int n);
void P_PRE_calc_turn_around(float ta_time[], float w_time[], float b_time[], int n);
void displayTable(int n, char *tok[], float A_time[], float b_Time[], float w_time[], float ta_time[]);
void gantt_chart_1(int n, int Time, int QT, char *tok[], float b_time[]);
void gantt_chart_2(int n, char *tok[], float b_time[]);
void gantt_chart_3(int n, char *tok[], float Arr_time[], float burst_Time[], int priority[], float wait_time[], float turn_around_time[]);
void printspaces(int s);
void printlines(int l);

void Sort1(float A[], float B[], float bcopy[], char *vid[], int n) {
  float temp;
  char IDtemp[5];
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (A[i] < A[j]) {
        temp = A[j];
        A[j] = A[i];
        A[i] = temp;

        temp = B[j];
        B[j] = B[i];
        B[i] = temp;

        temp = bcopy[j];
        bcopy[j] = bcopy[i];
        bcopy[i] = temp;

        strcpy(IDtemp, vid[j]);
        strcpy(vid[j], vid[i]);
        strcpy(vid[i], IDtemp);
      }
    }
  }
}

void Sort2(float A[], float B[], int priority[], char *vid[], int n) {
  float temp;
  char IDtemp[5];
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (priority[i] < priority[j]) {
        temp = A[j];
        A[j] = A[i];
        A[i] = temp;

        temp = B[j];
        B[j] = B[i];
        B[i] = temp;

        temp = priority[j];
        priority[j] = priority[i];
        priority[i] = temp;

        strcpy(IDtemp, vid[j]);
        strcpy(vid[j], vid[i]);
        strcpy(vid[i], IDtemp);
      }
    }
  }
}

void Sort3(float A[], float B[], int priority[], char *vid[], int n) {
  float temp;
  char IDtemp[5];
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (A[i] < A[j]) {
        temp = A[j];
        A[j] = A[i];
        A[i] = temp;

        temp = B[j];
        B[j] = B[i];
        B[i] = temp;

        temp = priority[j];
        priority[j] = priority[i];
        priority[i] = temp;

        strcpy(IDtemp, vid[j]);
        strcpy(vid[j], vid[i]);
        strcpy(vid[i], IDtemp);
      }
    }
  }
}

int RR_waittime(float w_time[], float b_time[], float bcopy[], float A_time[],
                int QT, int n) {
  int f = 0;
  float time;
  time = 0;
  while (1) {
    int flag = 1;
    for (int i = 0; i < n; i++) {
      if (bcopy[i] > 0) {
        flag = 0;
        if (bcopy[i] > QT) {
          time += QT;
          bcopy[i] = bcopy[i] - QT;
        } else {
          time += bcopy[i];
          w_time[i] = time - b_time[i] - A_time[i];
          bcopy[i] = 0;
        }
      }
    }
    if (flag == 1) {
      break;
    }
  }
  return time;
}

void P_nonPRE_waittime(float w_time[], float b_time[], float A_time[], int n) {
  int flag = 0;
  for (int i = 0; i < n; i++) {
    if (A_time[i] != 0) {
      flag = 1;
      break;
    }
  }
  if (flag == 0) {
    w_time[0] = 0;
    for (int i = 1; i < n; i++) {
      w_time[i] = b_time[i - 1] + w_time[i - 1] + A_time[i - 1] - A_time[i];
    }
  } else if (flag == 1) {
    float sum = 0;
    w_time[0] = 0;
    for (int i = 1; i < n; i++) {
      sum = sum + b_time[i - 1];
      w_time[i] = sum - A_time[i];
    }
  }
}

void P_PRE_waittime(float w_time[], float A_time[], float TA_time[],
                    int priority[], float b_time[], float r_time[], int n) {
  int firstexe[n];
  float bcopy[n];
  int flag = 0, lessP = 0, completed = 0, Time = 0, min = 1000, finish;
  for (int i = 0; i < n; i++) {
    bcopy[i] = b_time[i];
    firstexe[i] = 1;
  }
  while (completed != n) {
    for (int j = 0; j < n; j++) {
      if ((A_time[j] <= Time) && (priority[j] < min) && (bcopy[j] > 0)) {
        min = priority[j];
        lessP = j;
         
        flag = 1;
      }
    }
    if (firstexe[lessP] == 1) {
      r_time[lessP] = Time;
      firstexe[lessP] = 0;
    }
    bcopy[lessP]--;
    if (bcopy[lessP] == 0) {

      completed++;
      min = 1000;
      flag = 0;
      finish = Time + 1;
      w_time[lessP] = finish - b_time[lessP] - A_time[lessP];
      if (w_time[lessP] < 0) {
        w_time[lessP] = 0;
      }
    }
    Time++;
  }
  for (int i = 0; i < n; i++) {
    r_time[i] = r_time[i] - A_time[i];
  }
}

void RR_calc_turn_around(float ta_time[], float w_time[], float b_time[],
                         int n) {
  for (int i = 0; i < n; i++) {
    ta_time[i] = b_time[i] + w_time[i];
  }
}

void P_nonPRE_calc_turn_around(float ta_time[], float w_time[], float b_time[],
                               float A_time[], int n) {
  float ta = 0, tsum = 0;
  for (int i = 0; i < n; i++) {
    ta = ta + b_time[i];
    ta_time[i] = ta - A_time[i];
  }
}

void P_PRE_calc_turn_around(float ta_time[], float w_time[], float b_time[],
                            int n) {
  for (int i = 0; i < n; i++) {
    ta_time[i] = w_time[i] + b_time[i];
  }
}

void displayTable(int n, char *tok[], float A_time[], float b_Time[],
                  float w_time[], float ta_time[]) {
  float tot_wt = 0.0;
  float tot_tat = 0.0;
  printf("\n---------------------------------------------------------------------------------");
  printf("\n|  Token no  |   ArrivalTime    |  Burst Time   |  Waiting Time |TurnAround  Time|");
  printf("\n---------------------------------------------------------------------------------\n");
  for (int i = 0; i < n; i++) {
    printf("   %s", tok[i]);
    printf("%9s", "|");
    printf("   %.2f   \t|", A_time[i]);
    printf("  %.2f   \t|", b_Time[i]);
    printf("    %.2f   \t|", w_time[i]);
    printf("   %.2f   \t|", ta_time[i]);
    printf("\n");
    tot_wt += w_time[i];
    tot_tat += w_time[i] + b_Time[i];
    // printf("\n");
  }
  printf("---------------------------------------------------------------------------------");
  printf("\nAverage waiting time     : %.2f", tot_wt / n);
  printf("\nAverage turn around time : %.2f", tot_tat / n);
  printf("\n---------------------------------------------------------------------------------");
}

void gantt_chart_1(int n, int time, int QT, char *pid[], float b_time[]) {
  int b = 0;
  float bcopy1[n], bcopy2[n];
  ;
  int pro;
  for (int i = 0; i < n; i++) {
    bcopy1[i] = b_time[i];
    bcopy2[i] = b_time[i];
  }
  printf("\nGANTT CHART\n");
  printlines(2 * time + n * 2 - QT * 2);
  printf("\n");
  float t = 0;
  while (1) {
    int flag = 1;
    for (int i = 0; i < n; i++) {
      if (bcopy1[i] > 0) {
        flag = 0;
        if (bcopy1[i] > QT) {
          t += QT;
          printf("|%s", pid[i]);
          printspaces(QT);
          bcopy1[i] = bcopy1[i] - QT;
        } else {
          t += bcopy1[i];
          printf("|%s", pid[i]);
          printspaces(QT);
          bcopy1[i] = 0;
        }
      }
    }
    if (flag == 1) {
      break;
    }
  }
  printf("|\n");
  printlines(2 * time + n * 2 - QT * 2);
  printf("\n");
  t = 0;
  printf("%d ", (int)t);
  printspaces(QT + 1);
  while (1) {
    int flag = 1;
    for (int i = 0; i < n; i++) {
      if (bcopy2[i] > 0) {
        flag = 0;
        if (bcopy2[i] > QT) {
          t += QT;
          printf("%d ", (int)t);
          if (t > 9) {
            printspaces(QT);
          } else {
            printspaces(QT + 1);
          }
          bcopy2[i] = bcopy2[i] - QT;
        } else {
          t += bcopy2[i];
          printf("%d ", (int)t);
          if (t > 9) {
            printspaces(QT);
          } else {
            printspaces(QT + 1);
          }

          bcopy2[i] = 0;
        }
      }
    }
    if (flag == 1) {
      break;
    }
  }
}

void gantt_chart_2(int n, char *pid[], float b_time[]) {
  float b = 0;
  printf("\nGANTT CHART\n");
  for (int i = 0; i < n; i++) {
    printlines((int)b_time[i]);
  }
  printf("\n\n");
  for (int i = 0; i < n; i++) {
    printf("| %s", pid[i]);
    printspaces((int)b_time[i]);
  }
  printf("|\n");
  for (int i = 0; i < n; i++) {
    printlines((int)b_time[i]);
  }
  printf("\n");
  for (int i = 0; i < n + 1; i++) {
    printf("%d   ", (int)b);
    b = b + b_time[i];
    if (i != n) {
      printspaces((int)b_time[i]);
    }
  }
}

void gantt_chart_3(int n, char *VID[], float A_time[], float b_time[], int priority[], float w_time[], float ta_time[]) {
  float bcopy[n];
  int prev = 0;
  int flag = 0, lessP = 0, completed = 0, Time = 0, min = 1000, finish;
  for (int i = 0; i < n; i++) {
    bcopy[i] = b_time[i];
  }
  printf("\nGANTT CHART\n");
  while (completed != n) {
    for (int j = 0; j < n; j++) {
      if ((A_time[j] <= Time) && (priority[j] < min) && (bcopy[j] > 0)) {
        min = priority[j];
        lessP = j;
        printspaces(Time - prev - 1);
        printf("|%s", VID[j]);
        prev = Time;
        flag = 1;
      }
    }
    if (flag == 0) {
      Time++;
      continue;
    }
    bcopy[lessP]--;
    if (bcopy[lessP] == 0) {
      completed++;
      min = 1000;
      flag = 0;
      finish = Time + 1;
    }
    Time++;
  }
  printf("|\n-----------------------------------\n");
  int s = 0;
  flag = lessP = completed = Time = 0;
  min = 1000;
  for (int i = 0; i < n; i++) {
    bcopy[i] = b_time[i];
  }
  while (completed != n) {
    for (int j = 0; j < n; j++) {
      if ((A_time[j] <= Time) && (priority[j] < min) && (bcopy[j] > 0)) {
        min = priority[j];
        lessP = j;
        printspaces(Time - prev);
        if (s != 0) {
          printf(" ");
        }
        s = 1;
        printf("%d", Time);
        prev = Time;
        flag = 1;
      }
    }
    if (flag == 0) {
      Time++;
      continue;
    }
    bcopy[lessP]--;
    if (bcopy[lessP] == 0) {
      completed++;
      min = 1000;
      flag = 0;
      finish = Time + 1;
    }
    Time++;
  }
  printf("%d", Time);
}

void printspaces(int s) {
  for (int i = 0; i < s; i++) {
    printf(" ");
  }
}

void printlines(int s) {
  for (int i = 0; i < s + 1; i++) {
    printf("%s", "-");
  }
}



