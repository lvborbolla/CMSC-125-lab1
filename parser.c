#include <string.h>
#include "shell.h"

void parse_command(char *line, Command *cmd) {
    char *token;
    int argc = 0;

    // reset struct
    cmd->command = NULL;
    for (int i = 0; i < MAX_ARGS; i++)
        cmd->args[i] = NULL;

    token = strtok(line, " \t\n");
    while (token != NULL && argc < MAX_ARGS - 1) {
        cmd->args[argc++] = token;
        token = strtok(NULL, " \t\n");
    }
    cmd->args[argc] = NULL;

    if (argc > 0)
        cmd->command = cmd->args[0];
}
