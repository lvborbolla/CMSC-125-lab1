#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>

// Command structure for storing parsed command info
typedef struct {
    char *command;         // Command name
    char *args[256];       // Arguments (NULL-terminated)
    char *input_file;      // For '<' redirection
    char *output_file;     // For '>' or '>>' redirection
    int append;            // 1 if '>>', 0 if '>'
    int background;        // 1 if '&', 0 otherwise
} Command;

// Function prototypes
Command parse_command(char *line);
void execute_command(Command *cmd);
int builtin_cd(char *path);
int builtin_pwd();
void builtin_exit();

#endif
