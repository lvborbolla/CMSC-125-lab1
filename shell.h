#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>

#define MAX_LINE 1024
#define MAX_ARGS 256

typedef struct {
    char *command;
    char *args[MAX_ARGS];
    char *input_file;
    char *output_file;
    bool append;
    bool background;
} Command;

void parse_command(char *line, Command *cmd);
int execute_command(Command *cmd);
void reap_background();

#endif
