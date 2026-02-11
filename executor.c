#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

// Reap finished background processes to prevent zombies
void reap_background() {
    // -1 means check ANY child process
    // WNOHANG means do not block
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// Execute command
int execute_command(Command *cmd) {

    // Empty command
    if (!cmd->command)
        return 0;

    // ======================
    // BUILT-IN COMMANDS
    // ======================

    // Exit shell
    if (strcmp(cmd->command, "exit") == 0)
        return -1;

    // Change directory
    if (strcmp(cmd->command, "cd") == 0) {
        char *dir = cmd->args[1];

        if (!dir)
            dir = getenv("HOME");

        if (chdir(dir) != 0)
            perror("cd");

        return 0;
    }

    // Print working directory
    if (strcmp(cmd->command, "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)))
            printf("%s\n", cwd);
        else
            perror("pwd");

        return 0;
    }

    // ======================
    // EXTERNAL COMMAND
    // ======================

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 0;
    }

    // CHILD PROCESS
    if (pid == 0) {

        // Handle input redirection
        if (cmd->input_file) {
            int fd = open(cmd->input_file, O_RDONLY);
            if (fd < 0) {
                perror("input file");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // Handle output redirection
        if (cmd->output_file) {
            int flags = O_WRONLY | O_CREAT;
            flags |= cmd->append ? O_APPEND : O_TRUNC;

            int fd = open(cmd->output_file, flags, 0644);
            if (fd < 0) {
                perror("output file");
                exit(1);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        // Replace child process with new program
        execvp(cmd->command, cmd->args);

        // If exec fails
        perror("exec failed");
        exit(127);
    }

    // PARENT PROCESS
    if (!cmd->background) {
        // Foreground → wait for child
        waitpid(pid, NULL, 0);
    } else {
        // Background → do not wait
        printf("Started background PID %d\n", pid);
    }

    return 0;
}
