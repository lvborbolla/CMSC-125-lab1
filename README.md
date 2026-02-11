# CMSC-125-lab1 
Unix Shell by
Luis Victor Borbolla and Eryl Joseph Aspera

# mysh — Simple Unix Shell in C

---

## Compilation and Usage Instructions

### Compilation

To compile the shell program, run:

```bash
make
```

This will generate the executable:
./mysh

To clean/remove compiled files, run:
make clean

### Usage

./mysh

You will see the interactive prompt:

mysh>

Example commands:

mysh> pwd
mysh> cd /tmp
mysh> ls -la
mysh> echo "Hello World"
mysh> exit

## List of Implemented Features
The mysh shell supports the following required features:

### Interactive Command Execution
Displays a prompt (mysh>)

Continuously reads and executes user commands

### Built-in Commands
Implemented directly in the shell without forking:

exit — terminates the shell

cd — changes the current working directory using chdir()

pwd — prints the current working directory using getcwd()

### External Command Execution
Executes non-built-in commands using:

fork()

execvp()

waitpid() (foreground processes)

Example:

mysh> ls -l
mysh> cat file.txt

### Input and Output Redirection
Supports standard redirection operators:

> overwrite output

>> append output

< redirect input

Examples:

mysh> echo test > file.txt
mysh> echo more >> file.txt
mysh> wc -l < file.txt
mysh> sort < input.txt > output.txt
Redirection implemented using:

open()

dup2()

close()

### Background Execution
Commands ending with & run in the background

Parent process does not wait

Background jobs are reaped using waitpid() with WNOHANG

Example:

mysh> sleep 10 &
mysh> echo Shell still responsive

### Known Limitations or Bugs
Piping (|) is not supported

Quoted strings with spaces are not fully handled

Limited recovery from malformed commands (e.g., missing filenames after redirection)

## Design Decisions and Architecture Overview
The shell is organized into modular components for clarity and maintainability:

mysh.c
Handles the main interactive shell loop, prompt display, and overall control flow.

parser.c
Tokenizes user input and constructs a Command structure containing:

command name

arguments

redirection files

background execution flag

executor.c
Executes parsed commands by:

running built-ins directly

forking child processes for external commands

applying redirection using file descriptors

managing background jobs

shell.h
Contains shared struct definitions and function prototypes used across modules.

## Screenshots Showing Functionality
(Add screenshots here demonstrating the required features)