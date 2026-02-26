# mysh Shell - Test Cases and Input Examples

This document contains comprehensive test cases to verify all features of the mysh shell implementation.

## Running Tests

To test the shell with these inputs, use one of the following methods:

### Method 1: Pipe input to shell
```bash
echo "commands here" | ./mysh
```

### Method 2: Use printf for multiple lines
```bash
printf "command1\ncommand2\ncommand3\nexit\n" | ./mysh
```

### Method 3: Redirect from file
```bash
./mysh < test_input.txt
```

---

## Test Category 1: Basic Built-in Commands

### 1.1 Test: pwd (Print Working Directory)
**Input:**
```
pwd
exit
```

**Expected Output:**
```
mysh> /path/to/current/directory
mysh> Exiting mysh...
```

---

### 1.2 Test: cd (Change Directory)
**Input:**
```
pwd
cd /tmp
pwd
exit
```

**Expected Output:**
```
mysh> /original/directory
mysh> mysh> /tmp
mysh> Exiting mysh...
```

---

### 1.3 Test: cd with no arguments (go to HOME)
**Input:**
```
cd
pwd
exit
```

**Expected Output:**
```
mysh> mysh> /home/username
mysh> Exiting mysh...
```

---

### 1.4 Test: exit command
**Input:**
```
exit
```

**Expected Output:**
```
mysh> Exiting mysh...
```

---

## Test Category 2: External Commands

### 2.1 Test: Simple echo command
**Input:**
```
echo "Hello, World!"
exit
```

**Expected Output:**
```
mysh> Hello, World!
mysh> Exiting mysh...
```

---

### 2.2 Test: Command with multiple arguments
**Input:**
```
echo arg1 arg2 arg3
exit
```

**Expected Output:**
```
mysh> arg1 arg2 arg3
mysh> Exiting mysh...
```

---

### 2.3 Test: ls command
**Input:**
```
ls
exit
```

**Expected Output:**
```
mysh> [list of files in current directory]
mysh> Exiting mysh...
```

---

### 2.4 Test: ls with arguments
**Input:**
```
ls -la /tmp
exit
```

**Expected Output:**
```
mysh> [detailed listing of /tmp directory]
mysh> Exiting mysh...
```

---

## Test Category 3: Output Redirection with >

### 3.1 Test: Basic output redirection (truncate)
**Input:**
```
echo "Hello World" > output.txt
cat output.txt
exit
```

**Expected Output:**
```
mysh> mysh> Hello World
mysh> Exiting mysh...
```

**File created:** `output.txt` containing "Hello World"

---

### 3.2 Test: Overwrite existing file
**Input:**
```
echo "First content" > test_file.txt
echo "Second content" > test_file.txt
cat test_file.txt
exit
```

**Expected Output:**
```
mysh> mysh> mysh> Second content
mysh> Exiting mysh...
```

**Result:** `test_file.txt` contains only "Second content"

---

### 3.3 Test: Redirect ls output
**Input:**
```
ls > file_list.txt
wc -l < file_list.txt
exit
```

**Expected Output:**
```
mysh> mysh> [number of files]
mysh> Exiting mysh...
```

---

### 3.4 Test: Redirect to /dev/null
**Input:**
```
echo "This goes nowhere" > /dev/null
echo "This is shown"
exit
```

**Expected Output:**
```
mysh> mysh> This is shown
mysh> Exiting mysh...
```

---

## Test Category 4: Output Redirection with >> (Append)

### 4.1 Test: Basic append
**Input:**
```
echo "Line 1" > append_test.txt
echo "Line 2" >> append_test.txt
cat append_test.txt
exit
```

**Expected Output:**
```
mysh> mysh> mysh> Line 1
Line 2
mysh> Exiting mysh...
```

---

### 4.2 Test: Multiple appends
**Input:**
```
echo "A" >> multi.txt
echo "B" >> multi.txt
echo "C" >> multi.txt
cat multi.txt
exit
```

**Expected Output:**
```
mysh> mysh> mysh> mysh> A
B
C
mysh> Exiting mysh...
```

---

