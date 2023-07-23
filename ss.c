#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2]; // File descriptors for the pipe: pipefd[0] - read end, pipefd[1] - write end
    pid_t pid;

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork the process
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process (cmd2)
        close(pipefd[1]); // Close the write end of the pipe

        // Redirect stdin to read end of the pipe
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        // Execute cmd2
        execlp("hi", "cmd2", NULL);
        perror("execlp"); // This line will execute only if execlp fails
        exit(EXIT_FAILURE);
    } else {
        // Parent process (cmd1)
        close(pipefd[0]); // Close the read end of the pipe

        // Redirect stdout to write end of the pipe
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        // Execute cmd1
        execlp("cmd1", "cmd1", NULL);
        perror("execlp"); // This line will execute only if execlp fails
        exit(EXIT_FAILURE);
    }

    return 0;
}
