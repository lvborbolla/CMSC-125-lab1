#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "shell.h"

void reap_background() {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int execute_command(Command *cmd) {

    if (!cmd->command) return 0;

    // built-ins
    if (!strcmp(cmd->command, "exit"))
        return -1;

    if (!strcmp(cmd->command, "cd")) {
        chdir(cmd->args[1] ? cmd->args[1] : getenv("HOME"));
        return 0;
    }

    if (!strcmp(cmd->command, "pwd")) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
        return 0;
    }

    pid_t pid = fork();

    if (pid == 0) {

        if (cmd->input_file) {
            int fd = open(cmd->input_file, O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if (cmd->output_file) {
            int flags = O_WRONLY | O_CREAT;
            flags |= cmd->append ? O_APPEND : O_TRUNC;
            int fd = open(cmd->output_file, flags, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(cmd->command, cmd->args);
        perror("exec");
        _exit(127);
    }

    if (!cmd->background)
        waitpid(pid, NULL, 0);
    else
        printf("Started background PID %d\n", pid);

    return 0;
}