### 4.3 Test: Append vs overwrite
**Input:**
```
echo "Original" > append_vs.txt
echo "Appended" >> append_vs.txt
cat append_vs.txt
echo "Overwritten" > append_vs.txt
cat append_vs.txt
exit
```

**Expected Output:**
```
mysh> mysh> mysh> Original
Appended
mysh> mysh> mysh> Overwritten
mysh> Exiting mysh...
```

---

## Test Category 5: Input Redirection with <

### 5.1 Test: Basic input redirection
**Input:**
```
wc -l < /etc/passwd
exit
```

**Expected Output:**
```
mysh> [number of lines in /etc/passwd]
mysh> Exiting mysh...
```

---

### 5.2 Test: cat input from file (via redirection)
**Input:**
```
cat < sort_input.txt
exit
```

**Expected Output:**
```
mysh> [contents of sort_input.txt]
mysh> Exiting mysh...
```

---

### 5.3 Test: Sort with input redirection
**Input:**
```
sort < unsorted.txt
exit
```

**Expected Output:**
```
mysh> [sorted contents of unsorted.txt]
mysh> Exiting mysh...
```

---

## Test Category 6: Combined Input and Output Redirection

### 6.1 Test: Input from file, output to file
**Input:**
```
sort < unsorted.txt > sorted.txt
cat sorted.txt
exit
```

**Expected Output:**
```
mysh> mysh> [sorted contents]
mysh> Exiting mysh...
```

**Files created:** `sorted.txt` with sorted contents

---

### 6.2 Test: Reversed order (output then input)
**Input:**
```
sort > reverse_sorted.txt < unsorted.txt
cat reverse_sorted.txt
exit
```

**Expected Output:**
```
mysh> mysh> [sorted contents]
mysh> Exiting mysh...
```

---

### 6.3 Test: Convert to uppercase with tail
**Input:**
```
head -n 3 < /etc/passwd | sort
exit
```

**Expected Output:**
```
mysh> [first 3 lines of /etc/passwd, sorted]
mysh> Exiting mysh...
```

---

## Test Category 7: Background Execution with &

### 7.1 Test: Single background job
**Input:**
```
sleep 2 &
echo "Shell is responsive"
exit
```

**Expected Output:**
```
mysh> [1] Started: sleep 2 (PID: [process-id])
mysh> Shell is responsive
mysh> Exiting mysh...
```

---

### 7.2 Test: Multiple background jobs
**Input:**
```
sleep 3 &
sleep 2 &
echo "Multiple jobs running"
sleep 1
exit
```

**Expected Output:**
```
mysh> [1] Started: sleep 3 (PID: [pid1])
mysh> [2] Started: sleep 2 (PID: [pid2])
mysh> Multiple jobs running
mysh> [2] Done: PID [pid2] exited with 0
mysh> [1] Done: PID [pid1] exited with 0
mysh> Exiting mysh...
```

---

### 7.3 Test: Background job with output redirection
**Input:**
```
ls > background_list.txt &
echo "Job started in background"
sleep 1
cat background_list.txt
exit
```

**Expected Output:**
```
mysh> [1] Started: ls > background_list.txt (PID: [pid])
mysh> Job started in background
mysh> [1] Done: PID [pid] exited with 0
mysh> [list of files]
mysh> Exiting mysh...
```

---

### 7.4 Test: Background job that exits quickly
**Input:**
```
echo "Quick background job" &
echo "Foreground continues"
exit
```

**Expected Output:**
```
mysh> [1] Started: echo "Quick background job" (PID: [pid])
mysh> Foreground continues
mysh> [1] Done: PID [pid] exited with 0
mysh> Exiting mysh...
```

---

## Test Category 8: Error Handling - Nonexistent Commands

### 8.1 Test: Command not found
**Input:**
```
nonexistent_command
exit
```

**Expected Output:**
```
mysh> mysh: command not found: nonexistent_command
mysh> Exiting mysh...
```

---

### 8.2 Test: Typo in command
**Input:**
```
ecco "test"
exit
```

**Expected Output:**
```
mysh> mysh: command not found: ecco
mysh> Exiting mysh...
```

---

