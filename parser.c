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

    int seen_input = 0;
    int seen_output = 0;

    char *token;
    int arg_index = 0;

    // Tokenize the line by whitespace
    token = strtok(line, " \t\n");
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            // Input redirection
            token = strtok(NULL, " \t\n");

            if (seen_input) {
                fprintf(stderr, "syntax error: multiple output redirections\n");
                cmd.command = NULL;
                return cmd;
            }
            seen_input = 1;

            if (!token) {
                fprintf(stderr, "syntax error: missing filename after '<'\n");
                cmd.command = NULL;
                return cmd;
            }

            cmd.input_file = strdup(token);
        } else if (strcmp(token, ">") == 0) {
            // Output redirection (overwrite)
            token = strtok(NULL, " \t\n");

            if (seen_output) {
                fprintf(stderr, "syntax error: multiple output redirections\n");
                cmd.command = NULL;
                return cmd;
            }
            seen_output = 1;

            if (!token) {
                fprintf(stderr, "syntax error: missing filename after '>'\n");
                cmd.command = NULL;   // Mark command invalid
                return cmd;
            }

            cmd.output_file = strdup(token);
            cmd.append = 0;
            
        } else if (strcmp(token, ">>") == 0) {
            // Output redirection (append)
            token = strtok(NULL, " \t\n");

            if (seen_output) {
                fprintf(stderr, "syntax error: multiple output redirections\n");
                cmd.command = NULL;
                return cmd;
            }
            seen_output = 1;
            
            if (!token) {
                fprintf(stderr, "syntax error: missing filename after '>>'\n");
                cmd.command = NULL;
                return cmd;
            }

            cmd.output_file = strdup(token);
            cmd.append = 1;
            
        } else if (strcmp(token, "&") == 0) {
            // Background execution
            cmd.background = 1;
        } else {
            // Command or argument
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

    return cmd;
}