#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

void need_cal(int max[100][100], int alloc[100][100], int need[100][100],
              int nop, int res) {
  for (int i = 0; i != nop; i++) {
    for (int j = 0; j != res; j++) {
      need[i][j] = max[i][j] - alloc[i][j];
    }
  }
}

int safety_sequence(int seq[], int pid[], int need[100][100],
                    int alloc[100][100], int avail[], int nop, int res) {
  int count = 0, temp, flag;
  int visit[100];
  for (int i = 0; i != nop; i++) {
    visit[i] = 0;
  }
  while (count < nop) {
    temp = 0;
    for (int i = 0; i != nop; i++) {
      if (visit[i] == 0) {
        flag = 1;
        for (int j = 0; j != res; j++) {
          if (need[i][j] > avail[j]) {
            flag = 0;
            break;
          }
        }
        if (flag == 0)
          continue;
        else {
          seq[count] = pid[i];
          for (int k = 0; k != res; k++) {
            avail[k] = avail[k] + alloc[i][k];
          }
          visit[i] = 1;
          count++;
          temp = 1;
        }
      }
    }
    if (temp == 0) {
      printf("\ndeadlock");
      return 0;
      break;
    }
  }
  return 1;
}

int request(int p, int seq[], int pid[], int need[100][100],
            int alloc[100][100], int avail[], int nop, int res,
            int max[100][100], int req[100]) {
  int can = 1, r;
  for (int i = 0; i != res; i++) {
    if (avail[i] - req[i] < 0 && need[p][i] - req[i] < 0) {
      can = 0;
    }
  }
  if (can == 0) {
    printf("\n cannot fulfill the request\n");
  } else {
    for (int i = 0; i != res; i++) {
      alloc[p][i] += req[i];
      need[p][i] -= req[i];
      avail[i] -= req[i];
    }
  }
  r = safety_sequence(seq, pid, need, alloc, avail, nop, res);
  if (r == 0)
    return 0;
  else
    return 1;
}

void main() {
  int option = 0;
  int nop;
  int res;
  int avail[100];
  int req[100];
  int max[100][100];
  int alloc[100][100];
  int need[100][100];
  int pid[100];
  int seq[100];
  char pn[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
  while (option != 5) {
    printf("\n 1 FOR READING DATA\n 2 PRINT DATA\n 3 FOR FINDING SAFETY "
           "SEQUENCE\n 4 FOR REQUEST\n 5 FOR EXIT\n");
    printf("ENTER THE OPTION:");
    scanf("%d", &option);
    if (option == 1) {
      printf("ENTER THE NUMBER OF PROCESSES:");
      scanf("%d", &nop);
      printf("ENTER THE NUMBER OF RESOURCES:");
      scanf("%d", &res);
      for (int i = 0; i != res; i++) {
        printf("ENTER THE AVAILABLE INSTANCES [%c]:", pn[i]);
        scanf("%d", &avail[i]);
      }
      for (int i = 0; i != nop; i++) {
        pid[i] = i;
        printf("PROCESS: %d\n", i + 1);
        for (int j = 0; j != res; j++) {
          printf("ENTER THE MAXIMUM REQUIRMENT [%c]:", pn[j]);
          scanf("%d", &max[i][j]);
        }
      }
      printf("---------------------------ALLOCATED "
             "INSTANCES--------------------------------------\n");
      for (int i = 0; i != nop; i++) {
        printf("PROCESS: %d\n", i + 1);
        for (int j = 0; j != res; j++) {
          printf("ENTER THE ALLOCATED [%c]:", pn[j]);
          scanf("%d", &alloc[i][j]);
        }
      }
      need_cal(max, alloc, need, nop, res);
    } else if (option == 2) {
      printf("\nTABLE\n");
      printf("PID\t ALLOC\t\t\t  MAX\t\t\t   NEED\t\t\t  AVAIL\n");
      for (int i = 0; i != nop; i++) {
        if (i == 0) {
          printf("\t");
          for (int i = 0; i != 4; i++) {
            for (int j = 0; j != res; j++) {
              printf("%c\t", pn[j]);
            }
          }
          printf("\n");
        }
        printf("P%d\t", pid[i]);
        for (int j = 0; j != res; j++) {
          printf("%d\t", alloc[i][j]);
        }
        for (int j = 0; j != res; j++) {
          printf("%d\t", max[i][j]);
        }
        for (int j = 0; j != res; j++) {
          printf("%d\t", need[i][j]);
        }
        if (i == 0) {
          for (int i = 0; i != res; i++) {
            printf("%d\t", avail[i]);
          }
        }
        printf("\n");
      }
    } else if (option == 3) {
      safety_sequence(seq, pid, need, alloc, avail, nop, res);
      printf("\nSAFETY SEQUENCE:  ");
      for (int i = 0; i != nop; i++) {
        printf("P%d\t", seq[i]);
      }
      printf("\n");
    } else if (option == 4) {
      int p, r;
      printf("Enter the process:");
      scanf("%d", &p);
      for (int i = 0; i != res; i++) {
        printf("ENTER THE REQUEST INSTANCES [%c]:", pn[i]);
        scanf("%d", &req[i]);
      }
      r = request(p, seq, pid, need, alloc, avail, nop, res, max, req);
      if (r == 1) {
        printf("\nSAFETY SEQUENCE:  ");
        for (int i = 0; i != nop; i++) {
          printf("P%d\t", seq[i]);
        }
        printf("\n");
      } else
        printf("\nUNSAFE STATE");
    }
  }
}


/*
5
3

3
3
2




*/