## Test Category 9: Error Handling - File Not Found

### 9.1 Test: Input file doesn't exist
**Input:**
```
wc -l < /nonexistent/file.txt
exit
```

**Expected Output:**
```
mysh> mysh: cannot open input file '/nonexistent/file.txt': No such file or directory
Command exited with code 1
mysh> Exiting mysh...
```

---

### 9.2 Test: Command reading nonexistent file
**Input:**
```
cat /nonexistent/file.txt
exit
```

**Expected Output:**
```
mysh> cat: /nonexistent/file.txt: No such file or directory
Command exited with code 1
mysh> Exiting mysh...
```

---

## Test Category 10: Error Handling - Permission Issues

### 10.1 Test: Permission denied (read)
**Input:**
```
chmod 000 /tmp/no_read.txt
cat < /tmp/no_read.txt
chmod 644 /tmp/no_read.txt
exit
```

**Expected Output:**
```
mysh> mysh> mysh: cannot open input file '/tmp/no_read.txt': Permission denied
Command exited with code 1
mysh> Exiting mysh...
```

---

## Test Category 11: Error Handling - Redirection Syntax Errors

### 11.1 Test: Missing filename after >
**Input:**
```
ls >
exit
```

**Expected Output:**
```
mysh> mysh: syntax error: missing filename after '>'
MISSING_FEATURES.md  file.txt  mysh    parser.o   test.txt
[... outputs from ls ...]
mysh> Exiting mysh...
```

---

### 11.2 Test: Missing filename after <
**Input:**
```
wc -l <
exit
```

**Expected Output:**
```
mysh> mysh: syntax error: missing filename after '<'
[... outputs from wc ...]
mysh> Exiting mysh...
```

---

### 11.3 Test: Missing filename after >>
**Input:**
```
echo "test" >>
exit
```

**Expected Output:**
```
mysh> mysh: syntax error: missing filename after '>>'
[... outputs ...]
mysh> Exiting mysh...
```

---

### 11.4 Test: Multiple output redirections
**Input:**
```
ls > file1.txt > file2.txt
exit
```

**Expected Output:**
```
mysh> mysh: syntax error: multiple output redirections
mysh> Exiting mysh...
```

---

### 11.5 Test: Multiple input redirections
**Input:**
```
cat < file1.txt < file2.txt
exit
```

**Expected Output:**
```
mysh> mysh: syntax error: multiple input redirections
mysh> Exiting mysh...
```

---

## Test Category 12: Error Handling - Malformed Input

### 12.1 Test: Only operators (no command)
**Input:**
```
< > &
exit
```

**Expected Output:**
```
mysh> mysh: error: no command provided
mysh> Exiting mysh...
```

---

### 12.2 Test: Only redirection operators
**Input:**
```
< input.txt > output.txt
exit
```

**Expected Output:**
```
mysh> mysh: error: no command provided
mysh> Exiting mysh...
```

---

### 12.3 Test: Empty input
**Input:**
```

exit
```

**Expected Output:**
```
mysh> mysh> Exiting mysh...
```

---

## Test Category 13: Edge Cases - Long Commands

### 13.1 Test: Very long command line (> 1023 chars)
**Bash command to test:**
```bash
printf "echo %s\nexit\n" "$(printf 'x%.0s' {1..1050})" | ./mysh 2>&1
```

**Expected Output:**
```
mysh> mysh: error: command line too long (max 1023 characters)
mysh> Exiting mysh...
```

---

### 13.2 Test: Command at the limit (exactly 1023)
**Bash command to test:**
```bash
printf "echo %s\nexit\n" "$(printf 'x%.0s' {1..1017})" | ./mysh
```

**Expected Output:**
```
mysh> xxx...xxx (1017 x's)
mysh> Exiting mysh...
```

---

## Test Category 14: Edge Cases - Many Arguments

### 14.1 Test: Command with too many arguments
**Bash command to test:**
```bash
printf "echo %s\nexit\n" "$(printf 'x %.0s' {1..260})" | ./mysh 2>&1
```

**Expected Output:**
```
mysh> mysh: error: too many arguments (max 255)
mysh> Exiting mysh...
```

