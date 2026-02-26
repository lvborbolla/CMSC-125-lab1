// mysh.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "shell.h"

#define MAX_LINE 1024

// Signal handler for Ctrl+C (SIGINT)
static void sigint_handler(int sig) {
    // Just let the loop continue - the prompt will be re-displayed
    printf("\n");
    fflush(stdout);
}

int main() {
    char line[MAX_LINE];

    // Register signal handler for Ctrl+C
    signal(SIGINT, sigint_handler);

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

        // Check if input was truncated (line too long)
        // If the line doesn't contain a newline and filled the buffer, it was truncated
        if (strlen(line) == MAX_LINE - 1 && line[MAX_LINE - 2] != '\0') {
            fprintf(stderr, "mysh: error: command line too long (max %d characters)\n", MAX_LINE - 1);
            // Clear the remaining input from the buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        // Skip empty input
        if (strlen(line) == 0) continue;

        // Parse the command
        Command cmd = parse_command(line);

        // Validate that a command was parsed (not just operators)
        if (!cmd.command) {
            fprintf(stderr, "mysh: error: no command provided\n");
            // Free any allocated resources
            if (cmd.input_file) free(cmd.input_file);
            if (cmd.output_file) free(cmd.output_file);
            continue;
        }

        // Execute the command
        execute_command(&cmd);
    }

    printf("Exiting mysh...\n");
    return 0;
}
