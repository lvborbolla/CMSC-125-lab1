# SUCCESS CASES

## Test 1: pwd command
pwd
# Expected: /mnt/c/Users/Botoy/Desktop/3rd Year/CMSC 125/mysh/CMSC-125-lab1

## Test 2: cd command
cd /tmp
# Expected: (no output, directory changed successfully)

## Test 3: External command (echo)
echo "Hello from mysh"
# Expected: Hello from mysh

## Test 4: Output redirection (>)
echo "Test output" > test_output.txt
# Expected: (no output, file created)

## Test 5: Input redirection (<)
cat < test_output.txt
# Expected: Test output

## Test 6: Background job (&)
sleep 2 &
# Expected: [1] <pid> (job number and process ID)

## Test 7: Multiple arguments
ls -la /tmp
# Expected: detailed listing of /tmp with permissions, owner, size, and timestamp

# Cleanup
rm -f test_output.txt
# Expected: (no output on success)

---

# FAILURE CASES

## Test 8: Command not found
nonexistentcommand
# Expected: nonexistentcommand: command not found

## Test 9: Input file not found
cat < nonexistent_file.txt
# Expected: nonexistent_file.txt: No such file or directory

## Test 10: Invalid directory for cd
cd /nonexistent/directory/path
# Expected: cd failed: No such file or directory

## Test 11: Permission denied (input redirect)
echo "test" > /root/test.txt
# Expected: /root/test.txt: Permission denied

## Test 12: Missing required argument for cd
cd
# Expected: (changes to home directory - actually succeeds, no failure)