---

### 14.2 Test: Command with many valid arguments (250)
**Bash command to test:**
```bash
printf "echo %s\nexit\n" "$(printf 'a %.0s' {1..250})" | ./mysh
```

**Expected Output:**
```
mysh> a a a ... a (250 times)
mysh> Exiting mysh...
```

---

## Test Category 15: Edge Cases - Special Characters

### 15.1 Test: Spaces around redirection operators
**Input:**
```
echo "test" > file_with_spaces.txt
cat file_with_spaces.txt
exit
```

**Expected Output:**
```
mysh> mysh> test
mysh> Exiting mysh...
```

---

### 15.2 Test: Multiple spaces between commands
**Input:**
```
echo    "test"    "arg"
exit
```

**Expected Output:**
```
mysh> test arg
mysh> Exiting mysh...
```

---

### 15.3 Test: Tab characters
**Input (with actual tabs):**
```
echo	"with	tabs"
exit
```

**Expected Output:**
```
mysh> with	tabs
mysh> Exiting mysh...
```

---

## Test Category 16: Practical Real-World Tests

### 16.1 Test: Search and sort workflow
**Input:**
```
grep "root" < /etc/passwd > root_entries.txt
cat root_entries.txt
exit
```

**Expected Output:**
```
mysh> mysh> [root entries from /etc/passwd]
mysh> Exiting mysh...
```

---

### 16.2 Test: File processing pipeline
**Input:**
```
echo -e "banana\napple\ncherry" > fruits.txt
sort < fruits.txt > sorted_fruits.txt
cat sorted_fruits.txt
exit
```

**Expected Output:**
```
mysh> mysh> mysh> apple
banana
cherry
mysh> Exiting mysh...
```

---

### 16.3 Test: Count lines in directory listing
**Input:**
```
ls | wc -l
exit
```

**Expected Output:**
```
mysh> [number of items in current directory]
mysh> Exiting mysh...
```

---

### 16.4 Test: Combine and redirect results
**Input:**
```
echo "First file content" > combined1.txt
echo "Second file content" > combined2.txt
cat combined1.txt combined2.txt > combined_result.txt
cat combined_result.txt
exit
```

**Expected Output:**
```
mysh> mysh> mysh> mysh> First file content
Second file content
mysh> Exiting mysh...
```

---

## Complete Comprehensive Test Suite

Run all tests at once with this input:

```
echo "=== Test 1: Basic Commands ==="
pwd
cd /tmp
pwd
cd
pwd

echo "=== Test 2: Output Redirection ==="
echo "test" > comprehensive_test.txt
cat comprehensive_test.txt
echo "appended" >> comprehensive_test.txt
cat comprehensive_test.txt

echo "=== Test 3: Input Redirection ==="
wc -l < comprehensive_test.txt

echo "=== Test 4: Combined ==="
sort < comprehensive_test.txt > sorted_comprehensive.txt
cat sorted_comprehensive.txt

echo "=== Test 5: Background Job ==="
sleep 1 &
echo "Shell responsive"

echo "=== Test 6: Error Handling ==="
nonexistent_cmd
ls > test1 > test2
< > &

echo "=== All Tests Complete ==="
exit
```

---

## Testing Checklist

Use this checklist to verify all features:

- [ ] pwd shows current directory
- [ ] cd changes directory
- [ ] cd with no arguments goes to HOME
- [ ] exit terminates shell
- [ ] External commands execute correctly
- [ ] Output redirection > creates/truncates files
- [ ] Output redirection >> appends to files
- [ ] Input redirection < reads from files
- [ ] Combined < > redirections work both ways
- [ ] Background jobs show [N] Started: message
- [ ] Shell is responsive during background jobs
- [ ] Background jobs show [N] Done: message
- [ ] Nonexistent commands show "command not found"
- [ ] Missing redirection files show helpful errors
- [ ] Missing filenames after operators show errors
- [ ] Multiple redirections show errors
- [ ] No command with operators shows error
- [ ] Very long commands rejected with error
- [ ] Too many arguments rejected with error
- [ ] Empty input handled gracefully

