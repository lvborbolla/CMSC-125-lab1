// mysh.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

#define MAX_LINE 1024

int main() {
    char line[MAX_LINE];

    while (1) {
        // Reap any finished background jobs
        reap_background_jobs();
        // Display prompt
        printf("mysh> ");
        fflush(stdout);

        // Read input
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n"); // Handle Ctrl+D gracefully
            break;
        }

        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';

        // Skip empty input
        if (strlen(line) == 0) continue;

        // Parse the command
        Command cmd = parse_command(line);

        // Execute the command
        execute_command(&cmd);
    }

    printf("Exiting mysh...\n");
    return 0;
}