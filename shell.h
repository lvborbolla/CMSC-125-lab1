#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>

#define MAX_LINE 1024
#define MAX_ARGS 256

// Structure representing a parsed command
typedef struct {
    char *command;        // Name of command (e.g., "ls")
    char *args[MAX_ARGS]; // Argument list (NULL-terminated)
    char *input_file;     // File for input redirection (<)
    char *output_file;    // File for output redirection (> or >>)
    bool append;          // true if >> used
    bool background;      // true if & present
} Command;

// Parses raw user input into Command struct
void parse_command(char *line, Command *cmd);

// Executes parsed command
int execute_command(Command *cmd);

// Cleans up background processes
void reap_background();

#endif
