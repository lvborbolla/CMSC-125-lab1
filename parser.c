#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

Command parse_command(char *line) {
    Command cmd;
    
    // Initialize struct
    cmd.command = NULL;
    for (int i = 0; i < 256; i++) cmd.args[i] = NULL;
    cmd.input_file = NULL;
    cmd.output_file = NULL;
    cmd.append = 0;
    cmd.background = 0;

    // Tokenize the line
    char *token;
    int arg_index = 0;

    token = strtok(line, " \t\n");
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            // Next token is input file
            token = strtok(NULL, " \t\n");
            if (token) cmd.input_file = strdup(token);
        } else if (strcmp(token, ">") == 0) {
            // Next token is output file (overwrite)
            token = strtok(NULL, " \t\n");
            if (token) {
                cmd.output_file = strdup(token);
                cmd.append = 0;
            }
        } else if (strcmp(token, ">>") == 0) {
            // Next token is output file (append)
            token = strtok(NULL, " \t\n");
            if (token) {
                cmd.output_file = strdup(token);
                cmd.append = 1;
            }
        } else if (strcmp(token, "&") == 0) {
            // Background execution
            cmd.background = 1;
        } else {
            // First token is command, rest are args
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
