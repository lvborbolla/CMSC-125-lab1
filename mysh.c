#include <stdio.h>
#include <string.h>
#include "shell.h"

int main() {
    char line[MAX_LINE];
    Command cmd;

    while (1) {
        printf("mysh> ");
        fflush(stdout);

        if (!fgets(line, MAX_LINE, stdin))
            break;

        parse_command(line, &cmd);

        int ret = execute_builtin(&cmd);
        if (ret == -1)
            break; // exit shell
    }

    return 0;
}
