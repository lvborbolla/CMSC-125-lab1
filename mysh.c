#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int main() {
    char line[1024];
    while (1) {
        printf("mysh> ");
        if (!fgets(line, sizeof(line), stdin)) break;

        Command cmd = parse_command(line);

        if (cmd.command != NULL)
            execute_command(&cmd);

        // Free memory after execution
        for (int i = 0; cmd.args[i]; i++) free(cmd.args[i]);
        if (cmd.input_file) free(cmd.input_file);
        if (cmd.output_file) free(cmd.output_file);
        if (cmd.command) free(cmd.command);
    }
    return 0;
}
