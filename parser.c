#include <string.h>
#include <stdlib.h>
#include "shell.h"

// Parse user input line into structured Command
void parse_command(char *line, Command *cmd) {

    // Reset command structure
    memset(cmd, 0, sizeof(Command));

    int argc = 0;

    // Break input string into tokens separated by space
    char *token = strtok(line, " \t\n");

    while (token != NULL) {

        // Input redirection
        if (strcmp(token, "<") == 0) {
            cmd->input_file = strtok(NULL, " ");
        }

        // Output redirection (overwrite)
        else if (strcmp(token, ">") == 0) {
            cmd->output_file = strtok(NULL, " ");
            cmd->append = false;
        }

        // Output redirection (append)
        else if (strcmp(token, ">>") == 0) {
            cmd->output_file = strtok(NULL, " ");
            cmd->append = true;
        }

        // Background execution
        else if (strcmp(token, "&") == 0) {
            cmd->background = true;
        }

        // Normal command or argument
        else {
            cmd->args[argc++] = token;
        }

        token = strtok(NULL, " \t\n");
    }

    // NULL terminate args array (required by execvp)
    cmd->args[argc] = NULL;

    // First argument is the command name
    cmd->command = cmd->args[0];
}
