#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t child_pid = fork();

    if (child_pid < 0) {
        // Error occurred while forking
        perror("Fork failed");
        exit(1);
    } else if (child_pid == 0) {
        // This is the child process
        printf("Child process executing...\n");
        // Perform the child process's task here
        exit(0); // Terminate child successfully
    } else {
        // This is the parent process
        int status;
        wait(&status); // Wait for child process to terminate
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            printf("Child process exited with status: %d\n", exit_status);
        } else {
            printf("Child process terminated abnormally.\n");
        }
    }

    return 0;
}
