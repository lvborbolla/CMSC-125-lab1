# CMSC-125-lab1 
Unix Shell by
Luis Victor Borbolla and Eryl Joseph Aspera

# mysh — Simple Unix Shell in C

A fully-featured Unix shell implementation in C with comprehensive error handling, input validation, and background job management.

---

## Table of Contents

- [Compilation and Usage](#compilation-and-usage)
- [Features](#features)
- [Error Handling](#error-handling)
- [Architecture](#architecture)
- [Testing](#testing)
- [Known Limitations](#known-limitations)

---

## Compilation and Usage

### Requirements

- GCC compiler
- POSIX-compliant system (Linux, macOS, WSL, etc.)
- Make utility

### Compilation

To compile the shell:

```bash
make
```

This will generate the executable: `./mysh`

To clean and rebuild:

```bash
make clean
make
```

### Running the Shell

Start the interactive shell:

```bash
./mysh
```

You will see the interactive prompt:

```
mysh>
```

### Example Session

```bash
mysh> pwd
/home/user/projects
mysh> cd /tmp
mysh> echo "Hello, World!" > greeting.txt
mysh> cat greeting.txt
Hello, World!
mysh> ls | wc -l
mysh> sleep 5 &
[1] Started: sleep 5 (PID: 12345)
mysh> exit
Exiting mysh...
```

---

## Features

### ✅ Interactive Command Execution

- Displays a `mysh>` prompt and reads commands continuously
- Handles Ctrl+D gracefully to exit
- Non-blocking background job cleanup ensures responsive shell

### ✅ Built-in Commands

Commands implemented directly in the shell (no forking):

| Command | Description |
|---------|-------------|
| `exit` | Terminates the shell and cleans up background jobs |
| `cd [path]` | Changes the current working directory; defaults to HOME if no path given |
| `pwd` | Prints the current working directory |

```bash
mysh> cd /tmp
mysh> pwd
/tmp
mysh> cd
mysh> pwd
/home/user
```

### ✅ External Command Execution

Executes non-built-in commands using `fork()` and `execvp()`:

```bash
mysh> ls -la
mysh> grep "pattern" file.txt
mysh> wc -l < document.txt
mysh> cat file1.txt file2.txt
```

### ✅ Input and Output Redirection

Full support for standard I/O redirection operators:

| Operator | Purpose | Example |
|----------|---------|---------|
| `>` | Redirect output (truncate file) | `echo "test" > output.txt` |
| `>>` | Redirect output (append to file) | `echo "more" >> output.txt` |
| `<` | Redirect input from file | `wc -l < input.txt` |
| `< >` | Combined input and output | `sort < unsorted.txt > sorted.txt` |

```bash
mysh> ls > file_list.txt
mysh> cat file_list.txt
mysh> echo "appended" >> file_list.txt
mysh> sort < data.txt > sorted_data.txt
mysh> wc -l < document.txt
```

Redirection operators work in any order:
```bash
mysh> sort < input.txt > output.txt
mysh> sort > output.txt < input.txt
```

Implementation uses:
- `open()` with appropriate flags (O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC, O_APPEND)
- `dup2()` for file descriptor redirection
- Proper file descriptor cleanup

### ✅ Background Execution

Commands ending with `&` run in the background without blocking the shell:

```bash
mysh> sleep 10 &
[1] Started: sleep 10 (PID: 12345)
mysh> echo "Shell is responsive"
Shell is responsive
mysh> # Background job completes...
[1] Done: PID 12345 exited with 0
mysh> exit
```

Features:
- Parent process continues immediately without waiting
- Job ID and PID displayed for tracking
- Completion status shown when job finishes
- Multiple concurrent background jobs supported
- Automatic zombie process cleanup using `waitpid(WNOHANG)`

### ✅ Comprehensive Error Handling

The shell provides clear, context-aware error messages:

**Command Not Found:**
```bash
mysh> nonexistent_cmd
mysh: command not found: nonexistent_cmd
```

**Missing Redirection File:**
```bash
mysh> cat < /nonexistent/file.txt
mysh: cannot open input file '/nonexistent/file.txt': No such file or directory
Command exited with code 1
```

**Syntax Errors:**
```bash
mysh> ls >
mysh: syntax error: missing filename after '>'

mysh> ls > file1.txt > file2.txt
mysh: syntax error: multiple output redirections

mysh> < > &
mysh: error: no command provided
```

**Input Validation:**
```bash
mysh> # Very long command (> 1023 chars)
mysh: error: command line too long (max 1023 characters)

mysh> echo [260+ arguments]
mysh: error: too many arguments (max 255)
```

### ✅ Signal Handling

- Graceful handling of Ctrl+C (SIGINT) — returns to prompt instead of crashing
- Detects child process termination by signal and reports it
- Proper cleanup of background jobs on exit

---

## Error Handling

The shell has been extensively hardened with input validation and error checking:

### Parser Validation
- **Array bounds checking**: Maximum 255 arguments per command
- **Buffer overflow protection**: Input limited to 1023 characters per line
- **Token validation**: Ensures redirection operators have filenames
- **Duplicate detection**: Prevents multiple input/output redirections
- **Safe string handling**: Uses `strdup()` to prevent `strtok()` data corruption

### Execution Validation
- **File access checking**: Reports permission denied errors clearly
- **Exit code reporting**: Non-zero exit codes displayed with context
- **Malformed input detection**: Rejects operator-only input with helpful message
- **Resource cleanup**: Ensures all file descriptors and memory are freed

---

## Architecture

The shell is organized into modular components for clarity, maintainability, and testability:

### `mysh.c` — Main Shell Loop
- Interactive command-line interface with prompt
- Input reading and processing
- Background job reaping and cleanup
- Signal handling setup
- Long command line validation
- Malformed input detection

### `parser.c` — Command Parsing
- Tokenizes user input by whitespace
- Detects and parses redirection operators (`<`, `>`, `>>`)
- Identifies background execution (`&`)
- Constructs `Command` structure with:
  - Command name
  - Arguments (up to 255)
  - Input/output file paths
  - Append flag
  - Background flag
- Comprehensive error reporting for syntax errors

### `executor.c` — Command Execution
- Executes built-in commands directly (cd, pwd, exit)
- Creates child processes for external commands using `fork()`
- Applies I/O redirection using `dup2()`
- Manages foreground and background processes
- Maintains background job table (up to 128 concurrent jobs)
- Reaps completed processes and reports status
- Improves error messages with file context

### `shell.h` — Declarations
- `Command` struct definition
- Function prototypes shared across modules
- Compilation flags for POSIX compliance

### Data Structures

```c
typedef struct {
    char *command;         // Command name
    char *args[256];       // Arguments (NULL-terminated)
    char *input_file;      // For '<' redirection
    char *output_file;     // For '>' or '>>' redirection
    int append;            // 1 if '>>', 0 if '>'
    int background;        // 1 if '&', 0 otherwise
} Command;
```

---

## Testing

Comprehensive test cases are provided in [TEST_CASES.md](TEST_CASES.md), covering:

- 16 test categories
- Basic built-in commands
- External command execution
- Input/output redirection
- Background execution
- Error handling (nonexistent commands, missing files, permissions)
- Edge cases (long commands, many arguments, special characters)
- Real-world workflows

To run tests:

```bash
# Test basic functionality
printf "pwd\ncd /tmp\npwd\nexit\n" | ./mysh

# Test output redirection
printf "echo 'Hello' > test.txt\ncat test.txt\nexit\n" | ./mysh

# Test background jobs
printf "sleep 2 &\necho responsive\nexit\n" | ./mysh
```

See [IMPLEMENTATION_SUMMARY.md](IMPLEMENTATION_SUMMARY.md) for detailed test results and feature verification.

---

## Known Limitations

The following features are **not** supported (typical for simplified shell implementations):

- **Piping** (`|`) — Cannot connect commands together
- **Quoted strings** — Spaces in quotes treated as separate arguments (e.g., `"hello world"` becomes two arguments)
- **Environment variables** — No `$VAR` expansion
- **Wildcards** — No glob expansion (*, ?, [])
- **Command chaining** — No support for `;`, `&&`, or `||` operators
- **Job control** — No `fg`, `bg` commands or Ctrl+Z (suspend)
- **Aliases** — Cannot create command aliases
- **History** — No command history or readline support

---

## Implementation Notes

### System Calls Used

**Process Management:**
- `fork()` — Create child processes
- `execvp()` — Execute external commands
- `wait()` / `waitpid()` — Wait for child completion
- `getpid()` / `getppid()` — Process identification

**File I/O:**
- `open()` — Open files for redirection
- `dup2()` — Redirect file descriptors
- `close()` — Close file descriptors

**Directory Management:**
- `chdir()` — Change working directory
- `getcwd()` — Get current working directory

**Signal Handling:**
- `signal()` — Register signal handlers

### Compilation Flags

```bash
gcc -Wall -std=c99 -D_POSIX_C_SOURCE=200809L
```

- `-Wall` — Enable all warnings
- `-std=c99` — Use C99 standard
- `-D_POSIX_C_SOURCE=200809L` — Enable POSIX features

---

## File Structure

```
├── mysh.c                      # Main shell loop and I/O handling
├── parser.c                    # Command parsing and tokenization
├── executor.c                  # Command execution and job management
├── shell.h                     # Data structures and function declarations
├── Makefile                    # Build configuration
├── README.md                   # This file
├── TEST_CASES.md              # Comprehensive test cases and examples
├── IMPLEMENTATION_SUMMARY.md  # Detailed feature verification
└── MISSING_FEATURES.md        # Issue tracking and resolution
```

---

## Building and Running

```bash
# Compile
make

# Run
./mysh

# Clean build artifacts
make clean
```

---

## Future Enhancements

Potential improvements for future versions:

- [ ] Pipe operator (`|`) support
- [ ] Proper quoted string handling
- [ ] Environment variable expansion
- [ ] Command history with readline
- [ ] Job control (fg, bg, suspend)
- [ ] Wildcard/glob expansion
- [ ] Multiple command chaining
- [ ] Built-in variables (PWD, HOME, PATH)

---

## Authors

- Luis Victor Borbolla
- Eryl Joseph Aspera

**Course:** CMSC 125 (Operating Systems)  
**Last Updated:** February 2026