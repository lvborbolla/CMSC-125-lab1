// executor.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "shell.h"

// Simple job counter for background jobs
static int job_id = 1;

/**
 * Execute built-in 'cd' command
 */
int builtin_cd(char *path) {
    if (!path) path = getenv("HOME"); // default to home
    if (chdir(path) != 0) {
        perror("cd failed");
        return -1;
    }
    return 0;
}

/**
 * Execute built-in 'pwd' command
 */
int builtin_pwd() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
        return 0;
    } else {
        perror("getcwd failed");
        return -1;
    }
}

/**
 * Execute built-in 'exit' command
 */
void builtin_exit() {
    printf("Exiting mysh...\n");
    exit(0);
}

/**
 * Execute any command (built-in or external)
 */
void execute_command(Command *cmd) {
    if (!cmd || !cmd->command) return;

    // --- Handle built-in commands ---
    if (strcmp(cmd->command, "cd") == 0) {
        builtin_cd(cmd->args[1]);
    } else if (strcmp(cmd->command, "pwd") == 0) {
        builtin_pwd();
    } else if (strcmp(cmd->command, "exit") == 0) {
        builtin_exit();
    } else {
        // --- External command execution ---
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            return;
        }

        if (pid == 0) { // Child process
            // Input redirection
            if (cmd->input_file) {
                int fd = open(cmd->input_file, O_RDONLY);
                if (fd < 0) {
                    perror("open input file");
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            // Output redirection
            if (cmd->output_file) {
                int flags = O_WRONLY | O_CREAT;
                flags |= cmd->append ? O_APPEND : O_TRUNC;
                int fd = open(cmd->output_file, flags, 0644);
                if (fd < 0) {
                    perror("open output file");
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            // Execute command
            execvp(cmd->command, cmd->args);
            perror("exec failed");
            exit(127);
        } else { // Parent process
            if (!cmd->background) {
                int status;
                waitpid(pid, &status, 0);
                if (WIFEXITED(status)) {
                    int exit_code = WEXITSTATUS(status);
                    if (exit_code != 0) {
                        printf("Command exited with code %d\n", exit_code);
                    }
                }
            } else {
                printf("[%d] Started: %s (PID: %d)\n", job_id++, cmd->command, pid);
                // TODO: add to background job list for reaping later
            }
        }
    }

    // --- Free memory allocated by parser ---
    if (cmd->command) free(cmd->command);
    // Skip cmd->args[0] since it's the same as cmd->command
    for (int i = 1; cmd->args[i]; i++)
        free(cmd->args[i]);
    if (cmd->input_file) free(cmd->input_file);
    if (cmd->output_file) free(cmd->output_file);
}
