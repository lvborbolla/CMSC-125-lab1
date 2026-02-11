#include <string.h>
#include "shell.h"

void parse_command(char *line, Command *cmd) {
    memset(cmd, 0, sizeof(Command));

    char *token = strtok(line, " ");
    int argc = 0;

    while (token) {

        if (!strcmp(token, "<"))
            cmd->input_file = strtok(NULL, " ");

        else if (!strcmp(token, ">")) {
            cmd->output_file = strtok(NULL, " ");
            cmd->append = false;
        }

        else if (!strcmp(token, ">>")) {
            cmd->output_file = strtok(NULL, " ");
            cmd->append = true;
        }

        else if (!strcmp(token, "&"))
            cmd->background = true;

        else
            cmd->args[argc++] = token;

        token = strtok(NULL, " ");
    }

    cmd->args[argc] = NULL;
    cmd->command = cmd->args[0];
}
