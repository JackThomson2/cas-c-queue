# Define the compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -pedantic -lpthread -O1

# Define your target executable
TARGET = app

# Define source files
SRCS = main.c queue.c

# Define object files
OBJS = $(SRCS:.c=.o)

# Rule to make the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule for object files, depends on source files and headers
%.o: %.c queue.h
	$(CC) $(CFLAGS) -c $<

# Rule for cleaning the build
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets
.PHONY: clean
