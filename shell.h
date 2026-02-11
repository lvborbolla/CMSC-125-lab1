#ifndef SHELL_H
#define SHELL_H

#define MAX_LINE 1024
#define MAX_ARGS 256

typedef struct {
    char *command;
    char *args[MAX_ARGS];
} Command;

// parse user input into command struct
void parse_command(char *line, Command *cmd);

// execute built-in command
int execute_builtin(Command *cmd);

#endif
