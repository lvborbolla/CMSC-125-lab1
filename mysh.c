#include <stdio.h>
#include <string.h>
#include "shell.h"

int main() {

    char line[MAX_LINE];
    Command cmd;

    while (1) {

        // Clean finished background processes
        reap_background();

        // Display prompt
        printf("mysh> ");
        fflush(stdout);

        // Read user input
        if (!fgets(line, MAX_LINE, stdin))
            break;

        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';

        // Parse input into Command struct
        parse_command(line, &cmd);

        // Execute parsed command
        if (execute_command(&cmd) == -1)
            break;
    }

    return 0;
}
