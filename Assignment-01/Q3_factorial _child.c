// Develop a C program to create one child process using fork system call. 
// Let the child process reads the first command line argument and computes the factorial of the given number. 
// Let the parent process reads the second command line argument and print the series up to that number
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s number1....number2\n", argv[0]);
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork error");
        return 1;
    } else if (pid == 0) {
        // child process
        int n = atoi(argv[1]);
        int fact = 1;
        for (int i = 1; i <= n; i++) {
            fact *= i;
        }
        printf("Factorial of %d is %d\n", n, fact);
    } else {
        // parent process
        int n = atoi(argv[2]);
        for (int i = 1; i <= n; i++) {
            printf("%d ", i);
        }
        printf("\n");

        int status;
        wait(&status);
    }

    return 0;
}
