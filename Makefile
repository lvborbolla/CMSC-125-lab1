CC = gcc
CFLAGS = -Wall -Wextra -std=c11

OBJ = mysh.o parser.o executor.o

# Link
mysh: $(OBJ)
	$(CC) $(OBJ) -o mysh

# Compile
%.o: %.c shell.h
	$(CC) $(CFLAGS) -c $<

# Cleanup
clean:
	rm -f *.o mysh
