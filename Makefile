# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99

# Source files
SOURCES = tests.c dfa.c nfa.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Executable name
EXECUTABLE = program

# Default target
all: $(EXECUTABLE)

# Link object files to create executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

# Compile source files to object files
%.o: %.c dfa.h nfa.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Phony targets
.PHONY: all clean