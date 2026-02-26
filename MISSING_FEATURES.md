# Missing Features & Issues in mysh Implementation

## Critical Issues (Must Fix)

### 1. **Error Message for Nonexistent Commands**
**Status**: ❌ MISSING  
**Requirement**: Should print `mysh: command not found: <command>`  
**Current Behavior**: Command exits silently with code 127  
**Impact**: User can't distinguish real errors from missing commands  
**Fix Location**: `executor.c` - Parent process needs to detect exec failure

```c
// Current: Child exits with 127 but parent doesn't know why
// Should: Check if child exited with 127 and print appropriate error
```

### 2. **Array Bounds Check - Buffer Overflow Prevention**
**Status**: ⚠️ UNSAFE  
**Location**: `parser.c` line ~48  
```c
if (cmd.command == NULL) {
    cmd.command = strdup(token);
    cmd.args[arg_index++] = cmd.command;  // ← No check if arg_index < 256
}
```
**Fix**: Add bounds check: `if (arg_index >= 255) break;`

### 3. **Memory Leak in parse_command()**
**Status**: ⚠️ INCOMPLETE  
**Issue**: If redirection operators (`<`, `>`, `>>`) appear without filenames, allocated memory isn't freed
```c
// If we hit this without a following token:
token = strtok(NULL, " \t\n");
if (token) cmd.input_file = strdup(token);  // Memory leak if token is NULL
```

### 4. **strtok() Destroys Input**
**Status**: ⚠️ RISKY  
**Issue**: `strtok()` modifies the input string, which could cause issues if the command string is reused  
**Fix**: Make a copy of the line before tokenizing:
```c
char *line_copy = strdup(line);
token = strtok(line_copy, " \t\n");
// ... then free(line_copy) when done
```

## High Priority Issues

### 5. **No Input Validation for Redirection**
**Status**: ❌ MISSING  
**Cases Not Handled**:
- `ls >` (output file missing)
- `wc -l <` (input file missing)
- `cat > >` (malformed)

**Fix**: Check `if (!token)` after redirection operator

### 6. **Exit Status Reporting**
**Status**: ⚠️ INCORRECT  
**Current**: Prints "Command exited with code 0" for successful commands  
**Should**: Only print if exit code is non-zero, or don't print at all

```c
// Current check is wrong:
if (exit_code != 0) {
    printf("Command exited with code %d\n", exit_code);
}
// This only prints on non-zero, which is correct, but verify behavior
```

### 7. **cd Command with No Arguments**
**Status**: ⚠️ INCOMPLETE  
**Issue**: `cd` without args should go to HOME directory  
**Fix**: Already partially handled with `if (!path) path = getenv("HOME");` ✅

### 8. **Background Job Completion Display**
**Status**: ✅ Probably works but needs testing  
**Verify**: That `[1] Done: PID X exited with 0` appears when background job completes

## Medium Priority Issues

### 9. **Permission Denied Handling**
**Status**: ⚠️ INCOMPLETE  
**Cases**: 
- Write to read-only file
- Read from permission-denied file

**Current**: Shows "open output file: Permission denied"  
**Should be**: More helpful error message in context

### 10. **Long Command Line Handling**
**Status**: ⚠️ NO VALIDATION  
**Issue**: MAX_LINE = 1024, no validation in parser  
**Current**: Would just be truncated by fgets()  
**Should**: Show error if line is too long

### 11. **Malformed Input Edge Cases**
**Cases Not Fully Tested**:
- Multiple redirections: `ls > file1.txt > file2.txt`
- Operators in unusual positions: `& ls` (background before command)
- Only operators: `< > &` or `> &`
- Mixed redirections with no command: `< input.txt > output.txt`

### 12. **Parser Error Handling**
**Status**: ⚠️ MINIMAL  
**Missing**: What if someone types:
- Empty line after operator (e.g., `echo "test" >`)
- Command name is an operator (e.g., type just `<`)

## Testing Gaps

### 13. **No Tests for Edge Cases**
**Not Verified**:
- Very long command lines (> 1024 chars)
- Commands with 256+ arguments
- Rapid background job sequences
- Background job completing before next prompt
- `/dev/null` redirection
- Spaces around operators: `ls>file.txt` vs `ls > file.txt`
- Mixed operator orderings all working: `sort < in.txt > out.txt` vs `sort > out.txt < in.txt`

### 14. **Signal Handling**
**Status**: ❌ MISSING  
**Missing**: 
- SIGCHLD handler for background jobs
- Handling Ctrl+C (SIGINT) in parent
- Handling Ctrl+Z (SIGTSTP) - stop/continue jobs

## Summary of Fixes Needed

| Issue | Priority | Complexity | Lines Affected |
|-------|----------|------------|-----------------|
| Error message for nonexistent commands | HIGH | Medium | executor.c ~160-180 |
| Array bounds check | CRITICAL | Low | parser.c ~48 |
| Memory leak on missing redirection file | HIGH | Low | parser.c ~35-45 |
| strtok() input corruption | MEDIUM | Medium | parser.c ~30 |
| Input validation for redirections | HIGH | Low | parser.c ~35-45 |
| Long line validation | MEDIUM | Low | mysh.c ~20 |
| Comprehensive error handling | HIGH | Medium | executor.c ~120-140 |
| Signal handling (Ctrl+C, SIGCHLD) | LOW | High | executor.c + mysh.c |

## Recommended Implementation Order

1. **First**: Fix buffer overflow in parser (arg_index bounds)
2. **Second**: Add input validation for missing redirection filenames
3. **Third**: Fix nonexistent command error message
4. **Fourth**: Handle exit status display correctly
5. **Fifth**: Copy input string before tokenizing (strtok safety)
6. **Sixth**: Add comprehensive input validation
7. **Seventh**: Test thoroughly with provided test scenarios
