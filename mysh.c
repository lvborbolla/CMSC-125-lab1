#include <stdio.h>
#include <string.h>
#include "shell.h"

int main() {

    char line[MAX_LINE];
    Command cmd;

    while (1) {

        reap_background();

        printf("mysh> ");
        fflush(stdout);

        if (!fgets(line, MAX_LINE, stdin))
            break;

        line[strcspn(line, "\n")] = '\0';

        parse_command(line, &cmd);

        if (execute_command(&cmd) == -1)
            break;
    }

    return 0;
}
