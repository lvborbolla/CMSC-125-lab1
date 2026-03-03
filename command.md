Problem Analysis
This laboratory requires implementing a simplified Unix shell using the POSIX API in C. The shell must demonstrate:
Process creation (fork)
Program execution (execvp)
Synchronization (waitpid)
I/O redirection (dup2, open)
Background execution (&)
Built-in commands (cd, pwd, exit)

The challenge is separating command parsing, execution, and job management while preventing:
zombie processes
file descriptor leaks
incorrect redirection
built-ins executing in child


Solution Architecture
The solution is modular:
User Input
   ↓
Parser → Command struct
   ↓
Executor → built-in OR fork/exec
   ↓
Job manager → background cleanup


File Responsibilities
| File        | Responsibility                           |
| ----------- | ---------------------------------------- |
| mysh.c      | main loop + shell control                |
| parser.c    | tokenize + build Command struct          |
| executor.c  | execute commands (fork/exec/redirection) |
| shell.h     | structs + shared prototypes              |
| commands.md | feature documentation                    |
| Makefile    | compilation                              |
| *.o         | object files                             |


Data Structure
typedef struct {
    char *command;
    char *args[256];
    char *input_file;
    char *output_file;
    bool append;
    bool background;
} Command;

Execution Flow
while shell running:
    reap zombies
    read input
    parse
    if built-in → run
    else fork
         child → redirection + exec
         parent → wait or background

Edge Cases Considered
empty input
multiple spaces
nonexistent command
missing redirection file
zombie processes
background finishing early

Built-in Commands
| Command | Description             |
| ------- | ----------------------- |
| exit    | terminate shell         |
| cd      | change directory        |
| pwd     | print working directory |

Redirection
| Operator | Meaning          |
| -------- | ---------------- |
| >        | overwrite output |
| >>       | append output    |
| <        | input file       |