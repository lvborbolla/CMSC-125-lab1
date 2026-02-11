#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

int execute_builtin(Command *cmd) {
    if (!cmd->command) return 0;

    if (strcmp(cmd->command, "exit") == 0) {
        return -1; // signal to shell to exit
    }

    if (strcmp(cmd->command, "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s\n", cwd);
        else
            perror("getcwd");
        return 0;
    }

    if (strcmp(cmd->command, "cd") == 0) {
        char *dir = cmd->args[1];
        if (!dir) dir = getenv("HOME"); // default to HOME
        if (chdir(dir) != 0)
            perror("cd");
        return 0;
    }

    printf("mysh: command not found: %s\n", cmd->command);
    return 0;
}
