// parser.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

/**
 * Parse a raw input line into a Command struct.
 * Handles:
 *  - Command name and arguments
 *  - Input redirection (<)
 *  - Output redirection (> and >>)
 *  - Background execution (&)
 */
Command parse_command(char *line) {
    Command cmd;

    // Initialize struct
    cmd.command = NULL;
    for (int i = 0; i < 256; i++) cmd.args[i] = NULL;
    cmd.input_file = NULL;
    cmd.output_file = NULL;
    cmd.append = 0;
    cmd.background = 0;

    // Make a copy of line to avoid strtok() modifying the original
    char *line_copy = strdup(line);
    if (!line_copy) return cmd;

    char *token;
    int arg_index = 0;

    // Tokenize the line by whitespace
    token = strtok(line_copy, " \t\n");
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            // Input redirection
            token = strtok(NULL, " \t\n");
            if (token) {
                cmd.input_file = strdup(token);
            } else {
                fprintf(stderr, "mysh: syntax error: missing filename after '<'\n");
            }
        } else if (strcmp(token, ">") == 0) {
            // Output redirection (overwrite)
            token = strtok(NULL, " \t\n");
            if (token) {
                cmd.output_file = strdup(token);
                cmd.append = 0;
            } else {
                fprintf(stderr, "mysh: syntax error: missing filename after '>'\n");
            }
        } else if (strcmp(token, ">>") == 0) {
            // Output redirection (append)
            token = strtok(NULL, " \t\n");
            if (token) {
                cmd.output_file = strdup(token);
                cmd.append = 1;
            } else {
                fprintf(stderr, "mysh: syntax error: missing filename after '>>'\n");
            }
        } else if (strcmp(token, "&") == 0) {
            // Background execution
            cmd.background = 1;
        } else {
            // Command or argument
            // Check bounds to prevent buffer overflow
            if (arg_index >= 255) {
                fprintf(stderr, "mysh: error: too many arguments (max 255)\n");
                break;
            }
            if (cmd.command == NULL) {
                cmd.command = strdup(token);
                cmd.args[arg_index++] = cmd.command;
            } else {
                cmd.args[arg_index++] = strdup(token);
            }
        }

        token = strtok(NULL, " \t\n");
    }

    // Null-terminate args array
    cmd.args[arg_index] = NULL;

    // Free the copy of the line
    free(line_copy);

    return cmd;
}
