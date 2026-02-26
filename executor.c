// executor.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "shell.h"

// Job table for background jobs
#define MAX_JOBS 128

typedef struct {
    int id;
    pid_t pid;
    char *cmd;   // strdup'd command string
    int running; // 1 if active, 0 if free
} Job;

static Job jobs[MAX_JOBS];
static int next_job_id = 1;

// Helper: build a single string representation of the command+args
static char *build_cmd_string(Command *cmd) {
    if (!cmd || !cmd->command) return NULL;
    size_t bufsize = 1024;
    char *buf = malloc(bufsize);
    if (!buf) return NULL;
    buf[0] = '\0';
    for (int i = 0; cmd->args[i]; i++) {
        size_t need = strlen(buf) + strlen(cmd->args[i]) + 2;
        if (need > bufsize) {
            bufsize = need + 256;
            char *n = realloc(buf, bufsize);
            if (!n) { free(buf); return NULL; }
            buf = n;
        }
        if (i > 0) strcat(buf, " ");
        strcat(buf, cmd->args[i]);
    }
    return buf;
}

// Add a background job to the table. Returns the job id or -1 on failure.
static int add_job(pid_t pid, const char *cmdstr) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (!jobs[i].running) {
            jobs[i].id = next_job_id++;
            jobs[i].pid = pid;
            jobs[i].cmd = cmdstr ? strdup(cmdstr) : NULL;
            jobs[i].running = 1;
            return jobs[i].id;
        }
    }
    return -1; // no slot
}

// Remove job by pid and free resources
static void remove_job_by_pid(pid_t pid) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].running && jobs[i].pid == pid) {
            jobs[i].running = 0;
            if (jobs[i].cmd) { free(jobs[i].cmd); jobs[i].cmd = NULL; }
            return;
        }
    }
}

// Find job id by pid, or -1
static int find_job_id_by_pid(pid_t pid) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].running && jobs[i].pid == pid) return jobs[i].id;
    }
    return -1;
}

// Reap any finished background jobs (non-blocking)
void reap_background_jobs() {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        int jid = find_job_id_by_pid(pid);
        if (jid != -1) {
            // Print summary
            if (WIFEXITED(status)) {
                int ec = WEXITSTATUS(status);
                printf("[%d] Done: PID %d exited with %d\n", jid, pid, ec);
            } else if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                printf("[%d] Killed: PID %d terminated by signal %d\n", jid, pid, sig);
            } else {
                printf("[%d] Done: PID %d\n", jid, pid);
            }
            remove_job_by_pid(pid);
        } else {
            // Not a tracked job; just reap quietly
        }
    }
}

// Cleanup remaining background jobs when exiting: try to terminate and wait
static void cleanup_jobs_on_exit() {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].running) {
            pid_t pid = jobs[i].pid;
            kill(pid, SIGTERM);
        }
    }
    // Wait for them to exit (blocking)
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, 0)) > 0) {
        remove_job_by_pid(pid);
    }
}

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
    // Clean up any background jobs before exiting
    cleanup_jobs_on_exit();
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
                    fprintf(stderr, "mysh: cannot open input file '%s': ", cmd->input_file);
                    perror(NULL);
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
                    fprintf(stderr, "mysh: cannot open output file '%s': ", cmd->output_file);
                    perror(NULL);
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
                    if (exit_code == 127) {
                        printf("mysh: command not found: %s\n", cmd->command);
                    } else if (exit_code != 0) {
                        printf("Command exited with code %d\n", exit_code);
                    }
                } else if (WIFSIGNALED(status)) {
                    int sig = WTERMSIG(status);
                    printf("Command terminated by signal %d\n", sig);
                }
            } else {
                // Build command string and register job
                char *cmdstr = build_cmd_string(cmd);
                int jid = add_job(pid, cmdstr);
                if (jid < 0) {
                    printf("Failed to add background job for PID %d\n", pid);
                } else {
                    printf("[%d] Started: %s (PID: %d)\n", jid, cmdstr ? cmdstr : cmd->command, pid);
                }
                if (cmdstr) free(cmdstr);
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
