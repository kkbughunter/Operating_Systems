// Develop a C program to understand the working of fork(), getpid(), getppid(), sleep() and wait()
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status;

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        exit(1);
    } else if (pid == 0) { // child process
        printf("child process-pid is %d. Child parent's-pid is %d.\n", getpid(), getppid());
        sleep(2);
        printf("Child process ended.\n");
        exit(0);
    } else { // parent process
        printf("I am the parent process. My pid is %d. My child's pid is %d.\n", getpid(), pid);
        wait(&status);
        printf("Child process exited with status %d.\n", status);
    }

    return 0;
}
